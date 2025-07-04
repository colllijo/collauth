cmake_minimum_required(VERSION 3.28)

project(CollAuth VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(SOURCES
  # "${SOURCE_DIR}/cryptography/Random.cpp"
  # "${SOURCE_DIR}/cryptography/SHA256.cpp"
  # "${SOURCE_DIR}/cryptography/ecc/MontgomeryCurve.cpp"
  # "${SOURCE_DIR}/cryptography/ecc/Point.cpp"
  # "${SOURCE_DIR}/cryptography/ecc/WeierstrassCurve.cpp"
  # "${SOURCE_DIR}/events/EpollPoller.cpp"
  # "${SOURCE_DIR}/events/SelectPoller.cpp"
  # "${SOURCE_DIR}/http/HTTPMethod.cpp"
  # "${SOURCE_DIR}/http/HTTPParser.cpp"
  # "${SOURCE_DIR}/http/HTTPStatus.cpp"
  # "${SOURCE_DIR}/http/HTTPResponse.cpp"
  # "${SOURCE_DIR}/http/HTTPRequest.cpp"
  # "${SOURCE_DIR}/http/HTTPVersion.cpp"
  "${SOURCE_DIR}/logging/Logger.cpp"
  "${SOURCE_DIR}/math/Montgomery.cpp"
  "${SOURCE_DIR}/math/Number.cpp"
  "${SOURCE_DIR}/math/operations/Addition.cpp"
  "${SOURCE_DIR}/math/operations/Bitwise.cpp"
  "${SOURCE_DIR}/math/operations/Comparison.cpp"
  "${SOURCE_DIR}/math/operations/Division.cpp"
  "${SOURCE_DIR}/math/operations/Multiplication.cpp"
  "${SOURCE_DIR}/math/operations/Shifting.cpp"
  "${SOURCE_DIR}/math/operations/Subtraction.cpp"
  # "${SOURCE_DIR}/networking/BufferReader.cpp"
  # "${SOURCE_DIR}/networking/BufferWriter.cpp"
  # "${SOURCE_DIR}/networking/Connection.cpp"
  # "${SOURCE_DIR}/networking/ConnectionFactory.cpp"
  # "${SOURCE_DIR}/networking/HTTPConnection.cpp"
  # "${SOURCE_DIR}/networking/HTTPSConnection.cpp"
  # "${SOURCE_DIR}/networking/Socket.cpp"
  # "${SOURCE_DIR}/signal/SignalHandler.cpp"
  # "${SOURCE_DIR}/tls/ClientHello.cpp"
  # "${SOURCE_DIR}/tls/ServerHello.cpp"
  # "${SOURCE_DIR}/tls/TLS.cpp"
  # "${SOURCE_DIR}/tls/TLSContext.cpp"
  # "${SOURCE_DIR}/tls/TLSPlaintext.cpp"
  # "${SOURCE_DIR}/tls/TLSSignature.cpp"
  # "${SOURCE_DIR}/tls/TLSVersion.cpp"
  # "${SOURCE_DIR}/tls/extension/TLSExtension.cpp"
  # "${SOURCE_DIR}/tls/extension/TLSKeyShare.cpp"
  # "${SOURCE_DIR}/tls/extension/TLSServerName.cpp"
  # "${SOURCE_DIR}/webserver/Middleware.cpp"
  # "${SOURCE_DIR}/webserver/MiddlewareManager.cpp"
  # "${SOURCE_DIR}/webserver/Router.cpp"
  # "${SOURCE_DIR}/webserver/WebServer.cpp"
)

set(HEADERS
  # "${INCLUDE_DIR}/cryptography/Random.hpp"
  # "${INCLUDE_DIR}/cryptography/SHA256.hpp"
  # "${INCLUDE_DIR}/cryptography/ecc/MontgomeryCurve.hpp"
  # "${INCLUDE_DIR}/cryptography/ecc/Point.hpp"
  # "${INCLUDE_DIR}/cryptography/ecc/WeierstrassCurve.hpp"
  # "${INCLUDE_DIR}/events/EventPoller.hpp"
  # "${INCLUDE_DIR}/events/EpollPoller.hpp"
  # "${INCLUDE_DIR}/events/SelectPoller.hpp"
  # "${INCLUDE_DIR}/http/HTTPMethod.hpp"
  # "${INCLUDE_DIR}/http/HTTPParser.hpp"
  # "${INCLUDE_DIR}/http/HTTPStatus.hpp"
  # "${INCLUDE_DIR}/http/HTTPResponse.hpp"
  # "${INCLUDE_DIR}/http/HTTPRequest.hpp"
  # "${INCLUDE_DIR}/http/HTTPVersion.hpp"
  "${INCLUDE_DIR}/math/Montgomery.hpp"
  "${INCLUDE_DIR}/math/Number.hpp"
  "${INCLUDE_DIR}/math/operations/Addition.hpp"
  "${INCLUDE_DIR}/math/operations/Bitwise.hpp"
  "${INCLUDE_DIR}/math/operations/Comparison.hpp"
  "${INCLUDE_DIR}/math/operations/Division.hpp"
  "${INCLUDE_DIR}/math/operations/Multiplication.hpp"
  "${INCLUDE_DIR}/math/operations/Shifting.hpp"
  "${INCLUDE_DIR}/math/operations/Subtraction.hpp"
  "${INCLUDE_DIR}/logging/Logger.hpp"
  # "${INCLUDE_DIR}/networking/BufferReader.hpp"
  # "${INCLUDE_DIR}/networking/BufferWriter.hpp"
  # "${INCLUDE_DIR}/networking/Connection.hpp"
  # "${INCLUDE_DIR}/networking/ConnectionFactory.hpp"
  # "${INCLUDE_DIR}/networking/HTTPConnection.hpp"
  # "${INCLUDE_DIR}/networking/HTTPSConnection.hpp"
  # "${INCLUDE_DIR}/networking/Socket.hpp"
  # "${INCLUDE_DIR}/signal/SignalHandler.hpp"
  # "${INCLUDE_DIR}/tls/ClientHello.hpp"
  # "${INCLUDE_DIR}/tls/ServerHello.hpp"
  # "${INCLUDE_DIR}/tls/TLS.hpp"
  # "${INCLUDE_DIR}/tls/TLSContext.hpp"
  # "${INCLUDE_DIR}/tls/TLSPlaintext.hpp"
  # "${INCLUDE_DIR}/tls/TLSSignature.hpp"
  # "${INCLUDE_DIR}/tls/TLSVersion.hpp"
  # "${INCLUDE_DIR}/tls/extension/TLSExtension.hpp"
  # "${INCLUDE_DIR}/tls/extension/TLSKeyShare.hpp"
  # "${INCLUDE_DIR}/tls/extension/TLSServerName.hpp"
  # "${INCLUDE_DIR}/webserver/Middleware.hpp"
  # "${INCLUDE_DIR}/webserver/MiddlewareManager.hpp"
  # "${INCLUDE_DIR}/webserver/Router.hpp"
  # "${INCLUDE_DIR}/webserver/WebServer.hpp"
)

include_directories(${INCLUDE_DIR})

include(cmake/Style.cmake)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    include(cmake/Tests.cmake)
endif()

add_library("lib${PROJECT_NAME}" ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME} "${SOURCE_DIR}/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "lib${PROJECT_NAME}")
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wpedantic -Werror
)
