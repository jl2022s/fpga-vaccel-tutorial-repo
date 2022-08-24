## SLog Logging Library - 1.8 build 22
SLog is simple and thread safe logging library for C/C++ with possibilities to easily control verbosity levels, tag and colorize output, log to file, on the fly change configuration parameters and many more.

### Installation
Installation is possible with `Makefile` (CMake lists is also included in the project).
```
git clone https://github.com/kala13x/slog.git
cd slog
make
sudo make install
```

### Usage
If you want to use slog in your C/C++ application, include `slog.h` header in your source file and link slog library with `-lslog` linker flag while compiling your project. See example directory for more information.

### Logging flags
SLog has it's own logging flags to control log levels and to display messages with tagged and colorized output.

- `SLOG_NOTAG`
- `SLOG_LIVE`
- `SLOG_INFO`
- `SLOG_WARN`
- `SLOG_DEBUG`
- `SLOG_TRACE`
- `SLOG_ERROR`
- `SLOG_FATAL`

### Simple API
At first you should initialize slog:
```c
int nEnabledLevels = SLOG_NOTAG | SLOG_ERROR;
nEnabledLevels |= SLOG_WARN | SLOG_FATAL;

/* Setting SLOG_FLAGS_ALL will activate all logging levels */
// nEnabledLevels = SLOG_FLAGS_ALL;

slog_init("logfile", nEnabledLevels, 0);
```

 - First argument is the name of the file where we want to save logs.
 - Second argument is the logging level flags which are allowed to print. 
 - Third argument is a thread safety flag *(1 enabled, 0 disabled)*.

If thread safety flag is greater than zero, function initializes mutex and every other call of any slog function is protected by lock.

With the above slog initialization example only errors, warnings and not tagged messages will be displayed because there are no other flags activated during initializarion. We can also activate or deactivate any logging level after slog initialization by setting the flag with `slog_enable()` and `slog_disable()` functions.

```c
/* Enable all logging levels */
slog_enable(SLOG_FLAGS_ALL);

/* Disable trace level (trace logs will not be displayed anymore) */
slog_disable(SLOG_TRACE);

/* Enable trace messages again */
slog_enable(SLOG_TRACE);

/* Disable all logging levels */
slog_disable(SLOG_FLAGS_ALL);
```

Deinitialization needed only if the thread safety flag is greater than zero (nTdSafe > 0) while initialization.
```c
slog_destroy();
```
Function destroys the mutex context and resets thread safety flag to zero.


### Print and log something in the file
Here is an example on how use slog:
```c
slog("Simple message with time and date");
```

SLog ends strings automatically with the new line character `\n`. If you want to display output without adding new line character, you must use `slogwn()` function.
```c
slogwn("Simple message without new line character");
```

You can use old way logging function with a bit more control of parameters
```c
slog_print(SLOG_DEBUG, 0, "Simple debug message without new line character");
```

 - First argument is a log level flag of current message.
 - Second argument is the flag to add new line character at the end of the output *(1 add, 0 don't add)*.
 - Third argument is the formated string which we want to display in the output.

#### Macros
SLog has cleaner option to log messages without the need to provide the flag parameter. 

Here are defined macros based on the logging levels.

- `slog()`
- `slogwn()`
- `slog_live()`
- `slog_info()`
- `slog_warn()`
- `slog_debug()`
- `slog_error()`
- `slog_trace()`
- `slog_fatal()`

Each macro takes a formated string. Format tags prototype follows the same rules as the C standard library function `printf()`.

Here is an example that logs a formated debug message:
```c
slog_debug("The %s contains between %d and %d billion stars and at least %d billion planets.", "Milky Way", 200, 400, 100);
```

In addition, there are several options to print the corresponding file name and line number where a slog macro was called. This rule follows the macros which relate to a fatal or trace flag, and shown bellow:

- `slog_trace()`
- `slog_fatal()`

Display message with trace tag and print source location:
```c
slog_trace("Trace message throws source location.");
```

With expected output to be:
```
2017.01.22-19:03:17.03 - <trace> [example.c:71] Trace message throws source location.
```

We can also trace source location wothout any output message:
```c
slog_trace();
```

With expected output to be:
```
2017.01.22-19:03:17.03 - <trace> [example.c:72]
```

*Output, taken from example directory:*

![alt tag](https://github.com/kala13x/slog/blob/master/example/slog.png)

### Configuration

Since version 1.8.* config file is no longer supported by slog but there is a way to change configuration parameters at runtime.

Variables of `SLogConfig` structure:
Parameter    | Type              | Default        | Description
-------------|-------------------|----------------|---------------------------
sFileName    | char array        | "slog"         | Output file name for logs.
sFilePath    | char array        | "./"           | Output file path for logs.
eColorFormat | SLOG_COLOR_FMT_E  | SLOG_COLOR_TAG | Output coloring format control.
nTraceTid    | uint8_t           | 0 (disabled)   | Trace thread ID and display in output.
nToScreen    | uint8_t           | 1 (enabled)    | Enable or disable screen logging.
nToFile      | uint8_t           | 0 (disabled)   | Enable or disable file logging.
nFlush       | uint8_t           | 0 (disabled)   | Flush stdout after screen log.
nFlags       | uint16_t          | 0 (no logs)    | Allowed log level flags.

Any of those parameters above can be changed at runtime with the `slog_config_set()` function.

Example:
```c
SLogConfig slgCfg;

/* Setup configuration parameters */
slgCfg.eColorFormat = SLOG_COLOR_TAG;
strcpy(slgCfg.sFileName, "myproject");
strcpy(slgCfg.sFilePath, "./logs/");
slgCfg.nTraceTid = 1;
slgCfg.nToScreen = 1;
slgCfg.nToFile = 0;
slgCfg.nFlush = 1;
slgCfg.nFlags = SLOG_FLAGS_ALL;

/* Tread safe call to update slog configuration */
slog_config_set(&slgCfg);
```

If you want to change only few parameters without resetting other ones, you can thread safe read current working configuration and update only needed parameters.

```c
SLogConfig slgCfg;
slog_config_get(&slgCfg);

/* Update needed parameters */
slgCfg.nTraceTid = 1;
slgCfg.nToFile = 1;

/* Tread safe call to update slog configuration */
slog_config_set(&slgCfg);
```

### Coloring
SLog also has coloring control possibility to colorize whole line, just tag or disable coloring at all.
```c
SLogConfig slgCfg;
slog_config_get(&slgCfg);

/* Colorize only tags */
slgCfg.eColorFormat = SLOG_COLOR_TAG;
slog_config_set(&slgCfg);
slog_debug("Message with colorized tag");

/* Colorize full line */
slgCfg.eColorFormat = SLOG_COLOR_FULL;
slog_config_set(&slgCfg);
slog_debug("Message with full line color");

/* Disable coloring at all */ 
slgCfg.eColorFormat = SLOG_COLOR_DISABLE;
slog_config_set(&slgCfg);
slog_debug("Message without coloring");
```

### Thread ID tracing
If you are looking for additional information about threads while debugging, you can trace thread IDs and display in the output.

Here is an example:
```c
SLogConfig slgCfg;
slog_config_get(&slgCfg);
slgCfg.nTraceTid = 1;
slog_config_set(&slgCfg);

slog_debug("Message with thread id");
```

With expected output to be:
```
(15203) 2017.01.22-19:03:17.03 - <debug> Message with thread id.
```
Where `15203` is a thread identifier from which the message was printed.

### Version
There are two ways to get and print slog version with this library. Function `slog_version()` returns char pointer of static array where slog version string is located. If argument is more than zero function returns string with only version and build number. Otherwise it returns full version format with build date.

Usage:
```c
printf("slog version: %s", slog_version(0));
```

Output will be something like that:
```
slog version: 1.8 build 22 (Dec 14 2020)
```

### Output
Here is en example of the log file context created by slog:
```
2020.12.13-19:41:41.27 - Simple message without anything
2020.12.13-19:41:41.27 - Simple message with our own new line character
2020.12.13-19:41:41.27 - <debug> Old way printed debug message with our own new line character
2020.12.13-19:41:41.27 - <error> Old way printed error message with auto new line character
2020.12.13-19:41:41.27 - <warn> Warning message without variable
2020.12.13-19:41:41.27 - <info> Info message with string variable: test string
2020.12.13-19:41:41.27 - <note> Note message with integer variable: 69
(15203) 2020.12.13-19:41:41.27 - <debug> Debug message with enabled thread id tracing
(15203) 2020.12.13-19:41:41.27 - <error> Error message with errno string: Success
(15203) 2020.12.13-19:41:41.27 - <debug> Debug message in the file with full line color enabled
(15203) 2020.12.13-19:41:41.27 - <trace> [example.c:95] Trace message throws source location
(15203) 2020.12.13-19:41:41.27 - <fatal> [example.c:98] Fatal message also throws source location
(15203) 2020.12.13-19:41:41.28 - <debug> Disabled output coloring
(15203) 2020.12.13-19:41:41.28 - <trace> [example.c:108] 
(15203) 2020.12.13-19:41:41.28 - <debug> Above we traced source location without output message
```
