#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>

int main() 
{
    char port[] = "\\\\.\\COM7";
    HANDLE hComm;
    
    hComm = CreateFile( port,                         // Port name
                        GENERIC_READ | GENERIC_WRITE, // Read/Write
                        0,                            // No Sharing
                        NULL,                         // No Security
                        OPEN_EXISTING, // Open existing port only
                        0,             // Non Overlapped I/O
                        NULL );        // Null for Comm Devices
    
    if (hComm == INVALID_HANDLE_VALUE)
        printf("Error in opening serial port\n");
    else
        printf("Opening serial port successful\n");

    DCB iDCB;
    printf("Getting DCB");
    GetCommState(hComm, &iDCB);
    printf("\n%d\n", iDCB.BaudRate);
    printf("%d\n", iDCB.ByteSize);
    printf("%d\n", iDCB.DCBlength);
    printf("%d\n", iDCB.fBinary);
    printf("%d\n", iDCB.fParity);
    
    iDCB.BaudRate = (DWORD) CBR_9600;
    if(SetCommState(hComm, &iDCB) == 0) return 0;
    




    /*
    char DataBuffer[] = "Das";
    DWORD dwBytesToWrite = (DWORD) strlen(DataBuffer);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;
    
    bErrorFlag = WriteFile(hComm,            // open file handle
                           DataBuffer,       // start of data to write
                           dwBytesToWrite,   // number of bytes to write
                           &dwBytesWritten,  // number of bytes that were written
                           NULL);            // no overlapped structure

    if (FALSE == bErrorFlag) {
        printf("Terminal failure: Unable to write to file.\n");
    } else {
        if (dwBytesWritten != dwBytesToWrite) {
            printf("Error: dwBytesWritten != dwBytesToWrite\n");
        } else {
            printf("Wrote bytes to successfully.\n-\n");
        }
    }
    */




    DWORD event = EV_RXCHAR;
    SetCommMask(hComm, event);
    WaitCommEvent(hComm, &event, NULL);

    #define BUFFERSIZE 2
    DWORD  dwBytesRead;
    char ReadBuffer[BUFFERSIZE] = {0};
    int result[6] = {0};
    char linebuffer[100];
    int linecounter = 0;

    POINT mousepos;    
    mousepos.x = 0;
    mousepos.y = 0;

    // Sensor data
    /*
        acc multiplied by 512
        gyro mult by 2048
        sampling rate 20 Hz
    */
    //printf("\033c");
    while(!(GetAsyncKeyState(VK_SPACE) & 0x01)) {
        //while(){
            dwBytesRead = 0;
            if( FALSE == ReadFile(hComm, ReadBuffer, BUFFERSIZE-1, &dwBytesRead, NULL) ){
                printf("Terminal failure: Unable to read from file.\n");
                CloseHandle(hComm);
                return;
            }
            if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE-1) {
                ReadBuffer[dwBytesRead]='\0'; // NULL character
                //printf("Data read from (%d bytes): \n", dwBytesRead);
                //printf("%s", ReadBuffer);
                linebuffer[linecounter] = ReadBuffer[0];
                linecounter += 1;

                if(linebuffer[linecounter - 1] == '\n'){
                    //sscanf(linebuffer, "%d %d %d %d %d %d\n", 
                    //    &result[0], &result[1], &result[2], &result[3], &result[4], &result[5]);
                    sscanf(linebuffer, "%d %d %d\n",&result[0], &result[1], &result[2]);
                    //printf("i %d j %d k %d        i. %d j %d. k. %d; \n\r",
                    printf("i %d j %d k %d \n\r",
                        result[0], result[1], result[2]);// result[3], result[4], result[5]);
                    GetCursorPos(&mousepos);
                    SetCursorPos(mousepos.x+((result[0]-1300)>>8)-((result[2]-16250)>>9),
                        mousepos.y-((result[1]-1750)>>8)+((result[2]-16250)>>9)) ;
                    linecounter  = 0; 
                }
            } else if (dwBytesRead == 0) {
                printf("No data read from file");
            } else {
                printf("\n ** Unexpected value for dwBytesRead ** \n");
            }
        //}  
    }

    CloseHandle(hComm); //Closing the Serial Port
    return 0;
}
