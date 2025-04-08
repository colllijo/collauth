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
  "${SOURCE_DIR}/http/HttpMethod.cpp"
  "${SOURCE_DIR}/http/HttpParser.cpp"
  "${SOURCE_DIR}/http/HttpStatus.cpp"
  "${SOURCE_DIR}/http/HttpResponse.cpp"
  "${SOURCE_DIR}/http/HttpRequest.cpp"
  "${SOURCE_DIR}/http/HttpVersion.cpp"
  "${SOURCE_DIR}/logging/Logger.cpp"
  "${SOURCE_DIR}/networking/Connection.cpp"
  "${SOURCE_DIR}/networking/ConnectionFactory.cpp"
  "${SOURCE_DIR}/networking/HttpConnection.cpp"
  "${SOURCE_DIR}/networking/HttpsConnection.cpp"
  "${SOURCE_DIR}/networking/Socket.cpp"
  "${SOURCE_DIR}/signal/SignalHandler.cpp"
  "${SOURCE_DIR}/tls/ByteBuffer.cpp"
  "${SOURCE_DIR}/tls/TLS.cpp"
  "${SOURCE_DIR}/webserver/Middleware.cpp"
  "${SOURCE_DIR}/webserver/MiddlewareManager.cpp"
  "${SOURCE_DIR}/webserver/Router.cpp"
  "${SOURCE_DIR}/webserver/WebServer.cpp"
)

set(HEADERS
  "${INCLUDE_DIR}/events/EventPoller.hpp"
  "${INCLUDE_DIR}/events/EpollPoller.hpp"
  "${INCLUDE_DIR}/events/SelectPoller.hpp"
  "${INCLUDE_DIR}/http/HttpMethod.hpp"
  "${INCLUDE_DIR}/http/HttpParser.hpp"
  "${INCLUDE_DIR}/http/HttpStatus.hpp"
  "${INCLUDE_DIR}/http/HttpResponse.hpp"
  "${INCLUDE_DIR}/http/HttpRequest.hpp"
  "${INCLUDE_DIR}/http/HttpVersion.hpp"
  "${INCLUDE_DIR}/logging/Logger.hpp"
  "${INCLUDE_DIR}/networking/Connection.hpp"
  "${INCLUDE_DIR}/networking/ConnectionFactory.hpp"
  "${INCLUDE_DIR}/networking/HttpConnection.hpp"
  "${INCLUDE_DIR}/networking/HttpsConnection.hpp"
  "${INCLUDE_DIR}/networking/Socket.hpp"
  "${INCLUDE_DIR}/signal/SignalHandler.hpp"
  "${INCLUDE_DIR}/tls/ByteBuffer.hpp"
  "${INCLUDE_DIR}/tls/TLS.hpp"
  "${INCLUDE_DIR}/webserver/Middleware.hpp"
  "${INCLUDE_DIR}/webserver/MiddlewareManager.hpp"
  "${INCLUDE_DIR}/webserver/Router.hpp"
  "${INCLUDE_DIR}/webserver/WebServer.hpp"
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
