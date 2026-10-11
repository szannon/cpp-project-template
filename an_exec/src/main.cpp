#include <SDL_events.h>
#include <fmt/base.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <Game.hpp>
#include <a_library/ByteBuffer.hpp>
#include <a_library/essentials.hpp>
#include <a_library/graphics/opengl/GLWindow.hpp>
#include <a_library/graphics/vertex.hpp>
#include <a_library/init.hpp>
#include <a_library/linealg/VecN.hpp>

void blaa(char, int) {};

int main() {
    a_library::ensure_sx_init();
    a_library::ByteBuffer atest(float(73), uint64_t(73), size_t(73));
    SPDLOG_DEBUG(
        "testing 123: {}\nsizeof float+uint64_t: {}\nsizeof float+uint64_t+size_t: {}",
        a_library::is_serializable<a_library::ByteBuffer>(),
        a_library::calc_size(float(1), uint64_t(69)),
        atest.size()
    );

    atest.append(float(1), uint64_t(69));
    SPDLOG_DEBUG(
        "testing 123: {}\nsizeof float+uint64_t: {}\nsizeof 2float+2uint64_t+size_t: {}",
        a_library::is_serializable<a_library::ByteBuffer>(),
        a_library::calc_size(float(1), uint64_t(69)),
        atest.size()
    );

    a_library::linealg::Vec2<int> a(1, 2), b(4, 8);
    a = (a + b) * 10;
    SPDLOG_DEBUG("({}, {})", a.x, a.y);

    a_library::graphics::test_vertex();

    SPDLOG_DEBUG(
        "testing 123, test {}",
        a_library::SignatureOf<decltype(blaa), void(char, int)> ? "works ^_^" : "failed >_<!"
    );

    SPDLOG_ERROR("testi-test ^_^");

    return 0;
}
