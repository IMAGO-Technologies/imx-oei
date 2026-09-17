/*
 * Copyright 2025-2026 IMAGO Technologies GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "clock.h"
#include "oei.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_ccm.h"
#include "fsl_clock.h"

#if defined(DEBUG)
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Debug UART base pointer list */
static LPUART_Type *const s_uartBases[] = LPUART_BASE_PTRS;

/* Debug UART clock list */
static uint32_t const s_uartClks[] =
{
    0U,
    CLOCK_ROOT_LPUART1,
    CLOCK_ROOT_LPUART2,
    CLOCK_ROOT_LPUART3,
    CLOCK_ROOT_LPUART4,
    CLOCK_ROOT_LPUART5,
    CLOCK_ROOT_LPUART6,
    CLOCK_ROOT_LPUART7,
    CLOCK_ROOT_LPUART8
};

/* Debug UART configuration info */
static board_uart_config_t const s_uartConfig =
{
    .base = s_uartBases[BOARD_DEBUG_UART_INSTANCE],
    .clockId = s_uartClks[BOARD_DEBUG_UART_INSTANCE],
    .baud = BOARD_DEBUG_UART_BAUDRATE,
    .inst = BOARD_DEBUG_UART_INSTANCE
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*--------------------------------------------------------------------------*/
/* Return the debug UART info                                               */
/*--------------------------------------------------------------------------*/
const board_uart_config_t *BOARD_GetDebugUart(void)
{
    return &s_uartConfig;
}

/*--------------------------------------------------------------------------*/
/* Initialize debug console                                                 */
/*--------------------------------------------------------------------------*/
void BOARD_InitDebugConsole(void)
{
    if (s_uartConfig.base != NULL)
    {
#if 0
        uint64_t rate = CCM_RootGetRate(s_uartConfig.clockId);
#else
        uint64_t rate = 24000000;
#endif

        /* Configure debug UART */
        lpuart_config_t lpuart_config;
        LPUART_GetDefaultConfig(&lpuart_config);
        lpuart_config.baudRate_Bps = s_uartConfig.baud;
        lpuart_config.rxFifoWatermark = ((uint8_t)
            FSL_FEATURE_LPUART_FIFO_SIZEn(s_uartConfig.base)) - 1U;
        lpuart_config.txFifoWatermark = ((uint8_t)
            FSL_FEATURE_LPUART_FIFO_SIZEn(s_uartConfig.base)) - 1U;
        lpuart_config.enableTx = true;
        lpuart_config.enableRx = true;
        (void) LPUART_Init(s_uartConfig.base, &lpuart_config,
            (uint32_t) rate & 0xFFFFFFFFU);
    }
	
	// clk_root_set_rate_enable(CLOCK_ROOT_CCMCKO1, CLOCK_SRC_OSC24M, 1, true);
}
#endif


#ifdef OEI_DDR

#include "ddr.h"

struct dram_timing_info dram_timing;

extern struct dram_timing_info dram_timing_8gb;
#if defined(SREV_B0)
extern struct dram_timing_info dram_timing_2gb;
extern struct dram_timing_info dram_timing_4gb;
extern struct dram_timing_info dram_timing_4gb_sr;	// single-rank
#endif

static void MemCopy(void *dst, const void *src, unsigned int len)
{
	unsigned int i;
	unsigned char *dst_u8 = dst;
	const unsigned char *src_u8 = src;

	for (i = 0; i < len; i++)
	{
		dst_u8[i] = src_u8[i];
	}
}

#endif


/*--------------------------------------------------------------------------*/
/* Initialize board                                                         */
/*--------------------------------------------------------------------------*/
void BOARD_InitHardware(void)
{
    Clock_Init();

    BOARD_InitPins();

#if defined(DEBUG)
    BOARD_InitDebugConsole();
#endif

#ifdef OEI_DDR
#if defined(SREV_A0)
	MemCopy(&dram_timing, &dram_timing_8gb, sizeof(struct dram_timing_info));
#elif defined(SREV_B0)
	unsigned int gpio1_in = *(unsigned int *)(0x47400000 + 0x50);
	unsigned int ram_mode = ((gpio1_in >> 12) & 0x1) + ((gpio1_in >> (14-1)) & 0x2);

	printf("RAM_MODE: %u\n", ram_mode);

	if (ram_mode == 0x2)
	{
		MemCopy(&dram_timing, &dram_timing_2gb, sizeof(struct dram_timing_info));
	}
	else if (ram_mode == 0x0)
	{
		MemCopy(&dram_timing, &dram_timing_4gb_sr, sizeof(struct dram_timing_info));
	}
	else
	{
		MemCopy(&dram_timing, &dram_timing_8gb, sizeof(struct dram_timing_info));
		if (ram_mode == 0x1)
		{
			// in case of 4GB, we only apply differences to 8GB setting to save space
			dram_timing.ddrc_cfg = dram_timing_4gb.ddrc_cfg;
			dram_timing.ddrc_cfg_num = dram_timing_4gb.ddrc_cfg_num;
			dram_timing.fsp_cfg = dram_timing_4gb.fsp_cfg;
			dram_timing.fsp_cfg_num = dram_timing_4gb.fsp_cfg_num;
		}
	}
#else
	#error SREV_A0 / SREV_B0 is not defined
#endif
#endif
}
