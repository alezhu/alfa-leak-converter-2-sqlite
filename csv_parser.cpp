#include "csv_parser.h"

void csv_parser::_skipBOM() {
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

csv_parser::csv_parser(const QString &path) : m_file{path}, m_lpStart{nullptr} {
  m_file.open(QIODevice::OpenModeFlag::ReadOnly |
              QIODevice::OpenModeFlag::ExistingOnly);
  auto lpStart = m_file.map(0, m_file.size());
  auto lpEnd = lpStart + m_file.size();
  m_file.close();
  m_lpStart = static_cast<char16_t *>(static_cast<void *>(lpStart));
  m_lpEnd = static_cast<char16_t *>(static_cast<void *>(lpEnd));
  m_lpCurrent = m_lpStart;

  _skipBOM();
}

bool csv_parser::next() {
  if (m_lpCurrent < m_lpEnd) {
    _parse_row();
    return true;
  }
  return false;
}

QStringList &csv_parser::values() { return m_current; }

uint csv_parser::fieldIndex(const fields field) const {
  return static_cast<uint>(field);
}

const QString &csv_parser::value(const fields field) const {
  return m_current.at(fieldIndex(field));
}

quint64 csv_parser::position() const {
  auto result = static_cast<char *>(static_cast<void *>(m_lpCurrent)) -
                static_cast<char *>(static_cast<void *>(m_lpStart));
  return result;
}

quint64 csv_parser::size() const {
  auto result = static_cast<char *>(static_cast<void *>(m_lpEnd)) -
                static_cast<char *>(static_cast<void *>(m_lpStart));
  return result;
}

void csv_parser::setPosition(quint64 value) {
  auto result = static_cast<char *>(static_cast<void *>(m_lpStart)) + value;
  m_lpCurrent = static_cast<char16_t *>(static_cast<void *>(result));
  _skipBOM();
}

void csv_parser::_parse_row() {
  m_current.clear();
  for (int i = 0; i < 6; ++i) {
    m_current << _token();
  }
}

char16_t csv_parser::_pop() { return *m_lpCurrent++; }

char16_t csv_parser::_peek() { return *m_lpCurrent; }

void csv_parser::_skip(int count) { m_lpCurrent += count; }

const auto NL = char16_t('\n');
const auto CR = char16_t('\r');
const auto COMMA = char16_t(';');

QString csv_parser::_token() {
  const auto lp_Start = m_lpCurrent;
  auto lp_End = m_lpCurrent;
  auto stop = false;
  while (!stop && m_lpCurrent < m_lpEnd) {
    lp_End = m_lpCurrent;
    auto ch = _pop();
    switch (ch) {
    case NL:
    case CR:
      while (m_lpCurrent < m_lpEnd) {
        auto next = _peek();
        if (next == NL || next == CR)
          _skip();
        break;
      }
      stop = true;
      break;
    case COMMA:
      stop = true;
      break;
    }
  }
  return QString::fromUtf16(lp_Start, lp_End - lp_Start);
}
