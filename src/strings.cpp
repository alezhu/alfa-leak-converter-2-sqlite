#include "strings.h"

#include <stdexcept>

strings::strings(QStringView chunk_prefix, size_t bufferSize)
    : m_bufferSize{bufferSize}
    , m_chunk_prefix{chunk_prefix.toString()}
{}

strings::~strings() { clear(); }

void strings::clear() {
  for (auto it : m_chunks) {
    delete it;
  }
  m_chunks.clear();
}

void strings::append(QStringView value) {
  auto iChunk = m_chunks.size();
  string_buffer *chunk = nullptr;
  if (iChunk == 0) {
    chunk = _newChunk();
  } else {
    chunk = m_chunks[iChunk - 1];
  }

  if (!chunk->append(value)) {
    chunk->lock();
    chunk = _newChunk();
    if (!chunk->append(value)) {
      throw std::logic_error("Can't append data to string buffer");
    }
  }
}

size_t strings::chunksCount() const { return m_chunks.size(); }

string_buffer *strings::getChunk(const size_t index) const {
    return m_chunks[index];
}

void strings::freeChunk(const size_t index)
{
    auto chunk = getChunk(index);
    m_chunks[index] = nullptr;
    delete chunk;
}

string_buffer *strings::_newChunk() {
  auto result = new string_buffer{m_bufferSize};
  result->append(m_chunk_prefix);
  m_chunks.append(result);
  return result;
}
