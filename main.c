/* 
 * File:   main.c
 * Author: Shashikant
 *
 * Created on 2 July, 2018, 12:43 PM
 */

//Options
#include <16F877A.h>
//#include<18F45K22.h>
#fuses NOPROTECT,NOLVP,NOWDT
#use delay(crystal=4000000)
#use rs232( baud=19200, xmit=PIN_C6, rcv=PIN_C7, bits=8,ERRORS)

//LCD module connections
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_ENABLE_PIN PIN_D2
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
//End LCD module connections

#define CS PIN_A5



#include <lcd.c>

#include <stdio.h>
#include <stdlib.h>

//spi modes
#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H)
#define SPI_MODE_1  (SPI_L_TO_H)
#define SPI_MODE_2  (SPI_H_TO_L)
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H)

#include "rfid.h"

////
//  Mifare Card Types Depending upon the codes of SAK from AN1083.PDF
////

enum MIFARE_CARD_TYPE {
    UID_FAILED = 0x00,
    UID_NOT_COMPLETE = 0x04,
    MIFARE_MINI = 0x09,
    MIFARE_CLASSIC_1K = 0x08,
    MIFARE_CLASSIC_4K = 0x18,
    MIFARE_DESFIRE = 0x20
};




byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[MAX_LEN]; // Variable used to store Full Tag Data
byte serial[5];
byte i; //for looping
byte j; //for looping again
byte status; // to store the return status of command operations
byte data[MAX_LEN]; //to store any data

char buffer2[20];

//keys for the card
byte keyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
byte keyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


//writeable data 
byte writeData[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};



/////////
///     Check RFID Hardware
/////////

void CheckRFIDHardware() {

    char version; ///Version of RFID read/writer is stored 0x92 is version 2.0
    ///Version 1.0 is 0x91

    lcd_putc("\f Checking RFID...");
    printf("\r\nChecking If Rfid Is Connected");

    //Request from the register
    version = readFromRegister(VersionReg);
    delay_ms(100);


    //response is 00 then hardware is not connected
    if (!version) {
        lcd_putc("\f RFID Not Found \n Check Again..");
        printf("\r\nRFID Hw not Found");
        delay_ms(5000);
    }
    if (version) {
        lcd_putc("\f RFID Found");
        printf(lcd_putc, "\nV: %x", version);
        printf("\r\nRFID Found and HW version is: 0x%2X\r\n", version);
        delay_ms(1000);
    }

}


//////
///     Function to read all the data from the card
///     Read all the data from the mifare card and out puts to serial terminal    
/////

void readMifareCard(void) {

    //s50 has 64 blocks of memory
    for (i = 0; i < 64; i++) {
        // Try to authenticate each block first with the A key.
        status = authenticate(MF1_AUTHENT1A, i, keyA, serial);
        if (MI_OK == status) {
            // printf("Authenticated block 0x%2X with A\r\n",i);
            printf("\r\n"); //a new line each block
            //now read the block i 
            status = readFromTag(i, data);
            if (MI_OK == status) {
                for (j = 0; j < 15; j++) {
                    printf("0x%2X, ", data[j]);
                }
                printf("0x%2X", data[15]);
            } else {
                printf("Read failed\r\n");
            }

        } else {
            //try to authenticate with key B
            status = authenticate(MF1_AUTHENT1B, i, keyB, serial);
            if (MI_OK == status) {
                //printf("Authenticated block 0x%2X with key B",i);
                printf("\r\n"); //a new line each block

                //read from block
                status = readFromTag(i, data);

                if (MI_OK == status) {
                    for (j = 0; j < 15; j++) {
                        printf("%2X, ", data[j]);
                    }
                    printf("%2X, ", data[15]);
                } else {
                    printf("Read failed \r\n");
                }

            } else {
                printf("Access denied at block 0x%2X", i);
            }
        }
    }


}

void writeMifare(void){
    int block = 4;

                    status = authenticate(MF1_AUTHENT1A, 2, keyA, serial);

                    //authenticated with key A
                    if (MI_OK == status) {
                        printf("Authenticated with key A \r\n");
                        status = writeToTag(2, writeData);
                        if (MI_OK == status) {
                            printf("Write Successful\r\n");
                        }else{
                            printf("Write failed\r\n");
                        }


                    } else {
                        //try to authenticate with key b
                        status = authenticate(MF1_AUTHENT1B, 2, keyB, serial);
                        if (MI_OK == status) {
                            printf("Authenticated with key B \r\n");
                            status = writeToTag(2, writeData);
                            if (MI_OK == status) {
                                printf("Write Successful\r\n");
                            }else{
                                printf("Write Failed");
                            }

                        } else {
                            printf("Couldn't access block 0x%X write failed", 2);
                        }

                    }


}

void main() {

    //setup spi line 

    setup_spi(SPI_MASTER | SPI_MODE_0 | SPI_CLK_DIV_16);

    delay_ms(20);

    //init lcd and put some string
    lcd_init();

    lcd_putc("\f Starting...");

    printf("\r\nstarting.."); //output to terminal    

    //initialize RFID Reader

    delay_ms(100);
    MFRC_begin();

    //check if the card reader is connected.
    CheckRFIDHardware();

    /////  


    while (TRUE) {

        //        
        //      //  Check to see if a Tag was detected
        //     // If yes, then the variable FoundTag will contain "MI_OK"
        FoundTag = requestTag(MF1_REQIDL, TagData);

        //
        //If found tag
        if (FoundTag == MI_OK) {

            printf("\r\nFound Tag: ");

            ReadTag = antiCollision(TagData);

            ///has tag passed anti collision test?
            if (ReadTag == MI_OK) {

                //then put out UUID 
                for (i = 0; i <= 2; i++) {
                    sprintf(buffer2, "%2X", (TagData[i])); //display version in hexadecimal
                    delay_ms(20);
                    printf(buffer2);
                    printf(", ");
                }

                sprintf(buffer2, "%2X", (TagData[i])); //display version in hexadecimal
                printf(buffer2);

                memcpy(serial, TagData, 5);

                //now get SAK for card version check
                byte sak = selectTag(serial);

                printf("\r\nSAK: 0x%2x", sak);


                if (UID_FAILED == sak) {
                    printf("\r\n Failed to read card");
                }//UID was read incompletely
                else if (UID_NOT_COMPLETE == sak) {
                    printf("\r\n Incomplete UID");
                } else {
                    printf("\r\n Is a Mifare Card");

                    //everything went well so far                        
                    //now start reading memory
                    readMifareCard();
                    //writeMifare();
                   

                    haltTag();
                    

                }


            }


        }

        delay_ms(2000);
        //    
    }

    return;
}

