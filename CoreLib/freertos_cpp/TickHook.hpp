#ifndef LIB_FREERTOS_CPP_READ_WRITE_LOCK_HPP_
#define LIB_FREERTOS_CPP_READ_WRITE_LOCK_HPP_

#include "FreeRTOS.h"
#include "task.h"
#include <list>

#if (configUSE_TICK_HOOK == 0)

/**
 *  FreeRTOS expects this function to exist and requires it to be 
 *  named as such with the following signature.
 */
extern "C" void vApplicationTickHook(void);

namespace freertos {

/**
 *  Wrapper class for Tick hooks, functions you want to run within 
 *  the tick ISR. 
 *
 *  This is an abstract base class.
 *  To use this, you need to subclass it. All of your tick functions 
 *  should be derived from this class. Then implement the virtual run
 *  function. 
 *
 *  You can register multiple hooks with this class. The order of 
 *  execution should not be assumed. All tick hooks will execute 
 *  every tick.
 */
  class TickHook {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //  Available from anywhere.
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
     *  Constructor.
     */
    TickHook();

    /**
     *  Destructor
     */
    virtual ~TickHook();

    /**
     *  After this is called your run routine will execute in the
     *  Tick ISR. This registration cannot be done in the base class
     *  constructor. Once your object is fully constructed, you "may"
     *  call this in your derived class's constructor.
     *  @note Immedately after you call this function, your TickHook
     *  run() method will run, perhaps before you even return from this
     *  call. You "must" be ready to run before you call register().
     */
    void registerTickHook();

    /**
     *  disable the tick hook from running, without removing it
     *  from the tick hook list.
     */
    void disable();

    /**
     *  enable this Idle Hook to run. This call is not necessary
     *  if you haven't called disable.
     */
    void enable();


    /////////////////////////////////////////////////////////////////////////
    //
    //  Protected API
    //
    /////////////////////////////////////////////////////////////////////////
    protected:
    /**
     *  Implementation of your actual Tick Hook code.
     *  You must override this function.
     */
    virtual void run() = 0;


    /////////////////////////////////////////////////////////////////////////
    //
    //  Private API
    //  The internals of this wrapper class.
    //
    /////////////////////////////////////////////////////////////////////////
    private:
    /**
     *  List of Tick Hook callbacks that are executed in the
     *  Tick ISR.
     */
    static std::list<TickHook *> callbacks;

    /**
     *  Should the tick hook run?
     */
    bool enabled;

    /**
     *  Allow the global vApplicationTickHook() function access
     *  to the internals of this class. This simplifies the overall
     *  design.
     */
    friend void::vApplicationTickHook();
  };


}
#endif

#endif
