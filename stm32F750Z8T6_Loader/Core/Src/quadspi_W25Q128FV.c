/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"


uint8_t CSP_QUADSPI_Init(void) {

	QSPI_CommandTypeDef s_command;
	uint8_t value = W25Q128FV_FSR_QE;

	//prepare QSPI peripheral for ST-Link Utility operations
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK) {
		return HAL_ERROR;
	}

	MX_QUADSPI_Init();

	if (QSPI_ResetChip() != HAL_OK) {
		return HAL_ERROR;
	}

	//HAL_Delay(1);

	/*if (QSPI_AutoPollingMemReady() != HAL_OK) {
		return HAL_ERROR;
	}*/

	/* Enable write operations */
	QSPI_WriteEnable();

	/* Set status register for Quad Enable,the Quad IO2 and IO3 pins are enable */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_STATUS_REG2_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	/* Configure the command */
	if(HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
	  return HAL_ERROR;
	}
	/* Transmit the data */
	if(HAL_QSPI_Transmit(&hqspi, &value, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
	  return HAL_ERROR;
	}

	QSPI_WriteEnable();
//	//W25QXX_Wait_Busy();
//	if (QSPI_AutoPollingMemReady() != HAL_OK)
//		return HAL_ERROR;

	s_command.Instruction        = INDIVIDUAL_BLOCK_UNLOCK_CMD;
	s_command.Address            = 0x00;
	s_command.AlternateBytes     = 0x00;
	s_command.AddressSize        = QSPI_ADDRESS_24_BITS;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DummyCycles        = 0;
	s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
	s_command.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode           = QSPI_DATA_1_LINE;
	s_command.NbData             = 1;
	s_command.DdrMode            = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

	if(HAL_QSPI_Command(&hqspi, &s_command, 5000) != HAL_OK){
		return HAL_ERROR;
	}

	//W25QXX_Wait_Busy();
	if (QSPI_AutoPollingMemReady() != HAL_OK)
		return HAL_ERROR;

	HAL_GPIO_TogglePin(Alive_LED_GPIO_Port, Alive_LED_Pin);

	return HAL_OK;
}


uint8_t CSP_QSPI_Erase_Chip(void) {
	QSPI_CommandTypeDef sCommand;


	if (QSPI_WriteEnable() != HAL_OK) {
		return HAL_ERROR;
	}


	/* Erasing Sequence --------------------------------- */
	sCommand.Instruction = CHIP_ERASE_CMD;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;


	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	if (QSPI_AutoPollingMemReady() != HAL_OK) {
				return HAL_ERROR;
			}



	HAL_GPIO_TogglePin(Alive_LED_GPIO_Port, Alive_LED_Pin);


	return HAL_OK;
}

uint8_t QSPI_AutoPollingMemReady(void) {

	QSPI_CommandTypeDef sCommand;
	 QSPI_AutoPollingTypeDef sConfig;

	/* Configure automatic polling mode to wait for memory ready ------ */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	sConfig.Match = 0x00;
	sConfig.Mask = 0x01;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

uint8_t QSPI_WriteEnable(void)
{
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Enable write operations ------------------------------------------ */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = WRITE_ENABLE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	/* Configure automatic polling mode to wait for write enabling ---- */
	sConfig.Match = 0x02;
	sConfig.Mask = 0x02;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}


	HAL_GPIO_TogglePin(Alive_LED_GPIO_Port, Alive_LED_Pin);

	return HAL_OK;
}

#if 0
/*Enable quad mode and set dummy cycles count*/
uint8_t QSPI_Configuration(void) {
	uint8_t value = W25Q128FV_FSR_QE;
	QSPI_CommandTypeDef sCommand;
	uint8_t test_buffer[4] = { 0 };
	/*read status register*/
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_QSPI_Receive(&hqspi, test_buffer,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	/*read configuration register*/
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_CONFIGURATION_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_QSPI_Receive(&hqspi, &(test_buffer[1]),
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	/*modify buffer to enable quad mode*/
	test_buffer[0] |= 0x40;

	/*set dummy cycles*/
	test_buffer[1] |= 0xC0;

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = WRITE_STATUS_REG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.NbData = 2;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}

	if (HAL_QSPI_Transmit(&hqspi, test_buffer,
	HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
		return HAL_ERROR;
	}










	  /* QSPI memory reset */
	  QSPI_ResetChip();

	  /* Enable write operations */
	  //QSPI_WriteEnable();
	  QSPI_WriteEnable();

	  /* Set status register for Quad Enable,the Quad IO2 and IO3 pins are enable */
	  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	  sCommand.Instruction       = WRITE_STATUS_REG2_CMD;
	  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	  sCommand.DataMode          = QSPI_DATA_1_LINE;
	  sCommand.DummyCycles       = 0;
	  sCommand.NbData            = 1;
	  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	  /* Configure the command */
	  if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	  {
	      return HAL_ERROR;
	  }
	  /* Transmit the data */
	  if(HAL_QSPI_Transmit(&hqspi, &value, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	  {
	      return HAL_ERROR;
	  }




	return HAL_OK;
}
#endif

uint8_t CSP_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress) {

	QSPI_CommandTypeDef sCommand;

	EraseStartAddress = EraseStartAddress
			- EraseStartAddress % MEMORY_SECTOR_SIZE;

	/* Erasing Sequence -------------------------------------------------- */

	sCommand.Instruction        = SECTOR_ERASE_CMD;
	sCommand.AlternateBytes     = 0x00;
	sCommand.AddressSize        = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.DummyCycles        = 0;
	sCommand.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressMode        = QSPI_ADDRESS_1_LINE;
	sCommand.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode           = QSPI_DATA_NONE;
	sCommand.NbData             = 1;
	sCommand.DdrMode            = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;



	while (EraseEndAddress >= EraseStartAddress) {


		HAL_GPIO_TogglePin(Alive_LED_GPIO_Port, Alive_LED_Pin);

		sCommand.Address = (EraseStartAddress & 0x0FFFFFFF);

		if (QSPI_WriteEnable() != HAL_OK) {
			return HAL_ERROR;
		}

		//W25QXX_Wait_Busy();
		if (QSPI_AutoPollingMemReady() != HAL_OK)
			return HAL_ERROR;

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
				!= HAL_OK) {
			return HAL_ERROR;
		}
		EraseStartAddress += MEMORY_SECTOR_SIZE;

		//W25QXX_Wait_Busy();
		if (QSPI_AutoPollingMemReady() != HAL_OK)
			return HAL_ERROR;
	}

	return HAL_OK;
}



//0x13/0xEC/0x12
uint8_t CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size)
{

	QSPI_CommandTypeDef sCommand;
	uint32_t end_addr, current_size, current_addr;

	/* Calculation of the size between the write address and the end of the page */
	current_addr = 0;
	//buffer_size++;

	//
	while (current_addr <= address) {
		current_addr += MEMORY_PAGE_SIZE;
	}
	current_size = current_addr - address;

	/* Check if the size of the data is less than the remaining place in the page */
	if (current_size > buffer_size) {
		current_size = buffer_size;
	}

	/* Initialize the adress variables */
	current_addr = address;
	end_addr = address + buffer_size ;//+1

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode = QSPI_DATA_4_LINES;
	sCommand.NbData = buffer_size;
	sCommand.Address = address;
	sCommand.DummyCycles = 0;

	/* Perform the write page by page */
	do {

		HAL_GPIO_TogglePin(Alive_LED_GPIO_Port, Alive_LED_Pin);

		sCommand.Address = current_addr;
		sCommand.NbData = current_size;

		if (current_size == 0) {
			return HAL_OK;
		}

		/* Enable write operations */
		if (QSPI_WriteEnable() != HAL_OK) {
			return HAL_ERROR;
		}

		/* Configure the command */
		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
				!= HAL_OK) {

			return HAL_ERROR;
		}

		/* Transmission of the data */
		if (HAL_QSPI_Transmit(&hqspi, buffer/*-1*/, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {

			return HAL_ERROR;
		}

		/* Configure automatic polling mode to wait for end of program */
		if (QSPI_AutoPollingMemReady() != HAL_OK) {
			return HAL_ERROR;
		}

		/* Update the address and size variables for next page programming */
		current_addr += current_size;
		buffer += current_size;
		current_size =
				((current_addr + MEMORY_PAGE_SIZE) > end_addr) ?
						(end_addr - current_addr) : MEMORY_PAGE_SIZE;
	} while (current_addr <= end_addr);
	//HAL_GPIO_TogglePin(Alive_LED_GPIO_Port, Alive_LED_Pin);

	//QSPI_WriteEnable();
	return HAL_OK;
}


uint8_t CSP_QSPI_EnableMemoryMappedMode(void)
{

	QSPI_CommandTypeDef sCommand;
	QSPI_MemoryMappedTypeDef sMemMappedCfg;

	/* Enable Memory-Mapped mode-------------------------------------------------- */

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode = QSPI_DATA_4_LINES;
	sCommand.NbData = 0;
	sCommand.Address = 0;
	sCommand.Instruction = QUAD_OUT_FAST_READ_CMD;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_QUAD;

	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

	if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint8_t QSPI_ResetChip()
{
	QSPI_CommandTypeDef sCommand;
	uint32_t temp = 0;
	/* Erasing Sequence -------------------------------------------------- */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = RESET_ENABLE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
		return HAL_ERROR;
	}
	for (temp = 0; temp < 0x2f; temp++) {
		__NOP();
	}

	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = RESET_EXECUTE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.Address = 0;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)!= HAL_OK) {

		return HAL_ERROR;
	}
	return HAL_OK;

}




/* USER CODE END 1 */
