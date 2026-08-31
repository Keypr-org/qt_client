
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
    ../src/mailaliasconfig.cpp ../src/mailaliasconfig.h
)

target_link_libraries(tst_mailaliascontroller
    PRIVATE
        Qt6::Test
        Qt6::Network
)

add_test(NAME tst_mailaliascontroller COMMAND tst_mailaliascontroller)
