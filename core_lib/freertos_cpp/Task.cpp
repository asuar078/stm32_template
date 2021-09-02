/*
 * Task.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: kolban
 */

#include <Task.hpp>
#include <FreeRTOS.h>
#include <task.h>
#include <string>

namespace freertos {

/**
 * @brief Create an instance of the task class.
 *
 * @param [in] taskName The name of the task to create.
 * @param [in] stackSize The size of the stack.
 * @return N/A.
 */
  Task::Task(const char* taskName, uint16_t stackSize, uint8_t priority)
      : m_taskName(taskName),
        m_stackSize(stackSize),
        m_priority(priority),
        m_taskData(nullptr),
        m_handle(nullptr),
        taskStarted(false)
  {

  } // Task

  Task::~Task()
  {
#if (INCLUDE_vTaskDelete == 1)
    vTaskDelete(m_handle);
    m_handle = (TaskHandle_t) -1;
#else
    configASSERT(!"Cannot actually delete a thread object "
                  "if INCLUDE_vTaskDelete is not defined.");
#endif
  } // ~Task

  bool Task::start(void* taskData)
  {
    if (taskStarted) {
      return false;
    }
    else {
      taskStarted = true;
    }

    m_taskData = taskData;
    BaseType_t rc = xTaskCreate(taskFunctionAdapter,
        m_taskName,
        m_stackSize,
        this,
        m_priority,
        &m_handle);

    return rc != pdPASS ? false : true;
  }

  void Task::taskFunctionAdapter(void* pvParameters)
  {
    Task* thread = static_cast<Task*>(pvParameters);
    thread->run();

#if (INCLUDE_vTaskDelete == 1)

    thread->cleanup();

    vTaskDelete(thread->m_handle);

#else
    configASSERT(!"Cannot return from a thread.run function "
                  "if INCLUDE_vTaskDelete is not defined.");
#endif
  }

//
//  Deliberately empty. If this is needed, it will be overloaded.
//
  void Task::cleanup()
  {
  }

/**
 * @brief Stop the task.
 *
 * @return N/A.
 */
  void Task::stop()
  {
    if (m_handle == nullptr) {
      return;
    }
    TaskHandle_t temp = m_handle;
    m_handle = nullptr;
    ::vTaskDelete(temp);
  } // stop

/**
 * @brief Set the stack size of the task.
 *
 * @param [in] stackSize The size of the stack for the task.
 * @return N/A.
 */
  void Task::setStackSize(uint16_t stackSize)
  {
    m_stackSize = stackSize;
  } // setStackSize

  uint16_t Task::getStackSize()
  {
    return m_stackSize;
  }

/**
 * @brief Set the name of the task.
 *
 * @param [in] name The name for the task.
 * @return N/A.
 */
  void Task::setName(const char* name)
  {
    m_taskName = name;
  }

  const char* Task::getName()
  {
    return m_taskName;
  }

#if (INCLUDE_vTaskDelayUntil == 1)

  void Task::delayUntil(const TickType_t Period)
  {
    if (!delayUntilInitialized) {
      delayUntilInitialized = true;
      delayUntilPreviousWakeTime = xTaskGetTickCount();
    }

    vTaskDelayUntil(&delayUntilPreviousWakeTime, Period);
  }

  void Task::resetDelayUntil()
  {
    delayUntilInitialized = false;
  }

#endif

} // namespace freertos
