cmake_minimum_required(VERSION 3.21)

project(ggwget VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(GGWGET_INSTALL_PREFIX "" CACHE PATH "Installation prefix for ggwget")
if(GGWGET_INSTALL_PREFIX STREQUAL "")
    message(FATAL_ERROR
            "Please set GGWGET_INSTALL_PREFIX to the desired install directory:\n"
            "  cmake -S . -B build -DGGWGET_INSTALL_PREFIX=/path/to/prefix")
endif()
set(CMAKE_INSTALL_PREFIX "${GGWGET_INSTALL_PREFIX}" CACHE PATH
    "Install prefix for ggwget" FORCE)

# Configure libcurl to build as a static library using FetchContent.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libraries static" FORCE)
set(CURL_STATICLIB ON CACHE BOOL "Use static libcurl" FORCE)
set(CURL_ENABLE_SHARED OFF CACHE BOOL "Disable shared libcurl build" FORCE)
set(CURL_BUILD_EXAMPLES OFF CACHE BOOL "Skip libcurl examples" FORCE)
set(CURL_BUILD_TESTING OFF CACHE BOOL "Skip libcurl tests" FORCE)
set(CURL_USE_LIBSSH OFF CACHE BOOL "Disable libssh support" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "Disable libpsl support" FORCE)

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_7_1
    GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(curl)

add_library(ggwget STATIC src/Downloader.cpp)
target_link_libraries(ggwget PUBLIC libcurl)
target_include_directories(
    ggwget
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${curl_SOURCE_DIR}/include)
target_compile_definitions(ggwget PUBLIC CURL_STATICLIB)
target_compile_features(ggwget PUBLIC cxx_std_20)

add_executable(ggwget_example examples/ggwget_cli.cpp)
target_link_libraries(ggwget_example PRIVATE ggwget)

install(TARGETS ggwget EXPORT ggwgetTargets ARCHIVE DESTINATION lib)
install(TARGETS ggwget_example RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT ggwgetTargets NAMESPACE ggwget:: DESTINATION lib/cmake/ggwget)

add_custom_target(
    install_ggwget
    COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}"
    DEPENDS ggwget ggwget_example
    COMMENT "Installing ggwget to ${CMAKE_INSTALL_PREFIX}")
