cmake_minimum_required(VERSION 3.16)
project(MusicManagementSystem LANGUAGES C)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

enable_testing()

add_subdirectory(MMSUtils)
add_subdirectory(tests)
add_subdirectory(ui)
