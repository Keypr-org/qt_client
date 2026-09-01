cmake_minimum_required(VERSION 3.16)

project(qt_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
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
    src/mainContent/viewentries.h src/mainContent/viewentries.cpp src/mainContent/viewentries.ui
    src/component/entrieitem.h src/component/entrieitem.cpp src/component/entrieitem.ui
    src/formOverlay/createvaultoverlay.h src/formOverlay/createvaultoverlay.cpp src/formOverlay/createvaultoverlay.ui
    src/sideBar/categoriesselection.h src/sideBar/categoriesselection.cpp src/sideBar/categoriesselection.ui
    src/formOverlay/newentryoverlay.h src/formOverlay/newentryoverlay.cpp src/formOverlay/newentryoverlay.ui
    src/formOverlay/createcategoryoverlay.h src/formOverlay/createcategoryoverlay.cpp src/formOverlay/createcategoryoverlay.ui
    src/utils/hoverwidget.h src/utils/hoverwidget.cpp
    src/mainContent/addwebsiteform.h src/mainContent/addwebsiteform.cpp src/mainContent/addwebsiteform.ui
    src/mainContent/entries/websiteentry.h src/mainContent/entries/websiteentry.cpp src/mainContent/entries/websiteentry.ui
    src/component/card.h src/component/card.cpp src/component/card.ui
    src/utils/cardnumberutils.h src/utils/cardnumberutils.cpp
    src/mainContent/creditcardform.h src/mainContent/creditcardform.cpp src/mainContent/creditcardform.ui
    src/component/cardnumberinput.h src/component/cardnumberinput.cpp src/component/cardnumberinput.ui
    src/mainContent/entries/creditcardentry.h src/mainContent/entries/creditcardentry.cpp src/mainContent/entries/creditcardentry.ui
    src/mainContent/wifiform.h src/mainContent/wifiform.cpp src/mainContent/wifiform.ui
    src/mainContent/entries/wifientry.h src/mainContent/entries/wifientry.cpp src/mainContent/entries/wifientry.ui
    src/mainContent/passwordgenerator.h src/mainContent/passwordgenerator.cpp src/mainContent/passwordgenerator.ui
    src/component/toggleswitch.h src/component/toggleswitch.cpp
    src/component/notificationtooltip.h src/component/notificationtooltip.cpp src/component/notificationtooltip.ui
    src/mainContent/personadisplay.h src/mainContent/personadisplay.cpp src/mainContent/personadisplay.ui
    src/component/personaitem.h src/component/personaitem.cpp src/component/personaitem.ui
    src/mainContent/newpersonaform.h src/mainContent/newpersonaform.cpp src/mainContent/newpersonaform.ui
    src/formOverlay/editpersonaoverlay.h src/formOverlay/editpersonaoverlay.cpp src/formOverlay/editpersonaoverlay.ui
    src/model/persona.h
    src/model/personarepository.h src/model/personarepository.cpp
    src/model/entry.h
    src/model/websiteentrydata.h
    src/model/wifientrydata.h
    src/model/creditcardentrydata.h
    src/model/entryrepository.h src/model/entryrepository.cpp
    src/vaultcontroller.h src/vaultcontroller.cpp
    src/nativeMessaging/nativemessaging.h src/nativeMessaging/nativemessaging.cpp
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
