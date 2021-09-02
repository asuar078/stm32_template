#include "TickHook.hpp"

#if (configUSE_TICK_HOOK == 0)

using namespace std;
using namespace freertos;

list<TickHook *> TickHook::callbacks;


TickHook::TickHook()
    : enabled(true) {
}


TickHook::~TickHook() {
  taskENTER_CRITICAL();
  callbacks.remove(this);
  taskEXIT_CRITICAL();
}


void TickHook::registerTickHook() {
  taskENTER_CRITICAL();
  callbacks.push_front(this);
  taskEXIT_CRITICAL();
}


void TickHook::disable() {
  taskENTER_CRITICAL();
  enabled = false;
  taskEXIT_CRITICAL();
}


void TickHook::enable() {
  taskENTER_CRITICAL();
  enabled = true;
  taskEXIT_CRITICAL();
}


/**
 *  We are a friend of the Tick class, which makes this much simplier.
 */
void vApplicationTickHook(void) {
  for (list<TickHook *>::iterator it = TickHook::callbacks.begin();
       it != TickHook::callbacks.end();
       ++it) {

    TickHook *tickHookObject = *it;

    if (tickHookObject->enabled) {
      tickHookObject->run();
    }
  }
}

#endif
