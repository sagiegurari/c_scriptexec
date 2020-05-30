
cmake_minimum_required (VERSION 3.7)

project (scriptexec C)

set(CMAKE_BUILD_TYPE Release)
if(NOT WIN32)
  set(X_CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wcast-align -Wunused -Wshadow -Wpedantic")
endif()

set(X_CMAKE_PROJECT_ROOT_DIR ${CMAKE_BINARY_DIR}/..)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(ExternalProject)
macro(add_external_lib)
  message("Adding External Lib: ${ARGV0}")
  execute_process(
    COMMAND git clone git@github.com:sagiegurari/c_string_buffer.git
  )
  execute_process(
    COMMAND git checkout tags/v0.1.1
    WORKING_DIRECTORY c_string_buffer
  )
  string(TOUPPER ${ARGV0} X_EXTERNAL_LIB_NAME)
  file(GLOB "${X_EXTERNAL_LIB_NAME}_SOURCES" "target/c_${ARGV0}/src/*.c")
  set("${X_EXTERNAL_LIB_NAME}_INCLUDE" "target/c_${ARGV0}/include")
endmacro(add_external_lib)
add_external_lib("string_buffer")

include_directories(include "${STRING_BUFFER_INCLUDE}")

# define all sources
file(GLOB SOURCES "src/*.c")
file(GLOB HEADER_SOURCES "include/*.h")
file(GLOB TEST_SOURCES "tests/*")
file(GLOB COMMON_TEST_SOURCES "tests/test.*")

# format code
if(DEFINED ENV{X_CMAKE_SCRIPTEXEC_UNCRUSTIFY_ENABLED})
  add_custom_target(uncrustify ALL
    uncrustify
    -c ${X_CMAKE_PROJECT_ROOT_DIR}/uncrustify.cfg
    --no-backup ${SOURCES} ${HEADER_SOURCES} ${TEST_SOURCES})
endif()

# create static library
add_library(scriptexec STATIC ${SOURCES} ${STRING_BUFFER_SOURCES})

if(NOT WIN32)
  set_target_properties(scriptexec PROPERTIES COMPILE_FLAGS "${X_CMAKE_C_FLAGS} -Wconversion")
endif()

# tests
enable_testing()

macro(setup_test)
  message("Adding Test: ${ARGV0}")
  add_executable(test_${ARGV0} tests/test_${ARGV0}.c ${COMMON_TEST_SOURCES})
  target_link_libraries(test_${ARGV0} scriptexec)
  set_target_properties(test_${ARGV0} PROPERTIES COMPILE_FLAGS "${X_CMAKE_C_FLAGS}")
  add_test(NAME ${ARGV0}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_${ARGV0})
endmacro(setup_test)

setup_test("valid_command")
setup_test("invalid_command")
setup_test("valid_multiple_commands")
setup_test("valid_multiple_commands_default_options")
setup_test("valid_multiple_commands_with_valid_runner")
setup_test("valid_multiple_commands_with_invalid_runner")
setup_test("valid_multiple_commands_with_print")
setup_test("exit_on_error_valid")
setup_test("exit_on_error_break")
setup_test("exit_on_error_ignore")
