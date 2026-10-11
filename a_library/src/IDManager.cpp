#include <cassert>
#include <cstdint>
#include <mutex>
#include <a_library/IDManager.hpp>

namespace a_library {
IDManager::IDManager(uint32_t start)
    : m_next(start) {}

uint32_t IDManager::allocate() {
    std::lock_guard lk(m_mut);
    if (m_free_ids.empty()) return m_next++;

    uint32_t recycled(m_free_ids.back());
    m_free_ids.pop_back();
    return recycled;
}

void IDManager::release(uint32_t id) {
    std::lock_guard lk(m_mut);
    assert(id < m_next);
    m_free_ids.push_back(id);
}
}  // namespace a_library
