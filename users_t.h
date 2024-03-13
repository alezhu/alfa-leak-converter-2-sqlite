#include "strings.h"
#include "user.h"
#include <QChar>
#include <QMap>

#pragma once


class users_t
{
    public:
    users_t(const size_t  chunk_size = STRING_BUFFER_SIZE);
    ~users_t();

    qsizetype count() const;
    void add(const user_t &&value);

    strings* getStrings();

    void clear();

    private:
    size_t m_count;
    const size_t m_chunk_size;
    strings m_strings;

};
