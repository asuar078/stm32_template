/*
 * Tick.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#ifndef LIB_FREERTOS_CPP_TICK_HPP_
#define LIB_FREERTOS_CPP_TICK_HPP_

#include "FreeRTOS.h"
#include "task.h"

namespace freertos {

  /**
   *  Class encapsulating FreeRTOS idea of a tick.
   */
  class Ticks {

    public:
    /**
     *  Get the current tick count.
     *
     *  @return Current tick count.
     */
    static inline TickType_t GetTicks() {
      return xTaskGetTickCount();
    }

    /**
     *  Get the current tick count from ISR context.
     *
     *  @return Current tick count.
     */
    static inline TickType_t GetTicksFromISR() {
      return xTaskGetTickCountFromISR();
    }

    /**
     *  Convert from ticks to ms.
     *
     *  @param ticks ticks to convert.
     *  @return milliseconds.
     */
    static inline TickType_t TicksToMs(TickType_t ticks) {
      return ticks * portTICK_PERIOD_MS;
    }

    /**
     *  Convert from ms to ticks.
     *
     *  @param milliseconds milliseconds to convert.
     *  @return ticks
     */
    static inline TickType_t MsToTicks(TickType_t milliseconds) {
      return milliseconds / portTICK_PERIOD_MS;
    }

    /**
     *  Convert from seconds to ticks.
     *
     *  @param seconds seconds to convert.
     *  @return ticks
     */
    static inline TickType_t SecondsToTicks(TickType_t seconds) {
      return (seconds * 1000) / portTICK_PERIOD_MS;
    }
  };

}

#endif /* LIB_FREERTOS_CPP_TICK_HPP_ */
