#include "timer.h"
#include <kernel/arch/arch.h>
#include <kernel/utils/logger.h>


#ifdef __x86_64__
#include <kernel/arch/x86_64/timer/pit.h>
enum class Timer {
  PIT,
};
#endif

static Timer selectedTimer;

auto arch::timer::initialize() -> void {

#ifdef __x86_64__
  selectedTimer = Timer::PIT;
#endif

}

auto arch::timer::wait(u64 nanos) -> void {

#ifdef __x86_64__
  switch (selectedTimer) {
    case Timer::PIT:
      x86_64::PIT::wait(nanos);
      break;
  }
#endif

}


auto arch::timer::setInterruptCallback(arch::interrupt::InterruptHandler func, u64 nanos) -> void {
  log::debug("timer: setting timer interrupt callback");
  arch::interrupt::setInterruptHandler(32, func);

  switch (selectedTimer) {
    case Timer::PIT:
      x86_64::PIT::setPeriodic(nanos);
      break;
  }

}
