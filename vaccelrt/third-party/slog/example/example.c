/*
 *  example/example.c
 *
 *  2015 - 2020 (c) Sun Dro (f4tb0y@protonmail.com)
 *
 *  This source file is a part of the "slog" project
 *  Read LICENSE file for more details about copyright
 *
 * Implementation example of SLog library
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <slog.h>

void greet() 
{
    /* Get and print slog version */
    printf("=========================================\n");
    printf("SLog Version: %s\n", slog_version(0));
    printf("=========================================\n");
}

int main()
{
    /* Used variables */
    SLogConfig slgCfg;
    int nInteger = 69;
    char sBuffer[12];

    strcpy(sBuffer, "test string");
    uint16_t nLogFlags = SLOG_ERROR | SLOG_NOTAG;

    /* Greet users */
    greet();

    /* Initialize slog and allow only error and not tagged output */
    slog_init("example", nLogFlags, 0);

    /* Just simple log without anything (color, tag, thread id)*/
    slog("Simple message without anything");

    /* Simple log without adding new line character at the end */
    slogwn("Simple message with our own new line character\n");

    /* Enable all logging flags */
    slog_enable(SLOG_FLAGS_ALL);

    /* Old way logging function with debug tag and disabled new line from argument */
    slog_print(SLOG_DEBUG, 0, "Old way printed debug message with our own new line character\n");

    /* Old way logging function with error tag and enabled new line from argument */
    slog_print(SLOG_ERROR, 1, "Old way printed error message with %s", "auto new line character");

    /* Warning message */
    slog_warn("Warning message without variable");

    /* Info message with char*/
    slog_info("Info message with string variable: %s", sBuffer);

    /* Note message */
    slog_note("Note message with integer variable: %d", nInteger);

    /* Trace thread id and print in output */
    slog_config_get(&slgCfg);
    slgCfg.nTraceTid = 1;
    slog_config_set(&slgCfg);

    /* Debug message with string and integer */
    slog_debug("Debug message with enabled thread id tracing");

    /* Error message with errno string (in this case must be 'Success')*/
    slog_error("Error message with errno string: %s", strerror(errno));

    /* Disable trace tag */
    slog_disable(SLOG_TRACE);

    /* This will never be printed while SLOG_TRACE is disabled by slog_disable() function */
    slog_trace("Test log with disabled tag");

    /* Enable file logger and color the whole log output instead of coloring only tags*/
    slog_config_get(&slgCfg);
    slgCfg.eColorFormat = SLOG_COLOR_FULL;
    slgCfg.nToFile = 1;
    slog_config_set(&slgCfg);

    /* Print message and save log in the file */
    slog_debug("Debug message in the file with full line color enabled");

    /* Enable trace tag */
    slog_enable(SLOG_TRACE);

    /* We can trace function and line number with and without output message */
    slog_trace("Trace message throws source location");

    /* Fatal error message */
    slog_fatal("Fatal message also throws source location");

    /* Disable output coloring*/
    slog_config_get(&slgCfg);
    slgCfg.eColorFormat = SLOG_COLOR_DISABLE;
    slog_config_set(&slgCfg);

    slog_debug("Disabled output coloring");

    /* Just throw source location without output message */
    slog_trace();

    slog_debug("Above we traced source location without output message");

    return 0;
}
