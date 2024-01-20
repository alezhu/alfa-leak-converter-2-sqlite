#include "cards_t.h"

cards_t::cards_t(const qsizetype chunk_size)
    : m_size{0}
    , m_chunk_size{chunk_size}
{}

cards_t::~cards_t()
{
    clear();
}

qsizetype cards_t::size() const { return m_size; }

qsizetype cards_t::add(const card_t &&value) {
  auto chunks_count = m_chunks.size();
  if (chunks_count == 0) {
    m_chunks.append(new card_chunk_t{m_chunk_size});
    chunks_count++;
  }
  auto chunk = m_chunks[chunks_count - 1];
  if (chunk->size() == chunk->capacity()) {
    m_chunks.append(new card_chunk_t{m_chunk_size});
    chunks_count++;
    chunk = m_chunks[chunks_count - 1];
  }
  chunk->add(std::move(value));
  return m_size++;
}

qsizetype cards_t::emplaceBack(const card_t &&value) {
  return add(std::move(value));
}

card_t &cards_t::at(qsizetype index) const {
  // Not safe
  auto chunk_id = index / m_chunk_size;
  auto in_chunk_id = index % m_chunk_size;
  auto chunk = m_chunks.at(chunk_id);
  return chunk->at(in_chunk_id);
}

void cards_t::clear() {
  for (auto it : m_chunks) {
    delete it;
  }
  m_chunks.clear();
  m_size = 0;
}
