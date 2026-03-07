#include <iostream>
#include <string>
#include <stdexcept>

// Dbcad 核心接口
#include "DbcadAPI.h"

// ACIS 核心头文件
#include "lists.hxx"
#include "kernapi.hxx"
#include "cstrapi.hxx"

// 引入实体类型的头文件（为了使用 is_BODY() 等判断宏）
#include "body.hxx"

int main()
{
    std::cout << "=== Starting DBCAD C++ Native Debug Session ===" << std::endl;

    if (!dbcad::InitializeEnvironment())
    {
        std::cerr << "ERROR: Failed to initialize ACIS environment!" << std::endl;
        return 1;
    }

    try
    {
        dbcad::DbConfig cfg;
        cfg.host = "127.0.0.1";
        cfg.port = 7687;
        cfg.username = "neo4j";
        cfg.password = "password";

        std::string part_name = "TestSphere_CppNative";
        dbcad::ModelerSession session(cfg, part_name);

        std::cout << "\n[1] Creating test SPHERE (Radius: 50.0)..." << std::endl;
        class BODY* sphere = nullptr;
        API_BEGIN;
            api_solid_sphere(SPAposition(0, 0, 0), 50.0, sphere);
        API_END;

        if (!result.ok())
        {
            throw std::runtime_error("ACIS API failed to create sphere!");
        }

        // 1. 获取所有活跃实体（里面混入了历史流等垃圾）
        ENTITY_LIST active_list;
        session.GetActiveEntities(active_list);
        std::cout << "  -> Active entities (raw) count: " << active_list.iteration_count() << std::endl;

        // 2. [关键修复]：创建一个只包含“干净”拓扑实体的列表
        ENTITY_LIST clean_list;
        for (class ENTITY* ent = active_list.first(); ent != nullptr; ent = active_list.next())
        {
            // 只保留顶层几何实体（通常是 BODY，如果是零散的面或边也可以加上）
            if (is_BODY(ent))
            {
                clean_list.add(ent);
            }
            // 如果你也允许单独画的面、边作为顶层实体存在，可以解开下面的注释：
            // else if (is_FACE(ent) || is_EDGE(ent) || is_VERTEX(ent) || is_WIRE(ent)) {
            //     clean_list.add(ent);
            // }
        }
        std::cout << "  -> Filtered CLEAN entities count: " << clean_list.iteration_count() << std::endl;

        // 3. 用干净的列表去保存
        std::cout << "\n[2] Performing Full Save to Database..." << std::endl;
        session.SavePartFull(clean_list);
        std::cout << "  -> Success! Sphere saved to graph database without crashing." << std::endl;

        // 4. 全量读取
        std::cout << "\n[3] Performing Full Load from Database..." << std::endl;
        ENTITY_LIST loaded_list;
        session.LoadPartFull(loaded_list);
        std::cout << "  -> Success! Loaded " << loaded_list.iteration_count() << " top-level entities." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[EXCEPTION CAUGHT]: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "\n[UNKNOWN EXCEPTION CAUGHT]" << std::endl;
    }

    dbcad::TerminateEnvironment();
    std::cout << "\n=== Debug Session Finished ===" << std::endl;
    return 0;
}
