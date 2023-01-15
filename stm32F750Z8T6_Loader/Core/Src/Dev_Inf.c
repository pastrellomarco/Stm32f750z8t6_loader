/*
 * Dev_Inf.c
 *
 */
#include "Dev_Inf.h"
#include "quadspi.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo const StorageInfo = {
#endif

#ifdef W25Q128FV

		"QSPI_STM32F750Z8T6_W25Q128FV", 	 	 // Device Name + version number

#endif

#ifdef W25Q128JV

		"QSPI_STM32F750Z8T6_W25Q128JV", 	 	 // Device Name + version number

#endif

#ifdef W25Q64JV

		"QSPI_STM32F750Z8T6_W25Q64JV", 	 	 // Device Name + version number

#endif

		NOR_FLASH,                  		 // Device Type
		0x90000000,                			 // Device Start Address
		0x01000000,//MEMORY_FLASH_SIZE,      // Device Size in Bytes
		MEMORY_PAGE_SIZE,                    // Programming Page Size
		0xFF,                                // Initial Content of Erased Memory

      //  (MEMORY_FLASH_SIZE / MEMORY_SECTOR_SIZE),  // Sector Numbers,
       // (uint32_t) MEMORY_SECTOR_SIZE,

		0x00000100, 0x00010000,     		// Sector Num : 256 ,Sector Size: 64KBytes
		0x00000000, 0x00000000,

		};
