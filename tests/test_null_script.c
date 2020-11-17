#include "scriptexec.h"
#include "test.h"


void test_impl()
{
  struct ScriptExecResult result = scriptexec_run(NULL);

  assert_true(result.code == -1);
  assert_true(result.out == NULL);
  assert_true(result.err == NULL);
}


int main()
{
  test_run(test_impl);
}

