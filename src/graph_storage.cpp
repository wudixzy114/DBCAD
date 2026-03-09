#include "graph_storage.hxx"

namespace dbcad::storage
{
    GraphStorage::GraphStorage(const Neo4jPart &conn) : conn_(conn)
    {
    }

    void GraphStorage::SavePartFull(const ENTITY_LIST &entityList) const
    {
        api_save_entity_list_neo4j_part(conn_, entityList);
    }

    void GraphStorage::LoadPartFull(ENTITY_LIST &entityList) const
    {
        api_restore_entity_list_neo4j_part(conn_, entityList);
    }

    int64_t GraphStorage::CountPartNode() const
    {
        return count_partnode(conn_);
    }

    void GraphStorage::SavePartIncremental(IncrementalContext &ctx) const
    {
        api_save_neo4j(conn_, ctx);
    }

    void GraphStorage::LoadPartIncremental(int generation, IncrementalContext &ctx) const
    {
        api_restore_neo4j(conn_, generation, ctx);
    }
}
