cmake_minimum_required(VERSION 3.30)

project(CollAuth)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "src")
set(INCLUDE_DIR "include")

set(SOURCES
  "${SOURCE_DIR}/main.cpp"
  "${SOURCE_DIR}/networking/Socket.cpp"
)

set(HEADERS
  "${INCLUDE_DIR}/networking/Socket.hpp"
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
