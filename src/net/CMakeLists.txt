add_library(net net.h net.cpp)
target_link_libraries(net graph)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
