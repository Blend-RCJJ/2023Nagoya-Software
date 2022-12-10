#include "RTOS-Kit.h"

RTOS_Kit::RTOS_Kit() {
}

void RTOS_Kit::create(TaskFunction_t funcPtr, bool isMainApp) {
    for (int i = 0; i < maximumAppCount; i++) {
        if (funcPtrArray[i] == NULL) {
            funcPtrArray[i] = funcPtr;

            xTaskCreate(funcPtrArray[i], "app", 1000, NULL, 1,
                        &handlerArray[i]);
            vTaskSuspend(handlerArray[i]);

            if (isMainApp) {
                mainAppNumber = i;
            }
            break;
        }
    }
}

void RTOS_Kit::start(TaskFunction_t funcPtr) {
    for (int i = 0; i < maximumAppCount; i++) {
        if (funcPtrArray[i] == funcPtr) {
            vTaskResume(handlerArray[i]);

            break;
        }
    }
}

void RTOS_Kit::stop(TaskFunction_t funcPtr) {
    for (int i = 0; i < maximumAppCount; i++) {
        if (funcPtrArray[i] == funcPtr) {
            vTaskSuspend(handlerArray[i]);

            break;
        }
    }
}

void RTOS_Kit::startRTOS(void) {
    vTaskResume(handlerArray[mainAppNumber]);
    vTaskStartScheduler();
}