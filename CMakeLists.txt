cmake_minimum_required(VERSION 3.5)

project(virtual-trying-on VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(Path to your python include directory)

link_directories(Path to your python lib directory)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(virtual-trying-on
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        pycaller.h pycaller.cpp
        GroupItem.h
        GroupItem.cpp
        GarmentPageWidget.h GarmentPageWidget.cpp
        SingleImageItem.h
        SingleImageItem.cpp
        buttonImg.qrc
        ModelPageWidget.h
        ModelPageWidget.cpp
        util.h
        util.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET virtual-trying-on APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(virtual-trying-on SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(virtual-trying-on
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(virtual-trying-on PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(virtual-trying-on PRIVATE python3.12)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.virtual-trying-on)
endif()
set_target_properties(virtual-trying-on PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS virtual-trying-on
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(virtual-trying-on)
endif()
