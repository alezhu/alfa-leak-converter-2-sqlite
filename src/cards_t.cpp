#include "cards_t.h"
#include "common.h"

cards_t::cards_t(const size_t chunk_size)
    : m_count{0}
    , m_chunk_size{chunk_size}
    , m_strings{u"INSERT OR IGNORE INTO cards (user_id,value,expire_date) VALUES ", m_chunk_size}
{
}

cards_t::~cards_t() { clear(); }

qsizetype cards_t::count() const { return m_count; }

void cards_t::add(const card_t &&value) {
  //    "INSERT OR IGNORE INTO cards (user_id,value,expire_date) VALUES "
  //    "(?,?,?)"
  //    ",(?,?,?)"
  //    ",(?,?,?)"

  QString values =
      QString("(%1,%2,'%3'),")
          .arg(QString::number(value.user_id), QString::number(value.value),
               toDateEx(value.expireDate));
  m_strings.append(values);
  m_count++;
}

strings* cards_t::getStrings()
{
    return &m_strings;
}

void cards_t::clear() {
  m_strings.clear();
    m_count = 0;
}
