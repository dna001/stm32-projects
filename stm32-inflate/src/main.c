/* Copyright 2013 */

#include <stdio.h>
#include <string.h>
#include <stm32f4xx.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "tinfl.h"
#include "gzfile.h"

#define IN_BUF_SZ 2048
#define OUT_BUF_SZ (8*1024)
#define MIN(a, b) ((a < b)?a:b)

//static uint8_t in_buf[IN_BUF_SZ];
static uint8_t out_buf[OUT_BUF_SZ];
static uint8_t dictionary[TINFL_LZ_DICT_SIZE];

static void Delay(__IO uint32_t nCount);
static int decompress_test(void);
static int write_buf(const void* p_buf, int len, tinfl_status status);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* GPIOD Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |
                                  GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* GPIOA Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Configure PA0 (Button) in input mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    while (1)
    {
        /* PD12 to be toggled */
        GPIO_SetBits(GPIOD, GPIO_Pin_12);

        /* Decompress test */
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            if (decompress_test() == 0)
            {
                /* PD13 to be toggled */
                GPIO_SetBits(GPIOD, GPIO_Pin_13);
            }
            else
            {
                /* PD14 to be toggled */
                GPIO_SetBits(GPIOD, GPIO_Pin_14);
            }
        }
#if 0
        /* Insert delay */
        Delay(0x3FFFFF);

        /* PD15 to be toggled */
        GPIO_SetBits(GPIOD, GPIO_Pin_15);
#endif
        /* Insert delay */
        Delay(0x7FFFFF);
        GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 |
                       GPIO_Pin_14 | GPIO_Pin_15);

        /* Insert delay */
        Delay(0xFFFFFF);
    }
    return 0;
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}

static int decompress_test(void)
{
    int result = 0;
    static tinfl_decompressor decomp;
    uint8_t* p_dict = dictionary;
    size_t dict_ofs = 0;
    int flags = TINFL_FLAG_PARSE_ZLIB_HEADER | TINFL_FLAG_HAS_MORE_INPUT;
    int i;

    tinfl_init(&decomp);
    for (i=0;i<test_gz_size;i+=2048)
    {
        uint8_t* p_in_buf = &test_gz[i];
        size_t in_buf_ofs = 0;
        size_t chunk_size = MIN(test_gz_size - i, 2048);
        for ( ; ; )
        {
            size_t in_buf_size = chunk_size - in_buf_ofs;
            size_t dst_buf_size = TINFL_LZ_DICT_SIZE - dict_ofs;
            tinfl_status status = tinfl_decompress(&decomp,
                (const mz_uint8*)p_in_buf + in_buf_ofs, &in_buf_size,
                p_dict, p_dict + dict_ofs, &dst_buf_size,
                (flags & ~(TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF)));
            in_buf_ofs += in_buf_size;
            if ((dst_buf_size) && (!(write_buf(p_dict + dict_ofs,
                (int)dst_buf_size, status))))
            {
                break;
            }
            if ((status != TINFL_STATUS_HAS_MORE_OUTPUT) &&
                (status != TINFL_STATUS_NEEDS_MORE_INPUT))
            {
                result = (status == TINFL_STATUS_DONE);
                break;
            }
            dict_ofs = (dict_ofs + dst_buf_size) & (TINFL_LZ_DICT_SIZE - 1);
            if (status == TINFL_STATUS_NEEDS_MORE_INPUT)
            {
                break;
            }
        }
    }
    return result;
}

static int write_buf(const void* p_buf, int len, tinfl_status status)
{
    memcpy(out_buf, p_buf, MIN(len, OUT_BUF_SZ));
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
