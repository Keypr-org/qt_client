cmake_minimum_required(VERSION 3.16)

project(qt_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt_client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(qt_client SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(qt_client
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    set(APP_ICON_ICO "${CMAKE_SOURCE_DIR}/resources/app_icon.ico")

    if(EXISTS "${APP_ICON_ICO}")
        set(APP_ICON_RC "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc")

        file(
            WRITE
            "${APP_ICON_RC}"
            "IDI_ICON1 ICON \"${APP_ICON_ICO}\"\n"
        )

        target_sources(qt_client PRIVATE "${APP_ICON_RC}")
    else()
        message(WARNING "app_icon.ico introuvable (${APP_ICON_ICO}), l'exécutable sera compilé sans icône personnalisée.")
    endif()
endif()

if(APPLE)
    set(APP_ICON_ICNS "${CMAKE_SOURCE_DIR}/resources/app_icon.icns")

    if(EXISTS "${APP_ICON_ICNS}")
        set_source_files_properties(
            "${APP_ICON_ICNS}"
            PROPERTIES MACOSX_PACKAGE_LOCATION Resources
        )

        target_sources(
            qt_client
            PRIVATE
            "${APP_ICON_ICNS}"
        )

        set_target_properties(
            qt_client
            PROPERTIES
            MACOSX_BUNDLE_ICON_FILE app_icon.icns
        )
    else()
        message(WARNING "app_icon.icns introuvable (${APP_ICON_ICNS}), le bundle .app sera compilé sans icône personnalisée.")
    endif()
endif()

target_link_libraries(qt_client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt_client)
endif()

set_target_properties(qt_client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS qt_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt_client)
endif()

# Tests

enable_testing()

add_executable(tst_example
    tests/tst_simple_test.cpp
)

target_link_libraries(tst_example
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
)

add_test(
    NAME tst_example
    COMMAND tst_example
)