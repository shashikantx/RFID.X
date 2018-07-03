/* 
 * File:   main.c
 * Author: Shashikant
 *
 * Created on 2 July, 2018, 12:43 PM
 */

//Options
#include <16F877A.h>
//#include<18F45K22.h>
#fuses NOPROTECT,NOLVP
#use delay(crystal=4000000)
#use rs232( baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8)

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


byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[16]; // Variable used to store Full Tag Data
byte GoodTagSerialNumber[4] = {0x9E, 0x80, 0xB1, 0x59}; // The Tag Serial number we are looking for
int i=0;
char buffer[20],buffer2[20],text[20];
byte version;
int GoodTag=0; // Variable used to confirm good Tag Detected 


void CheckRFIDHardware(){
lcd_putc("\f Checking RFID...");
//check request
version = readFromRegister(VersionReg);
delay_ms(1000);

//response is 00 then hardware is not connected
if(!version){
    lcd_putc("\f RFID Not Found \n Check Again..");
    delay_ms(5000);
}
if(version){
    lcd_putc("\f RFID Found");
    printf(lcd_putc,"\nV: %x",version);
    delay_ms(1000);
}

} 

void readRFID()
{
    
    lcd_putc("\fHold Tag Still");
    
    delay_ms(500);
    // Get anti-collision value to properly read information from the Tag
    ReadTag = antiCollision(TagData);
    
    lcd_putc("\f");
    
    lcd_putc("Data: ");
     
   for(i=0;i<=2;i++)
   {
    sprintf(buffer2,"%2X",(TagData[i])); //display version in hexadecimal
    delay_ms(20);
    printf(lcd_putc,buffer2);
  
   }
    sprintf(buffer2,"%2X",(TagData[i])); //display version in hexadecimal
    delay_ms(20);
    printf(lcd_putc,buffer2); 
    
    
    delay_ms(2000);
 
}





void main() {   
    
    //setup spi line 
    setup_spi(SPI_MASTER|SPI_MODE_0|SPI_CLK_DIV_16); 
    
    //init lcd and put some string
    lcd_init();    
    lcd_putc("\f Starting...");    
    printf("starting..");
    delay_ms(500);
    
    //initialize RFID Reader
    MFRC_begin();
    
    //check if the card reader is connected.
    CheckRFIDHardware();
    
    
    while(TRUE){
        
      //  Check to see if a Tag was detected
     // If yes, then the variable FoundTag will contain "MI_OK"
    FoundTag = requestTag(MF1_REQIDL, TagData);
    
    lcd_putc("\fChecking for Tag \r\n");
    printf("\r\n Check For Tag");
    delay_ms(500);
   
    
    //tag is found
    if(FoundTag == MI_OK){
        
        //if tag is found PIN_B0 is set to high,
        output_high(PIN_B0);
        
        //put that rfid is found on display
        lcd_putc("\f Tag Found");
        
        delay_ms(200);

        
        readRFID();
        
        //set PIN_BO to low
        output_low(PIN_B0);
       
    }else{//tag not found
        lcd_putc("\f Tag Not Found");
        delay_ms(1000);
    }
    
    delay_ms(1000);
        
    }
    
    
    return;
}

