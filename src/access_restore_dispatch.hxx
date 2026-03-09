#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace AccessUtils::Restore::Dispatch
{
    void ApplyNeo4jEdge(
        const std::string& relTypename,
        int64_t relStartNodeId,
        int64_t relEndNodeId,
        std::unordered_map<int64_t, void*>& id2ptr);
}
