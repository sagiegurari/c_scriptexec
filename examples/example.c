#include "scriptexec.h"
#include <stdio.h>

int main()
{
  // invoke the script with default options
  struct ScriptExecResult result = scriptexec_run("echo 1\necho 2\necho 3\necho 4");
  printf("Code: %d\nOutput:\n%s\nError:\n%s\n", result.code, result.out, result.err);

  // invoke the script with custom options
  struct ScriptExecOptions options = scriptexec_create_options();
  options.runner = "bash"; // default is 'sh'
  options.working_directory = "./target"; // default to current working directory
  options.exit_on_error = true; // default true, will break script execution on any error
  options.print_commands = true; // default false, if true will print every command before invocation

  result = scriptexec_run_with_options("echo 1\necho 2\necho 3\necho 4", options);
  printf("Code: %d\nOutput:\n%s\nError:\n%s\n", result.code, result.out, result.err);
}
