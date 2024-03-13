#include "csv_parser.h"

void csv_parser::_skipBOM()
{
    const auto bom = QChar(0xFEFF);
    const auto bom2 = QChar(0xFFFE);
    do {
        auto ch = _peek();
        if (ch == bom || ch == bom2) {
            _skip();
        } else {
            break;
        }
    } while (true);
}

csv_parser::csv_parser(const QString& path)
    : m_file{path}
    , m_lpStart{nullptr}
{
    m_file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::ExistingOnly);
    auto lpStart = m_file.map(0, m_file.size());
    auto lpEnd = lpStart + m_file.size();
    m_file.close();
    m_lpStart = static_cast<char16_t*>(static_cast<void*>(lpStart));
    m_lpEnd = static_cast<char16_t*>(static_cast<void*>(lpEnd));
    m_lpCurrent = m_lpStart;

    _skipBOM();
}

bool csv_parser::next()
{
    if (m_lpCurrent < m_lpEnd) {
        _parse_row();
        return true;
    }
    return false;
}

QStringList& csv_parser::values()
{
    return m_current;
}

uint csv_parser::fieldIndex(const fields field) const
{
    return static_cast<uint>(field);
}

QString EMPTY_STRING;
const QString& csv_parser::value(const fields field) const
{
    auto index = fieldIndex(field);
    if (index < m_current.size())
        return m_current.at(index);
    return EMPTY_STRING;
}

quint64 csv_parser::position() const
{
    auto result = static_cast<char*>(static_cast<void*>(m_lpCurrent))
                  - static_cast<char*>(static_cast<void*>(m_lpStart));
    return result;
}

quint64 csv_parser::size() const
{
    auto result = static_cast<char*>(static_cast<void*>(m_lpEnd))
                  - static_cast<char*>(static_cast<void*>(m_lpStart));
    return result;
}

void csv_parser::setPosition(quint64 value)
{
    auto result = static_cast<char*>(static_cast<void*>(m_lpStart)) + value;
    m_lpCurrent = static_cast<char16_t*>(static_cast<void*>(result));
    _skipBOM();
}

void csv_parser::_parse_row()
{
    m_current.clear();
    bool rowEnd = false;
    for (int i = 0; i < 6 && !rowEnd; ++i) {
        m_current << _token(rowEnd);
    }
}

inline char16_t csv_parser::_pop()
{
    return *m_lpCurrent++;
}

inline char16_t csv_parser::_peek()
{
    return *m_lpCurrent;
}

inline void csv_parser::_skip(int count)
{
    m_lpCurrent += count;
}

const auto NL = char16_t('\n');
const auto CR = char16_t('\r');
const auto COMMA = char16_t(',');
const auto SEMICOLON = char16_t(';');
const auto DQUOTE = char16_t('"');
const auto SLASH = char16_t('\\');

QString csv_parser::_token(bool& rowEnd)
{
    auto lp_Start = m_lpCurrent;
    auto lp_End = m_lpCurrent;
    bool stop = false;
    bool quoted = false;
    bool inString = false;
    while (!stop && m_lpCurrent < m_lpEnd) {
        lp_End = m_lpCurrent;
        auto ch = _pop();
        switch (ch) {
            case DQUOTE:
                if (*(m_lpEnd - 1) != SLASH) {
                    quoted = true;
                    if (!inString) {
                        lp_Start = m_lpCurrent; //Next
                    }
                    inString = !inString;
                }
                break;
            case CR:
                if (!inString) {
                    while (m_lpCurrent < m_lpEnd) {
                        auto next = _peek();
                        if (next == NL)
                            _skip();
                        break;
                    }
                    rowEnd = true;
                    stop = true;
                }
                break;
            case NL:
                if (!inString) {
                    rowEnd = true;
                    stop = true;
                }
                break;
            case SEMICOLON:
                if (!inString) {
                    stop = true;
                }
                break;
        }
    }
    auto result = QString::fromUtf16(lp_Start, lp_End - lp_Start - ((quoted) ? 1 : 0));
    if (quoted) {
        result = result.remove(CR).replace(NL, QChar(' ')).replace("\\\"", "\"");
    }
    return result;
}
