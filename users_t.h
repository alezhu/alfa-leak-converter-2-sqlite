#include "chunk_t.h"
#include "global.h"
#include "user.h"
#include <QChar>
#include <QMap>

#pragma once
constexpr auto NAME_LEN = 40;

class users_t
{
    struct item {
        user_id_t id;
        date_t birthDate;
        QChar name[NAME_LEN];
    };
    class user_chunk_t : public chunk_t<item> {};

    public:
    users_t(const qsizetype chunk_size = 10000);
    ~users_t();


    qsizetype size() const;
    qsizetype add(const user_t&& value);
    qsizetype emplaceBack(const user_t&& value);
    user_t at(qsizetype index) const;

    void clear();

    // Iterator begin() { return Iterator(this, 0); }
    // Iterator end() { return Iterator(this, size()); }

    private:
    qsizetype m_size;
    const qsizetype m_chunk_size;
    QList<user_chunk_t*> m_chunks{};
    QMap<qsizetype, QString> m_longValue{};
};
