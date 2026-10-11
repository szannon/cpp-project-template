#include <cstddef>
#include <a_library/ByteBuffer.hpp>
#include <a_library/essentials.hpp>

namespace a_library {
void ByteBuffer::mf_append(const ByteBuffer& b) { mf_append(b.m_buf); }

size_t ByteBuffer::size() const { return m_buf.size(); }

uint8_t* ByteBuffer::data() { return m_buf.data(); }

ByteBuffer ByteBuffer::operator+(const ByteBuffer& other) const { return ByteBuffer(*this, other); }

size_t calc_size(const ByteBuffer& b) { return b.size(); }

ByteBuffer& ByteBuffer::operator+=(const ByteBuffer& other) {
    append(other);
    return *this;
}

ByteBuffer::iterator ByteBuffer::begin() { return m_buf.begin(); }
ByteBuffer::iterator ByteBuffer::end() { return m_buf.end(); }

ByteBuffer::const_iterator ByteBuffer::begin() const { return m_buf.begin(); }
ByteBuffer::const_iterator ByteBuffer::end() const { return m_buf.end(); }

ByteBuffer::const_iterator ByteBuffer::cbegin() const { return m_buf.cbegin(); }
ByteBuffer::const_iterator ByteBuffer::cend() const { return m_buf.cend(); }

void ByteBuffer::reserve_additional(size_t p_additional) { m_buf.resize(m_buf.size() + p_additional); }
}  // namespace a_library
