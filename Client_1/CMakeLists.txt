cmake_minimum_required(VERSION 3.10)
project(myproject)

set(NATS_LIB_DIR C:/library)

include_directories(${NATS_LIB_DIR}/include)

link_directories(${NATS_LIB_DIR}/lib)

set(SOURCES Clien_1.cpp)

add_executable(main ${SOURCES})

target_link_libraries(main nats)
