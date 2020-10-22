#include "scriptexec.h"
#include "test.h"


void test_impl()
{
  struct ScriptExecResult result = scriptexec_run("echo test");

  assert_num_equal(result.code, 0);
  assert_string_equal(result.out, "test\n");
  assert_string_equal(result.err, "");

  free(result.out);
  free(result.err);
}


int main()
{
  test_run(test_impl);
}

