#pragma once

#include <string>

class ENTITY_LIST;
class HISTORY_STREAM;


namespace dbcad::access
{
    struct DbConnectionInfo
    {
        std::string host;
        int port;
        std::string username;
        std::string password;
    };

    bool InitializeEnvironment();
    void TerminateEnvironment();

    class DbManager
    {
    public:
        explicit DbManager(const DbConnectionInfo& connInfo);
        ~DbManager();

        // 将实体列表全量保存到数据库
        void SavePartFull(const std::string& partName, const ENTITY_LIST& entityList);

        // 从数据库全量加载实体列表
        void LoadPartFull(const std::string& partName, ENTITY_LIST& outEntityList);

        // 获取零件在数据库中存在的节点数量（用于查重或存在性校验）
        int64_t GetPartNodeCount(const std::string& partName);

        // 将当前历史流的状态增量保存到数据库
        void SavePartIncremental(const std::string& partName);

        // 从数据库按版次(generation)增量加载
        void LoadPartIncremental(const std::string& partName, int generation, ENTITY_LIST& outEntityList);

        // 清理当前内存中的增量状态缓存（针对 access.cpp 中的全局变量 ptr2nodeid 等）
        void ClearIncrementalState();

    private:
        DbConnectionInfo m_connInfo;
    };
}

