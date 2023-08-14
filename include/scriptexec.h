#ifndef SCRIPTEXEC_H
#define SCRIPTEXEC_H

#include <stdbool.h>

struct ScriptExecOptions
{
  char *runner;
  char *working_directory;
  bool exit_on_error;
  bool print_commands;
  // The directory used to write the temporary script in
  // If NULL, it will use the system temporary directory as parent
  char *temporary_directory;
};

struct ScriptExecResult
{
  int  code;
  char *out;
  char *err;
};

struct ScriptExecOptions scriptexec_create_options(void);

struct ScriptExecResult  scriptexec_run(const char * /* script */);
struct ScriptExecResult  scriptexec_run_with_options(const char * /* script */, struct ScriptExecOptions);

#endif

