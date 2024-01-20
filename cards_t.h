#pragma once

#include "card.h"
#include "chunk_t.h"
#include "qtypes.h"

#include <QList>


class cards_t {
    class card_chunk_t : public chunk_t<card_t> {};
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = card_t;
    using pointer = value_type *;
    using reference = value_type &;

    Iterator(cards_t *ptr, qsizetype index) : m_index{index}, m_ptr(ptr){};

    reference operator*() const { return m_ptr->at(m_index); }
    pointer operator->() { return &m_ptr->at(m_index); }
    Iterator &operator++() {
      m_index++;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.m_ptr == b.m_ptr && a.m_index == b.m_index;
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.m_ptr != b.m_ptr || a.m_index != b.m_index;
    };

  private:
    qsizetype m_index;
    cards_t *m_ptr;
  };

public:
  cards_t(const qsizetype chunk_size = 10000);
~cards_t();
  qsizetype size() const;
  qsizetype add(const card_t &&value);
  qsizetype emplaceBack(const card_t &&value);
  card_t &at(qsizetype index) const;

  void clear();

  Iterator begin() { return Iterator(this, 0); }
  Iterator end() { return Iterator(this, size()); }

private:
  qsizetype m_size;
  const qsizetype m_chunk_size;
  QList<card_chunk_t*> m_chunks;
};
