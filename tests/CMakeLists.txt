qt_add_executable(tst_example
    tst_simple_test.cpp
)

target_link_libraries(tst_example
    PRIVATE
        Qt6::Test
        keypr-core
)

add_test(NAME tst_example COMMAND tst_example)


qt_add_executable(tst_crypto
    tst_crypto_test.cpp
)

target_link_libraries(tst_crypto
    PRIVATE
        Qt6::Test
        keypr-core
)

add_test(NAME tst_crypto COMMAND tst_crypto)
