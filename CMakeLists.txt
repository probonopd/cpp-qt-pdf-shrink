CMAKE_MINIMUM_REQUIRED(VERSION 3.4)

PROJECT(pdf-shrink)

SET(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)

ADD_EXECUTABLE(pdf-shrink
    src/main.cpp
    src/mainwindow.cpp
    assets/resources.qrc
)

target_link_libraries(pdf-shrink
    Qt5::Widgets
)
