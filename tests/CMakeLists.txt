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

target_link_libraries(test_graph gtest_main gmock_main glthreads graph)

add_test(
    NAME test_graph
    COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_graph
)

add_executable(
    test_utils
    test_utils.cpp
)

target_link_libraries(test_utils gtest_main gmock_main glthreads utils)

add_test(
    NAME test_utils
    COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_utils
)

add_executable(
    test_net
    test_net.cpp
)

target_link_libraries(test_net gtest_main gmock_main glthreads net utils)

add_test(
    NAME test_net
    COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_net
)

