add_executable(
    test_glthreads
    test_glthreads.cpp
)

target_link_libraries(test_glthreads gtest_main gmock_main glthreads)

add_test(
    NAME test_glthreads
    COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_glthreads
)

add_executable(
    test_graph
    test_graph.cpp
)

target_link_libraries(test_graph gtest_main gmock_main glthreads)

add_test(
    NAME test_graph
    COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_graph
)

