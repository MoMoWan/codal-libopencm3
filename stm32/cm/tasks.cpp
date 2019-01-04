//  Background Tasks
#include <logger/logger.h>
#include <libopencm3/cm3/scb.h>  //  For scb_reset_system
#include <core/CodalFiber.h>
#include <core/EventModel.h>
#include "tasks.h"

enum CM_SOURCE {
    //  22569-22570 are used by https://github.com/lupyuen/microbit-sigfox/blob/master/task.ts
    CM_SOURCE_BOOTLOADER = 22580,   //  Bootloader
};

enum CM_EVT {
    //  Event IDs must be above 1024.
    CM_EVT_RESTART = 2205,   //  Restart the device.
};

static codal::Fiber *flush_task_fibre = NULL;
static codal::Event restart_event(  //  This event will be triggered for a restart request.
    CM_SOURCE_BOOTLOADER, CM_EVT_RESTART, codal::CREATE_ONLY);

static void restart_handler(codal::Event evt) {
    //  Handle a restart request.  Flush the log then restart.
    if (evt.value != CM_EVT_RESTART) { return; }
    debug_println("restarting...");
    debug_force_flush();
    scb_reset_system();
}

int restart_callback(void) {
    //  Send a restart request.  Used by bootloader running in Application Mode.
    debug_println("restart callback");
    if (!codal::EventModel::defaultEventBus) {
        debug_println("*** ERROR: missing event bus");
        return -1;
    }
    int status = codal::EventModel::defaultEventBus->send(restart_event);
    if (status) {
        debug_print("*** ERROR: unable to send restart event ");
        debug_print_unsigned(status); debug_println("");
        return status;
    }
    return 0;
}

static void flush_task(void) {
    //  Flush the log periodically.
    while (true) {
        debug_flush();
        codal::fiber_sleep(200);
    }
}

int start_background_tasks(void) {
    //  Start the background tasks to flush the log and wait for restart requests.
    if (flush_task_fibre) { return 0; }
    flush_task_fibre = codal::create_fiber(flush_task);

    //  Listen for restart requests.
    if (!codal::EventModel::defaultEventBus) {
        debug_println("*** ERROR: missing event bus");
        return -1;
    }
    int status = codal::EventModel::defaultEventBus->listen(
        CM_SOURCE_BOOTLOADER, CM_EVT_RESTART, restart_handler);
    if (status) {
        debug_print("*** ERROR: unable to listenfor restart event ");
        debug_print_unsigned(status); debug_println("");
        return status;
    }
    return 0;
}
