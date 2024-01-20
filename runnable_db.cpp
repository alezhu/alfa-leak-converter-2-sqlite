#include "runnable_db.h"
#include "common.h"

#include <QThread>
#include <QThreadPool>

runnable_db::runnable_db(state_t *state, context *ctx, database *db)
    : runnable_base(state, ctx), m_db{db} {}

void runnable_db::_step() {
  auto container = m_ctx->container();
  if (!m_state->stop && container->users.size() < DB_SAVE_BY_SIZE &&
      container->cards.size() < DB_SAVE_BY_SIZE &&
      container->contacts.size() < DB_SAVE_BY_SIZE) {
    QThread::yieldCurrentThread();
    return;
  }

  auto snapshot = m_ctx->detachContainer();

  m_db->startTransaction();

  if (snapshot->cards.size()) {
    m_log << "Insert cards partially...";

    // m_db->exec("CREATE TEMP TABLE IF NOT EXISTS cards_t ("
    //            "user_id UNSIGNED BIG INT,"
    //            "value TEXT,"
    //            "expire_date DATE"
    //            ")");

    QSqlQuery q{m_db->db()};
    q.prepare("INSERT OR IGNORE INTO cards (user_id,value,expire_date) VALUES "
              "(?,?,?)");
    QVariantList user_ids;
    QVariantList values;
    QVariantList expire_dates;
    for (const auto &card : snapshot->cards) {
      user_ids << card.user_id;
      values << card.value;
      expire_dates << toDateEx(card.expireDate);
    }
    q.addBindValue(user_ids);
    q.addBindValue(values);
    q.addBindValue(expire_dates);
    m_db->execBatch(q);
    // if (!m_state->stop)
      // snapshot->cards.clear();
    // m_db->exec("INSERT INTO cards"
    //            " SELECT t.user_id, t.value, t.expire_date"
    //            " FROM cards_t as t"
    //            " WHERE NOT EXISTS ("
    //            "   SELECT rowid FROM cards as e "
    //            "    WHERE e.user_id = t.user_id"
    //            "      AND e.value = t.value"
    //            " )");
    // m_db->exec("DROP TABLE cards_t");
  }

  if (snapshot->contacts.size()) {
    m_log << "Insert contacts partially...";

    // m_db->exec("CREATE TEMP TABLE IF NOT EXISTS contacts_t ("
    //            "user_id UNSIGNED BIG INT,"
    //            "type INT,"
    //            "value TEXT"
    //            ")");
    QSqlQuery q{m_db->db()};
    q.prepare(
        "INSERT OR IGNORE INTO contacts (user_id,type,value) VALUES (?,?,?)");
    QVariantList user_ids;
    QVariantList types;
    QVariantList values;
    auto size = snapshot->contacts.size();
    for (qsizetype i = 0; i < size; ++i) {
      auto contact = snapshot->contacts.at(i);
      user_ids << contact.user_id;
      types << static_cast<int>(contact.type);
      values << contact.value.toLower();
    }
    q.addBindValue(user_ids);
    q.addBindValue(types);
    q.addBindValue(values);
    m_db->execBatch(q);
    // if (!m_state->stop)
    //   snapshot->contacts.clear();

    // m_db->exec("INSERT INTO contacts"
    //            " SELECT t.user_id, t.type, t.value"
    //            " FROM contacts_t as t"
    //            " WHERE NOT EXISTS ("
    //            "   SELECT rowid FROM contacts as e "
    //            "    WHERE e.user_id = t.user_id"
    //            "      AND e.value = t.value"
    //            " )");
    // m_db->exec("DROP TABLE contacts_t");
  }

  if (snapshot->users.size()) {

    m_log << "Insert users partially...";
    QSqlQuery q{m_db->db()};
    q.prepare(
        "INSERT OR IGNORE INTO users (id,name,birth_date) VALUES  (?,?,?)");

    QVariantList ids;
    QVariantList names;
    QVariantList birth_dates;
    auto size = snapshot->users.size();
    for (qsizetype i = 0; i < size; ++i) {
      auto user = snapshot->users.at(i);
      ids << user.id;
      names << user.name.toUpper();
      birth_dates << toDateEx(user.birthDate);
    }
    q.addBindValue(ids);
    q.addBindValue(names);
    q.addBindValue(birth_dates);
    m_db->execBatch(q);
    // if (!m_state->stop)
    //   snapshot->users.clear();
  }

  {
    m_log << "Update processed position";
    QSqlQuery q{m_db->db()};
    // q.prepare("INSERT INTO params (name,value)"
    //           " VALUES (:name, :value)"
    //           " ON CONFLICT (name) DO UPDATE SET"
    //           " value = excluded.value;");
    q.prepare(
        "INSERT OR REPLACE INTO params (name,value) VALUES (:name, :value)");

    q.bindValue(":name", DB_PARAM_PROCESSED_POSITION);
    q.bindValue(":value", snapshot->processed_position);
    m_db->exec(q);
    // snapshot->processed_position = 0;
  }

  m_db->commit();
  m_log << "DB Updated";

  if (!m_state->stop) {
      QThreadPool::globalInstance()->start([=]() { delete snapshot; });
  }
}
