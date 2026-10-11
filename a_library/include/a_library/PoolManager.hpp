#pragma once

#include <spdlog/spdlog.h>

#include <cassert>
#include <concepts>
#include <cstdint>
#include <deque>
#include <mutex>
#include <optional>
#include <a_library/IDManager.hpp>
#include <a_library/essentials.hpp>
#include <typeindex>
#include <utility>

namespace a_library {
template <typename T>
class PoolManager {
   public:
    std::optional<T> pop(uint32_t idx);
    T& get_ref(uint32_t idx);
    T* get_ptr(uint32_t idx);

    template <typename... Args>
        requires(std::constructible_from<T, Args...>)
    uint32_t emplace(Args&&... args);

   private:
    static constexpr uint32_t C_INIT_IDX = 1;

    std::deque<std::optional<T>> m_entries{C_INIT_IDX};
    IDManager m_id_man{C_INIT_IDX};
    std::mutex m_mutex;
};
}  // namespace a_library

namespace a_library {
template <typename T>
std::optional<T> PoolManager<T>::pop(uint32_t idx) {
    std::lock_guard lk(m_mutex);
    if (idx >= m_entries.size()) return std::nullopt;

    std::optional<T>& pcell = m_entries[idx];
    if (!pcell.has_value()) return std::nullopt;

    std::optional<T> pval(std::move(pcell));
    pcell.reset();
    m_id_man.release(idx);
    return pval;
}

template <typename T>
T* PoolManager<T>::get_ptr(uint32_t idx) {
    if (idx >= m_entries.size()) return nullptr;
    std::optional<T>& qcell = m_entries[idx];
    return qcell.has_value() ? &(*qcell) : nullptr;
}

template <typename T>
T& PoolManager<T>::get_ref(uint32_t idx) {
    T* ptr = get_ptr(idx);
    assert(ptr);
    return *ptr;
}

template <typename T>
template <typename... Args>
    requires(std::constructible_from<T, Args...>)
uint32_t PoolManager<T>::emplace(Args&&... args) {
    std::lock_guard lk(m_mutex);
    uint32_t f_idx(m_id_man.allocate());
    if (f_idx == m_entries.size()) m_entries.resize(f_idx + 1);
    m_entries[f_idx].emplace(std::forward<Args>(args)...);

    SPDLOG_DEBUG("allocated instance of {} -> {}", std::type_index(typeid(T)).name(), f_idx);
    return f_idx;
}
}  // namespace a_library
