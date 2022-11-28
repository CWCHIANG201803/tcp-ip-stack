add_library(graph graph.h graph.cpp)
target_link_libraries(graph glthreads)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
