#include "DbcadAPI.h"

#include <stdexcept>
#include <utility>

#include "acis_runtime.hxx"
#include "graph_storage.hxx"
#include "neo4j.hxx"
#include "gme_mesh.hxx"
#include "access.hxx"
extern bool CreateMeshFromEntityList(ENTITY_LIST &el, GmeMesh::DisplayData &dd);

namespace dbcad
{
    bool InitializeEnvironment()
    {
        if (!acis_runtime::Initialize())
            return false;
        mg_init(); // 初始化 Memgraph/Neo4j 客户端
        return true;
    }

    void TerminateEnvironment()
    {
        mg_finalize();
        acis_runtime::Terminate();
    }

    struct ModelerSession::Impl
    {
        DbConfig config;
        std::string partName;
        IncrementalContext incCtx; // 每个 Session 有独立的增量上下文

        Impl(DbConfig c, std::string p) : config(std::move(c)), partName(std::move(p))
        {
        }

        std::unique_ptr<Neo4jPart> CreateConnection()
        {
            return std::make_unique<Neo4jPart>(
                config.host.c_str(),
                config.port,
                config.username.c_str(),
                config.password.c_str(),
                partName);
        }
    };

    ModelerSession::ModelerSession(const DbConfig &config, const std::string &partName)
        : pImpl(std::make_unique<Impl>(config, partName))
    {
    }

    ModelerSession::~ModelerSession() = default;

    void ModelerSession::GetActiveEntities(ENTITY_LIST &outList)
    {
        acis_runtime::GetNoAttribTopLevelActiveEntities(outList);
    }

    void ModelerSession::SavePartFull(const ENTITY_LIST &entityList) const
    {
        auto conn = pImpl->CreateConnection();
        storage::GraphStorage graph(*conn);
        graph.SavePartFull(entityList);
    }

    void ModelerSession::LoadPartFull(ENTITY_LIST &outEntityList) const
    {
        auto conn = pImpl->CreateConnection();
        storage::GraphStorage graph(*conn);
        int64_t count = graph.CountPartNode();
        if (count == 0)
            throw std::runtime_error("Part does not exist.");
        if (count > 1)
            throw std::runtime_error("Part is not unique in DB.");

        graph.LoadPartFull(outEntityList);
    }

    int64_t ModelerSession::CheckPartExists() const
    {
        auto conn = pImpl->CreateConnection();
        storage::GraphStorage graph(*conn);
        return graph.CountPartNode();
    }

    void ModelerSession::SavePartIncremental() const
    {
        auto conn = pImpl->CreateConnection();
        storage::GraphStorage graph(*conn);
        graph.SavePartIncremental(pImpl->incCtx);
    }

    void ModelerSession::LoadPartIncremental(int generation, ENTITY_LIST &outEntityList) const
    {
        auto conn = pImpl->CreateConnection();
        storage::GraphStorage graph(*conn);
        int64_t count = graph.CountPartNode();
        if (count == 0)
            throw std::runtime_error("Part does not exist.");

        graph.LoadPartIncremental(generation, pImpl->incCtx);
        acis_runtime::GetNoAttribTopLevelActiveEntities(outEntityList);
    }

    bool ModelerSession::GenerateMesh(ENTITY_LIST &el, GmeMesh::DisplayData &dd)
    {
        return CreateMeshFromEntityList(el, dd);
    }
}
