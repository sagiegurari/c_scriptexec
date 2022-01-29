#include "fsio.h"
#include "scriptexec.h"
#include "string_buffer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// private functions
static struct ScriptExecResult _scriptexec_create_result();
static char *_scriptexec_read_and_delete_text_file(char *);

struct ScriptExecOptions scriptexec_create_options()
{
  struct ScriptExecOptions options =
  {
    .runner = NULL, .working_directory = NULL, .exit_on_error = true, .print_commands = false
  };

  return(options);
}


struct ScriptExecResult scriptexec_run(const char *script)
{
  struct ScriptExecOptions options = scriptexec_create_options();

  return(scriptexec_run_with_options(script, options));
}


struct ScriptExecResult scriptexec_run_with_options(const char *script, struct ScriptExecOptions options)
{
  struct ScriptExecResult result = _scriptexec_create_result();

  if (script == NULL)
  {
    result.code = -1;
    return(result);
  }

  struct StringBuffer *buffer = string_buffer_new();

  // move to cwd
  char *cwd = getcwd(NULL, 0);
  string_buffer_append_string(buffer, "cd ");
  string_buffer_append_string(buffer, cwd);
  free(cwd);
  string_buffer_append(buffer, '\n');

  // move to requested working directory
  if (options.working_directory != NULL)
  {
    string_buffer_append_string(buffer, "cd ");
    string_buffer_append_string(buffer, options.working_directory);
    string_buffer_append(buffer, '\n');
  }

  if (options.exit_on_error)
  {
    string_buffer_append_string(buffer, "set -e\n");
  }

  if (options.print_commands)
  {
    string_buffer_append_string(buffer, "set -x\n");
  }

  string_buffer_append_string(buffer, (char *)script);
  char *updated_script = string_buffer_to_string(buffer);

  char template[] = "/tmp/scriptexec_XXXXXX";
  char *dir_name  = mkdtemp(template);

  if (dir_name == NULL)
  {
    string_buffer_release(buffer);
    free(updated_script);

    result.code = -1;
    return(result);
  }

  string_buffer_clear(buffer);
  string_buffer_append_string(buffer, dir_name);
  string_buffer_append_string(buffer, "/script");
  char *script_file = string_buffer_to_string(buffer);

  // write script file
  bool written = fsio_write_text_file(script_file, updated_script);
  free(updated_script);
  if (!written)
  {
    rmdir(dir_name);

    string_buffer_release(buffer);
    free(script_file);

    result.code = -1;
    return(result);
  }

  string_buffer_append_string(buffer, ".out");
  char *out_file = string_buffer_to_string(buffer);

  string_buffer_clear(buffer);
  string_buffer_append_string(buffer, script_file);
  string_buffer_append_string(buffer, ".err");
  char *err_file = string_buffer_to_string(buffer);

  // create command
  char *runner = options.runner;
  if (runner == NULL)
  {
    runner = "sh";
  }
  string_buffer_clear(buffer);
  string_buffer_append_string(buffer, runner);
  string_buffer_append(buffer, ' ');
  string_buffer_append_string(buffer, script_file);
  string_buffer_append_string(buffer, " 2> ");
  string_buffer_append_string(buffer, err_file);
  string_buffer_append_string(buffer, " 1> ");
  string_buffer_append_string(buffer, out_file);
  char *command = string_buffer_to_string(buffer);
  string_buffer_release(buffer);

  result.code = system(command);
  free(command);

  // delete files
  remove(script_file);
  rmdir(dir_name);
  free(script_file);

  // read out/err
  result.out = _scriptexec_read_and_delete_text_file(out_file);
  free(out_file);
  result.err = _scriptexec_read_and_delete_text_file(err_file);
  free(err_file);

  return(result);
} /* scriptexec_run_with_options */

static struct ScriptExecResult _scriptexec_create_result()
{
  struct ScriptExecResult result = { .code = 0, .out = NULL, .err = NULL };

  return(result);
}


static char *_scriptexec_read_and_delete_text_file(char *file)
{
  char *text = fsio_read_text_file(file);

  remove(file);

  return(text);
}
