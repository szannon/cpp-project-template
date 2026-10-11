#pragma once
#include <cstdint>
#include <mutex>
#include <vector>

namespace a_library {
struct IDManager {
    uint32_t allocate();
    void release(uint32_t id);
    IDManager(uint32_t start = 1);

   private:
    uint32_t m_next;
    std::mutex m_mut;
    std::vector<uint32_t> m_free_ids;
};
}  // namespace a_library
