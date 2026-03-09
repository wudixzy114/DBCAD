#include "acis_runtime.hxx"

#include "acis.hxx"

namespace dbcad::acis_runtime
{
    bool Initialize()
    {
        return initialize_acis() != 0;
    }

    void Terminate()
    {
        terminate_acis(2);
    }

    void GetNoAttribTopLevelActiveEntities(ENTITY_LIST &entityList)
    {
        acis_get_noattrib_toplevel_active_entities(entityList);
    }
}
