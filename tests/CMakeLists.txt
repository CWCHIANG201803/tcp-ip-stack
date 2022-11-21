add_executable(
    test_glthreads
    test_glthreads.cpp
)

target_link_libraries(test_glthreads gtest_main gmock_main glthreads)

add_test(
    NAME unit
    COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_glthreads
)


