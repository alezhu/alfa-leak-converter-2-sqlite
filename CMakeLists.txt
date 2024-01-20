cmake_minimum_required(VERSION 3.14)

project(qt_alfa_leak LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Widgets)

add_executable(qt_alfa_leak
  main.cpp
  user.h users_t.h users_t.cpp
  contact.h contacts_t.h contacts_t.cpp
  card.h cards_t.h cards_t.cpp
  common.h
  csv_parser.h csv_parser.cpp
  context.h context.cpp
  database.h database.cpp
  loger.h loger.cpp
  state.h
  runnable_base.h runnable_base.cpp
  runnable_parser.h runnable_parser.cpp
  runnable_db.h runnable_db.cpp

  MainWindow.cpp
  MainWindow.h
  MainWindow.ui
  global.h
  common.cpp
  model.h model.cpp
)
target_link_libraries(qt_alfa_leak Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(qt_alfa_leak PROPERTIES
    WIN32_EXECUTABLE FALSE
)

include(GNUInstallDirs)
install(TARGETS qt_alfa_leak
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
