cmake_minimum_required(VERSION 3.10)
project(tlsock CXX)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(
    tlsock_lib STATIC 
        ./src/socket.cpp 
        ./src/tcp_connection.cpp
        ./src/server.cpp
        ./src/context.cpp
        ./src/tls_connection.cpp 
)

add_executable(tlsock 
    ./src/main.cpp
)

add_executable(tlsock_tests tests/test_connection.cpp)

target_include_directories(tlsock_lib PUBLIC src)
target_link_libraries(tlsock_lib PUBLIC OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(tlsock tlsock_lib)
target_link_libraries(tlsock_tests tlsock_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tlsock_tests)