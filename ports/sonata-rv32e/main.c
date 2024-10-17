#include <stdio.h> 
#include <stdbool.h>
#include <string.h> 

#include "py/mpconfig.h"
#ifdef QEMU_DEBUG
#include "ns16550a.h"
#endif 

#include "py/builtin.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "shared/runtime/pyexec.h"

#include "common/defs.h"
#include "common/timer-utils.hh"

/*!
 * Linker provided symbols to calculate 
 * locations and offsets in memory 
 */
extern void uart_boot();
extern uint32_t  etext, edata, end; 
extern uint32_t _estack; 


/* _estack to be defined and exported from the linker script */
static char *argv[] = { "mpy_sonata" }; 
static uintptr_t *stack_top;
static char heap[MICROPY_HEAP_SIZE];

#if MICROPY_ENABLE_GC
void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *current_fp = __builtin_frame_address(0);
    gc_collect_start();
    gc_collect_root(&current_fp, ((mp_uint_t)stack_top - (mp_uint_t)current_fp) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info(&mp_plat_print);
}
#endif

mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

int main(int argc, char **argv) 
{
    /* Init UART */
    uart_boot(); 

    #if MICROPY_ENABLE_GC 
    gc_init(heap, heap + sizeof(heap));
    #endif
    mp_init();


    #if MICROPY_ENABLE_COMPILER
    #if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
    for (;;) {
        int c = mp_hal_stdin_rx_chr();
        if (pyexec_event_repl_process_char(c)) {
            break;
        }
    }
    #else /* !MICROPY_REPL_EVENT_DRIVEN */
    pyexec_friendly_repl();
    #endif /* !MICROPY_REPL_EVENT_DRIVEN */
    #else 
    #endif /* MICROPY_ENABLE_COMPILER */

    mp_deinit();
    return 0;
}


void nlr_jump_fail(void *val) {
    while (true) {
        ;
    }
} 


__attribute__((noreturn)) 
void mpy_start()
{
  /* Store stack top for GC */
  stack_top = (uintptr_t *)&_estack;

  /* Jump to main function */ 
  main(sizeof(argv)/sizeof(char *), argv);
  while (true) {
    __asm__("wfi");   /* Infinite loop */
  }
}


