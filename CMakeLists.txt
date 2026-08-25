cmake_minimum_required(VERSION 3.16)

project(qt_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Test)

qt_standard_project_setup()

add_subdirectory(keypr-core)

qt_add_executable(qt_client
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    resources.qrc
    src/sideBar/vaultselection.h src/sideBar/vaultselection.cpp src/sideBar/vaultselection.ui
    src/utils/clickablewidget.h src/utils/clickablewidget.cpp
    src/mainContent/novaultselected.h src/mainContent/novaultselected.cpp src/mainContent/novaultselected.ui
    src/mainContent/unlockvaultmodal.h src/mainContent/unlockvaultmodal.cpp src/mainContent/unlockvaultmodal.ui
    src/component/passwordinput.h src/component/passwordinput.cpp src/component/passwordinput.ui
    src/component/linetextinput.h src/component/linetextinput.cpp src/component/linetextinput.ui
    src/component/textinput.h src/component/textinput.cpp src/component/textinput.ui
    src/formOverlay/createvaultoverlay.h src/formOverlay/createvaultoverlay.cpp src/formOverlay/createvaultoverlay.ui
)

target_include_directories(qt_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(qt_client
    PRIVATE
        Qt6::Widgets
        keypr-core
)

set_target_properties(qt_client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt_client
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

include(GNUInstallDirs)
install(TARGETS qt_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CTest)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
