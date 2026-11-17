cmake_minimum_required(VERSION 3.21)
project(n4_25 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
  set(N4_25_PLATFORM "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(N4_25_PLATFORM "linux")
else()
  message(FATAL_ERROR "n4-25-lib поддерживает только Windows и Linux.")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(N4_25_ARCH "x86")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(N4_25_ARCH "amd64")
else()
  message(FATAL_ERROR "Не удалось определить разрядность целевой платформы.")
endif()

set(N4_25_ARTIFACT_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/${N4_25_PLATFORM}-${N4_25_ARCH}"
  CACHE PATH "Каталог с динамической библиотекой")

add_library(n4_25 SHARED IMPORTED GLOBAL)
add_library(n4_25::n4_25 ALIAS n4_25)

target_include_directories(n4_25 INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(WIN32)
  set_target_properties(n4_25 PROPERTIES
    IMPORTED_LOCATION "${N4_25_ARTIFACT_DIR}/n4-25.dll"
    IMPORTED_IMPLIB "${N4_25_ARTIFACT_DIR}/n4-25.lib"
  )
else()
  set_target_properties(n4_25 PROPERTIES
    IMPORTED_LOCATION "${N4_25_ARTIFACT_DIR}/libn4-25.so"
  )
endif()

option(N4_25_BUILD_EXAMPLES "Собрать пример работы с Н4-25" OFF)

if(N4_25_BUILD_EXAMPLES)
  enable_language(CXX)
  add_subdirectory(examples/amplitude-set-get)
endif()
