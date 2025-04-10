cmake_minimum_required(VERSION 3.30)
project(FLA_Nodes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 헤더 파일 디렉토리를 명시적으로 추가
include_directories(${CMAKE_SOURCE_DIR}/include)

# 소스 파일과 헤더 파일을 변수로 관리
set(SOURCES
        src/main.cpp
        src/version.cpp
        src/network/network.cpp
)

set(HEADERS
        include/version.h
        include/network/network.h
)

# 실행 파일 생성
add_executable(Nodes ${SOURCES} ${HEADERS})

# 컴파일러 경고를 활성화 (선택 사항)
target_compile_options(Nodes PRIVATE -Wall -Wextra -Wpedantic)