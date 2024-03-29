/*
 * ParseFile.h
 *
 *  Created on: Mar 26, 2024
 *      Author: Phong Pham-Thanh
 */

#ifndef INC_PARSE_FILE_INTEL_HEX_
#define INC_PARSE_FILE_INTEL_HEX_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "ReadFile.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define CONVERT_ERROR             -1
#define START_FIELD               0U
#define START_BYTE_COUNT_FIELD    1U
#define START_ADD_FIELD           3U
#define START_TYPE_FIELD          7U
#define START_DATA_FIELD          9U
#define DATA_RECORD               0U
#define EOF_RECORD                1U
#define EXTENDED_SEGMENT          2U
#define START_SEGMENT_ADDRESS     3U
#define EXTENDED_LINEAR           4U
#define START_LINEAR              5U
#define false                     0U
#define true                      1U
/*******************************************************************************
 * Typedef structs & enums
 ******************************************************************************/

/*
 * @name:
 * ----------------------------
 * @brief:
 */
typedef enum {
    CHECK_START_FAILED,
    CHECK_START_SUCCESSFUL,
    CHECK_SYNTAX_ASCII_FAILED,
    CHECK_SYNTAX_ASCII_SUCCESSFUL,
    CHECK_SUM_FAILED,
    CHECK_SUM_SUCCESSFUL,
    CHECK_RECORD_TYPE_FAILED,
    CHECK_RECORD_TYPE_SUCCESSFUL,
    CHECK_BYTE_COUNT_FAILED,
    CHECK_BYTE_COUNT_SUCCESSFUL,
    CHECK_FILE_FAILED,
    CHECK_FILE_SUCCESSFUL,
    CHECK_EOF_FAILED,
} ParseLine_t;

typedef void (*func)(uint32_t, uint8_t*);

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*
 * @name: PF_Check_File
 * ----------------------------
 * @brief: Parses an input file for validity and correctness.
 *         Checks each line of the file for various criteria such as start field,
 *         syntax, checksum, record type, byte count, and end of file.
 * @param[out] fileName: The name of the file to be checked
 * @reVal: An enum indicating the status of the file checking:
 *                 - CHECK_FILE_SUCCESSFUL if the file passed all checks.
 *                 - CHECK_FILE_FAILED if there was an error opening the file or if fileName is NULL.
 *                 - Other ParseLine_t values indicating specific errors encountered during file parsing.
 */
extern ParseLine_t PF_Check_File(const char* fileName);

/*
 * @name: PF_Export_Data
 * ----------------------------
 * @brief: Reads data from an input file and performs actions as requested by the application layer.
 *         In this specific case, the function exports data to the screen.
 * @param[out] fileName: The name of the file to be read
 * @param[in] Print_Address_Data: A function pointer to the callback function responsible for printing data
 * @reVal: None
 * @note: Due to the unknown size of the input file, implementing code using a linked list approach is not feasible.
 *        Instead, I employ the following method: If the input file passes initial checks without errors,
 *        I will call the PF_Export_Data function to traverse the file once again. Subsequently, various output
 *        functions can be performed according to user requirements (via callback)
 */
extern void PF_Export_Data(const char* fileName, func Print_Address_Data);
#endif /* INC_PARSE_FILE_INTEL_HEX_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
