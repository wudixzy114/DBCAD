#pragma once

// 针对 Windows 平台的导出宏设置
#if defined(_WIN32) || defined(_WIN64)
#ifdef DBCAD_EXPORTS // 如果定义了这个宏，说明我们正在"编译"这个库
#define DBCAD_API __declspec(dllexport)
#else                // 如果没有定义，说明是第三方正在"使用"这个库
#define DBCAD_API __declspec(dllimport)
#endif
#else
// Linux/Mac 环境下设为空
#define DBCAD_API
#endif

#include <string>
#include <memory>

// ACIS 前置声明
class ENTITY_LIST;

// 引入 Mesh 数据结构
#include "gme_mesh.hxx"


namespace dbcad
{
    // 数据库连接配置
    struct DbConfig
    {
        std::string host;
        int port;
        std::string username;
        std::string password;
    };

    // 系统初始化与清理（对应 initialize_acis 和 terminate_acis）
    DBCAD_API bool InitializeEnvironment();
    DBCAD_API void TerminateEnvironment();

    // 核心管理器类
    class DBCAD_API ModelerSession
    {
    public:
        ModelerSession(const DbConfig& config, const std::string& partName);
        ~ModelerSession();

        // ------------------ ACIS 原生功能 ------------------
        // 获取当前活跃的顶级实体
        void GetActiveEntities(ENTITY_LIST& outList);
        // 执行布尔运算、生成基本体等操作的封装可根据需要后续补充

        // ------------------ 全量存取 (Neo4j) ------------------
        void SavePartFull(const ENTITY_LIST& entityList) const;
        void LoadPartFull(ENTITY_LIST& outEntityList) const;
        int64_t CheckPartExists() const; // 返回图节点数量，用于校验

        // ------------------ 增量存取 (Neo4j) ------------------
        void SavePartIncremental() const;
        void LoadPartIncremental(int generation, ENTITY_LIST& outEntityList) const;

        // ------------------ 渲染/显示支持 ------------------
        // 生成渲染用的网格数据
        bool GenerateMesh(ENTITY_LIST& el, GmeMesh::DisplayData& dd);

    private:
        struct Impl; // PIMPL 模式，隐藏 mgclient 和 ACIS 头文件依赖
        std::unique_ptr<Impl> pImpl;
    };
} // namespace dbcad
