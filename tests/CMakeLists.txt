
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

qt_add_executable(tst_nativemessaging
    tst_nativemessaging.cpp
    ../src/nativeMessaging/nativemessaging.h ../src/nativeMessaging/nativemessaging.cpp
)

target_link_libraries(tst_nativemessaging
    PRIVATE
        Qt6::Test
)

add_test(NAME tst_nativemessaging COMMAND tst_nativemessaging)
