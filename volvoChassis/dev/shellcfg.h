#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "usbcfg.h"
#include "shell.h"

    extern const ShellCommand commands[];
    extern const ShellConfig shell_cfg;

    void shellStart(void);

#ifdef __cplusplus
}
#endif