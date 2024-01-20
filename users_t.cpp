#include "users_t.h"

users_t::users_t(const qsizetype chunk_size)
    : m_size{0}
    , m_chunk_size{chunk_size}
{}

users_t::~users_t()
{
    clear();
}

qsizetype users_t::size() const {
  return m_size;
}

qsizetype users_t::add(const user_t &&value) {
  auto chunks_count = m_chunks.size();
  if (chunks_count == 0) {
    m_chunks.append(new user_chunk_t{m_chunk_size});
    chunks_count++;
  }
  auto chunk = m_chunks[chunks_count - 1];
  if (chunk->size() == chunk->capacity()) {
      m_chunks.append(new user_chunk_t{m_chunk_size});
      chunks_count++;
      chunk = m_chunks[chunks_count - 1];
  }

  auto index = m_size++;
  item item{.id = value.id,
            .birthDate = value.birthDate,
            .name{QChar::SpecialCharacter::Null}};
  if (value.name.size() <= NAME_LEN) {
    for (int i = 0; i < value.name.size(); ++i) {
      item.name[i] = value.name.at(i).unicode();
    }
    chunk->add(item);
  } else {
    chunk->add(item);
    m_longValue.insert(index, value.name);
  }
  return index;
}

qsizetype users_t::emplaceBack(const user_t &&value) {
  return add(std::move(value));
}

user_t users_t::at(qsizetype index) const {
  // Not safe
  auto chunk_id = index / m_chunk_size;
  auto in_chunk_id = index % m_chunk_size;
  auto chunk = m_chunks.at(chunk_id);
  auto &item = chunk->at(in_chunk_id);
  user_t result{.id = item.id,
                .name = (item.name[0] == QChar::SpecialCharacter::Null)
                            ? m_longValue.find(index).value()
                            : QString(&item.name[0]),
                .birthDate = item.birthDate};
  return result;
}

void users_t::clear() {
    for (auto it : m_chunks) {
        delete it;
    }
  m_chunks.clear();
  m_longValue.clear();
  m_size = 0;
}
