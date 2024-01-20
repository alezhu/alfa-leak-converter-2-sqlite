#pragma once

#include "csv_parser.h"
#include "database.h"
#include "state.h"

#include <QObject>

class model : public QObject
{
    Q_OBJECT
public:
    explicit model(const QString &csvFile, const QString &dbFile, QObject *parent = nullptr);
    quint64 fileSize() const;
    quint64 processedUsers();
    quint64 position();
private:
    bool m_bDbExists;
    csv_parser m_parser;
    database m_db;
    context m_ctx{};
    state_t m_state{.pause = false, .stop = false, .done = false};
public slots:
    void start();
    void stop();
    void togglePause();
    void finish();
        void setPosition(quint64);
signals:
    void started();
    void stoped();
    void paused();
    void resumed();
    void finished();
    void positionChanged(quint64);
};
