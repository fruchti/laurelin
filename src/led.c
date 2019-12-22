#include "led.h"
#include "stm32f030x6.h"

#define LED_ODR_MASK            ((1 << PIN_LED_R_0) | (1 << PIN_LED_G_0) \
                                | (1 << PIN_LED_B_0) | (1 << PIN_LED_R_1) \
                                | (1 << PIN_LED_G_1) | (1 << PIN_LED_B_1) \
                                | (1 << PIN_LED_R_2) | (1 << PIN_LED_G_2) \
                                | (1 << PIN_LED_B_2) | (1 << PIN_LED_R_3) \
                                | (1 << PIN_LED_G_3) | (1 << PIN_LED_B_3))

#define LED_MODER_MASK          ((3 << PIN_LED_R_0 * 2) | (3 << PIN_LED_G_0 * 2) \
                                | (3 << PIN_LED_B_0 * 2) | (3 << PIN_LED_R_1 * 2) \
                                | (3 << PIN_LED_G_1 * 2) | (3 << PIN_LED_B_1 * 2) \
                                | (3 << PIN_LED_R_2 * 2) | (3 << PIN_LED_G_2 * 2) \
                                | (3 << PIN_LED_B_2 * 2) | (3 << PIN_LED_R_3 * 2) \
                                | (3 << PIN_LED_G_3 * 2) | (3 << PIN_LED_B_3 * 2))

#define LED_MODER               ((1 << PIN_LED_R_0 * 2) | (1 << PIN_LED_G_0 * 2) \
                                | (1 << PIN_LED_B_0 * 2) | (1 << PIN_LED_R_1 * 2) \
                                | (1 << PIN_LED_G_1 * 2) | (1 << PIN_LED_B_1 * 2) \
                                | (1 << PIN_LED_R_2 * 2) | (1 << PIN_LED_G_2 * 2) \
                                | (1 << PIN_LED_B_2 * 2) | (1 << PIN_LED_R_3 * 2) \
                                | (1 << PIN_LED_G_3 * 2) | (1 << PIN_LED_B_3 * 2))

uint8_t LED_Data[LED_ROWS][LED_COLUMNS] = {{0}};

// TIM3 is clocked by APB1 and thus receives only half the system clock. The 4
// LSBs have bit lengths 2, 4, 8, and 16 cycles and are generated blocking from
// an inline assembly block and are thus not in this table.
static const uint16_t LED_BitLengths[LED_BITS - 4] =
{
    16, 32, 64, 128, 256, 512, 1024, 2048
};

static const int LED_Pins[LED_COLUMNS] =
{
    PIN_LED_R_0, PIN_LED_G_0, PIN_LED_B_0,
    PIN_LED_R_1, PIN_LED_G_1, PIN_LED_B_1,
    PIN_LED_R_2, PIN_LED_G_2, PIN_LED_B_2,
    PIN_LED_R_3, PIN_LED_G_3, PIN_LED_B_3
};

static uint16_t LED_DMABuffer[LED_ROWS * (LED_BITS + 1)];

static void LED_RefreshDMABuffer(void)
{
    for(int r = 0; r < LED_ROWS; r++)
    {
        for(int i = 0; i < LED_COLUMNS; i++)
        {
            uint16_t gamma_corrected = (uint16_t)LED_Data[r][i];
            gamma_corrected *= gamma_corrected;
            gamma_corrected >>= 16 - LED_BITS;

            for(int j = 0; j < LED_BITS; j++)
            {
                if(gamma_corrected & (1 << j))
                {
                    LED_DMABuffer[r * (LED_BITS + 1) + j] &=
                        ~(1 << LED_Pins[i]);
                }
                else
                {
                    LED_DMABuffer[r * (LED_BITS + 1) + j] |= 1 << LED_Pins[i];
                }
            }
        }
        // Data to reset outputs after all data bits are sent
        LED_DMABuffer[r * (LED_BITS + 1) + LED_BITS] = LED_ODR_MASK;
    }
}

static void LED_StartBCM(int row)
{
    // Reset DMA and timer
    TIM3->CR1 = 0x0000;
    TIM3->DIER = 0x0000;
    TIM3->CNT = 0;
    DMA1_Channel3->CCR = 0x0000;
    DMA1_Channel4->CCR = 0x0000;

    TIM3->ARR = 1; // Delay before everything starts with the first DMA request
    TIM3->CR1 = TIM_CR1_ARPE;
    // Since ARPE is set, this write goes to ARR's shadow register
    TIM3->ARR = LED_BitLengths[0];
    TIM3->DIER = TIM_DIER_UDE | TIM_DIER_CC1DE;

    // DMA channel 3: Output data to port a on TIM3 update
    DMA1_Channel3->CMAR = (uint32_t)&(LED_DMABuffer[row * (LED_BITS + 1) + 4]);
    // One transfer for each bit plus one to set the outputs to zero again.
    // The first 4 are sent out with assembly before the first DMA transfer.
    DMA1_Channel3->CNDTR = LED_BITS + 1 - 4;
    // Highest priority, 16 bits, increment memory, memory to peripheral
    DMA1_Channel3->CCR = DMA_CCR_PL | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0
        | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_TCIE;

    // DMA channel 3: Output data to port a on TIM3 update
    // The bit lengths table is offset because the first value is already in
    // the timer's ARR shadow register.
    DMA1_Channel4->CMAR = (uint32_t)&(LED_BitLengths[1]);
    // The first four bits are sent out with the assembly code below, so only
    // LED_BITS - 5 bit lengths are needed.
    DMA1_Channel4->CNDTR = LED_BITS - 1 - 4;
    // 16 bits, memory increment mode, memory to peripheral
    DMA1_Channel4->CCR = DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC
        | DMA_CCR_DIR | DMA_CCR_EN;

    // Send the 4 LSBs, set to zero at the end (so the timing is independent
    // from the delay introduced by the DMA)
    __asm__ volatile(".syntax unified\n"
            "str %[d0], [%[odr]];"
            "str %[d1], [%[odr]];"
            "nop;"
            "nop;"
            "str %[d2], [%[odr]];"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "str %[d3], [%[odr]];"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "nop;"
            "str %[off], [%[odr]];"
            :
            : [odr] "l" ((uint32_t)&(GPIOA->ODR)),
              [d0] "r" (LED_DMABuffer[row * (LED_BITS + 1) + 0]),
              [d1] "r" (LED_DMABuffer[row * (LED_BITS + 1) + 1]),
              [d2] "r" (LED_DMABuffer[row * (LED_BITS + 1) + 2]),
              [d3] "r" (LED_DMABuffer[row * (LED_BITS + 1) + 3]),
              [off] "r" (LED_ODR_MASK)
            :);

    // Enable TIM3 for the rest of the bits
    TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}

static inline void LED_PulseRowClock(void)
{
    __asm__ volatile("nop");
    GPIOF->BSRR = (1 << PIN_ROW_SCK);
    __asm__ volatile("nop");
    GPIOF->BRR = (1 << PIN_ROW_SCK);
}

void LED_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    GPIOF->ODR &= ~(1 << PIN_ROW_SCK) & ~(1 << PIN_ROW_DATA);
    GPIOF->MODER = (GPIOF->MODER
        & ~(0x3 << PIN_ROW_SCK * 2) & ~(0x3 << PIN_ROW_DATA * 2))
        | (0x1 << PIN_ROW_SCK * 2) | (0x1 << PIN_ROW_DATA * 2);

    // Reset the shift register. Since RCK and SCK are shorted together, one
    // additional clock cycle is needed.
    GPIOF->BSRR = (1 << PIN_ROW_DATA);
    for(int i = 0; i < LED_ROWS + 1; i++)
    {
        LED_PulseRowClock();
    }
    // All shift register outputs are now '1'. Because the rows are driven with
    // external transistors, this means all rows are off.

    GPIOA->ODR |= LED_ODR_MASK;
    GPIOA->PUPDR &= ~LED_MODER_MASK;
    GPIOA->OTYPER |= LED_ODR_MASK;
    GPIOA->MODER = (GPIOA->MODER & ~LED_MODER_MASK) | LED_MODER;

    TIM3->CR1 = 0x0000;
    TIM3->PSC = 0;
    // CH1 will be used for DMA requests updating ARR
    TIM3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    TIM3->CCER = TIM_CCER_CC1E;
    TIM3->CCR1 = 1;

    // DMA channel 3: Output data to port a on TIM3 update
    DMA1_Channel3->CPAR = (uint32_t)&(GPIOA->ODR);

    // DMA channel 4: Update TIM3 ARR on TIM3 CH1 (which is set to 0)
    DMA1_Channel4->CPAR = (uint32_t)&(TIM3->ARR);

    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

    LED_RefreshDMABuffer();
    LED_StartBCM(0);
}

void DMA1_Channel2_3_IRQHandler(void)
{
    // Interrupt when all bits have been sent
    DMA1->IFCR = DMA_IFCR_CTCIF3;

    static int current_row = 0;
    current_row++;
    if(current_row >= LED_ROWS)
    {
        GPIOF->BRR = 1 << PIN_ROW_DATA;
        current_row = 0;
        LED_PulseRowClock();
        GPIOF->BSRR = 1 << PIN_ROW_DATA;
    }
    else
    {
        LED_PulseRowClock();
    }

    // Start sending bits out again. The row offset caused by the shift
    // register "lagging" one clock cycle behind because RCK and SCK are
    // connected to the same signal doesn't matter here: we're not paying much
    // attention to which rows are which anyway.
    LED_StartBCM(current_row);
}
