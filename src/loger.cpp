#include "loger.h"

#ifdef _WINDOWS
#include <QMutex>
#include <Windows.h>
#endif

loger::loger() {
#ifdef _WINDOWS
  SetConsoleOutputCP(CP_UTF8);
#endif
  out.setEncoding(QStringConverter::Utf8);
}

loger &loger::get() {
  static loger instance;
  return instance;
}

QMutex mutex;

QTextStream &loger::operator<<(QAnyStringView s) {
  return operator<<(s.toString());
}

QTextStream &loger::operator<<(const QString &s) {
  QMutexLocker loker(&mutex);
  return out << s << Qt::endl;
}

QTextStream &loger::operator<<(const char* s) {
    QMutexLocker loker(&mutex);
    return out << s << Qt::endl;
}

QTextStream &loger::operator<<(QLatin1StringView s) {
  QMutexLocker loker(&mutex);
  return out << s << Qt::endl;
}

QTextStream &loger::operator<<(QStringView s) {
  QMutexLocker loker(&mutex);
  return out << s << Qt::endl;
}
