cmake_minimum_required(VERSION 4.1)
project(DBCAD255)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

set(ACIS_RELEASE_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/acis/win_b64/code")
set(ACIS_DEBUG_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/acis/win_b64_debug/code")
set(ACIS_COMMON_INCLUDE "${CMAKE_SOURCE_DIR}/thirdparty/acis/include")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_library(project_core STATIC ${SOURCES} ${HEADERS})
if (MSVC)
    target_compile_options(project_core PUBLIC
            /utf-8
            /EHa
            /Zc:__cplusplus
    )
    target_compile_definitions(project_core PUBLIC
            _CRT_SECURE_NO_WARNINGS
            _WINDOWS
            _WINDOWS_SOURCE
            _CAT_ANSI_STREAMS
            CAT_ENABLE_NATIVE_EXCEPTION
            SPAACISDS
            PLATEFORME_DS64
            $<$<CONFIG:Debug>:SPA_DEBUG>
    )
    target_link_options(project_core INTERFACE
            /STACK:4194304
            /MACHINE:x64
    )
endif ()
target_link_libraries(project_core PUBLIC
        $<$<CONFIG:Debug>:${ACIS_DEBUG_ROOT}/lib/SPAAcisDs.lib>
        $<$<CONFIG:Release>:${ACIS_RELEASE_ROOT}/lib/SPAAcisDs.lib>
)
target_link_directories(project_core
        PUBLIC
        $<$<CONFIG:Debug>:${ACIS_DEBUG_ROOT}/lib>
        $<$<CONFIG:Release>:${ACIS_RELEASE_ROOT}/lib>
)
target_include_directories(project_core PUBLIC
        src
        SYSTEM ${CMAKE_SOURCE_DIR}/thirdparty
        SYSTEM ${ACIS_COMMON_INCLUDE}
        SYSTEM ${CMAKE_SOURCE_DIR}/thirdparty/glazeA/include
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

