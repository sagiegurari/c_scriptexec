#include "scriptexec.h"
#include "test.h"
#include <string.h>


void test_impl()
{
  struct ScriptExecResult result = scriptexec_run("badcommand");

  assert_true(result.code > 0);
  assert_string_equal(result.out, "");
  assert_true(strlen(result.err) > 0);

  free(result.out);
  free(result.err);
}


int main()
{
  test_run(test_impl);
}

