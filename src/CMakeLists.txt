cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

add_executable(QTTest main.cpp
        FilesysSim_ui/helpers/PathHelper.cpp
        FilesysSim_ui/helpers/PathHelper.h)
target_link_libraries(QTTest
        Qt5::Core
)

