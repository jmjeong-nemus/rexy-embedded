/**************************************************************************
*                                                                         *
*   PROJECT     : ARM port for UCOS-II                                    *
*                                                                         *
*   MODULE      : CONSOL.c                                                *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : S3c2410x (32 bit ARM920T RISC core from Samsung)        *
*                                                                         *
*   IDE         : SDT 2.51 & ADS 1.2                                      *
*                                                                         *
*   DESCRIPTION :                                                         *
*   This is the CONSOL Driver module. Supports multiple RS232 console     *
*   interfaces.                                                           *
*                                                                         *
**************************************************************************/


#include "def.h"
#include "S3c2410x.h"
#include "consol.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* ********************************************************************* */
/* Global definitions */


/* ********************************************************************* */
/* File local definitions */

static int __wChannel=1;

#define __isalpha(c) (c >'9')
#define __isupper(c) !(c & 0x20)


/* ********************************************************************* */
/* Local functions */


/* ********************************************************************* */
/* Global functions */

/*
*********************************************************************************************
*                                       CONSOL_Select
*
* Description: This routine selects the consol port for active usage.
*
* Arguments  : wCh - UART channel number.
*                    1 - UART1.
*                    2 - UART2.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_Select(int wCh)
{
    __wChannel = wCh;
}

/*
*********************************************************************************************
*                                       CONSOL_Init
*
* Description: This routine initializes the consol port by setting its baud rate & FIFO
*              buffer.
*
* Arguments  : wBaud     - Baud Rate in bit/sec.
*
* Return     : none.
*
* Note(s)    : Before calling this, CONSOL_Select needs to be called for selecting the consol
*              port.
*********************************************************************************************
*/

void CONSOL_Init(int wBaud)
{
    switch(__wChannel)
    {
    case 0:
        rUFCON0 = (3<<6) | (3<<4) | (1<<2) | (1<<1) | (1<<0);   //UART channel 0 FIFO control register
        rUMCON0 = 0;
        rULCON0 = 0x3;   //Line control register : Normal,No parity,1 stop,8 bits
        rUCON0  = 0x245; // Control register
        rUBRDIV0=( (int)((FCLK/4)/16./wBaud+0.5) -1 );   //Baud rate divisior register 0
        break;

    case 1:
        rUFCON1 = (3<<6) | (3<<4) | (1<<2) | (1<<1) | (1<<0);   //UART channel 0 FIFO control register
        rUMCON1 = 0;
        rULCON1 = 0x3;   //Line control register : Normal,No parity,1 stop,8 bits
        rUCON1  = 0x245; // Control register
        rUBRDIV1=( (int)((FCLK/4)/16./wBaud+0.5) -1 );   //Baud rate divisior register 0
        break;

    case 2:
        rUFCON2 = (3<<6) | (3<<4) | (1<<2) | (1<<1) | (1<<0);   //UART channel 0 FIFO control register
        rULCON2 = 0x3;   //Line control register : Normal,No parity,1 stop,8 bits
        rUCON2  = 0x245; // Control register
        rUBRDIV2=( (int)((FCLK/4)/16./wBaud+0.5) -1 );   //Baud rate divisior register 0
        break;
    }
}

/*
*********************************************************************************************
*                                       CONSOL_GetCh
*
* Description: This routine waits for a character from the CONSOL port & returns it.
*
* Arguments  : none.
*
* Return     : Returns the character read from the consol port.
*
* Note(s)    : 
*********************************************************************************************
*/

char CONSOL_GetCh(void)
{
    switch(__wChannel)
    {
    case 0:
        while(!(rUFSTAT0 & 0x0f)); //Receive data ready
        return rURXH0;

    case 1:
        while(!(rUFSTAT1 & 0x0f)); //Receive data ready
        return rURXH1;

    case 2:
        while(!(rUFSTAT2 & 0x0f)); //Receive data ready
        return rURXH2;
    }
}

/*
*********************************************************************************************
*                                       CONSOL_GetChar
*
* Description: This routine reads a character from the consol port if available.
*
* Arguments  : pbData - Pointer to return the received data.
*
* Return     : The status of the RX buffer.
*              True  - Data returned is valid.
*              False - Data returned is invalid.
*
* Note(s)    : 
*********************************************************************************************
*/

char CONSOL_GetChar(char * pbData)
{
    switch(__wChannel)
    {
    case 0:
        if(rUFSTAT0 & 0x0f)
        {
            *pbData = rURXH0;
            return True;
        }
        return False;

    case 1:
        if(rUFSTAT1 & 0x0f)
        {
            *pbData = rURXH1;
            return True;
        }
        return False;

    case 2:
        if(rUFSTAT2 & 0x0f)
        {
            *pbData = rURXH2;
            return True;
        }
        return False;
    }
}

/*
*********************************************************************************************
*                                       CONSOL_GetString
*
* Description: This routine waits for a string from the CONSOL port & returns it.
*
* Arguments  : pbString - Pointer to return the received string.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_GetString(char *pbString)
{
    char *pbString2=pbString;
    char bC;

    while((bC=CONSOL_GetCh())!='\r')
    {
        if(bC=='\b')
        {
            if((int)pbString2<(int)pbString){CONSOL_Printf("\b \b");pbString--;}
        }
        else
        {
            *pbString++=bC;CONSOL_SendCh(bC);
        }
    }
    *pbString='\0';
    CONSOL_SendCh('\n');
}

/*
*********************************************************************************************
*                                       CONSOL_GetIntNum
*
* Description: This routine waits for a Integer from the CONSOL port & returns it.
*
* Arguments  : none.
*
* Return     : Returns the received Integer value.
*
* Note(s)    : 
*********************************************************************************************
*/

int CONSOL_GetIntNum(void)
{
    char abStr[30];
    char *pbString=abStr;
    int wBase=10;
    int wMinus=0;
    int wLastIndex;
    int wResult=0;
    int wI;

    CONSOL_GetString(pbString);

    if(pbString[0]=='-'){wMinus=1;pbString++;}

    if(pbString[0]=='0' && (pbString[1]=='x' || pbString[1]=='X'))
    {
        wBase=16;
        pbString+=2;
    }

    wLastIndex=strlen(pbString)-1;
    if( pbString[wLastIndex]=='h' || pbString[wLastIndex]=='H' )
    {
        wBase=16;
        pbString[wLastIndex]=0;
        wLastIndex--;
    }

    if(wBase==10)
    {
        wResult=atoi(pbString);
        wResult=wMinus ? (-1*wResult):wResult;
    }
    else
    {
        for(wI=0; wI<=wLastIndex; wI++)
        {
            if(__isalpha(pbString[wI]))
            {
                if(__isupper(pbString[wI]))
                    wResult=(wResult<<4)+pbString[wI]-'A'+10;
                else
                    wResult=(wResult<<4)+pbString[wI]-'a'+10;
            }
            else
            {
                wResult=(wResult<<4)+pbString[wI]-'0';
            }
        }
        wResult=wMinus ? (-1*wResult):wResult;
    }
    return wResult;
}

/*
*********************************************************************************************
*                                       CONSOL_SendChar
*
* Description: This routine waits till the character is sent.
*
* Arguments  : bData - Data to be sent.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_SendChar(char bData)
{
    switch(__wChannel)
    {
    case 0:
        while(rUFSTAT0 & 0x200); //Wait until THR is empty.
        //                Delay(4);
        rUTXH0 = bData;
        break;

    case 1:
        while(rUFSTAT1 & 0x200); //Wait until THR is empty.
        //                Delay(4);
        rUTXH1 = bData;
        break;

    case 2:
        while(rUFSTAT2 & 0x200); //Wait until THR is empty.
        //                Delay(4);
        rUTXH2 = bData;
        break;
    }
}

/*
*********************************************************************************************
*                                       CONSOL_SendCh
*
* Description: This routine waits till the character is sent. It also sends an extra carriage
*              return character when sending a new line character
*
* Arguments  : bData - Data to be sent.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_SendCh(char bData)
{
    if(bData == '\n')
    {
        CONSOL_SendChar('\r');
    }

    CONSOL_SendChar(bData);
}

/*
*********************************************************************************************
*                                       CONSOL_SendString
*
* Description: This routine waits till the string is sent.
*
* Arguments  : pbString - String to be sent.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_SendString(char *pbString)
{
    while(*pbString)CONSOL_SendCh(*pbString++);
}

/*
*********************************************************************************************
*                                       CONSOL_Scanf
*
* Description: Reads input from the consol stream, under control of the string pointed to by
*              format that specifies the admissible input sequences and how they are to be
*              converted for assignment, using subsequent arguments as pointers to the
*              objects to receive the converted input. If there are insufficient arguments
*              for the format, the behavior is undefined. If the format is exhausted while
*              arguments remain, the excess arguments are ignored.
*
* Arguments  : pcFmt - Format string. It can contain only the following format specifiers:
*                      %s - String.
*                      %c - character.
*                      %i - Integer.
*              ...   - Are the passed parameters (pointers to the objects to receive the
*                      converted input).
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_Scanf(char *pcFmt,...)
{
    va_list pArg;
    char cChar;
    int *pwInt;
    char *pbChar;

    va_start(pArg, pcFmt);
    while((cChar=*pcFmt++) != '\0')
    {
        if(cChar != '%')continue;
        switch(*pcFmt)
        {
        case 's':
        case 'S':
            pbChar = va_arg (pArg, char *);
            CONSOL_GetString(pbChar);
            break;
        case 'i':
        case 'I':
            pwInt = va_arg (pArg, int *);
            *pwInt = CONSOL_GetIntNum();
            break;
        case 'c':
        case 'C':
            pbChar = va_arg (pArg, char *);
            *pbChar = CONSOL_GetCh();
            break;
        }
    }
    va_end(pArg);
}

/*
*********************************************************************************************
*                                       CONSOL_Printf
*
* Description: Writes output to the consol stream, under control of the string pointed to by
*              format that specifies how subsequent arguments are converted for output. If 
*              there are insufficient arguments for the format, the behavior is undefined.
*              If the format is exhausted while arguments remain, the excess arguments are
*              ignored.
*
* Arguments  : pcFmt - Format string. It can contain all the format specifies.
*              ...   - Are the passed parameters (pointers to the objects to receive the
*                      converted input).
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_Printf(char *pcFmt,...)
{
    va_list ap;
    char pbString[256];

    va_start(ap,pcFmt);
    vsprintf(pbString,pcFmt,ap);
    CONSOL_SendString(pbString);
    va_end(ap);
}


/* ********************************************************************* */
