#include "model.h"
#include "runnable_db.h"
#include "runnable_parser.h"

#include <QFile>
#include <QThreadPool>

model::model(const QString &csvFile, const QString &dbFile, QObject *parent)
    : QObject(parent), m_bDbExists{QFile::exists(dbFile)}, m_parser{csvFile},
      m_db{dbFile} {}

quint64 model::fileSize() const { return m_parser.size(); }

quint64 model::processedUsers() { return m_ctx.loaded().size(); }

quint64 model::position() { return m_ctx.container()->processed_position; }

void model::start()
{
    bool skipHeader = true;
    if (m_bDbExists) {
        auto processed_pos = m_db.get_param(DB_PARAM_PROCESSED_POSITION);
        if (processed_pos.isValid()) {
            auto pos = processed_pos.toULongLong();
            m_ctx.container()->processed_position = pos;
            m_parser.setPosition(pos);
            skipHeader = false;
        }
        m_db.reload(m_ctx, m_state);
    }
  auto pool = QThreadPool::globalInstance();

  pool->start(new runnable_db(&m_state, &m_ctx, &m_db));
    pool.start(new runnable_parser(&m_state, &m_ctx, &m_parser, skipHeader, [&](quint64 pos) {
        setPosition(pos);
    }));
}

void model::stop() {
  if (m_state.stop)
    return;
  m_state.stop = true;
  //m_ctx.loaded().clear();
  auto pool = QThreadPool::globalInstance();
  pool->waitForDone();
  emit stoped();
}

void model::togglePause() {
  m_state.pause = !m_state.pause;
  if (m_state.pause)
    emit paused();
  else
    emit resumed();
}

void model::finish() {
  m_state.done = true;
  emit finished();
  auto pool = QThreadPool::globalInstance();
  pool->waitForDone();
  m_db.createIndexes();
}

void model::setPosition(quint64 value) {
  m_ctx.container()->processed_position = value;
  // emit positionChanged(value);
}
