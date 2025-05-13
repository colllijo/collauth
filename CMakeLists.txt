cmake_minimum_required(VERSION 3.30)

project(CollAuth)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(SOURCES
  "${SOURCE_DIR}/main.cpp"
  "${SOURCE_DIR}/cryptography/Random.cpp"
  "${SOURCE_DIR}/cryptography/SHA256.cpp"
  "${SOURCE_DIR}/cryptography/ecc/EllipticCurve.cpp"
  "${SOURCE_DIR}/cryptography/ecc/Point.cpp"
  "${SOURCE_DIR}/cryptography/ecc/WeierstrassCurve.cpp"
  "${SOURCE_DIR}/events/EpollPoller.cpp"
  "${SOURCE_DIR}/events/SelectPoller.cpp"
  "${SOURCE_DIR}/http/HTTPMethod.cpp"
  "${SOURCE_DIR}/http/HTTPParser.cpp"
  "${SOURCE_DIR}/http/HTTPStatus.cpp"
  "${SOURCE_DIR}/http/HTTPResponse.cpp"
  "${SOURCE_DIR}/http/HTTPRequest.cpp"
  "${SOURCE_DIR}/http/HTTPVersion.cpp"
  "${SOURCE_DIR}/logging/Logger.cpp"
  "${SOURCE_DIR}/math/Number.cpp"
  "${SOURCE_DIR}/networking/BufferReader.cpp"
  "${SOURCE_DIR}/networking/BufferWriter.cpp"
  "${SOURCE_DIR}/networking/Connection.cpp"
  "${SOURCE_DIR}/networking/ConnectionFactory.cpp"
  "${SOURCE_DIR}/networking/HTTPConnection.cpp"
  "${SOURCE_DIR}/networking/HTTPSConnection.cpp"
  "${SOURCE_DIR}/networking/Socket.cpp"
  "${SOURCE_DIR}/signal/SignalHandler.cpp"
  "${SOURCE_DIR}/tls/ClientHello.cpp"
  "${SOURCE_DIR}/tls/ServerHello.cpp"
  "${SOURCE_DIR}/tls/TLS.cpp"
  "${SOURCE_DIR}/tls/TLSContext.cpp"
  "${SOURCE_DIR}/tls/TLSKeyShare.cpp"
  "${SOURCE_DIR}/tls/TLSPlaintext.cpp"
  "${SOURCE_DIR}/tls/TLSServerName.cpp"
  "${SOURCE_DIR}/tls/TLSSignature.cpp"
  "${SOURCE_DIR}/tls/TLSVersion.cpp"
  "${SOURCE_DIR}/webserver/Middleware.cpp"
  "${SOURCE_DIR}/webserver/MiddlewareManager.cpp"
  "${SOURCE_DIR}/webserver/Router.cpp"
  "${SOURCE_DIR}/webserver/WebServer.cpp"
)

set(HEADERS
  "${INCLUDE_DIR}/cryptography/Random.hpp"
  "${INCLUDE_DIR}/cryptography/SHA256.hpp"
  "${INCLUDE_DIR}/cryptography/ecc/EllipticCurve.hpp"
  "${INCLUDE_DIR}/cryptography/ecc/Point.hpp"
  "${INCLUDE_DIR}/cryptography/ecc/WeierstrassCurve.hpp"
  "${INCLUDE_DIR}/events/EventPoller.hpp"
  "${INCLUDE_DIR}/events/EpollPoller.hpp"
  "${INCLUDE_DIR}/events/SelectPoller.hpp"
  "${INCLUDE_DIR}/http/HTTPMethod.hpp"
  "${INCLUDE_DIR}/http/HTTPParser.hpp"
  "${INCLUDE_DIR}/http/HTTPStatus.hpp"
  "${INCLUDE_DIR}/http/HTTPResponse.hpp"
  "${INCLUDE_DIR}/http/HTTPRequest.hpp"
  "${INCLUDE_DIR}/http/HTTPVersion.hpp"
  "${INCLUDE_DIR}/math/Number.hpp"
  "${INCLUDE_DIR}/logging/Logger.hpp"
  "${INCLUDE_DIR}/networking/BufferReader.hpp"
  "${INCLUDE_DIR}/networking/BufferWriter.hpp"
  "${INCLUDE_DIR}/networking/Connection.hpp"
  "${INCLUDE_DIR}/networking/ConnectionFactory.hpp"
  "${INCLUDE_DIR}/networking/HTTPConnection.hpp"
  "${INCLUDE_DIR}/networking/HTTPSConnection.hpp"
  "${INCLUDE_DIR}/networking/Socket.hpp"
  "${INCLUDE_DIR}/signal/SignalHandler.hpp"
  "${INCLUDE_DIR}/tls/ClientHello.hpp"
  "${INCLUDE_DIR}/tls/ServerHello.hpp"
  "${INCLUDE_DIR}/tls/TLS.hpp"
  "${INCLUDE_DIR}/tls/TLSContext.hpp"
  "${INCLUDE_DIR}/tls/TLSKeyShare.hpp"
  "${INCLUDE_DIR}/tls/TLSPlaintext.hpp"
  "${INCLUDE_DIR}/tls/TLSServerName.hpp"
  "${INCLUDE_DIR}/tls/TLSSignature.hpp"
  "${INCLUDE_DIR}/tls/TLSVersion.hpp"
  "${INCLUDE_DIR}/webserver/Middleware.hpp"
  "${INCLUDE_DIR}/webserver/MiddlewareManager.hpp"
  "${INCLUDE_DIR}/webserver/Router.hpp"
  "${INCLUDE_DIR}/webserver/WebServer.hpp"
)

include_directories(${INCLUDE_DIR})

add_custom_target(format
    COMMAND clang-format -i -Werror ${SOURCES} ${HEADERS}
    COMMENT "Formatting source files with clang-format"
)

add_custom_target(format-check
    COMMAND clang-format --dry-run -Werror ${SOURCES} ${HEADERS}
    COMMENT "Checking source files with clang-format"
)

add_custom_target(tidy
    COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} ${SOURCES} ${HEADERS}
            --format-style=file
    COMMENT "Running clang-tidy without fixing"
)

add_custom_target(tidy-fix
    COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} ${SOURCES} ${HEADERS}
            --fix --format-style=file
    COMMENT "Running clang-tidy with fixes"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
