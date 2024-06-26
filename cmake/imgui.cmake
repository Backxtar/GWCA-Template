include_guard()
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 9aae45eb4a05a5a1f96be1ef37eb503a12ceb889)

FetchContent_GetProperties(imgui)

if (imgui_POPULATED)
    return()
endif()

FetchContent_Populate(imgui)
add_library(imgui)

set(SOURCES
    "${imgui_SOURCE_DIR}/imgui.h"
    "${imgui_SOURCE_DIR}/imgui_internal.h"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp")

source_group(TREE "${imgui_SOURCE_DIR}" FILES ${SOURCES})
target_sources(imgui PRIVATE ${SOURCES})

target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
# target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/src/imconfig.h")