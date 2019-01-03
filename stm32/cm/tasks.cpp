//  Background Tasks
#include <libopencm3/cm3/scb.h>  //  For scb_reset_system
#include <CodalFiber.h>
#include "tasks.h"

void restart_handler(Event evt) {
    //  Handle a restart request.  Flush the log then restart.
    debug_force_flush();
    scb_reset_system();
}

void restart_callback(void) {
    //  Send a restart request.  Used by bootloader running in Application Mode.
}
