#include "shellcfg.h"

/*===========================================================================*/
/* Definitions of shell command functions                                    */
/*===========================================================================*/

void cmd_test(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    chprintf(chp, "test");
}

/**
 * @brief array of shell commands, put the corresponding command and functions below
 * {"command", callback_function}
 */
const ShellCommand commands[] =
    {
        {"test", cmd_test},
        {NULL, NULL}};

const ShellConfig shell_cfg =
    {
        (BaseSequentialStream *)&SDU1,
        commands};

THD_WORKING_AREA(Shell_thread_wa, 1024);

/**
 * @brief start the shell service
 * @require enable the corresponding serial ports in mcuconf.h and board.h
 *          Select the SERIAL_CMD port in main.h
 *
 * @api
 */
void shellStart(void)
{

    /*
   * Initializes a serial-over-USB CDC driver.
   */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */

    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);

    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    shellInit();

    // chThdCreateStatic(Shell_thread_wa, sizeof(Shell_thread_wa), NORMALPRIO, shell_thread, (void *)&shell_cfg);

    shellCreateStatic(&shell_cfg, Shell_thread_wa, sizeof(Shell_thread_wa), NORMALPRIO);
}
