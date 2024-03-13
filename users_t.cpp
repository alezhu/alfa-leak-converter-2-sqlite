#include "users_t.h"
#include "common.h"

users_t::users_t(const size_t chunk_size)
    : m_count{0}, m_chunk_size{chunk_size},
      m_strings{u"INSERT OR IGNORE INTO users (id,name,birth_date) VALUES ",
                chunk_size} {}

users_t::~users_t() { clear(); }

qsizetype users_t::count() const { return m_count; }

void users_t::add(const user_t &&value) {
  //    INSERT OR IGNORE INTO users (id,name,birth_date) VALUES
  //    (?,?,?)
  //    ,(?,?,?)
  //    ,(?,?,?)
  auto name = value.name.toUpper().replace("'", "''");
  QString values = QString("(%1,'%2','%3'),")
                       .arg(QString::number(value.id), name,
                            toDateEx(value.birthDate));
  m_strings.append(values);
  m_count++;
}

 strings *users_t::getStrings()  { return &m_strings; }

void users_t::clear() {
  m_strings.clear();
  m_count = 0;
}
