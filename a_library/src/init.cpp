#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <mutex>
#include <a_library/init.hpp>

namespace a_library {
void ensure_sx_init() {
    static bool v_initialized(false);
    static std::mutex v_mut;

    if (v_initialized) return;
    std::lock_guard lk(v_mut);
    if (v_initialized) return;

    spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
    stbi_set_flip_vertically_on_load(false);

    v_initialized = true;
}
}  // namespace a_library
