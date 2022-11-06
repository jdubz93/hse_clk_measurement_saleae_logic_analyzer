
#include <stdio.h>
#include <stdint.h>

#define RCC_BASE_ADDR           (0x40023800UL)
#define RCC_CFGR_REG_OFFSET     (0x08UL)
#define RCC_CFGR_REG_ADDR       (RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)

#define GPIOA_BASE_ADDR         (0x40020000UL)


int main(void)
{
  // Configure the RCC_CFGR register
  uint32_t *pRCC_CFGR_REG = (uint32_t*)RCC_CFGR_REG_ADDR;

  // Configure the RCC_CFGR MCO1 field to HSI clock src.
  // 0b00: HSI clock selected
  // Bits 22:21 MCO1: | 00 = HSI clock selected
  *pRCC_CFGR_REG &= ~(0b11 << 21); // clear 21st and 22nd bit positions which selects HSI as clock src.

  // Configure MC01 prescaler
  *pRCC_CFGR_REG |= (0b11 << 25);

  // Enable peripheral clock for GPIOA peripheral
  uint32_t *pRCC_AHB1ENR_REG = (uint32_t*)(RCC_BASE_ADDR + 0x30U); // I can get the AHB1ENR register address by adding 0x30 to the RCC_BASE_ADDR
  *pRCC_AHB1ENR_REG |= (1 << 0); // enable clock for GPIOA peripheral

  // in RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) bit 0 is for GPIOA peripheral clock enable
  // 0: GPIOA clock disabled
  // 1: GPIOA clock enabled

  // Configure the GPIOA peripheral mode register (GPIOx_MODER) to configure PA8 as alternate function mode

  // GPIOA_MODER register address
  uint32_t *pGPIOA_MODER_REG = (uint32_t*)0x40020000UL;
  // 0b10: Alternate function mode
  *pGPIOA_MODER_REG &= ~(0b11 << 16); // clear 16th and 17th bit positions
  *pGPIOA_MODER_REG |= (0b10 << 16);  // configure PA8 as alternate function mode

  // Configure the GPIOA peripheral alternate function high register (GPIOx_AFRH) to configure PA8 as AF0 mode
  // GPIOA_AFRH register address
  uint32_t *pGPIOA_AFRH_REG = (uint32_t*)(GPIOA_BASE_ADDR + 0x24U); // I can get the AFRH register address by adding (offset) 0x24 to the GPIOA_BASE_ADDR
  // 0b0000: AF0 mode
  *pGPIOA_AFRH_REG &= ~(0b1111 << 0); // clear 0th to 3rd bit positions
  // 0b1111 = 15 = 0xF
  *pGPIOA_AFRH_REG |= (0b0000 << 0);  // configure PA8 as AF0 mode





  for(;;);
  return 0;
}