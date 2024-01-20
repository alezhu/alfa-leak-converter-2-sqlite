#pragma once

#include "chunk_t.h"
#include "contact.h"
#include "global.h"

#include <QList>
#include <QMap>

class contacts_t
{
    struct item {
        user_id_t user_id;
        contact_type type;
        quint64 value;
    };
    class contact_chunk_t : public chunk_t<item> {};

    public:
    contacts_t(const qsizetype chunk_size = 10000);
    ~contacts_t();

    qsizetype size() const;
    qsizetype add(const contact_t&& value);
    qsizetype emplaceBack(const contact_t&& value);
    contact_t at(qsizetype index) const;

    void clear();

    // Iterator begin() { return Iterator(this, 0); }
    // Iterator end() { return Iterator(this, size()); }

    private:
    qsizetype m_size;
    const qsizetype m_chunk_size;
    QList<contact_chunk_t*> m_chunks{};
    QMap<qsizetype, QString> m_longValue{};
};
