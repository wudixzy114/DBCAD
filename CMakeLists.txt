cmake_minimum_required(VERSION 4.1)
project(DBCAD255)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

set(ACIS_RELEASE_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/acis")
set(ACIS_DEBUG_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/acis")
set(ACIS_COMMON_INCLUDE "${CMAKE_SOURCE_DIR}/thirdparty/acis/include")

include(FetchContent)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v3.0.2 # 可以指定最新稳定版
)
FetchContent_MakeAvailable(pybind11)
set(BINDING_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/DbcadAPI.cpp"
)

set(CORE_SOURCES
        src/acis.cpp
        src/acis_runtime.cpp
        src/access.cpp
        src/access_restore_dispatch.cpp
        src/access_switch_registry.cpp
        src/common.cpp
        src/gme_dump_object.cpp
        src/gme_mesh.cpp
        src/graph_storage.cpp
        src/neo4j.cpp
        src/DbcadAPI.cpp
)
set(CORE_HEADERS
        src/acis.hxx
        src/acis_runtime.hxx
        src/access.hxx
        src/access_restore_dispatch.hxx
        src/access_switch_registry.hxx
        src/common.hxx
        src/gme_dump_object.hxx
        src/gme_mesh.hxx
        src/graph_storage.hxx
        src/neo4j.hxx
        # 注意：DbcadAPI.h 如果包含 pybind 代码，不要放这里；
        # 如果只是纯 C++ 接口声明，可以放这里。暂时先放这里方便 IDE 查看。
        src/DbcadAPI.h
        src/main.cpp
)
set(BINDING_SOURCES
        src/bindings.cpp
)

add_library(project_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_compile_definitions(project_core PUBLIC PROJECT_CORE_STATIC)
#target_compile_definitions(project_core PRIVATE DBCAD_EXPORTS)
if(MSVC)
        target_compile_options(project_core PUBLIC
                /utf-8
                /Zc:__cplusplus
                /Zc:wchar_t # VS: /Zc:wchar_t
                /Zc:inline # VS: /Zc:inline
                /MP # 多核编译
        )
        target_compile_definitions(project_core PUBLIC
                _CRT_SECURE_NO_WARNINGS
                WIN32_LEAN_AND_MEAN
                UNICODE _UNICODE # Windows 现代开发通常默认开启
        )
        target_compile_definitions(project_core PUBLIC

                # ---- ACIS 文档强制要求的预处理器宏 ----
                _WINDOWS # MS specific
                _WINDOWS_SOURCE # MS specific
                NT_VC16_64 # 定义基础架构为 VS2019 x64
                SPA_NO_AUTO_LINK # 禁用 ACIS 库自动链接（因为我们在下面手动链接了）
                _WIN32_WINNT=0x0600 # 定义主要操作系统版本

                # Debug 专属宏
                $<$<CONFIG:Debug>:SPA_DEBUG>
                $<$<CONFIG:Debug>:NT_VC16_64_DLLD>

                # Release 专属宏
                $<$<CONFIG:Release>:NDEBUG>
                $<$<CONFIG:Release>:NT_VC16_64_DLL>
        )
        target_link_options(project_core INTERFACE
                /STACK:4194304
                /MACHINE:AMD64
        )
endif()
target_link_libraries(project_core PUBLIC
        opengl32.lib
        ws2_32.lib

        # --- Debug 专用库 --- 使用绝对路径防止 CMake 误加后缀
        $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/thirdparty/acis/lib/SpaACISd.lib>
        $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/thirdparty/openssl/lib/libssl64MDd.lib>
        $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/thirdparty/openssl/lib/libcrypto64MDd.lib>

        # --- Release 专用库 --- 使用绝对路径
        $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/thirdparty/acis/lib/SpaACIS.lib>
        $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/thirdparty/openssl/lib/libssl64MD.lib>
        $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/thirdparty/openssl/lib/libcrypto64MD.lib>
)

target_include_directories(project_core SYSTEM PUBLIC
        src
        ${CMAKE_SOURCE_DIR}/thirdparty
        ${ACIS_COMMON_INCLUDE}
        ${CMAKE_SOURCE_DIR}/thirdparty/glazeA/include
)

set(ACIS_BIN_DIR "$<$<CONFIG:Debug>:${ACIS_DEBUG_ROOT}/bin>$<$<CONFIG:Release>:${ACIS_RELEASE_ROOT}/bin>")
add_custom_command(TARGET project_core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ACIS_BIN_DIR}"
        "$<TARGET_FILE_DIR:project_core>"
        COMMENT "Copying ACIS DLLs..."
)

message(STATUS "================ ACIS Build Configuration ================")
message(STATUS "[Platform] Pointer Size: ${CMAKE_SIZEOF_VOID_P} Bytes (8 = 64-bit)")
message(STATUS "[Platform] System Name:  ${CMAKE_SYSTEM_NAME}")
message(STATUS "[Config]   Build Type:    ${CMAKE_BUILD_TYPE}")
get_target_property(TEMP_MSVC_RUNTIME project_core MSVC_RUNTIME_LIBRARY)
message(STATUS "[Runtime]  MSVC Runtime:  ${TEMP_MSVC_RUNTIME}")
message(STATUS "[Flags]    Global CXX Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "[Flags]    Debug CXX Flags:  ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "[Flags]    Release CXX Flags: ${CMAKE_CXX_FLAGS_RELEASE}")
get_target_property(TEMP_CORE_OPTIONS project_core COMPILE_OPTIONS)
message(STATUS "[Flags]    project_core Specific Options: ${TEMP_CORE_OPTIONS}")
get_target_property(TEMP_CORE_DEFS project_core COMPILE_DEFINITIONS)
message(STATUS "[Macros]   project_core Definitions: ${TEMP_CORE_DEFS}")
message(STATUS "==========================================================")

add_executable(DbcadTest src/main.cpp)
target_link_libraries(DbcadTest PRIVATE project_core)
set(ACIS_BIN_DIR "$<$<CONFIG:Debug>:${ACIS_DEBUG_ROOT}/bin>$<$<CONFIG:Release>:${ACIS_RELEASE_ROOT}/bin>")
add_custom_command(TARGET DbcadTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ACIS_BIN_DIR}"
        "$<TARGET_FILE_DIR:DbcadTest>"
        COMMENT "Copying ACIS DLLs to Executable Directory..."
)

# 使用 pybind11 提供的函数创建模块
pybind11_add_module(PyDbcad ${BINDING_SOURCES})

# 关键：让 Python 模块链接到你的核心库和 ACIS 等
target_link_libraries(PyDbcad PRIVATE project_core)

message(STATUS "Core Sources: ${CORE_SOURCES}")
message(STATUS "Binding Sources: ${BINDING_SOURCES}")
