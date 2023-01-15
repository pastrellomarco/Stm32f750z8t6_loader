/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.h
  * @brief   This file contains all the function prototypes for
  *          the quadspi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
extern QSPI_HandleTypeDef hqspi;
/* USER CODE END Includes */

extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN Private defines */
#ifdef W25Q128FV							// W25Q128FV
	#define MEMORY_BLOCK_NUMBER				256
	#define MEMORY_BLOCK_SIZE				0x10000				// Each block is 64Kbytes
	#define MEMORY_PAGE_SIZE				0x100     			// 256 bytes each page
	#define MEMORY_SECTOR_NUMBER			0x10    			// every Block contain 16 sector (total bytes per sector = 4096)
	#define MEMORY_SECTOR_SIZE				0x1000    			// every sector (4kBytes) has 16 pages (total bytes per sector = 4096)
	#define MEMORY_FLASH_SIZE				(MEMORY_BLOCK_NUMBER * MEMORY_SECTOR_SIZE * MEMORY_SECTOR_NUMBER) 	// 128MBits => 16MBytes (n° of blocks



	/*W25Q128FV commands */
	#define WRITE_ENABLE_CMD 				0x06

	#define READ_STATUS_REG1_CMD 			0x05
	#define READ_STATUS_REG2_CMD 			0x35
	#define READ_STATUS_REG3_CMD 			0x15

	#define WRITE_STATUS_REG1_CMD 			0x01		// 0x01
	#define WRITE_STATUS_REG2_CMD 			0x31		// 0x01
	#define WRITE_STATUS_REG3_CMD 			0x11		// 0x01

	#define SECTOR_ERASE_CMD 				0x20
	#define BLOCK_ERASE_CMD 				0xD8

	#define CHIP_ERASE_CMD 					0xC7
	#define QUAD_IN_FAST_PROG_CMD 			0x32

	#define QUAD_READ_IO_CMD 				0xEB		// 0xEC
	#define QUAD_OUT_FAST_READ_CMD 			0x6B
	#define QPI_ENABLE_CMD 					0x38		// 0x35
	#define DUMMY_CLOCK_CYCLES_READ_QUAD 	8

	#define RESET_ENABLE_CMD 				0x66
	#define RESET_EXECUTE_CMD 				0x99
	#define GLOBAL_BLOCK_LOCK_CMD 			0x7E
	#define GLOBAL_BLOCK_UNLOCK_CMD 		0x98
	#define DISABLE_QIP_MODE 				0xFF		// 0xf5

	#define ENABLE_QUAD_FLAG_MODE 			0x50		// 0xf5Volatile SR Write Enable
#endif

#ifdef W25Q128JV							// W25Q128JV
	#define MEMORY_BLOCK_NUMBER				256
	#define MEMORY_BLOCK_SIZE				0x10000				// Each block is 64Kbytes
	#define MEMORY_PAGE_SIZE				0x100     			// 256 bytes each page
	#define MEMORY_SECTOR_NUMBER			0x10    			// every Block contain 16 sectors (total bytes per sector = 4096)
	#define MEMORY_SECTOR_SIZE				0x10000  //1000  			// every sector (4kBytes) has 16 pages (total bytes per sector = 4096)
	#define MEMORY_FLASH_SIZE				(MEMORY_BLOCK_NUMBER * MEMORY_BLOCK_SIZE ) 	// 128MBits => 8MBytes (n° of blocks


	/*W25Q128FJ commands */
	#define WRITE_ENABLE_CMD 				0x06
	#define READ_STATUS_REG_CMD 			0x05		// read status register1
	#define WRITE_STATUS_REG_CMD 			0x01		// write status register1
	#define SECTOR_ERASE_CMD 				0x20
	#define BLOCK_ERASE_CMD 				0xD8
	#define CHIP_ERASE_CMD 					0x60		// 0xc7
	#define QUAD_IN_FAST_PROG_CMD 			0x32
	#define READ_QE_FLAH_STATUS 			0x35		// read status register2
	#define WRITE_QE_FLAH_STATUS 			0x31		// write status register2

	#define QUAD_READ_IO_CMD 				0xEB
	#define QUAD_OUT_FAST_READ_CMD 			0x6B
	#define QPI_ENABLE_CMD 					0x38
	#define DUMMY_CLOCK_CYCLES_READ_QUAD 	8

	#define RESET_ENABLE_CMD 				0x66
	#define RESET_EXECUTE_CMD 				0x99
	#define DISABLE_QIP_MODE 				0xFF

	#define VOLATILE_SR_WRITE_EN_CMD		0x50		// Volatile SR Write Enable
#endif


#ifdef W25Q64JV

	#define MEMORY_BLOCK_NUMBER				128
	#define MEMORY_BLOCK_SIZE				0x10000				// Each block is 64Kbytes
	#define MEMORY_PAGE_SIZE				0x100     			// 256 bytes each page
	#define MEMORY_SECTOR_NUMBER			0x10    			// every Block contain 16 sector (total bytes per sector = 4096)
	#define MEMORY_SECTOR_SIZE				0x1000    			// every sector (4kBytes) has 16 pages (total bytes per sector = 4096)
	#define MEMORY_FLASH_SIZE				(MEMORY_BLOCK_NUMBER * MEMORY_SECTOR_SIZE * MEMORY_SECTOR_NUMBER ) 	// 64MBits => 8MBytes (n° of blocks


	/*W25Q64FW commands */
	#define WRITE_ENABLE_CMD 				0x06
	#define READ_STATUS_REG_CMD 			0x05		// read status register1
	#define WRITE_STATUS_REG_CMD 			0x01		// write status register1
	#define SECTOR_ERASE_CMD 				0x20
	#define BLOCK_ERASE_CMD 				0xD8
#error "Replace SECTOR_ERASE_CMD with BLOCK_ERASE_CMD - And verify if it work correctly"
	#define CHIP_ERASE_CMD 					0x60		// 0xc7
	#define QUAD_IN_FAST_PROG_CMD 			0x32
	#define READ_QE_FLAH_STATUS 			0x35		// read status register2
	#define WRITE_QE_FLAH_STATUS 			0x31		// write status register2

	#define QUAD_READ_IO_CMD 				0xEB
	#define QUAD_OUT_FAST_READ_CMD 			0x6B
	#define QPI_ENABLE_CMD 					0x38
	#define DUMMY_CLOCK_CYCLES_READ_QUAD 	8

	#define RESET_ENABLE_CMD 				0x66
	#define RESET_EXECUTE_CMD 				0x99
	#define DISABLE_QIP_MODE 				0xFF

	#define VOLATILE_SR_WRITE_EN_CMD		0x50		// Volatile SR Write Enable
#endif

/* USER CODE END Private defines */

void MX_QUADSPI_Init(void);

/* USER CODE BEGIN Prototypes */

uint8_t CSP_QUADSPI_Init(void);
uint8_t CSP_QSPI_Erase_Chip(void);
uint8_t CSP_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
uint8_t CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size);
uint8_t CSP_QSPI_EnableMemoryMappedMode(void);

//uint8_t WriteEnable(void);
uint8_t QSPI_Configuration(void);
uint8_t QSPI_WriteEnable(void);
uint8_t QSPI_AutoPollingMemReady(void);
uint8_t CSP_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
uint8_t CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size);
uint8_t CSP_QSPI_EnableMemoryMappedMode(void);
uint8_t QSPI_ResetChip(void);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __QUADSPI_H__ */

