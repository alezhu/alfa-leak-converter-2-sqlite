#include <QFile>
#include <QString>
#include <QTextStream>

#pragma once



class csv_parser
{
public:
    enum class fields {
        user_id,
        name,
        birth_date,
        contact,
        card,
        expiredate
    };


    csv_parser(const QString &path);
    bool next();
    QStringList &values();

    inline uint fieldIndex(const fields field) const;
    const QString& value(const fields field) const;
    quint64 position() const;
    quint64 size() const;
    void setPosition(quint64 value);
private:
    QFile m_file;
    // QByteArray m_byteArray;
    char16_t* m_lpStart;
    char16_t* m_lpEnd;
    char16_t* m_lpCurrent;
    QStringList m_current;

    void _parse_row();
    char16_t _pop();
    char16_t _peek();
    void _skip(int count = 1);
    QString _token();
    void _skipBOM();
};
