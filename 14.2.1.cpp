#include "stm32f10x.h"

void Delay_ms(uint32_t ms)
{
    SysTick->LOAD = 72000 - 1;    
    SysTick->VAL = 0;
    SysTick->CTRL = 5;           

    for(uint32_t i = 0; i < ms; i++)
        while(!(SysTick->CTRL & (1 << 16)));

    SysTick->CTRL = 0;
}

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // Enable GPIOA clock

    // PA8 = output push-pull, 10MHz
    GPIOA->CRH &= ~(0xF << 0);           
    GPIOA->CRH |=  (0x1 << 0);           

    while(1)
    {
        GPIOA->ODR ^= (1 << 8); // Toggle PA8
        Delay_ms(5000);         // 5 giây
    }
}