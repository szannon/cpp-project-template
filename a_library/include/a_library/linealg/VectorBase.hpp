#pragma once
#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <a_library/essentials.hpp>
#include <type_traits>
#include <utility>

namespace a_library::linealg {
template <typename T, size_t N>
struct storage {
    std::array<T, N> data;

    template <typename... Args>
        requires(std::convertible_to<std::remove_cvref_t<Args>, T> && ...)
    constexpr storage(Args&&... args)
        : data{static_cast<T>(std::forward<Args>(args))...} {}
};

template <typename T>
struct storage<T, 2> {
    union {
        std::array<T, 2> data;
        struct {
            T x, y;
        };
    };

    template <typename... Args>
        requires(std::convertible_to<std::remove_cvref_t<Args>, T> && ...)
    constexpr storage(Args&&... args)
        : data{static_cast<T>(std::forward<Args>(args))...} {}
};

template <typename T>
struct storage<T, 3> {
    union {
        std::array<T, 3> data;
        struct {
            T x, y, z;
        };
    };

    template <typename... Args>
        requires(std::convertible_to<std::remove_cvref_t<Args>, T> && ...)
    constexpr storage(Args&&... args)
        : data{static_cast<T>(std::forward<Args>(args))...} {}
};

template <typename T>
struct storage<T, 4> {
    union {
        std::array<T, 4> data;
        struct {
            T x, y, z, w;
        };
    };

    template <typename... Args>
        requires(std::convertible_to<std::remove_cvref_t<Args>, T> && ...)
    constexpr storage(Args&&... args)
        : data{static_cast<T>(std::forward<Args>(args))...} {}
};

template <std::convertible_to<double> T, std::size_t N, typename StorageType = storage<T, N>>
struct VectorBase : public StorageType {
    using value_type = T;
    using Derived    = VectorBase;

    static constexpr std::size_t size_v = {N};
    using StorageType::data;

    constexpr std::size_t size() const noexcept { return N; }
    constexpr T* ptr() { return data.data(); }
    constexpr const T* ptr() const { return data.data(); }
    constexpr T& operator[](size_t idx);
    constexpr const T& operator[](size_t idx) const;

    constexpr VectorBase<T, N, StorageType> operator+(const VectorBase<T, N, StorageType>& rhs) const;
    constexpr VectorBase<T, N, StorageType> operator-(const VectorBase<T, N, StorageType>& rhs) const;
    constexpr VectorBase<T, N, StorageType> operator*(const VectorBase<T, N, StorageType>& rhs) const;
    constexpr VectorBase<T, N, StorageType> operator/(const VectorBase<T, N, StorageType>& rhs) const;
    constexpr VectorBase<T, N, StorageType>& operator+=(const VectorBase<T, N, StorageType>& rhs);
    constexpr VectorBase<T, N, StorageType>& operator-=(const VectorBase<T, N, StorageType>& rhs);
    constexpr VectorBase<T, N, StorageType>& operator*=(const VectorBase<T, N, StorageType>& rhs);
    constexpr VectorBase<T, N, StorageType>& operator/=(const VectorBase<T, N, StorageType>& rhs);

    constexpr VectorBase<T, N, StorageType> operator*(T scalar) const;
    constexpr VectorBase<T, N, StorageType> operator/(T scalar) const;

    T length_sqr() const;
    double length() const;

    template <typename... U>
        requires((sizeof...(U) == N) && (std::convertible_to<std::remove_cvref_t<U>, T> && ...))
    constexpr VectorBase(U&&... args)
        : StorageType(std::forward<U>(args)...) {}

    template <typename... U>
        requires(std::convertible_to<std::remove_cvref_t<U>, T> && ...)
    constexpr VectorBase(const T& f, U&&... args)
        : StorageType(f, std::forward<U>(args)...) {}

    template <typename... U>
        requires(std::convertible_to<std::remove_cvref_t<U>, T> && ...)
    constexpr VectorBase(T&& f, U&&... args)
        : StorageType(std::move(f), std::forward<U>(args)...) {}

    template <typename O>
        requires(std::convertible_to<std::remove_cvref_t<O>, T>)
    constexpr VectorBase(const VectorBase<O, N>& other) noexcept {
        for (std::size_t idx(0); idx < N; ++idx) data[idx] = static_cast<T>(other.data[idx]);
    }

    constexpr VectorBase() = default;

   private:
    constexpr VectorBase<T, N, StorageType>& self() noexcept;
    constexpr const VectorBase<T, N, StorageType>& self() const noexcept;
};

template <typename Ve>
concept VectorImpl = std::derived_from<Ve, VectorBase<typename Ve::value_type, Ve::size_v>>;
}  // namespace a_library::linealg

namespace a_library::linealg {
template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType>& VectorBase<T, N, StorageType>::self() noexcept {
    return static_cast<VectorBase<T, N, StorageType>&>(*this);
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr const VectorBase<T, N, StorageType>& VectorBase<T, N, StorageType>::self() const noexcept {
    return static_cast<const VectorBase<T, N, StorageType>&>(*this);
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr T& VectorBase<T, N, StorageType>::operator[](size_t idx) {
    return *(self().ptr() + idx);
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr const T& VectorBase<T, N, StorageType>::operator[](size_t idx) const {
    return *(self().ptr() + idx);
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType> VectorBase<T, N, StorageType>::operator+(
    const VectorBase<T, N, StorageType>& rhs
) const {
    const VectorBase<T, N, StorageType>& s = self();
    Derived res;
    for (size_t idx(0); idx < N; ++idx) res[idx] = s[idx] + rhs[idx];
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType> VectorBase<T, N, StorageType>::operator-(
    const VectorBase<T, N, StorageType>& rhs
) const {
    const VectorBase<T, N, StorageType>& s = self();
    Derived res;
    for (size_t idx(0); idx < N; ++idx) res[idx] = s[idx] - rhs[idx];
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType> VectorBase<T, N, StorageType>::operator*(T rhs) const {
    const VectorBase<T, N, StorageType>& s = self();
    Derived res;
    for (size_t idx(0); idx < N; ++idx) res[idx] = s[idx] * rhs;
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType> VectorBase<T, N, StorageType>::operator/(T rhs) const {
    const VectorBase<T, N, StorageType>& s = self();
    Derived res;
    for (size_t idx(0); idx < N; ++idx) res[idx] = s[idx] / rhs;
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType>& VectorBase<T, N, StorageType>::operator+=(
    const VectorBase<T, N, StorageType>& rhs
) {
    VectorBase<T, N, StorageType>& s = self();
    for (size_t idx(0); idx < N; ++idx) s[idx] += rhs[idx];
    return s;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType>& VectorBase<T, N, StorageType>::operator-=(
    const VectorBase<T, N, StorageType>& rhs
) {
    VectorBase<T, N, StorageType>& s = self();
    for (size_t idx(0); idx < N; ++idx) s[idx] -= rhs[idx];
    return s;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
T VectorBase<T, N, StorageType>::length_sqr() const {
    const VectorBase<T, N, StorageType>& s = self();
    T res(0);
    for (size_t idx(0); idx < N; ++idx) res += s[idx] * s[idx];
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
double VectorBase<T, N, StorageType>::length() const {
    return std::sqrt(length_sqr());
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType> VectorBase<T, N, StorageType>::operator*(
    const VectorBase<T, N, StorageType>& rhs
) const {
    const VectorBase<T, N, StorageType>& s = self();
    Derived res;
    for (size_t idx(0); idx < N; ++idx) res[idx] = s[idx] * rhs[idx];
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType> VectorBase<T, N, StorageType>::operator/(
    const VectorBase<T, N, StorageType>& rhs
) const {
    const VectorBase<T, N, StorageType>& s = self();
    Derived res;
    for (size_t idx(0); idx < N; ++idx) res[idx] = s[idx] / rhs[idx];
    return res;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType>& VectorBase<T, N, StorageType>::operator*=(
    const VectorBase<T, N, StorageType>& rhs
) {
    VectorBase<T, N, StorageType>& s = self();
    for (size_t idx(0); idx < N; ++idx) s[idx] *= rhs[idx];
    return s;
}

template <std::convertible_to<double> T, std::size_t N, typename StorageType>
constexpr VectorBase<T, N, StorageType>& VectorBase<T, N, StorageType>::operator/=(
    const VectorBase<T, N, StorageType>& rhs
) {
    VectorBase<T, N, StorageType>& s = self();
    for (size_t idx(0); idx < N; ++idx) s[idx] /= rhs[idx];
    return s;
}
}  // namespace a_library::linealg
