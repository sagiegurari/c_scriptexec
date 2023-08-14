#include "fsio.h"
#include "scriptexec.h"
#include "test.h"

#define TEMPORARY_DIRECTORY    "./tempscripts/"


void test_impl()
{
  struct ScriptExecOptions options = scriptexec_create_options();

  fsio_remove(TEMPORARY_DIRECTORY);
  assert_true(!fsio_dir_exists(TEMPORARY_DIRECTORY));
  options.temporary_directory = TEMPORARY_DIRECTORY;

  struct ScriptExecResult result = scriptexec_run_with_options("echo 1", options);

  assert_num_equal(result.code, 0);

  free(result.out);
  free(result.err);

  assert_true(fsio_dir_exists(TEMPORARY_DIRECTORY));
  fsio_remove(TEMPORARY_DIRECTORY);
}


int main()
{
  test_run(test_impl);
}

