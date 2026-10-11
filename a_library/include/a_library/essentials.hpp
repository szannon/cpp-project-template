#pragma once
#include <concepts>
#include <cstddef>
#include <ranges>
#include <type_traits>

namespace a_library {
template <typename T, typename U>
concept CvrefOf = std::same_as<std::remove_cvref<T>, U>;

template <typename T, typename U>
concept CanConstruct = std::constructible_from<U, T>;

template <typename T, typename... Opts>
concept CanConstructAny = (CanConstruct<T, Opts> || ...);

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename R>
concept Range = std::ranges::range<R>;

template <typename R>
concept ContigousRange = Range<R> && std::ranges::contiguous_range<R>;

template <typename R, typename Elem>
concept RangeOf = Range<R> && std::is_same_v<std::ranges::range_value_t<R>, Elem>;

template <typename T>
constexpr bool is_serializable();

template <typename T>
concept Serializable = is_serializable<T>();

template <Serializable... T>
size_t calc_size(const T&... objs);

template <typename R>
concept ArithmeticRange = Range<R> && Arithmetic<std::ranges::range_value_t<R>>;

template <typename R>
concept ContigousArithmetic = ContigousRange<R> && ArithmeticRange<R>;

template <typename Cont>
concept ProvidesBuffer = requires(Cont& cont) {
    cont.ptr();
    { cont.size() } -> std::convertible_to<size_t>;
};

template <typename Cont, typename T>
concept ProvidesBufferOf = ProvidesBuffer<Cont> && requires(Cont& cont) {
    { cont.ptr() } -> std::convertible_to<T*>;
};

template <typename Cont>
concept ProvidesSerialBuffer = ProvidesBuffer<Cont> && requires(Cont& cont) {
    { *cont.ptr() } -> Serializable;
};

template <typename Cont>
concept ProvidesArithmeticBuffer = ProvidesSerialBuffer<Cont> && requires(Cont& cont) {
    { *cont.ptr() } -> Arithmetic;
};

template <typename F, typename Sig>
struct signature_of_helper;

template <typename F, typename R, typename... Args>
struct signature_of_helper<F, R(Args...)> {
    static constexpr bool value
        = std::invocable<F, Args...> && std::convertible_to<std::invoke_result_t<F, Args...>, R>;
};

template <typename F, typename Sig>
concept SignatureOf = signature_of_helper<F, Sig>::value;

template <typename F, typename Sig>
constexpr bool signature_of_v = signature_of_helper<F, Sig>::value;
}  // namespace a_library

namespace a_library {
template <typename T>
constexpr bool is_serializable() {
    using U = std::remove_cvref_t<T>;
    if constexpr (Arithmetic<U>) return true;
    if constexpr (ProvidesSerialBuffer<U>) return true;
    if constexpr (Range<U>) return is_serializable<std::ranges::range_value_t<T>>();
    return false;
}

template <Arithmetic T>
size_t calc_size(const T& v) {
    return sizeof(T);
}

template <Serializable R>
    requires(Range<R> && Arithmetic<std::ranges::range_value_t<R>>)
size_t calc_size(const R& r) {
    return std::ranges::size(r) * sizeof(std::ranges::range_value_t<R>);
}

template <ProvidesSerialBuffer Cont>
size_t calc_size(const Cont& cont) {
    size_t res(0);
    for (auto *cur = cont.ptr(), *end = cur + cont.size(); cur < end; res += calc_size(*cur++));
    return res;
}

template <ProvidesArithmeticBuffer Cont>
size_t calc_size(const Cont& cont) {
    return cont.size() * sizeof(std::remove_cvref_t<decltype(*cont.ptr())>);
}

template <Serializable R>
    requires(Range<R>)
size_t calc_size(const R& val) {
    size_t res(0);
    for (const auto& el : val) res += calc_size(el);
    return res;
}

template <Serializable... T>
size_t calc_size(const T&... objs) {
    return (calc_size(objs) + ...);
}
}  // namespace a_library
