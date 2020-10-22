#include "scriptexec.h"
#include "test.h"


void test_impl()
{
  struct ScriptExecOptions options = scriptexec_create_options();

  options.print_commands = true;

  struct ScriptExecResult result = scriptexec_run_with_options("echo 1\necho 2\necho 3\necho 4", options);

  assert_num_equal(result.code, 0);
  assert_string_equal(result.out, "1\n2\n3\n4\n");
  assert_string_equal(result.err, "+ echo 1\n+ echo 2\n+ echo 3\n+ echo 4\n");

  free(result.out);
  free(result.err);
}


int main()
{
  test_run(test_impl);
}

