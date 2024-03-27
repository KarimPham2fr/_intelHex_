/*
 * ReadFile.h
 *
 *  Created on: Mar 26, 2024
 *      Author: Phong Pham-Thanh
 */
 
#ifndef INC_READ_FILE_INTEL_HEX_
#define INC_READ_FILE_INTEL_HEX_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MAX_CHAR_EACH_LINE 521 /* 1+ 2 + 4 + 2 + 255 * 2 (max data) + 2*/
#define MAX_DATA_FIELD     510 /* 255 byte * 2 */
/*******************************************************************************
 * Typedef structs & enums
 ******************************************************************************/
typedef enum {
    FILE_INIT_SUCCESSFUL,
    FILE_INIT_FAILED,
    FILE_DEINIT_SUCCESSFUL,
    FILE_DEINIT_FAILED,
    READ_LINE_SUCCESSFUL,
    READ_LINE_FAILED,
} ReadFile_t;
/*******************************************************************************
 * APIs
 ******************************************************************************/
 
/*
 * @name: RF_Init
 * ----------------------------
 * @brief: Opens the specified file for reading
 * @param[out] fileName: The name of the file to be opened
 * @reVal: - FILE_INIT_SUCCESSFUL if the file was successfully opened
           - FILE_INIT_FAILED if there was an error opening the file or if fileName is NULL
 */
extern ReadFile_t RF_Init(const char* fileName);
/*
 * @name: RF_DeInit
 * ----------------------------
 * @brief: Closes the previously opened file
 * @param: None
 * @reVal: - FILE_DEINIT_SUCCESSFUL if the file was successfully closed
           - FILE_DEINIT_SUCCESSFUL if the file was successfully closed
 */
extern ReadFile_t RF_DeInit(void);
/*
 * @name: Read_Line
 * ----------------------------
 * @brief: Reads a line from the input file and stores it in the provided buffer
 * @param[in] Buff: Pointer to the buffer where the read line will be stored
 * @reVal: - READ_LINE_SUCCESSFUL if a line was successfully read from the file
           - READ_LINE_FAILED if there was an error reading the line or if Buff is NULL.
 */
extern ReadFile_t Read_Line(uint8_t* Buff);
#endif /* INC_READ_FILE_INTEL_HEX_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
 
