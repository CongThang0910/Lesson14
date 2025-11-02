#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define USE_SLEEP 1   

volatile uint32_t tick_count = 0;

/* ----- Ngat SysTick 1ms ----- */
void SysTick_Handler(void)
{
    tick_count++;
}

/* ----- Hàm delay su dung Sleep hoac NOP ----- */
void delay_ms(uint32_t ms)
{
    uint32_t start = tick_count;
    while ((tick_count - start) < ms)
    {
#if USE_SLEEP
        __WFI();   
#else
        __NOP();   // CPU van chay bình thuong
#endif
    }
}

/* ----- Main ----- */
int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000); // tao ngat 1ms

    /* Bat clock cho PORTA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Cau hình PA8 làm output push-pull */
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_8;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);

    while (1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);  // LED ON (PA8 = 0)
        delay_ms(5000);

        GPIO_SetBits(GPIOA, GPIO_Pin_8);    // LED OFF (PA8 = 1)
        delay_ms(5000);
    }
}