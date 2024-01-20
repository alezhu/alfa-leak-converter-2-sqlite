#include "common.h"

date_t toDate(const QString &value)
{
    date_t result = 0;
    if (value.size() < 10)
        return result;
    for (int i = 0; i < 10; ++i) {
        auto ch = value.at(i);
        if (ch == '-') {
            continue;
        }
        if (ch == ' ') {
            break;
        }
        result = result * 10 + (ch.toLatin1() - '0');
    }
    return result;
}

QString toDateEx(const date_t value)
{
    auto temp = QString::number(value);

    return temp.left(4) + '-' + temp.mid(4, 2) + '-' + temp.mid(6);
}

contact_type determineContactType(const QString& value)
{
    if (value.isEmpty())
        return contact_type::other;
    if (value.contains('@'))
        return contact_type::email;
    bool ok{0};
    if (value.toULongLong(&ok) > 0 && ok)
        return contact_type::phone;
    return contact_type::other;
}
