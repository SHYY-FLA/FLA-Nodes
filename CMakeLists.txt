cmake_minimum_required(VERSION 3.30)
project(FLA_Nodes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Test 서브모듈 포함
add_subdirectory(lib/googletest)

# 헤더 파일 디렉토리 설정
include_directories(${CMAKE_SOURCE_DIR}/include)

# 소스 파일 정의
set(SOURCES
        src/main.cpp
        src/version.cpp
)

set(HEADERS
        include/version.h
)

# 네트워크 라이브러리 생성
add_library(NetworkLib STATIC
        src/network/network.cpp
        include/network/network.h
)

# 메인 실행 파일
add_executable(Nodes ${SOURCES} ${HEADERS})
target_link_libraries(Nodes PRIVATE NetworkLib)

# 테스트 파일 설정
set(TESTS test/network/network_test.cpp)

# 테스트 실행 파일
add_executable(NetworkTest ${TESTS})
target_link_libraries(NetworkTest
        PRIVATE
        NetworkLib
        GTest::gtest_main
        GTest::gmock
        pthread
)

# 테스트 활성화
enable_testing()
add_test(NAME NetworkTests COMMAND NetworkTest)