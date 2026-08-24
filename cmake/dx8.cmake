FetchContent_Declare(
    dx8
    GIT_REPOSITORY https://github.com/TheSuperHackers/min-dx8-sdk.git
    GIT_TAG        7bddff8c01f5fb931c3cb73d4aa8e66d303d97bc
)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    FetchContent_MakeAvailable(dx8)
else()
    FetchContent_GetProperties(dx8)
    if(NOT dx8_POPULATED)
        FetchContent_Populate(dx8)
    endif()
    add_library(d3d8lib INTERFACE)
    target_include_directories(d3d8lib INTERFACE ${dx8_SOURCE_DIR})
    target_compile_definitions(d3d8lib INTERFACE -DBUILD_WITH_D3D8)
    if(MSVC)
        target_link_libraries(d3d8lib INTERFACE legacy_stdio_definitions)
    endif()
    set(D3D8TO9_STATIC ON CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/d3d8to9 ${CMAKE_BINARY_DIR}/d3d8to9)
    target_link_libraries(d3d8lib INTERFACE d3d8to9 d3d9 dinput8 dxguid)
    add_library(d3dx8shim STATIC ${CMAKE_SOURCE_DIR}/Dependencies/dx8-x64/d3dx8_shim.cpp)
    target_include_directories(d3dx8shim PRIVATE ${dx8_SOURCE_DIR})
    target_link_libraries(d3d8lib INTERFACE d3dx8shim)
endif()
