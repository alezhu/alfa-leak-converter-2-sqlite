#include "contacts_t.h"

contacts_t::contacts_t(const size_t chunk_size)
    : m_count{0}, m_chunk_size{chunk_size},
      m_strings{u"INSERT OR IGNORE INTO contacts (user_id,type,value) VALUES ",
                chunk_size} {}

contacts_t::~contacts_t() { clear(); }

qsizetype contacts_t::count() const { return m_count; }

void contacts_t::add(const contact_t &&value) {
  //    INSERT OR IGNORE INTO contacts (user_id,type,value) VALUES
  //    (?,?,?)
  //    ,(?,?,?)
  //    ,(?,?,?)

  QString values =
      QString("(%1,%2,\"%3\"),")
          .arg(QString::number(value.user_id),
               QString::number(static_cast<int>(value.type)),
               value.value);
  m_strings.append(values);
  m_count++;
}

 strings *contacts_t::getStrings()  { return &m_strings; }

void contacts_t::clear() {
  m_strings.clear();
  m_count = 0;
}
