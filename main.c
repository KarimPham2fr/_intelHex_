/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "APP.h"
/*******************************************************************************
 * Main
 ******************************************************************************/
int main(int argc, char** argv) {
    ParseLine_t checkFile = CHECK_FILE_FAILED;

    system("cls");
    if (argc != 2)
    {
        printf("Usage: %s <file_name>\n", argv[0]);
    } else {
        checkFile = PF_Check_File(argv[1]); /* Check input file */
        switch (checkFile)
        {
            case CHECK_FILE_SUCCESSFUL:
                printf("%-5s %-30s %-60s\n", "ID", "Absolute Memory Address", "Data Field");
                PF_Export_Data(argv[1], APP_Print_Address_Data); /* Callback ... to export data to screen */
                break;
            case CHECK_START_FAILED:
                printf("Error: Start Field\n");
                break;
            case CHECK_SYNTAX_ASCII_FAILED:
                printf("Error: Syntax\n");
                break;
            case CHECK_SUM_FAILED:
                printf("Error: Checksum Field\n");
                break;
            case CHECK_RECORD_TYPE_FAILED:
                printf("Error: Type Field\n");
                break;
            case CHECK_BYTE_COUNT_FAILED:
                printf("Error: Bytecount Field\n");
                break;
            case CHECK_EOF_FAILED:
                printf("Error: No Found EOF Record\n");
                break;
            default:
                printf("Error: No found file or can't open your file");
                break;
        }
    }

    return 0;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

