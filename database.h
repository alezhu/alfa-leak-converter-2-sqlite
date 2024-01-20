#include "context.h"
#include "loger.h"
#include "state.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

#pragma once

class database
{
public:
    database(const QString& path);
    ~database();

    QVariant get_param(QAnyStringView name);
    void exec(const QString& sql, QAnyStringView log ={});
    void exec(QSqlQuery& query, QAnyStringView log ={});
    void execBatch(QSqlQuery& query, QAnyStringView log ={});

    QSqlDatabase db() const;

    void startTransaction();
    void commit();
    void rollack();

    void createIndexes();
    void reload(context& ctx, state_t& state);
private:
    QSqlDatabase m_db;
    loger& m_log;
    void _initDb();

    void _throwLastError(const QSqlError &&lastError);

    template<class T>
    void _checkResult(bool result,const T& lastErrorProvider) {
        if(result) return;
        _throwLastError(lastErrorProvider.lastError());
    }
};
