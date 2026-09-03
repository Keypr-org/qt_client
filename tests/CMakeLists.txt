
qt_add_executable(tst_vaultcontroller
    fakevaultrepository.h
    tst_vaultcontroller.cpp
    ../src/vaultcontroller.cpp ../src/vaultcontroller.h
    ../src/utils/qtypes/QCategory.cpp ../src/utils/qtypes/QCategory.h
    ../src/utils/qtypes/QCreditCard.cpp ../src/utils/qtypes/QCreditCard.h
    ../src/utils/qtypes/QEntry.cpp ../src/utils/qtypes/QEntry.h
    ../src/utils/qtypes/QPersona.cpp ../src/utils/qtypes/QPersona.h
    ../src/utils/qtypes/QWebsite.cpp ../src/utils/qtypes/QWebsite.h
    ../src/utils/qtypes/QWifi.cpp ../src/utils/qtypes/QWifi.h
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

