cmake_minimum_required(VERSION 3.21)
project(cpp_project_template)


# add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.)


set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(PROJECT_DEBUG_FLAGS

)


set(PROJECT_RELEASE_FLAGS

)


set(PROJECT_DEBUG_DEFINES
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)


set(PROJECT_RELEASE_DEFINES
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO
)


add_compile_options(
    $<$<CONFIG:Debug>:${PROJECT_DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${PROJECT_RELEASE_FLAGS}>
)

add_compile_definitions(
    $<$<CONFIG:Debug>:${PROJECT_DEBUG_DEFINES}>
    $<$<CONFIG:Release>:${PROJECT_RELEASE_DEFINES}>
)



find_package(fmt)
find_package(Boost)
find_package(frozen)
find_package(spdlog)


add_subdirectory(a_library)
add_subdirectory(an_exec)



file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
configure_file(
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    COPYONLY
)
