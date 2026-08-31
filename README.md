# DBCAD

> **基于 CAD 内核的数据库存储方案原型。**
> 把 CAD 几何对象（ACIS / OpenCascade）映射到图数据库（Neo4j）里，
> 用图查询替代传统 B-rep 遍历，做几何拓扑分析。

## 项目背景

这是我在清华大学软件学院跟随**刘英博老师**做的科研项目的一部分。
核心动机：传统 CAD 系统的几何数据用 ACIS / OCCT 私有格式存储，
要查询"两个零件之间有没有共用面"、"哪些边是孔的边界"这类问题，
只能遍历 B-rep 树，复杂度高、不利于分布式。

DBCAD 的思路是：

- 把每个几何实体（Vertex / Edge / Face / Body）当作图节点
- 把拓扑关系（相邻 / 父-子 / 共用）当作图的边
- 用 Neo4j / Cypher 做几何拓扑查询

## 仓库结构

```
DBCAD/
├─ CMakeLists.txt             # 顶层 CMake（用 vcpkg 管依赖）
├─ docs/                      # 设计文档
├─ src/
│  ├─ Neo4jAdapter/           # Neo4j 客户端封装
│  ├─ ACISBridge/             # ACIS → 图节点的转换器
│  ├─ OCCBridge/              # OpenCascade → 图节点的转换器
│  ├─ Schema/                 # 图 schema 定义
│  └─ Query/                  # 高层 Cypher 查询（共用面、邻接等）
├─ static/                    # 前端静态资源（演示页面）
├─ dynamic/                   # 运行时生成的中间产物
└─ test/                      # 单元测试与集成测试
```

## 技术栈

- **语言**：C++17
- **CAD 内核**：ACIS（RVM/SAT 格式）+ OpenCascade（STEP/IGES 格式）
- **图数据库**：Neo4j（C++ 驱动 `neo4j-cpp`）
- **构建**：CMake + vcpkg
- **前端**：TypeScript + Vue（演示查询结果可视化）

## 关键模块

| 模块 | 作用 |
|---|---|
| `Neo4jAdapter` | Neo4j 连接的 RAII 封装，Cypher 异步执行 |
| `ACISBridge` | 把 ACIS 的 `ENTITY` 树映射到图节点（每个 entity 一个 node） |
| `OCCBridge` | 把 OCCT 的 `TopoDS_Shape` 映射到图节点 |
| `Schema` | 节点标签（`Vertex` / `Edge` / `Face` / `Body`）和关系类型定义 |
| `Query` | 高级查询：共用面检测、孔特征识别、装配树遍历 |

## 已完成

- ACIS → Neo4j 转换器（支持 SAT 文件批量导入）
- OCCT → Neo4j 转换器（支持 STEP 文件）
- 基础查询：邻接关系、子结构、共用面
- 一个简单的 Web 演示页面，能用 Cypher 查询并可视化

## 进行中

- 性能优化：目前大模型（>100k 实体）导入太慢
- 增量更新：B-rep 修改后只更新变更的节点
- 反向：从图数据库重建 ACIS / OCCT 实体

## 本地构建

```bash
# 需 vcpkg
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## 论文 / 报告

- 项目组内中期报告（2025）
- 计划投的会议：CAD/CG 方向

## License

项目代码 MIT；ACIS / OCCT 各自遵守其原始 license。
