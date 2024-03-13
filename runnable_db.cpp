#include "runnable_db.h"
#include "common.h"

#include <QThread>
#include <QThreadPool>

runnable_db::runnable_db(state_t *state, context *ctx, database *db)
    : runnable_base(state, ctx), m_db{db} {}

void runnable_db::_insertChunks(strings *strings) const {
  auto count = strings->chunksCount();
    QSqlQuery q{m_db->db()};
  for (size_t i = 0; i < count; ++i) {
    auto chunk = strings->getChunk(i);
    if (chunk == nullptr)
        continue;
    chunk->lock();
    auto sql = chunk->toString();
    auto result = q.exec(sql);
    m_db->checkResult(result, q);
    if (!m_state->stop) {
        QThreadPool::globalInstance()->start([=]() { strings->freeChunk(i); });
    }
  }
}

void runnable_db::_step() {
  auto container = m_ctx->container();
  if (!m_state->done && container->users.count() < DB_SAVE_BY_SIZE &&
      container->cards.count() < DB_SAVE_BY_SIZE &&
      container->contacts.count() < DB_SAVE_BY_SIZE) {
      _sleep();
    return;
  }

  auto snapshot = m_ctx->detachContainer();

  m_db->startTransaction();

  if (snapshot->cards.count()) {
      m_log << QString("Insert %1 cards partially...").arg(snapshot->cards.count());

    auto strings = snapshot->cards.getStrings();
    _insertChunks(strings);
  }

  if (snapshot->contacts.count()) {
    m_log << QString("Insert %1 contacts partially...").arg(snapshot->contacts.count());

    auto strings = snapshot->contacts.getStrings();
    _insertChunks(strings);
  }

  if (snapshot->users.count()) {
    m_log << QString("Insert %1 users partially...").arg(snapshot->users.count());

    auto strings = snapshot->users.getStrings();
    _insertChunks(strings);
  }

  {
    m_log << "Update processed position";
    QSqlQuery q{m_db->db()};
    q.prepare(
        "INSERT OR REPLACE INTO params (name,value) VALUES (:name, :value)");

    q.bindValue(":name", DB_PARAM_PROCESSED_POSITION);
    q.bindValue(":value", snapshot->processed_position);
    m_db->exec(q);
  }

  m_db->commit();
  m_log << "DB Updated";

  if (!m_state->stop) {
      QThreadPool::globalInstance()->start([=]() { delete snapshot; });
  }
}
