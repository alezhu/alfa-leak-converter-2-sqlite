#include "string_buffer.h"

string_buffer::string_buffer(size_t capacity):m_capacity{capacity},
    m_buffer{new QChar[m_capacity]}
{}

QString string_buffer::toString() const
{
    return QString::fromRawData(m_buffer.get(), m_size);
}

bool string_buffer::append(QStringView value)
{

    auto len = value.size();
    if (len == 0)
        return true;

    auto new_size = m_size + len;
    if (new_size >= m_capacity)
        return false;

    auto dst = m_buffer.get() + m_size;

    memcpy(dst, value.data(), len*sizeof(QChar));
    m_size = new_size;
    dst += len;
    *dst = QChar::SpecialCharacter::Null;
    return true;
}

void string_buffer::lock()
{
    auto dst = m_buffer.get() + m_size - 1;
    if (*dst == u',') {
        *dst = QChar::SpecialCharacter::Null;
        m_size++;
    }
}


