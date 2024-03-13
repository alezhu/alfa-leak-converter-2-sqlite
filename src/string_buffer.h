#pragma once

#include "global.h"
#include <QChar>
#include <QScopedPointer>
#include <QString>
class string_buffer
{
    public:
    string_buffer(size_t capacity = STRING_BUFFER_SIZE);
    QString toString() const;
    bool append(QStringView value);

    void lock();

    protected:
    size_t m_capacity;
    size_t m_size{0};
    QScopedPointer<QChar> m_buffer;
};
