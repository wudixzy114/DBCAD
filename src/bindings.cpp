#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // 自动转换 std::vector 到 Python list
#include "DbcadAPI.h"
#include "gme_mesh.hxx"
#include "lists.hxx"

#include "acis/include/kernapi.hxx"
#include "acis/include/cstrapi.hxx"

namespace py = pybind11;

py::dict MeshDataToDict(const GmeMesh::DisplayData& dd)
{
    py::dict res;
    res["face_coords"] = dd.faceCoords; // 自动转为 list[float]
    res["triangles"] = dd.triangles; // 自动转为 list[int]
    res["edge_coords"] = dd.edgeCoords;
    res["vertex_coords"] = dd.vertexCoords;
    // 简化处理，暂不转换复杂的结构体数组，仅返回核心坐标用于前端渲染
    return res;
}

PYBIND11_MODULE(PyDbcad, m)
{
    m.doc() = "DbcadCore Python Bindings using pybind11";
    py::class_<dbcad::DbConfig>(m, "DbConfig")
        .def(py::init<>())
        .def_readwrite("host", &dbcad::DbConfig::host)
        .def_readwrite("port", &dbcad::DbConfig::port)
        .def_readwrite("username", &dbcad::DbConfig::username)
        .def_readwrite("password", &dbcad::DbConfig::password);

    m.def("initialize_env", &dbcad::InitializeEnvironment, "Initialize ACIS and MgClient");
    m.def("terminate_env", &dbcad::TerminateEnvironment, "Terminate ACIS and MgClient");

    py::class_<ENTITY_LIST>(m, "EntityList")
        .def(py::init<>())
        .def("count", &ENTITY_LIST::iteration_count, "Get the number of entities in the list");

    // 4. 导出 ModelerSession 类
    py::class_<dbcad::ModelerSession>(m, "ModelerSession")
        .def(py::init<const dbcad::DbConfig&, const std::string&>())

        .def("get_active_entities", [](dbcad::ModelerSession& self)
        {
            auto* list = new ENTITY_LIST();
            self.GetActiveEntities(*list);
            return list;
        }, py::return_value_policy::take_ownership)

        .def("check_exists", &dbcad::ModelerSession::CheckPartExists)

        // 全量保存与读取
        .def("save_full", [](dbcad::ModelerSession& self, ENTITY_LIST& el)
        {
            self.SavePartFull(el);
        })
        .def("load_full", [](dbcad::ModelerSession& self)
        {
            auto* list = new ENTITY_LIST();
            self.LoadPartFull(*list);
            return list;
        }, py::return_value_policy::take_ownership)

        // 增量保存与读取
        .def("save_incremental", &dbcad::ModelerSession::SavePartIncremental)
        .def("load_incremental", [](dbcad::ModelerSession& self, int gen)
        {
            auto* list = new ENTITY_LIST();
            self.LoadPartIncremental(gen, *list);
            return list;
        }, py::return_value_policy::take_ownership)

        // 提取网格渲染数据
        .def("get_mesh_data", [](dbcad::ModelerSession& self, ENTITY_LIST& el)
        {
            GmeMesh::DisplayData dd;
            if (!self.GenerateMesh(el, dd))
            {
                throw std::runtime_error("Failed to generate mesh. Entity list might be empty or invalid.");
            }
            return MeshDataToDict(dd);
        })

        // 测试专用：在当前历史流中创建一个立方体
        .def("create_test_block", [](dbcad::ModelerSession& self, double w, double d, double h)
        {
            class BODY* block = nullptr;
            API_BEGIN;
                // 调用 ACIS API 创建长方体
                api_solid_block(SPAposition(0, 0, 0), SPAposition(w, d, h), block);
            API_END;
            if (!result.ok()) throw std::runtime_error("Failed to create ACIS test block");
        })

        .def("create_test_sphere", [](dbcad::ModelerSession& self, double radius)
        {
            class BODY* sphere = nullptr;
            API_BEGIN;
                // 调用 ACIS API 创建球体
                api_solid_sphere(SPAposition(0, 0, 0), radius, sphere);
            API_END;
            if (!result.ok()) throw std::runtime_error("Failed to create ACIS test sphere");
        })

        // --- 原有的获取所有实体的函数 (保留，用于特殊用途) ---
        .def("get_all_entities", [](dbcad::ModelerSession& self)
        {
            auto* list = new ENTITY_LIST();
            self.GetActiveEntities(*list);
            return list;
        }, py::return_value_policy::take_ownership)

        // --- [关键新增]：只获取顶层 BODY 实体的函数 ---
        .def("get_active_bodies", [](dbcad::ModelerSession& self)
        {
            ENTITY_LIST raw_list;
            self.GetActiveEntities(raw_list);

            auto* clean_list = new ENTITY_LIST();
            for (class ENTITY* ent = raw_list.first(); ent != nullptr; ent = raw_list.next())
            {
                // 只保留顶层 Body，防止历史流、属性等杂质进入保存流程
                if (ent->identity(0) == BODY_TYPE)
                {
                    // 或者使用 is_BODY(ent)
                    clean_list->add(ent);
                }
            }
            return clean_list;
        }, py::return_value_policy::take_ownership);
}
