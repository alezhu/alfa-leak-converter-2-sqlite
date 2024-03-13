#include "database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>

#include <QDateTime>
#include <QException>
#include <QFuture>
#include <QPromise>

using namespace Qt::Literals::StringLiterals;

database::database(const QString &path)
    : m_db{QSqlDatabase::addDatabase(DB_TYPE_NAME)}, m_log{loger::get()}

{
  m_db.setDatabaseName(path);
  if (m_db.open()) {
    _initDb();
  }
}

database::~database() { m_db.close(); }

QVariant database::get_param(QAnyStringView name) {
  QSqlQuery query(m_db);
  query.prepare("SELECT value FROM params WHERE name = :name");
  query.bindValue(":name", name.toString());
  exec(query);
  return query.next() ? query.value(0) : QVariant{};
}

void database::_initDb() {
  exec("PRAGMA synchronous = OFF");
  exec("PRAGMA journal_mode = OFF");

  exec("CREATE TABLE IF NOT EXISTS users "
       "(id INTEGER PRIMARY KEY ASC NOT NULL UNIQUE,"
       "name TEXT,"
       "birth_date DATE)",
       "Creating users table...");

  exec("CREATE TABLE IF NOT EXISTS contacts ("
       "user_id UNSIGNED BIG INT,"
       "type INT,"
       "value TEXT,"
       " PRIMARY KEY(user_id,value) ON CONFLICT IGNORE "
       " FOREIGN KEY(user_id) REFERENCES users(id)"
       ")",
       "Creating contacts table...");

  exec("CREATE TABLE IF NOT EXISTS cards ("
       "user_id UNSIGNED BIG INT,"
       "value TEXT,"
       "expire_date DATE,"
       " PRIMARY KEY(user_id,value) ON CONFLICT IGNORE "
       " FOREIGN KEY(user_id) REFERENCES users(id)"
       ")",
       "Creating cards table...");

  exec("CREATE TABLE IF NOT EXISTS params"
       "("
       "name TEXT PRIMARY KEY ASC NOT NULL UNIQUE,"
       "value TEXT)",
       "Creating params table...");
}

void database::exec(const QString &sql, QAnyStringView log) {
  QSqlQuery query{m_db};
  query.prepare(sql);
  exec(query, log);
}

void database::exec(QSqlQuery &query, QAnyStringView log) {
  if (log.size() > 0)
    m_log << log;
  _checkResult(query.exec(), query);
}

void database::execBatch(QSqlQuery &query, QAnyStringView log) {
  if (log.size() > 0)
    m_log << log;
  _checkResult(query.execBatch(), query);
}

void database::_throwLastError(const QSqlError &&lastError) {
  const auto msg = lastError.text();
  m_log << QAnyStringView{msg};
  throw std::logic_error(msg.toStdString().c_str());
}

QSqlDatabase database::db() const { return m_db; }

void database::startTransaction() { _checkResult(m_db.transaction(), m_db); }

void database::commit() { _checkResult(m_db.commit(), m_db); }

void database::rollack() { _checkResult(m_db.rollback(), m_db); }

void database::createIndexes() {
  exec("CREATE INDEX IF NOT EXISTS idx_users_name ON users (name)",
       "Create index for users name...");

  exec("CREATE INDEX IF NOT EXISTS idx_contacts_value ON contacts (value)",
       "Create index for value in contacts...");

  exec("CREATE INDEX IF NOT EXISTS idx_cards_value ON cards (value)",
       "Create index for value in cards...");
}

void _wait_pause(state_t &state) {
  while (state.pause && !state.stop) {
    QThread::usleep(1);
    QThread::yieldCurrentThread();
  }
}
void database::reload(context &ctx, state_t &state) {
  m_log << "Load old data...";
  QPromise<void> promise1;
  auto future1 = promise1.future();
  auto thread1 = QThread::create(
      [&](QPromise<void> promise) {
        promise.start();
        auto db = QSqlDatabase::addDatabase(DB_TYPE_NAME, "users");
        db.setDatabaseName(m_db.databaseName());
        if (db.open()) {
          QSqlQuery q{db};
          q.prepare("SELECT id FROM users;");
          exec(q);
#ifdef CHECK_DOUBLES
          auto now = QDateTime::currentMSecsSinceEpoch();
#endif
          auto &loaded = ctx.loaded();
          while (!state.stop && q.next()) {
            auto user_id = q.value(0).toULongLong();
#ifdef CHECK_DOUBLES
            loaded.insert(user_id, now);
#else
            loaded.insert(user_id);
#endif
            _wait_pause(state);
          }
        }
        m_log << QString("Loaded users: %1").arg(ctx.loaded().size());
        promise.finish();
      },
      std::move(promise1));
  thread1->start();

  QPromise<void> promise2;
  auto future2 = promise2.future();
  auto thread2 = QThread::create(
      [&](QPromise<void> promise) {
        promise.start();
        // quint64 loaded = 0;
        // auto db = QSqlDatabase::addDatabase(DB_TYPE_NAME, "cards");
        // db.setDatabaseName(m_db.databaseName());
        // if (db.open()) {

        //   QSqlQuery q{db};
        //   // q.prepare("SELECT user_id, value FROM cards WHERE rowid >= "
        //   //           "(SELECT MAX(rowid) FROM cards) - 100000;");
        //   q.prepare("SELECT user_id, value FROM cards LIMIT 100000;");
        //   exec(q);

        //   auto &users_cards = ctx.users_cards();
        //   auto it = users_cards.end();
        //   while (!state.stop && q.next()) {
        //     auto user_id = q.value(0).toULongLong();
        //     if (it == users_cards.end() || it.key() != user_id) {
        //       it = users_cards.find(user_id);
        //       if (it == users_cards.end()) {
        //         it = users_cards.insert(user_id, {});
        //       }
        //     }
        //     auto card = q.value(1).toULongLong();
        //     it->insert(card);
        //     loaded++;
        //     _wait_pause(state);
        //   }
        // }
        // m_log << QString("Loaded cards: %1").arg(loaded);
        promise.finish();
      },
      std::move(promise2));
  thread2->start();

  QPromise<void> promise3;
  auto future3 = promise3.future();
  auto thread3 = QThread::create(
      [&](QPromise<void> promise) {
        promise.start();
        // quint64 loaded = 0;
        // auto db = QSqlDatabase::addDatabase(DB_TYPE_NAME, "contacts");
        // db.setDatabaseName(m_db.databaseName());
        // if (db.open()) {

        //   QSqlQuery q{db};
        //   // q.prepare("SELECT user_id, value FROM contacts WHERE rowid >= "
        //   //           "(SELECT MAX(rowid) FROM contacts) - 100000;");

        //   q.prepare("SELECT user_id, value FROM contacts LIMIT 100000;");
        //   exec(q);

        //   auto &users_contacts = ctx.users_contacts();
        //   auto it = users_contacts.end();
        //   while (!state.stop && q.next()) {
        //     auto user_id = q.value(0).toULongLong();
        //     if (it == users_contacts.end() || it.key() != user_id) {
        //       it = users_contacts.find(user_id);
        //       if (it == users_contacts.end()) {
        //         it = users_contacts.insert(user_id, {});
        //       }
        //     }
        //     auto contact = q.value(1).toString();
        //     it->insert(contact);
        //     loaded++;
        //     _wait_pause(state);
        //   }
        // }
        // m_log << QString("Loaded contacts: %1").arg(loaded);
        promise.finish();
      },
      std::move(promise3));
  thread3->start();

  QtFuture::whenAll(std::move(future1), std::move(future2), std::move(future3))
      .waitForFinished();
}
