cmake_minimum_required(VERSION 3.21)
project(n4_25 LANGUAGES CXX)

if(WIN32)
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR "Для n4-25.dll требуется 32-битное приложение.")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "Для libn4-25.so требуется 64-битное приложение.")
    endif()
else()
    message(FATAL_ERROR "n4-25-lib поддерживает только Windows и Linux.")
endif()

add_library(n4_25 INTERFACE)
add_library(n4_25::n4_25 ALIAS n4_25)

target_include_directories(n4_25 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(n4_25 INTERFACE cxx_std_11)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(n4_25 INTERFACE ${CMAKE_DL_LIBS})
endif()

option(N4_25_BUILD_EXAMPLES "Собирать пример работы с Н4-25" OFF)
if(N4_25_BUILD_EXAMPLES)
    add_subdirectory(examples/amplitude-set-get)
endif()
