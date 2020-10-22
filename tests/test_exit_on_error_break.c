#include "scriptexec.h"
#include "test.h"
#include <string.h>


void test_impl()
{
  struct ScriptExecOptions options = scriptexec_create_options();

  options.exit_on_error = true;

  struct ScriptExecResult result = scriptexec_run_with_options("echo 1\necho 2\nbadcommand\necho 3", options);

  assert_true(result.code > 0);
  assert_string_equal(result.out, "1\n2\n");
  assert_true(strlen(result.err) > 0);

  free(result.out);
  free(result.err);
}


int main()
{
  test_run(test_impl);
}

