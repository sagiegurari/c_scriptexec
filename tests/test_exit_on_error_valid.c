#include "scriptexec.h"
#include "test.h"
#include <string.h>


void test_impl()
{
  struct ScriptExecOptions options = scriptexec_create_options();

  options.exit_on_error = true;

  struct ScriptExecResult result = scriptexec_run_with_options("echo 1\necho 2\necho 3\necho 4", options);

  assert_num_equal(result.code, 0);
  assert_string_equal(result.out, "1\n2\n3\n4\n");
  assert_string_equal(result.err, "");
}


int main()
{
  test_run(test_impl);
}

