#include "scriptexec.h"
#include "test.h"
#include <string.h>


void test_impl()
{
  struct ScriptExecOptions options = scriptexec_create_options();

  options.runner = "badcommand";

  struct ScriptExecResult result = scriptexec_run_with_options("echo 1\necho 2\necho 3\necho 4", options);

  assert_true(result.code != 0);
  assert_string_equal(result.out, "");
  assert_true(strlen(result.err) > 0);

  free(result.out);
  free(result.err);
}


int main()
{
  test_run(test_impl);
}

