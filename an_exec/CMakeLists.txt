add_executable(an_exec)


target_include_directories(an_exec PRIVATE include)
target_sources(an_exec PRIVATE
    src/main.cpp
)


target_link_libraries(an_exec PRIVATE
    a_library
    fmt::fmt
)
