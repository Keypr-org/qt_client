cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
endif()

project(qt_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

add_subdirectory(keypr-core)

# ============================================
# Application principale (SANS tests)
# ============================================
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
        add_library(qt_client SHARED ${PROJECT_SOURCES})
    else()
        add_executable(qt_client ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(qt_client 
    PRIVATE 
        Qt${QT_VERSION_MAJOR}::Widgets
        keypr-core
)

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

# ============================================
# Tests
# ============================================
enable_testing()

# Test simple
add_executable(tst_example
    tests/tst_simple_test.cpp
)

target_link_libraries(tst_example 
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        keypr-core
)

add_test(NAME tst_example COMMAND tst_example)

# Test crypto (EXÉCUTABLE SÉPARÉ)
add_executable(tst_crypto
    tests/tst_crypto_test.cpp
)

target_link_libraries(tst_crypto 
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        keypr-core
)

add_test(NAME tst_crypto COMMAND tst_crypto)
