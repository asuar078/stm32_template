/*
 * EventGroup.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#ifndef LIB_FREERTOS_CPP_EVENTGROUP_HPP_
#define LIB_FREERTOS_CPP_EVENTGROUP_HPP_

#include "FreeRTOS.h"
#include "event_groups.h"

namespace freertos {

  /**
   *  @todo - document this class
   */
  class EventGroup {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:

    /**
     *  Construct a Event Group
     */
    EventGroup();

#if(configSUPPORT_STATIC_ALLOCATION == 1)
    /**
     *  Construct a Event Group with static allocation
     */
    EventGroup(StaticEventGroup_t *pxEventGroupBuffer);
#endif

    /**
     *  Our destructor
     */
    virtual ~EventGroup();

    /**
     *  Allow two or more tasks to use an event group to sync each other.
     *
     *  @param uxBitsToSet A bit mask that specifies the event bit, or
     *  event bits, to set 1 in the event group. The value of the event
     *  groups is updated by bitwise ORing the event group's existing
     *  value with the passed in uxBitsToSet.
     *
     *  @param uxBitsToWaitFor The bit or bits to set in the event
     *  group before determining if (and possibly waiting for), all the
     *  bits specified by the uxBitsToWait parameter are set.
     *
     *  @param xTicksToWait The maximum amount of time (specified in
     *  'ticks') to wait for all the bits specified by the
     *  uxBitsToWaitFor parameter value to become set.
     *
     *  @return If EventGroup::Sync returned because all the bits it
     *  was waiting for were set then the returned value is the event
     *  group value before any bits were automatically cleared.
     *  If EventGroup::Sync returned because its timeout expired then
     *  not all the bits being waited for will be set.
     */
    EventBits_t Sync(const EventBits_t uxBitsToSet,
                     const EventBits_t uxBitsToWaitFor,
                     TickType_t xTicksToWait);

    /**
     *  Read bits within an RTOS event group, optionally entering the
     *  Blocked state (with a timeout) to wait for a bit or group of
     *  bits to become set.
     *
     *  @param uxBitsToWaitFor A bitwise value that indicates the bit
     *  or bits to test inside the event group.
     *
     *  @param xClearOnExit If xClearOnExit is set to true then any bits
     *  set in the value passed as the uxBitsToWaitFor parameter will be
     *  cleared in the event group before EventGroup::WaitBits returns
     *  if EventGroup::WaitBits returns for any reason other than a
     *  timeout. The timeout value is set by the xTicksToWait parameter.
     *  If xClearOnExit is set to false then the bits set in the event
     *  group are not altered when the call to EventGroup::WaitBits
     *  returns.
     *
     *  @param xWaitForAllBits is used to create either a logical AND
     *  test (where all bits must be set) or a logical OR test (where
     *  one or more bits must be set) as follows:
     *
     *      @par    If xWaitForAllBits is set to true then
     *              EventGroup::WaitBits will return when either all the
     *              bits set in the value passed as the uxBitsToWaitFor
     *              parameter are set in the event group or the specified
     *              block time expires.
     *
     *      @par   If xWaitForAllBits is set to false then
     *              EventGroup::WaitBits will return when any of the bits
     *              set in the value passed as the uxBitsToWaitFor
     *              parameter are set in the event group or the specified
     *              block time expires.
     *
     *  @param xTicksToWait The maximum amount of time (specified in
     *  'ticks') to wait for one/all (depending on the xWaitForAllBits
     *  value) of the bits specified by uxBitsToWaitFor to become set.
     *
     *  @return The value of the event group at the time either the event
     *  bits being waited for became set, or the block time expired. The
     *  current value of the event bits in an event group will be
     *  different to the returned value if a higher priority task or
     *  interrupt changed the value of an event bit between the calling
     *  task leaving the Blocked state and exiting the
     *  EventGroup::WaitBits function. Test the return value to know
     *  which bits were set. If EventGroup::WaitBits returned because its
     *  timeout expired then not all the bits being waited for will be
     *  set. If EventGroup::WaitBits returned because the bits it was
     *  waiting for were set then the returned value is the event group
     *  value before any bits were automatically cleared because the
     *  xClearOnExit parameter was set to true.
     */
    EventBits_t WaitBits(const EventBits_t uxBitsToWaitFor, bool xClearOnExit,
                         bool xWaitForAllBits, TickType_t xTicksToWait);

    /**
     *  Clear bits (flags) within an event group.
     *
     *  @param uxBitsToClear A bitwise value that indicates the bit or
     *  bits to clear in the event group.
     *
     *  @return The value of the event group before the specified bits
     *  were cleared.
     */
    EventBits_t ClearBits(const EventBits_t uxBitsToClear);

    /**
     *  Clear bits (flags) within an event group from ISR context.
     *
     *  @param uxBitsToClear A bitwise value that indicates the bit or
     *  bits to clear in the event group.
     *
     *  @return The value of the event group before the specified bits
     *  were cleared.
     */
//      BaseType_t ClearBitsFromISR(const EventBits_t uxBitsToClear);

    /**
     *  Returns the current value of the event bits (event flags) in an
     *  event group.
     *
     *  @return The value of the event bits in the event group at the time
     *  EventGroup::GetBitsFromISR was called.
     */
    EventBits_t GetBits();

    /**
     *  Returns the current value of the event bits (event flags) in an
     *  event group from ISR context.
     *
     *  @return The value of the event bits in the event group at the
     *  time EventGroup::GetBitsFromISR was called.
     */
    EventBits_t GetBitsFromISR();

    /**
     *  Set bits (flags) within an event group.
     *
     *  @param uxBitsToSet A bitwise value that indicates the bit or bits
     *  to set in the event group.
     *
     *  @return The value of the event group at the time the call to
     *  EventGroup::SetBits returns
     */
    EventBits_t SetBits(const EventBits_t uxBitsToSet);

#if ((configUSE_TRACE_FACILITY == 1) && (INCLUDE_xTimerPendFunctionCall == 1) && (configUSE_TIMERS == 1))
    /**
     *  Set bits (flags) within an event group from ISR context.
     *
     *  @param uxBitsToSet A bitwise value that indicates the bit or bits
     *  to set in the event group.
     *
     *  @param pxHigherPriorityTaskWoken A bitwise value that indicates
     *  the bit or bits to set in the event group.
     *
     *  @return Calling this function will result in a message being sent
     *  to the RTOS daemon task. If the priority of the daemon task is
     *  higher than the priority of the currently running task (the
     *  task the interrupt interrupted) then *pxHigherPriorityTaskWoken
     *  will be set to true by EventGroupSetBitsFromISR, indicating that
     *  a context switch should be requested before the interrupt exits.
     *  For that reason *pxHigherPriorityTaskWoken must be initialised
     *  to false.
     */
    BaseType_t SetBitsFromISR( const EventBits_t uxBitsToSet,
        BaseType_t *pxHigherPriorityTaskWoken);

#endif

    /////////////////////////////////////////////////////////////////////////
    //
    //  Protected API
    //  Not intended for use by application code.
    //
    /////////////////////////////////////////////////////////////////////////
    protected:
    /**
     *  FreeRTOS Event Group handle.
     */
    EventGroupHandle_t handle;

  };
} /* namespace freertos */

#endif /* LIB_FREERTOS_CPP_EVENTGROUP_HPP_ */
