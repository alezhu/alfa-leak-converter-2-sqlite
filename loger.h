#include <QString>
#include <QStringView>
#include <QAnyStringView>
#include <QTextStream>
#include <QLatin1String>
#include <QLatin1StringView>

#pragma once

class loger {
public:
  static loger &get();


  QTextStream &operator<<(QAnyStringView s);
  QTextStream &operator<<(const QString &s);
  QTextStream &operator<<(const char*);
  QTextStream &operator<<(QStringView s);
  QTextStream &operator<<(QLatin1StringView s);

private:
  loger();
  QTextStream out{stdout};
};
