#include "scriptexec.h"
#include "test.h"
#include <string.h>


void test_impl()
{
  const char *script = "echo start\nls\necho end";

  for (int index = 0; index < 1000; index++)
  {
    struct ScriptExecResult result = scriptexec_run(script);

    assert_num_equal(result.code, 0);
    assert_true(strstr(result.out, "start") != NULL);
    assert_true(strstr(result.out, "end") != NULL);
    assert_string_equal(result.err, "");
  }
}


int main()
{
  test_run(test_impl);
}

