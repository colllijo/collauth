cmake_minimum_required(VERSION 3.30)

project(CollAuth)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "src")
set(INCLUDE_DIR "include")

set(SOURCES
  "${SOURCE_DIR}/main.cpp"
  "${SOURCE_DIR}/events/EpollPoller.cpp"
  "${SOURCE_DIR}/events/SelectPoller.cpp"
  "${SOURCE_DIR}/networking/Socket.cpp"
  "${SOURCE_DIR}/networking/WebServer.cpp"
  "${SOURCE_DIR}/signal/SignalHandler.cpp"
)

set(HEADERS
  "${INCLUDE_DIR}/events/EventPoller.hpp"
  "${INCLUDE_DIR}/events/EpollPoller.hpp"
  "${INCLUDE_DIR}/events/SelectPoller.hpp"
  "${INCLUDE_DIR}/networking/Socket.hpp"
  "${INCLUDE_DIR}/networking/WebServer.hpp"
  "${INCLUDE_DIR}/signal/SignalHandler.hpp"
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
