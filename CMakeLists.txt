
cmake_minimum_required (VERSION 3.7)

project (scriptexec C)

# include shared utilities
execute_process(COMMAND git clone https://github.com/sagiegurari/cmake-modules.git)
include("target/cmake-modules/src/utils.cmake")

set(CMAKE_BUILD_TYPE Release)
if(NOT WIN32)
  set(X_CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wcast-align -Wunused -Wshadow -Wpedantic")
endif()

set(X_CMAKE_PROJECT_ROOT_DIR ${CMAKE_BINARY_DIR}/..)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

macro(add_external_lib)
  utils_add_external_github_lib(sagiegurari c_${ARGV0} ${ARGV1} ${ARGV0} target)
endmacro(add_external_lib)
add_external_lib("string_buffer" "v0.1.2")

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

# example
add_executable(example examples/example.c)
target_link_libraries(example scriptexec)
set_target_properties(example PROPERTIES COMPILE_FLAGS "${X_CMAKE_C_FLAGS}")

# tests
enable_testing()

macro(setup_test)
  utils_setup_c_test(${ARGV0} "${COMMON_TEST_SOURCES}" "${X_CMAKE_C_FLAGS}" "${CMAKE_BINARY_DIR}/bin")
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
setup_test("stability")
