/* 
 * File:   main.c
 * Author: Shashikant
 *
 * Created on 2 July, 2018, 12:43 PM
 */


//LCD module connections
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_ENABLE_PIN PIN_D2
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
//End LCD module connections

#include <16F877A.h>
#use delay(crystal=8000000)
#include <lcd.c>


#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
void main() {
    
    lcd_init();
    
    lcd_putc('\f');
    lcd_putc("Hello");
    lcd_gotoxy(4,2);
    lcd_putc("Booting");
    
    
    delay_ms(1000);
    lcd_putc(".");
    delay_ms(1000);
    lcd_putc(".");
    delay_ms(1000);
    lcd_putc(".");
    
    
    
    while(TRUE){
        
        
        
    }
    
    
    return;
}

