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
  src/main.cpp
  src/user.h
  src/users_t.h
  src/users_t.cpp
  src/contact.h
  src/contacts_t.h
  src/contacts_t.cpp
  src/card.h
  src/cards_t.h
  src/cards_t.cpp
  src/common.h
  src/csv_parser.h
  src/csv_parser.cpp
  src/context.h
  src/context.cpp
  src/database.h
  src/database.cpp
  src/loger.h
  src/loger.cpp
  src/state.h
  src/runnable_base.h src/runnable_base.cpp
  src/runnable_parser.h src/runnable_parser.cpp
  src/runnable_db.h src/runnable_db.cpp

  src/MainWindow.cpp
  src/MainWindow.h
  src/MainWindow.ui
  src/global.h
  src/common.cpp
  src/model.h
  src/model.cpp
  src/string_buffer.h
  src/string_buffer.cpp
  src/strings.h
  src/strings.cpp
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
