#include "context.h"

context::context()
    :
#ifdef CHECK_DOUBLES
      m_loaded {
  new QMap<user_id_t, qint64> {}
}
#else
      m_loaded {
  new QSet<user_id_t> {}
}
#endif
, m_users_contacts{new QMap<user_id_t, QSet<QString>>()},
    m_users_cards{new QMap<user_id_t, QSet<quint64>>()} {
  m_current = _createContainer();
  // m_shadow = nullptr;
}

context::~context() {
  // Поскольку контекст разрушается только в случае когда мы останавливаем
  // процесс, то есть выходим из приложения, мы не очищаем и не удаляем ни
  // m_current, ни m_shadow, ни m_loaded, потому что их очистка приведет к
  // последовательному удаленияю их элементов, что занимает существенно время, а
  // на выходе нам это совсем не нужно. Поэтому мы явно допускаем "утечку
  // памяти", так как после закрытия приложения операционная система
  // автоматически освободит всю память, которую занимало приложение. И это
  // будет быстро

  /*if (m_current != nullptr) {
    delete m_current;
    m_current = nullptr;
  }*/
}

context::container_t *context::container() {
  QReadLocker locker(&lock);
  auto result = const_cast<container_t *>(m_current);
  return result;
}

context::container_t *context::detachContainer() {
  QWriteLocker locker(&lock);
  auto result = const_cast<container_t *>(m_current);
  m_current = _createContainer();
  m_current->processed_position = result->processed_position;
  return result;
}

context::container_t *context::_createContainer() {
  auto result = new container_t{};
  // result->users.reserve(DB_SAVE_BY_SIZE);
  // result->cards.reserve(DB_SAVE_BY_SIZE);
  // result->contacts.reserve(DB_SAVE_BY_SIZE);
  return result;
}

QMap<user_id_t, QSet<quint64>> &context::users_cards() {
  return *m_users_cards;
}

QMap<user_id_t, QSet<quint64>> const &context::users_cards() const {
  return *m_users_cards;
}

QMap<user_id_t, QSet<QString>> &context::users_contacts() {
  return *m_users_contacts;
}

QMap<user_id_t, QSet<QString>> const &context::users_contacts() const {
  return *m_users_contacts;
}

#ifdef CHECK_DOUBLES
QMap<user_id_t, qint64> const &context::loaded() const { return *m_loaded; }
QMap<user_id_t, qint64> &context::loaded() { return *m_loaded; }
#else
QSet<user_id_t> const &context::loaded() const { return *m_loaded; }
QSet<user_id_t> &context::loaded() { return *m_loaded; }
#endif
