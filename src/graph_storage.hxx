#pragma once

#include "access.hxx"
#include "neo4j.hxx"

namespace dbcad::storage
{
    class GraphStorage
    {
    public:
        explicit GraphStorage(const Neo4jPart &conn);

        void SavePartFull(const ENTITY_LIST &entityList) const;
        void LoadPartFull(ENTITY_LIST &entityList) const;
        int64_t CountPartNode() const;

        void SavePartIncremental(IncrementalContext &ctx) const;
        void LoadPartIncremental(int generation, IncrementalContext &ctx) const;

    private:
        const Neo4jPart &conn_;
    };
}
