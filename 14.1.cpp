#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define LED1_PIN GPIO_Pin_0
#define LED2_PIN GPIO_Pin_1
#define LED3_PIN GPIO_Pin_10
#define LED_PORT GPIOB

#define BIT_TASK1 (1 << 0)
#define BIT_TASK2 (1 << 1)
#define BIT_TASK3 (1 << 2)

EventGroupHandle_t xEventGroup;

void GPIO_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_Init(LED_PORT, &gpio);
}

void Task1(void *pvParameters) {
    for (;;) {
        xEventGroupWaitBits(xEventGroup, BIT_TASK1, pdTRUE, pdFALSE, portMAX_DELAY);
        GPIO_SetBits(LED_PORT, LED1_PIN);
        vTaskDelay(500 / portTICK_RATE_MS);
        GPIO_ResetBits(LED_PORT, LED1_PIN);
    }
}

void Task2(void *pvParameters) {
    for (;;) {
        xEventGroupWaitBits(xEventGroup, BIT_TASK2, pdTRUE, pdFALSE, portMAX_DELAY);
        GPIO_SetBits(LED_PORT, LED2_PIN);
        vTaskDelay(500 / portTICK_RATE_MS);
        GPIO_ResetBits(LED_PORT, LED2_PIN);
    }
}

void Task3(void *pvParameters) {
    for (;;) {
        xEventGroupWaitBits(xEventGroup, BIT_TASK3, pdTRUE, pdFALSE, portMAX_DELAY);
        GPIO_SetBits(LED_PORT, LED3_PIN);
        vTaskDelay(500 / portTICK_RATE_MS);
        GPIO_ResetBits(LED_PORT, LED3_PIN);
    }
}

void Task_Control(void *pvParameters) {
    for (;;) {
        xEventGroupSetBits(xEventGroup, BIT_TASK1);
        vTaskDelay(1000 / portTICK_RATE_MS);
        xEventGroupSetBits(xEventGroup, BIT_TASK2);
        vTaskDelay(1000 / portTICK_RATE_MS);
        xEventGroupSetBits(xEventGroup, BIT_TASK3);
        vTaskDelay(1000 / portTICK_RATE_MS);
        xEventGroupSetBits(xEventGroup, BIT_TASK1 | BIT_TASK2 | BIT_TASK3);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

int main(void) {
    SystemInit();
    GPIO_Config();
    xEventGroup = xEventGroupCreate();
    xTaskCreate(Task_Control, "Ctrl", 128, NULL, 2, NULL);
    xTaskCreate(Task1, "T1", 128, NULL, 1, NULL);
    xTaskCreate(Task2, "T2", 128, NULL, 1, NULL);
    xTaskCreate(Task3, "T3", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    while (1);
}