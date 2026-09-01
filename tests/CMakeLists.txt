
qt_add_executable(tst_vaultcontroller
    fakevaultrepository.h
    tst_vaultcontroller.cpp
    ../src/vaultcontroller.cpp ../src/vaultcontroller.h
)

target_link_libraries(tst_vaultcontroller
    PRIVATE
        Qt6::Test
        keypr-core
)

add_test(NAME tst_vaultcontroller COMMAND tst_vaultcontroller)

qt_add_executable(tst_mailaliascontroller
    fakemailaliasclient.h
    tst_mailaliascontroller.cpp
    ../src/mailaliascontroller.cpp ../src/mailaliascontroller.h
    ../src/mailaliasclient.cpp ../src/mailaliasclient.h
    ../src/appconfig.cpp ../src/appconfig.h
    ../src/jsonfileconfig.cpp ../src/jsonfileconfig.h
)

target_link_libraries(tst_mailaliascontroller
    PRIVATE
        Qt6::Test
        Qt6::Network
)

add_test(NAME tst_mailaliascontroller COMMAND tst_mailaliascontroller)

qt_add_executable(tst_vaultbridge
    tst_vaultbridge.cpp
    ../src/vaultbridge.cpp ../src/vaultbridge.h
    ../src/vaultcontroller.cpp ../src/vaultcontroller.h
)

target_include_directories(tst_vaultbridge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(tst_vaultbridge
    PRIVATE
        Qt6::Test
        keypr-core
)

add_test(NAME tst_vaultbridge COMMAND tst_vaultbridge)
