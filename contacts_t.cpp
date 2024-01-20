#include "contacts_t.h"

contacts_t::contacts_t(const qsizetype chunk_size)
    : m_size{0}
    , m_chunk_size(chunk_size)
{}

contacts_t::~contacts_t()
{
    clear();
}

qsizetype contacts_t::size() const { return m_size; }

qsizetype contacts_t::add(const contact_t &&value) {
  auto chunks_count = m_chunks.size();
  if (chunks_count == 0) {
    m_chunks.append(new contact_chunk_t{m_chunk_size});
    chunks_count++;
  }
  auto chunk = m_chunks[chunks_count - 1];
  if (chunk->size() == chunk->capacity()) {
    m_chunks.append(new contact_chunk_t{m_chunk_size});
      chunks_count++;
    chunk = m_chunks[chunks_count - 1];
  }

  auto index = m_size++;
  if (value.type == contact_type::phone) {
    chunk->add({.user_id = value.user_id,
                .type = value.type,
                .value = value.value.toULongLong()});
  } else {
    chunk->add({.user_id = value.user_id, .type = value.type, .value = 0});
    m_longValue.insert(index, value.value);
  }
  return index;
}

qsizetype contacts_t::emplaceBack(const contact_t &&value) {
  return add(std::move(value));
}

contact_t contacts_t::at(qsizetype index) const {
  // Not safe
  auto chunk_id = index / m_chunk_size;
  auto in_chunk_id = index % m_chunk_size;
  auto chunk = m_chunks.at(chunk_id);
  auto &item = chunk->at(in_chunk_id);
  contact_t result{.user_id = item.user_id,
                   .type = item.type,
                   .value = (item.type == contact_type::phone)
                                ? QString::number(item.value)
                                : m_longValue.find(index).value()};
  return result;
}

void contacts_t::clear() {
  for (auto it : m_chunks) {
    delete it;
  }
  m_chunks.clear();
  m_longValue.clear();
  m_size = 0;
}
