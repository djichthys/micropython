#include "common/defs.h"
#include "common/timer-utils.hh"


static inline mp_uint_t mp_hal_ticks_ms(void)
{
    return rdcycle64();
}

static inline void mp_hal_set_interrupt_char(char c)
{
}

static inline mp_uint_t mp_hal_cycles_per_ms(void)
{
  return CPU_TIMER_HZ / 1000;
}
