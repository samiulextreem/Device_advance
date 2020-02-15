

#include <Arduino.h>
#include <U8g2lib.h>
#include <array>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>

#endif

U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI LCD(U8G2_MIRROR, D0, D1, D2, D3, D4); //scl,si,cs,rs,rse //io16,io5,io4,io0,io2
void LCD_print(String x);
void LCD_clear();


void setup(void){
    LCD.begin();
    Serial.begin(9600);
    LCD.clearBuffer();
    LCD.setFont(u8g2_font_siji_t_6x10);
    LCD.setContrast(80);
}


void loop(void){
    LCD_print("welcome to the rice field");
    while(1==1){
        delay(1000);
        delay(1000);
    }

    
}

void LCD_print(String x){
    int x_pos = 0;
    int y_pos = 10;
    LCD.setCursor(x_pos,y_pos);

    for(int k = 0 ; k<62; k++){
        if(!x[k]){
            Serial.printf("%d",k);
            for (int i = 0; i <k+1; i++){
                LCD.print(x[i]);
                LCD.sendBuffer();

                if(i % 20 == 0 && i!=0){
                    y_pos = y_pos +10;
                    LCD.setCursor(x_pos,y_pos);
                    
                }
                delay(5);
            }
            break;
        }
    }
    
}


void LCD_clear(){
    LCD.clearDisplay();
    LCD.home();
}
