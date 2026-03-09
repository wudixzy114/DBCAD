# DBCAD 架构重构方案（第一阶段已落地）

## 目标

在不改变现有业务逻辑和存储语义的前提下，先完成可演进的架构分层，为后续元代码生成替换 `switch_case` 做准备。

## 当前痛点

1. `access.cpp` 同时包含 ACIS 解析、图数据库读写、对象装配，职责混杂。
2. 类型分发强依赖大段 `switch_case`，且与业务代码交织。
3. 低层实现直接暴露到高层 API，存在重复声明、重复结构体等不规范写法。

## 第一阶段（本次）已完成

### 1) 引入存储层门面

新增文件：

- `src/graph_storage.hxx`
- `src/graph_storage.cpp`

作用：

- 将 Neo4j/Memgraph 的全量与增量读写调用统一收口到 `dbcad::storage::GraphStorage`。
- 高层 `DbcadAPI.cpp` 不再直接散落调用 `api_save_* / api_restore_*`。

### 2) 引入 ACIS 运行时适配层

新增文件：

- `src/acis_runtime.hxx`
- `src/acis_runtime.cpp`

作用：

- 将 ACIS 初始化/终止、活跃实体提取行为单独封装到 `dbcad::acis_runtime`。
- `DbcadAPI.cpp` 不再直接依赖底层初始化函数。

### 3) 抽离 switch 类型注册表

新增文件：

- `src/access_switch_registry.hxx`
- `src/access_switch_registry.cpp`

变更：

- `Neo4jNode / Neo4jEdge` 枚举与字符串映射从 `access.cpp` 迁出。
- `access.cpp` 通过 `#include "access_switch_registry.hxx"` 使用同名映射。

收益：

- 未来可把映射改为自动生成（CSV/YAML/JSON -> codegen），不需要直接改 `access.cpp` 主逻辑。

### 4) 清理高层 API 的非规范耦合

文件：

- `src/DbcadAPI.cpp`

改动：

- 删除重复 `extern` 声明和重复 `IncrementalContext` 定义。
- 改为包含统一头文件 `access.hxx`，并调用新分层模块。

### 5) 构建系统接线

文件：

- `CMakeLists.txt`

改动：

- 将新增模块加入 `CORE_SOURCES` / `CORE_HEADERS`。

## 当前层次结构（第一阶段）

- `DbcadAPI`：对外会话/能力门面。
- `graph_storage`：数据库存储门面（全量/增量）。
- `acis_runtime`：ACIS 生命周期与活跃实体访问。
- `access`：保留原有核心实现（包含复杂 switch 与构造逻辑）。
- `access_switch_registry`：switch 的类型注册表。

## 第二阶段建议（下一步）

已完成子步骤（本次）：

1. 抽离 `Restore` 关系恢复的大型 switch

- 新增 `src/access_restore_dispatch.hxx`
- 新增 `src/access_restore_dispatch.cpp`
- `api_restore_entity_list_neo4j` 与 `api_restore_neo4j` 中重复的 `Neo4jEdge` switch 已统一收敛到 `AccessUtils::Restore::Dispatch::ApplyNeo4jEdge(...)`
- `access.cpp` 两处调用已替换为 dispatcher 调用，语义保持不变

1. 抽离 `switch(entity_ptr->identity(...))` 为独立 dispatcher 模块

- 新建 `access_entity_dispatcher.*`，按“创建节点/设置关系/恢复对象”拆分处理函数。
- 先做函数搬迁，不改 case 内部逻辑。

2. 抽离 `Restore` 大型节点/关系 switch

- 将 `Neo4jNode` 和 `Neo4jEdge` 的 case 分别拆到 `restore_node_factory.*`、`restore_edge_linker.*`。
- `api_restore_*` 只保留流程编排。

3. 统一资源管理

- 用 RAII 包装 `mg_map* / mg_list* / mg_value*` 生命周期，降低泄漏风险。
- 把裸指针临时对象替换为更明确的所有权模式。

4. 为 codegen 做数据驱动准备

- 定义 schema：节点类型、属性键、关系类型、字段解析规则。
- 先生成枚举与映射，再逐步生成 case 体。

## 不变性保证

本阶段没有修改任何对外 API 签名；业务执行路径仍调用原有 `access.cpp` 实现，逻辑语义保持一致。
