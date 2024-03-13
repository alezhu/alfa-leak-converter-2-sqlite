#pragma once

#include "card.h"
#include "qtypes.h"
#include "strings.h"

#include <QList>

class cards_t {
public:
  cards_t(const size_t chunk_size = STRING_BUFFER_SIZE);
  ~cards_t();
  qsizetype count() const;
  void add(const card_t &&value);

  strings* getStrings();

  void clear();

private:
  size_t m_count;
  const size_t m_chunk_size;
  strings m_strings;
};
