add_library(a_library)


target_include_directories(a_library PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


target_sources(a_library PRIVATE
    src/ByteBuffer.cpp
    src/graphics/Window.cpp
    src/graphics/vertex.cpp
    src/IDManager.cpp
    src/init.cpp
)


target_link_libraries(a_library PUBLIC
    fmt::fmt
    Boost::boost
    Boost::stacktrace
    spdlog::spdlog
    frozen::frozen
)
