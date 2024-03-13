#pragma once

#include "contact.h"
#include "strings.h"

#include <QList>
#include <QMap>

class contacts_t
{

    public:
    contacts_t(const size_t  chunk_size = STRING_BUFFER_SIZE);
    ~contacts_t();

    qsizetype count() const;
    void add(const contact_t &&value);

    strings* getStrings();

    void clear();

    private:
    size_t m_count;
    const size_t m_chunk_size;
    strings m_strings;

};
