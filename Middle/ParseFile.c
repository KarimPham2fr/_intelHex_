/*
 * ParseFile.c
 *
 *  Created on: Mar 26, 2024
 *      Author: Phong Pham-Thanh
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "ParseFile.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t  g_typeExtend  = 0;
static uint32_t g_valueExtend = 0;
static uint8_t  g_recordEOF   = false;
/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
 
/*
 * @name: convertStrToDec
 * ----------------------------
 * @brief: Converts a hexadecimal string to a decimal number.
 *         Converts a specified number of characters from the input string to an integer (base 10)
 * @param[out] Str: Pointer to the hexadecimal string to be converted
 * @param[out] numOfChar: Number of characters to be converted from the start of string
 * @reVal: The decimal equivalent of the hexadecimal string
 * @Note: If there is any error during conversion or if Str is NULL, CONVERT_ERROR is returned
 */
static uint32_t convertStrToDec(const uint8_t* Str, const uint8_t numOfChar)
{
    uint8_t    index = 0;
    uint8_t    temp  = 0;
    int32_t    reVal = 0;

    if (Str != NULL)
    {
        for (index = 0; index < numOfChar; ++index)
        {
            if (((Str[index] >= 'A') && (Str[index] <= 'F')) || ((Str[index] >= '0') && (Str[index] <= '9')))
            {
                temp = (Str[index] >= 'A') ? (Str[index] - 'A' + 10) : (Str[index] - '0');
                reVal += temp * pow(16, (numOfChar - index - 1));
            }
            else
            {
                index = numOfChar; /* Break loop */
                reVal = CONVERT_ERROR;
            }
        }
    }
    else
    {
        reVal = CONVERT_ERROR;
    }

    return reVal;
}

/*
 * @name: PF_Check_RC_Start
 * ----------------------------
 * @brief: Checks if the data record start field is valid
 * @param[out] Line: Pointer to the line containing the data record
 * @reVal: - CHECK_START_SUCCESSFUL if the start field is valid
           - CHECK_START_FAILED if the start field is invalid or if Line is NULL
 */
static ParseLine_t PF_Check_RC_Start(const uint8_t* const Line)
{
    ParseLine_t reVal = CHECK_START_FAILED;
    
    if (Line != NULL)
    {
        /* Check the first char in the line */
        if (Line[START_FIELD] == ':')
        {
            reVal = CHECK_START_SUCCESSFUL;
        } else {
            reVal = CHECK_START_FAILED;
        }
    } else {
        reVal = CHECK_START_FAILED;
    }
    
    return reVal;
}

/*
 * @name: PF_Check_SYNTAX
 * ----------------------------
 * @brief: Checks if the syntax of the input line is valid
 *         Verifies if all characters, except the first character (record start) and '\r\n' characters at the end, are hexadecimal
 * @param[out] Line: Pointer to the line to be checked.
 * @reVal:  - CHECK_SYNTAX_ASCII_FAILED if the syntax is invalid or if Line is NULL
            - CHECK_SYNTAX_ASCII_SUCCESSFUL if the syntax is valid
 */
static ParseLine_t PF_Check_SYNTAX(const uint8_t* const Line)
{
    ParseLine_t reVal  = CHECK_SYNTAX_ASCII_SUCCESSFUL;
    uint16_t    length = 0;
    uint16_t    index  = 0;

    if (Line != NULL)
    {
        length = strlen(Line) - 3; /* Not consider the ':' & '\r\n' character*/
        for (index = 1; index <= length; ++index)
        {
            if (!((Line[index] >= '0' && Line[index] <= '9') || (Line[index] >= 'A' && Line[index] <= 'F')))
            {
                index = length; /* Break loop */
                reVal = CHECK_SYNTAX_ASCII_FAILED;
            } else {

            }
        }
    } else {
        reVal = CHECK_SYNTAX_ASCII_FAILED;
    }
    return reVal;
}

/*
 * @name: PF_Check_SUM
 * ----------------------------
 * @brief: Check if the checksum of the input line is correct
 * @param[out] Line: Pointer to the line to be checked
 * @reVal: - CHECK_SUM_FAILED if the checksum is incorrect or if Line is NULL
           - CHECK_SUM_SUCCESSFUL if the checksum is correct
 */
static ParseLine_t PF_Check_SUM(const uint8_t* const Line)
{
    ParseLine_t reVal    = CHECK_SUM_FAILED;
    uint8_t     checkSum = 0;
    uint8_t     Sum      = 0;
    uint8_t     index    = 0;
    uint16_t    length   = 0;
    
    if (Line != NULL)
    {
        length = strlen(Line);
        for (index = 0; index < (length - 5) / 2; ++index)
        {
            Sum += convertStrToDec(&Line[index * 2 + 1], 2);
        }

        Sum = ~Sum + 0x01;
        checkSum = convertStrToDec(&Line[length - 4], 2);

        if (Sum == checkSum)
        {
            reVal = CHECK_SUM_SUCCESSFUL;
        } else {
            reVal = CHECK_SUM_FAILED;
        }
    } else {
        reVal = CHECK_SUM_FAILED;
    }

    return reVal;
}

/*
 * @name: PF_Check_Record_Type
 * ----------------------------
 * @brief: Checks if the record type field of the input line is valid
 * @param[out] Line: Pointer to the line to be checked
 * @reVal:  - CHECK_RECORD_TYPE_FAILED if the record type is invalid or if Line is NULL
            - CHECK_RECORD_TYPE_SUCCESSFUL if the record type is valid
 */
static ParseLine_t PF_Check_Record_Type(const uint8_t* const Line)
{
    ParseLine_t reVal      = CHECK_RECORD_TYPE_FAILED;
    uint8_t     recordType = 0;
    uint8_t     byteCount  = 0;
 
    if (Line != NULL)
    {
        recordType = convertStrToDec(&Line[START_TYPE_FIELD], 2);
        switch (recordType)
        {
            case DATA_RECORD:
            case START_LINEAR:
            case EXTENDED_LINEAR:
            case EXTENDED_SEGMENT:
            case START_SEGMENT_ADDRESS:
                reVal = CHECK_RECORD_TYPE_SUCCESSFUL;
                break;
            case EOF_RECORD:
                g_recordEOF = true;
                reVal = CHECK_RECORD_TYPE_SUCCESSFUL;
                break;
            default:
                reVal = CHECK_RECORD_TYPE_FAILED;
                break;
        }
    } else {
        reVal = CHECK_RECORD_TYPE_FAILED;
    }

    return reVal;
}

/*
 * @name: PF_Check_Byte_Count
 * ----------------------------
 * @brief: Checks if the byte count field of the input line is valid
 * @param[out] Line: Pointer to the line to be checked
 * @reVal: - CHECK_BYTE_COUNT_FAILED if the byte count is invalid or if Line is NULL
           - CHECK_BYTE_COUNT_SUCCESSFUL if the byte count is valid
 * @Note:
 */
static ParseLine_t PF_Check_Byte_Count(const uint8_t* const Line)
{
    ParseLine_t reVal     = CHECK_BYTE_COUNT_FAILED;
    uint8_t     byteCount = 0;
    uint16_t    length    = 0;
    
    if (Line != NULL)
    {
        byteCount = convertStrToDec(&Line[1], 2);
        length = strlen(Line);
        if ((byteCount * 2) == (length - 13))
        {
            reVal = CHECK_BYTE_COUNT_SUCCESSFUL;
        } else {
            reVal = CHECK_BYTE_COUNT_FAILED;
        }
    } else {
        reVal = CHECK_BYTE_COUNT_FAILED;
    }

    return reVal;
}

/*
 * @name: PF_Cal_ABS_Address
 * ----------------------------
 * @brief: Calculates the absolute address based on the given address field and the current extended type
 * @param[out] addressField: The relative address field of the input line
 * @reVal: The absolute address calculated based on the address field and the current extended type
 * @Note: The function uses the current extended type (g_typeExtend) to determine how to calculate the absolute address
 */
static uint32_t PF_Cal_ABS_Address(const uint32_t addressField)
{
    uint32_t ABS_Address  = 0;

    switch (g_typeExtend)
    {
        case DATA_RECORD:
            ABS_Address = (g_valueExtend << 0) + addressField;
            break;
        case EXTENDED_SEGMENT:
            ABS_Address = (g_valueExtend << 4) + addressField;
            break;
        case EXTENDED_LINEAR:
            ABS_Address = (g_valueExtend << 16) + addressField;
            break;
        default:
            break;
    }

    return ABS_Address;
}

ParseLine_t PF_Check_File(const char* fileName)
{
    ParseLine_t reVal       = CHECK_FILE_SUCCESSFUL;
    ParseLine_t checkStart  = CHECK_START_FAILED;
    ParseLine_t checkSyntax = CHECK_SYNTAX_ASCII_FAILED;
    ParseLine_t checkSum    = CHECK_SUM_FAILED;
    ParseLine_t checkType   = CHECK_RECORD_TYPE_FAILED;
    ParseLine_t checkCount  = CHECK_BYTE_COUNT_FAILED;
    ReadFile_t  openStatus  = FILE_INIT_FAILED;
    uint8_t     Line[MAX_CHAR_EACH_LINE];
    uint8_t     Error       = false;

    if (fileName != NULL)
    {
        /* Open file */
        openStatus = RF_Init(fileName);
        if (openStatus == FILE_INIT_SUCCESSFUL)
        {
            /* Parse each line to find an error or end of file*/
            while((Read_Line(Line) != READ_LINE_FAILED) && (Error == false))
            {
                checkStart = PF_Check_RC_Start(Line); /* Check start field */
                if (checkStart == CHECK_START_SUCCESSFUL)
                {
                    checkSyntax = PF_Check_SYNTAX(Line); /* Check systax */
                    if (checkSyntax == CHECK_SYNTAX_ASCII_SUCCESSFUL)
                    {
                        checkSum = PF_Check_SUM(Line); /* Check Checksum */
                        if (checkSum == CHECK_SUM_SUCCESSFUL)
                        {
                            checkType = PF_Check_Record_Type(Line); /* Check type field */
                            if (checkType == CHECK_RECORD_TYPE_SUCCESSFUL)
                            {
                                checkCount = PF_Check_Byte_Count(Line); /* Check byte count */
                                if (checkCount == CHECK_BYTE_COUNT_SUCCESSFUL)
                                {
                                    Error = false;
                                } else {
                                    reVal = CHECK_BYTE_COUNT_FAILED;
                                    Error = true;
                                }
                            } else {
                                reVal = CHECK_RECORD_TYPE_FAILED;
                                Error = true;
                            }
                        } else {
                            reVal = CHECK_SUM_FAILED;
                            Error = true;
                        }
                    } else {
                        reVal = CHECK_SYNTAX_ASCII_FAILED;
                        Error = true;
                    }
                } else {
                    reVal = CHECK_START_FAILED;
                    Error = true;
                }
            }

            /* Check record end of file */
            if (reVal == CHECK_FILE_SUCCESSFUL && g_recordEOF == false)
            {
                reVal = CHECK_EOF_FAILED;
            } else {
                g_recordEOF = false;
            }

            /* Close file */
            RF_DeInit();
        } else {
            reVal = CHECK_FILE_FAILED;
        }
    } else {
        reVal = CHECK_FILE_FAILED;
    }

    return reVal;
}

void PF_Export_Data(const char* fileName, func Print_Address_Data) 
{
    ReadFile_t  openStatus   = FILE_INIT_FAILED;
    uint8_t     Line[MAX_CHAR_EACH_LINE];
    uint32_t    ABS_Address  = 0;
    uint32_t    addressField = 0;
    uint8_t     DataField[MAX_DATA_FIELD];
    uint16_t    length       = 0;
    uint8_t     recordType   = 0;
    uint8_t     byteCount    = 0;

    if (fileName != NULL)
    {
        /* Open file */
        openStatus = RF_Init(fileName);
        if (openStatus == FILE_INIT_SUCCESSFUL)
        {
            /* Read until meet EOF */
            while((Read_Line(Line) != READ_LINE_FAILED))
            {
                recordType = convertStrToDec(&Line[START_TYPE_FIELD], 2);
                length = strlen(Line);

                switch (recordType)
                {
                    case DATA_RECORD:
                        addressField = convertStrToDec(&Line[START_ADD_FIELD], 4);
                        ABS_Address  = PF_Cal_ABS_Address(addressField);
                        memset(DataField, 0, MAX_DATA_FIELD);
                        memcpy(DataField, &Line[START_DATA_FIELD], length - 13);
                        Print_Address_Data(ABS_Address, DataField); /* Callback here */
                        break;
                    case EXTENDED_SEGMENT:
                        g_typeExtend  = EXTENDED_SEGMENT;
                        byteCount   = convertStrToDec(&Line[START_BYTE_COUNT_FIELD], 2);
                        g_valueExtend = convertStrToDec(&Line[START_DATA_FIELD], byteCount * 2);
                        break;
                    case EXTENDED_LINEAR:
                        g_typeExtend  = EXTENDED_LINEAR;
                        byteCount   = convertStrToDec(&Line[START_BYTE_COUNT_FIELD], 2);
                        g_valueExtend = convertStrToDec(&Line[START_DATA_FIELD], byteCount * 2);
                        break;
                    default:
                        break;
                }
            }
            /* Close file */
            RF_DeInit();
        } else {
            
        }
    } else {
        
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
 
