
#include <stdio.h>
#include <stdint.h>

#define RCC_BASE_ADDR           (0x40023800UL)
#define RCC_CFGR_REG_OFFSET     (0x08UL)
#define RCC_CFGR_REG_ADDR       (RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)

#define RCC_AHB1ENR_REG_OFFSET  (0x30UL)
#define RCC_AHB1ENR_REG_ADDR    (RCC_BASE_ADDR + RCC_AHB1ENR_REG_OFFSET)

#define GPIOA_BASE_ADDR         (0x40020000UL)
#define GPIOA_MODER_REG_OFFSET  (0x00UL)
#define GPIOA_MODER_REG_ADDR    (GPIOA_BASE_ADDR + GPIOA_MODER_REG_OFFSET)
#define GPIOA_AFRH_REG_OFFSET   (0x24UL)
#define GPIOA_AFRH_REG_ADDR     (GPIOA_BASE_ADDR + GPIOA_AFRH_REG_OFFSET)

#define RCC_CR_REG_OFFSET       (0x00UL)
#define RCC_CR_REG_ADDR         (RCC_BASE_ADDR + RCC_CR_REG_OFFSET)

int main(void)
{
  // Configure the RCC_CFGR register
  uint32_t *pRCC_CFGR_REG = (uint32_t*)RCC_CFGR_REG_ADDR;

  // Configure the RCC_CR register
  uint32_t *pRCC_CR_REG = (uint32_t*)RCC_CR_REG_ADDR;

  // Configure the RCC_CR_REG HSEON field to enable HSE clock src.
  // 0b1: HSE clock enabled
  // Bits 16 HSEON: | 1 = HSE clock enabled
  *pRCC_CR_REG |= (0b1 << 16); // set 16th bit position which enables HSE as clock src.

  // wait for HSERDY bit to be set
  if ((*pRCC_CR_REG & (0b1 << 17)) == 0) // if HSERDY bit is not set
  {
    // wait for HSERDY bit to be set
    while ((*pRCC_CR_REG & (0b1 << 17)) == 0);
  }

  // switch system clock to HSE
  // Bits 1:0 SW[1:0]: System clock switch
  // 0b01: HSE selected as system clock
  // 0b00: HSI oscillator selected as system clock
  // 0b10: PLL selected as system clock
  // *pRCC_CFGR_REG &= ~(0b11 << 0); // clear 0th and 1st bit positions | but its not allowed on stm32f4discovery
  *pRCC_CFGR_REG |= (0b01 << 0); // set 0th and 1st bit positions | 0b01: HSE selected as system clock | set HSEON bit

  // wait for SWS[1:0] bits to be set to 0b01
  if ((*pRCC_CFGR_REG & (0b11 << 2)) == 0) // if SWS[1:0] bits are not set to 0b01
  {
    // wait for SWS[1:0] bits to be set to 0b01
    while ((*pRCC_CFGR_REG & (0b11 << 2)) == 0);
  }

  // Configure the RCC_CFGR MCO1 field to HSE clock src.
  // 0b01: HSE clock selected
  // Bits 22:21 MCO1: | 01 = HSE clock selected
  *pRCC_CFGR_REG &= ~(0b11 << 21); // clear 21st and 22nd bit positions which selects HSE as clock src.
  *pRCC_CFGR_REG |= (1 << 22);  // set 22nd bit position which selects HSE as clock src.

  // configure mco1 prescaler
  // 0b011: divide by 4
  // Bits 26:25 MCO1PRE: | 100 = divide by 4
  *pRCC_CFGR_REG &= ~(0b111 << 25); // clear 25th, 26th and 27th bit positions which selects divide by 4 as clock src.
  *pRCC_CFGR_REG |= (0b011 << 25);  // set 25th, 26th bit positions which selects divide by 4 as clock src.

  // in RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) bit 0 is for GPIOA peripheral clock enable
  // 0: GPIOA clock disabled
  // 1: GPIOA clock enabled
  // Bits 0:0 GPIOAEN: | 1 = GPIOA clock enabled
  uint32_t *pRCC_AHB1ENR_REG = (uint32_t*)RCC_AHB1ENR_REG_ADDR;
  *pRCC_AHB1ENR_REG |= (1 << 0); // Enable peripheral clock for GPIOA peripheral

  // Configure the GPIOA peripheral mode register (GPIOx_MODER) to configure PA8 as alternate function mode
  // GPIOA_MODER register address
  uint32_t *pGPIOA_MODER_REG = (uint32_t*)GPIOA_MODER_REG_ADDR;
  // 0b10: Alternate function mode
  *pGPIOA_MODER_REG &= ~(0b11 << 16); // clear 16th and 17th bit positions
  *pGPIOA_MODER_REG |= (0b10 << 16);  // configure PA8 as alternate function mode

  // Configure the GPIOA peripheral alternate function high register (GPIOx_AFRH) to configure PA8 as AF0 mode
  // GPIOA_AFRH register address
  uint32_t *pGPIOA_AFRH_REG = (uint32_t*)(GPIOA_AFRH_REG_ADDR);
  // 0b0000: AF0 mode
  *pGPIOA_AFRH_REG &= ~(0b1111 << 0); // clear 0th to 3rd bit positions
  // 0b1111 = 15 = 0xF
  *pGPIOA_AFRH_REG |= (0b0000 << 0);  // configure PA8 as AF0 mode

  for(;;);
  return 0;
}
