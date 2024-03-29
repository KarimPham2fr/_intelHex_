/*
 * APP.h
 *
 *  Created on: Mar 26, 2024
 *      Author: Phong Pham-Thanh
 */

#ifndef INC_APP_INTEL_HEX_
#define INC_APP_INTEL_HEX_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "ParseFile.h"
/*******************************************************************************
 * APIs
 ******************************************************************************/
 
 /*
 * @name:
 * ----------------------------
 * @brief: Prints data to the screen.
 * @param[out] ABS_Address: The absolute address to be printed
 * @param[out] dataField: Pointer to the data field to be printed
 * @reVal: None
 * @note:  None
 */
void APP_Print_Address_Data(uint32_t ABS_Address, uint8_t* dataField);
#endif /* INC_APP_INTEL_HEX_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
