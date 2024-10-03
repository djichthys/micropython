#include "py/runtime.h"
#include "py/gc.h"

/* _estack to be defined and exported from the linker script */
extern uint32_t _estack; 
static char *argv[] = { "upy_sonata" }; 
static uintptr_t *stack_top;
static char heap[MICROPY_HEAP_SIZE];

int start_repl(int arg, char **argv)
{
    int idx, tval = arg+10;
    int c = tval;
    for( idx=0; idx < tval; idx++) { 
      c = idx + 10; 
    } 
    return c; 
}

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



int main(int argc, char **argv) 
{
    char *myfp = heap;

    #if MICROPY_ENABLE_GC 
    gc_init(heap, heap + sizeof(heap));
    #endif
    mp_init();

    __asm__ volatile( "mv t2, s0\n\t"
                      "mv %[o_reg], t2\n\t"
                      "add %[o_reg], %[o_reg], %[i_reg]\n\t"
                     : [o_reg] "+r" (myfp)
                     :     [i_reg] "r" (argc)
                 : "t2"); 
    return 0;
}


void nlr_jump_fail(void *val) {
    while (true) {
        ;
    }
} 


__attribute__((noreturn)) 
void upy_start()
{
  int retval, c = 0; 
  stack_top = (uintptr_t *)&_estack;
  retval = main(sizeof(argv)/sizeof(char *), argv);
  retval = retval + c; 
  while(1) {
    c++;  /* replace with sleep and yield interrupt routine */
  }
}


