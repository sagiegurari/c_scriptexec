#include "scriptexec.h"
#include "test.h"


void test_impl()
{
  struct ScriptExecResult result = scriptexec_run("echo 1\necho 2\necho 3\necho 4");

  assert_num_equal(result.code, 0);
  assert_string_starts_with(result.out, "1\n2\n3\n4");
  assert_string_equal(result.err, "");

  free(result.out);
  free(result.err);
}


int main()
{
  test_run(test_impl);
}

