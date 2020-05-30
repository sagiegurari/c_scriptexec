#ifndef __SCRIPTEXEC_H__
#define __SCRIPTEXEC_H__

#include <stdbool.h>

struct ScriptExecOptions
{
  char *runner;
  char *working_directory;
  bool exit_on_error;
  bool print_commands;
};

struct ScriptExecResult
{
  int  code;
  char *out;
  char *err;
};

struct ScriptExecOptions scriptexec_create_options();

struct ScriptExecResult  scriptexec_run(const char * /* script */);
struct ScriptExecResult  scriptexec_run_with_options(const char * /* script */, struct ScriptExecOptions);

#endif

