#include "MainWindow.h"
#include "loger.h"
#include "Windows.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {

#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        freopen_s(nullptr,"CONOUT$", "w", stdout);
        freopen_s(nullptr,"CONOUT$", "w", stderr);
    }
#endif

  QApplication a(argc, argv);

  auto &out = loger::get();

  auto args = a.arguments();
  if (args.count() < 3) {
    out << "Usage syntax: qt_alfa_leak <path-to-csv-file> "
           "<path-to-db-file>";
    return 1;
  }

  auto csvFile = args[1];
  if (!QFile::exists(csvFile)) {
    out << "File " << csvFile << " not found";
    return 2;
  }

  auto dbFile = args[2];
  model model{csvFile, dbFile};

  MainWindow w{model};
  w.show();

  return a.exec();
}
