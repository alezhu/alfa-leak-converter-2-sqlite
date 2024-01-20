#pragma once
#include "qtypes.h"

#include <QScopedPointer>
template <class T> class chunk_t {
public:
  chunk_t(qsizetype capacity) : m_capacity{capacity}, m_size{0} {
    auto size = m_capacity * sizeof(T);
    m_buffer.reset(static_cast<char *>(malloc(size)));
  };

  qsizetype size() const { return m_size; };
  qsizetype add(const T &&value) {
    // Not safe
    auto pos = m_size * sizeof(T);
    auto lpPos = m_buffer.get() + pos;
    memcpy(lpPos, &value, sizeof(T));
    return m_size++;
  };

  qsizetype add(const T &value) {
      return add(std::move(value));
  };
  T &at(qsizetype index) const {
    auto pos = index * sizeof(T);
    auto lpPos = m_buffer.get() + pos;
    return *static_cast<T *>(static_cast<void *>(lpPos));
  };

  qsizetype capacity() const { return m_capacity; };

protected:
  qsizetype m_capacity;
  qsizetype m_size;

  QScopedPointer<char> m_buffer;
};
