#include "fsio.h"
#include "scriptexec.h"
#include "stringbuffer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// private functions
static struct ScriptExecResult _scriptexec_create_result(void);
static char *_scriptexec_read_and_delete_text_file(char *);

struct ScriptExecOptions scriptexec_create_options(void)
{
  struct ScriptExecOptions options =
  {
    .runner              = NULL,
    .working_directory   = NULL,
    .exit_on_error       = true,
    .print_commands      = false,
    .temporary_directory = NULL
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

  struct StringBuffer *buffer = stringbuffer_new();

  // move to cwd
  char *cwd = getcwd(NULL, 0);
  stringbuffer_append_string(buffer, "cd ");
  stringbuffer_append_string(buffer, cwd);
  free(cwd);
  stringbuffer_append(buffer, '\n');

  // move to requested working directory
  if (options.working_directory != NULL)
  {
    stringbuffer_append_string(buffer, "cd ");
    stringbuffer_append_string(buffer, options.working_directory);
    stringbuffer_append(buffer, '\n');
  }

  if (options.exit_on_error)
  {
    stringbuffer_append_string(buffer, "set -e\n");
  }

  if (options.print_commands)
  {
    stringbuffer_append_string(buffer, "set -x\n");
  }

  stringbuffer_append_string(buffer, (char *)script);
  char *updated_script = stringbuffer_to_string(buffer);

  bool delete_temporary_directory = false;
  char *dir_name                  = NULL;
  if (options.temporary_directory == NULL)
  {
    char template[] = "/tmp/scriptexec_XXXXXX";
    dir_name                   = mkdtemp(template);
    delete_temporary_directory = true;
  }
  else
  {
    dir_name = options.temporary_directory;
  }

  if (dir_name == NULL)
  {
    stringbuffer_release(buffer);
    free(updated_script);

    result.code = -1;
    return(result);
  }

  stringbuffer_clear(buffer);
  char *script_file = fsio_join_paths(dir_name, "script");
  stringbuffer_append_string(buffer, script_file);

  // write script file
  bool written = fsio_write_text_file(script_file, updated_script);
  free(updated_script);
  if (!written)
  {
    if (delete_temporary_directory)
    {
      rmdir(dir_name);
    }

    stringbuffer_release(buffer);
    free(script_file);

    result.code = -1;
    return(result);
  }

  stringbuffer_append_string(buffer, ".out");
  char *out_file = stringbuffer_to_string(buffer);

  stringbuffer_clear(buffer);
  stringbuffer_append_string(buffer, script_file);
  stringbuffer_append_string(buffer, ".err");
  char *err_file = stringbuffer_to_string(buffer);

  // create command
  char *runner = options.runner;
  if (runner == NULL)
  {
    runner = "sh";
  }
  stringbuffer_clear(buffer);
  stringbuffer_append_string(buffer, runner);
  stringbuffer_append(buffer, ' ');
  stringbuffer_append_string(buffer, script_file);
  stringbuffer_append_string(buffer, " 2> ");
  stringbuffer_append_string(buffer, err_file);
  stringbuffer_append_string(buffer, " 1> ");
  stringbuffer_append_string(buffer, out_file);
  char *command = stringbuffer_to_string(buffer);
  stringbuffer_release(buffer);

  result.code = system(command);
  free(command);

  // delete files
  remove(script_file);
  free(script_file);

  // read out/err
  result.out = _scriptexec_read_and_delete_text_file(out_file);
  free(out_file);
  result.err = _scriptexec_read_and_delete_text_file(err_file);
  free(err_file);

  // delete temp directory
  if (delete_temporary_directory)
  {
    rmdir(dir_name);
  }

  return(result);
} /* scriptexec_run_with_options */

static struct ScriptExecResult _scriptexec_create_result(void)
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
