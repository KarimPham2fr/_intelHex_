/*
 * ReadFile.c
 *
 *  Created on: Mar 26, 2024
 *      Author: Phong Pham-Thanh
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "ReadFile.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
static FILE* pFile = NULL;
/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
ReadFile_t RF_Init(const char* fileName)
{
    ReadFile_t reVal = FILE_INIT_FAILED;
    
    if (fileName != NULL)
    {
        pFile = fopen(fileName, "rb");

        if (pFile != NULL)
        {
            reVal = FILE_INIT_SUCCESSFUL;
        } else {
            reVal = FILE_INIT_FAILED;
        }
    } else {
        reVal = FILE_INIT_FAILED;
    }

    return reVal;
}

ReadFile_t RF_DeInit(void)
{
    ReadFile_t reVal       = FILE_DEINIT_FAILED;
    uint32_t   statusClose = 0;

    statusClose = fclose(pFile);
    if (statusClose == 0)
    {
        reVal = FILE_DEINIT_SUCCESSFUL;
    } else {
        reVal = FILE_DEINIT_FAILED;
    }

    return reVal;
}

ReadFile_t Read_Line(uint8_t* Buff)
{
    ReadFile_t reVal   = READ_LINE_FAILED;
    int8_t*    pStatus = NULL;

    if (Buff != NULL)
    {
        memset(Buff, 0, MAX_CHAR_EACH_LINE);
        pStatus = fgets(Buff, MAX_CHAR_EACH_LINE, pFile);
        if (pStatus != NULL)
        {
            reVal = READ_LINE_SUCCESSFUL;
        } else {
            reVal = READ_LINE_FAILED;
        }
    } else {
        reVal = READ_LINE_FAILED;
    }

    return reVal;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
