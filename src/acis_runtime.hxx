#pragma once

#include "access.hxx"

namespace dbcad::acis_runtime
{
    bool Initialize();
    void Terminate();
    void GetNoAttribTopLevelActiveEntities(ENTITY_LIST &entityList);
}
