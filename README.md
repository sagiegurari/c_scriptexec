# scriptexec

[![CI](https://github.com/sagiegurari/c_scriptexec/workflows/CI/badge.svg?branch=master)](https://github.com/sagiegurari/c_scriptexec/actions)
[![Release](https://img.shields.io/github/v/release/sagiegurari/c_scriptexec)](https://github.com/sagiegurari/c_scriptexec/releases)
[![license](https://img.shields.io/github/license/sagiegurari/c_scriptexec)](https://github.com/sagiegurari/c_scriptexec/blob/master/LICENSE)

> Invoke complex multi command scripts with a single C api call.

* [Overview](#overview)
* [Usage](#usage)
* [Contributing](.github/CONTRIBUTING.md)
* [Release History](CHANGELOG.md)
* [License](#license)

<a name="overview"></a>
## Overview
This library enables to invoke complex multi command scripts with a single C api call and get the script status code and out/err.

<a name="usage"></a>
## Usage

```c
#include "scriptexec.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{
  // invoke the script with default options
  struct ScriptExecResult result = scriptexec_run("echo 1\necho 2\necho 3\necho 4");

  printf("Code: %d\nOutput:\n%s\nError:\n%s\n", result.code, result.out, result.err);

  // invoke the script with custom options
  struct ScriptExecOptions options = scriptexec_create_options();
  options.runner            = "bash";     // default is 'sh'
  options.working_directory = "./target"; // default to current working directory
  options.exit_on_error     = true;       // default true, will break script execution on any error
  options.print_commands    = true;       // default false, if true will print every command before invocation

  result = scriptexec_run_with_options("echo 1\necho 2\necho 3\necho 4", options);
  printf("Code: %d\nOutput:\n%s\nError:\n%s\n", result.code, result.out, result.err);

  free(result.out);
  free(result.err);
}
```

## Contributing
See [contributing guide](.github/CONTRIBUTING.md)

<a name="history"></a>
## Release History

See [Changelog](CHANGELOG.md)

<a name="license"></a>
## License
Developed by Sagie Gur-Ari and licensed under the Apache 2 open source license.
