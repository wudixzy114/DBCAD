#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // 自动转换 std::vector 到 Python list
#include "DbcadAPI.h"
#include "gme_mesh.hxx"
#include "lists.hxx"

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
        .def(py::init<>());

    // 4. 导出 ModelerSession 类
    py::class_<dbcad::ModelerSession>(m, "ModelerSession")
        .def(py::init<const dbcad::DbConfig&, const std::string&>())

        // 增量加载
        .def("load_incremental", [](const dbcad::ModelerSession& self, int generation)
        {
            auto* list = new ENTITY_LIST(); // 在堆上创建，交由 Python 管理生命周期
            self.LoadPartIncremental(generation, *list);
            return list;
        }, py::return_value_policy::take_ownership)

        // 全量加载
        .def("load_full", [](const dbcad::ModelerSession& self)
        {
            auto* list = new ENTITY_LIST();
            self.LoadPartFull(*list);
            return list;
        }, py::return_value_policy::take_ownership)

        // 保存
        .def("save_incremental", &dbcad::ModelerSession::SavePartIncremental)

        // 获取网格数据 (这是关键，直接返回 Python 字典)
        .def("get_mesh_data", [](dbcad::ModelerSession& self, ENTITY_LIST& el)
        {
            GmeMesh::DisplayData dd;
            bool success = self.GenerateMesh(el, dd);
            if (!success) throw std::runtime_error("Failed to generate mesh");
            return MeshDataToDict(dd);
        });
}

