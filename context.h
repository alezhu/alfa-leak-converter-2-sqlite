#pragma once

#include "cards_t.h"
#include "contacts_t.h"
#include "users_t.h"

#include <QMap>
#include <QReadWriteLock>
#include <QSet>

class context {
public:
  struct container_t {
    quint64 processed_position{0};
    users_t users{};
    contacts_t contacts{};
    cards_t cards{};
  };

  context();
  ~context();

  container_t *container();

  container_t *detachContainer();

#ifdef CHECK_DOUBLES
  QMap<user_id_t, qint64> const &loaded() const;
  QMap<user_id_t, qint64> &loaded();
#else
  QSet<user_id_t> const &loaded() const;
  QSet<user_id_t> &loaded();
#endif

  QMap<user_id_t, QSet<QString>> const &users_contacts() const;
  QMap<user_id_t, QSet<QString>> &users_contacts();

  QMap<user_id_t, QSet<quint64>> const &users_cards() const;
  QMap<user_id_t, QSet<quint64>> &users_cards();

private:
  container_t *m_current;
  QReadWriteLock lock;

  container_t *_createContainer();

#ifdef CHECK_DOUBLES
  QMap<user_id_t, qint64> *m_loaded;
#else
  QSet<user_id_t> *m_loaded;
#endif
  QMap<user_id_t, QSet<QString>>* m_users_contacts;
  QMap<user_id_t, QSet<quint64>>* m_users_cards;
};
