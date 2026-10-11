#pragma once
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <a_library/essentials.hpp>
#include <vector>

namespace a_library {
class ByteBuffer {
   private:
    std::vector<uint8_t> m_buf;

    template <Arithmetic T>
    void mf_append(const T& v);

    template <ContigousArithmetic R>
    void mf_append(const R& r);

    template <Range R>
    void mf_append(const R& r);

    template <ProvidesSerialBuffer Cont>
    void mf_append(const Cont& cont);

    template <ProvidesArithmeticBuffer Cont>
    void mf_append(const Cont& cont);

    void mf_append(const ByteBuffer& b);

   public:
    using iterator       = std::vector<uint8_t>::iterator;
    using const_iterator = std::vector<uint8_t>::const_iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    size_t size() const;

    void reserve_additional(size_t additional);

    template <Serializable... T>
    void append(const T&... objs);

    template <Serializable... T>
    ByteBuffer(const T&... objs);

    ByteBuffer& operator+=(const ByteBuffer& other);
    ByteBuffer operator+(const ByteBuffer& other) const;
    uint8_t* data();

    ByteBuffer()                             = default;
    ByteBuffer(const ByteBuffer&)            = default;
    ByteBuffer(ByteBuffer&&)                 = default;
    ByteBuffer& operator=(const ByteBuffer&) = default;
    ByteBuffer& operator=(ByteBuffer&&)      = default;
    ~ByteBuffer()                            = default;
};

size_t calc_size(const ByteBuffer& b);
}  // namespace a_library

namespace a_library {
template <>
constexpr bool is_serializable<ByteBuffer>() {
    return true;
}

template <Arithmetic T>
void ByteBuffer::mf_append(const T& v) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&v);
    m_buf.insert(m_buf.end(), ptr, ptr + sizeof(T));
}

template <ContigousArithmetic R>
void ByteBuffer::mf_append(const R& r) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(std::ranges::data(r));
    m_buf.insert(m_buf.end(), ptr, ptr + calc_size(r));
}

template <Range R>
void ByteBuffer::mf_append(const R& r) {
    for (const auto& v : r) mf_append(v);
}

template <ProvidesSerialBuffer Cont>
void ByteBuffer::mf_append(const Cont& cont) {
    for (const auto *v_cur = cont.ptr(), *v_end = v_cur + cont.size(); v_cur < v_end; mf_append(*v_cur++));
}

template <ProvidesArithmeticBuffer Cont>
void ByteBuffer::mf_append(const Cont& cont) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(cont.ptr());
    m_buf.insert(m_buf.end(), ptr, ptr + calc_size(cont));
}

template <Serializable... T>
void ByteBuffer::append(const T&... objs) {
    m_buf.reserve(m_buf.size() + calc_size(objs...));
    (mf_append(objs), ...);
}

template <Serializable... T>
ByteBuffer::ByteBuffer(const T&... objs) {
    append(objs...);
}
}  // namespace a_library
