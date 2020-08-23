
cmake_minimum_required (VERSION 3.7)

project (scriptexec C)

# include shared utilities
if(NOT EXISTS "target/cmake-modules/src/utils.cmake")
  execute_process(COMMAND git clone https://github.com/sagiegurari/cmake-modules.git)
endif()
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
  utils_add_external_github_lib(
    REPO_USERNAME sagiegurari
    REPO_NAME c_${ARGV0}
    TAG_NAME ${ARGV1}
    LIBRARY_NAME ${ARGV0}
    LIBRARY_PARENT_DIRECTORY target
  )
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

utils_setup_c_test(
  NAME
    valid_command
    invalid_command
    valid_multiple_commands
    valid_multiple_commands_default_options
    valid_multiple_commands_with_valid_runner
    valid_multiple_commands_with_invalid_runner
    valid_multiple_commands_with_print
    exit_on_error_valid
    exit_on_error_break
    exit_on_error_ignore
    stability
  ADDITIONAL_SOURCES "${COMMON_TEST_SOURCES}"
  COMPILATION_FLAGS "${X_CMAKE_C_FLAGS}"
  BINARY_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

