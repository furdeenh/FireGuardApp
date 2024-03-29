/*===================================CPEG222====================================
 * Program:      Project 3 template
 * Authors:     Chengmo Yang and Tommy White
 * Date:        10/19/2022
 * This is a template that you can use to write your project 3 code, for mid-stage and final demo.
==============================================================================*/
/*-------------- Board system settings. PLEASE DO NOT MODIFY THIS PART ----------*/
#ifndef _SUPPRESS_PLIB_WARNING          //suppress the plib warning during compiling
#define _SUPPRESS_PLIB_WARNING
#endif
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
/*----------------------------------------------------------------------------*/

#include <xc.h>   //Microchip XC processor header which links to the PIC32MX370512L header
#include <stdio.h>  // need this for sprintf
#include <sys/attribs.h>
#include "config.h" // Basys MX3 configuration header

#include "lcd.h"
#include "led.h"
#include "utils.h"
#include "ssd.h"
#include "adc.h"


#define sensor1 PORTDbits.RD0
#define sensor4 PORTDbits.RD1
#define sensor2 PORTDbits.RD2
#define sensor3 PORTDbits.RD3

#define sensor1 PORTDbits.RD8
#define sensor4 PORTDbits.RD9
#define sensor2 PORTDbits.RD10  
#define sensor3 PORTDbits.RD11

#define Sw7  PORTBbits.RB9

/* ------------------------ Constant Definitions ---------------------------- */
#define SYS_FREQ (80000000L) // 80MHz system clock
#define _80Mhz_ (80000000L)
#define LOOPS_NEEDED_TO_DELAY_ONE_MS_AT_80MHz 1426
#define LOOPS_NEEDED_TO_DELAY_ONE_MS (LOOPS_NEEDED_TO_DELAY_ONE_MS_AT_80MHz * (SYS_FREQ / _80Mhz_))




int LED = 0b00000000;
int v1 = 90;
int v2 = 90;
int v3 = 90;
int v4 = 90;
int d1 = 0;
int d2 = 1;
int d3 = 1;
int d4 = 1;
int state = 0;
int go = 0;
int go2 = 0;
char buff[17];
char buffer[17];
int count = 0;
int marker = 0;



void Timer2_Setup();
void Timer3_Setup();
void delay_ms();

int main() {
    LCD_Init();
    LED_Init();
    SSD_Init();
    ADC_Init();
    TRISA &= 0xFF00;
    /* The following line sets the tristate of Port F bit 0 to 1. BtnC is connected
to that
     pins. When the tristate of a pin is set high, the pin is configured as a
     digital input. */
    DDPCONbits.JTAGEN = 0; // Required to use Pin RA0 (connected to LED 0) as IO
    /* The following line sets the tristate of Port A bits 0-7 to 0. The LEDs are 
connected
     to those pins. When the tristate of a pin is set low, the pin is configured
     as a digital output. Notice an &= is used in conjunction with leading 1s
     (the FF) in order to keep the other bits of Port A (8-15) in their current 
state. */
    TRISBbits.TRISB8 = 1;
    ANSELBbits.ANSB8 = 0;
    TRISBbits.TRISB12 = 0; //RB12 set as output 
    ANSELBbits.ANSB12 = 0; //RB12 analog functionality disabled 
    TRISBbits.TRISB13 = 0; //RB13 set as output 
    ANSELBbits.ANSB13 = 0; //RB13 analog functionality disabled 
    TRISAbits.TRISA9 = 0; //RA9 set as output 
    TRISAbits.TRISA10 = 0; //RA10 set as output 
    TRISGbits.TRISG12 = 0; //RG12 set as output 
    TRISAbits.TRISA14 = 0; //RA14 set as output 
    TRISDbits.TRISD6 = 0; //RD6 set as output 
    TRISGbits.TRISG13 = 0; //RG13 set as output 
    TRISGbits.TRISG15 = 0; //RG15 set as output 
    TRISDbits.TRISD7 = 0; //RD7 set as output 
    TRISDbits.TRISD13 = 0; //RD13 set as output 
    TRISGbits.TRISG14 = 0; //RG14 set as output

    //swtich
    TRISBbits.TRISB9 = 1; // sets switch 7 as an input
    ANSELBbits.ANSB9 = 0; // takes switch 7 off analog
    TRISBbits.TRISB10 = 1; // sets switch 6 as an input
    ANSELBbits.ANSB10 = 0; // takes switch 6 off analog
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input 
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input 

    //pmod
    TRISDbits.TRISD0 = 1; //sets LD1 as input
    TRISDbits.TRISD1 = 1; //sets LD2 as input
    ANSELDbits.ANSD1 = 0; //turns analog off
    TRISDbits.TRISD2 = 1; //sets LD3 as input
    TRISDbits.TRISD3 = 1; //sets LD4 as input

    TRISBbits.TRISB9 = 1; // sets switch 7 as an input
    ANSELBbits.ANSB9 = 0; // takes switch 7 off analog

    Timer2_Setup();
    Timer3_Setup();

    //LED_SetGroupValue(LED);
    LCD_WriteStringAtPos("Mike Hawk Ready    ", 0, 0);
    SSD_WriteDigits(v1, v2, v3, v4, 0, 0, 0, 0);
    while (1) {
        LED_SetGroupValue(LED);
        ADC_AnalogRead(4);
        //delay_ms(1);

        //SSD_WriteDigits(d1, d2, d3, d4, 0, 0, 0, 0);
        if (ADC_AnalogRead(4) > 700) {
            go = 1;
            go2 = 0;
            LCD_WriteStringAtPos("    Mike Hawk!    ", 0, 0);

        }
        if (go == 1) {
            if (ADC_AnalogRead(4) > 700) {
                go2 = 1;
                LCD_WriteStringAtPos("    Mike Hawk!!!    ", 0, 0);
                //delay_ms(50);
            }
        }
        if (go2 == 1 && go == 1) {
            state = 1;
        } else if (Sw7 == 1) {
            state = 1;
        }
        sprintf(buffer, " %d%d%d%d", sensor1, sensor2, sensor3, sensor4);
        sprintf(buff, " %d", count);
        LCD_WriteStringAtPos(buff, 1, 4);
        LCD_WriteStringAtPos(buffer, 1, 8);
        if (state == 1) {
            T3CONbits.ON = 1;
            LCD_WriteStringAtPos("    Mike Hawk    ", 0, 0);
            //LED_SetGroupValue(LED);
            SSD_WriteDigits(v1, v2, v3, v4, 0, 0, 0, 0);
            delay_ms(10);
            if (sensor1 == 0 && sensor2 == 0 && sensor3 == 0 && sensor4 == 0) {
                LCD_WriteStringAtPos("STP", 1, 13);
                LCD_WriteStringAtPos("STP ", 1, 0);

                T2CONbits.ON = 1;
            }
            if (sensor1 == 1 && sensor2 == 0 && sensor3 == 0 && sensor4 == 1) {
                LCD_WriteStringAtPos("FWD", 1, 13);
                LCD_WriteStringAtPos("FWD ", 1, 0);
                T2CONbits.ON = 1;
            }
            if (sensor1 == 0 && sensor2 == 0 && sensor3 == 1 && sensor4 == 1) {
                LCD_WriteStringAtPos("STP", 1, 13);
                LCD_WriteStringAtPos("FWD ", 1, 0);
                T2CONbits.ON = 1;
            }
            if (sensor1 == 0 && sensor2 == 0 && sensor3 == 0 && sensor4 == 1) {
                LCD_WriteStringAtPos("STP", 1, 13);
                LCD_WriteStringAtPos("FWD ", 1, 0);
                T2CONbits.ON = 1;
            }

            if (sensor1 == 1 && sensor2 == 1 && sensor3 == 0 && sensor4 == 0) {
                LCD_WriteStringAtPos("FWD", 1, 13);
                LCD_WriteStringAtPos("STP ", 1, 0);
                T2CONbits.ON = 1;
            }
            if (sensor1 == 1 && sensor2 == 0 && sensor3 == 0 && sensor4 == 0) {
                LCD_WriteStringAtPos("FWD", 1, 13);
                LCD_WriteStringAtPos("STP ", 1, 0);
                T2CONbits.ON = 1;
            }
            if (sensor1 == 1 && sensor2 == 1 && sensor3 == 1 && sensor4 == 1) {
                LCD_WriteStringAtPos("REV", 1, 13);
                LCD_WriteStringAtPos("REV", 1, 0);
                T2CONbits.ON = 1;
            }



        }
    }



}

void Timer2_Setup() {
    PR2 = 3905; //set period register, generates one interrupt every 3 ms
    TMR2 = 0; // initialize count to 0
    T2CONbits.TCKPS = 7; // 1:64 prescale value
    T2CONbits.TGATE = 0; // not gated input (default)
    T2CONbits.TCS = 0; // PCBLK input (default)
    T2CONbits.ON = 0; // turn on Timer1
    IPC2bits.T2IP = 5; // priority
    IPC2bits.T2IS = 3; // subpriority
    IFS0bits.T2IF = 0; // clear interrupt flag
    IEC0bits.T2IE = 1; // enable interrupt

}

void __ISR(_TIMER_2_VECTOR)Timer2ISR(void) {
    IFS0bits.T2IF = 0; // clear interrupt flag

    if (state == 1) {
        v1 = d1;
        if (d1 == 9) {
            v2 = d2;

            if (d2 == 9) {
                v3 = d3;
                if (d3 == 9) {
                    v4 = d4;
                    if (d4 == 9) {
                        d1 = 0;
                        d2 = 0;
                        d3 = 0;
                        d4 = 0;


                    } else {
                        d4++;
                        d3 = 0;
                        d2 = 0;
                        d1 = 0;
                    }
                } else {
                    d3++;
                    d2 = 0;
                    d1 = 0;
                }
            } else {
                d2++;
                d1 = 0;
            }
        } else {
            d1++;
        }
        count++;
        if (count > 435) {
            marker = 2;
        }

        SSD_WriteDigits(v1, v2, v3, v4, 0, 0, 0, 0);
        //IFS0bits.T2IF = 0; // clear interrupt flag
        IEC0bits.T2IE = 1; // enable interrupt
    }
}

//    IFS0bits.T3IF = 0; // clear interrupt flag
//    IEC0bits.T3IE = 1; // enable interrupt

void Timer3_Setup() {
    PR3 = (int) (((float) (10000000) / (256 * 50)) + 0.5);
    TMR3 = 0; // initialize count to 0
    T3CONbits.TCKPS = 7; // 1:256 prescale value
    T3CONbits.TGATE = 0; // not gated input (default)
    T3CONbits.TCS = 0; // PCBLK input (default)
    T3CONbits.ON = 0; // turn on Timer1
    IPC3bits.T3IP = 7; // priority
    IPC3bits.T3IS = 3; // subpriority
    IFS0bits.T3IF = 0; // clear interrupt flag
    IEC0bits.T3IE = 1; // enable interrupt

    // enable
    // priorities


    //motor

    tris_SRV_S0PWM = 0;
    tris_SRV_S1PWM = 0;

    // disable analog
    ansel_SRV_S0PWM = 0;


    // remap pins
    rp_SRV_S0PWM = 0x0B; // 1101 = OC4 right- servo 0-
    rp_SRV_S1PWM = 0x0B; // 1101 = OC5 left - servo 1-


    // Configure Output compare Module 5

    OC5CONbits.ON = 0;
    OC5CONbits.OCM = 6;
    OC5CONbits.OCTSEL = 1;
    OC5CONbits.ON = 1;

    // Configure Output Compare Module 4

    OC4CONbits.ON = 0;
    OC4CONbits.OCM = 6;
    OC4CONbits.OCTSEL = 1;
    OC4CONbits.ON = 1;


    macro_enable_interrupts(); // enable interrupts at CPU
}

void __ISR(_TIMER_3_VECTOR)Timer3ISR(void) {

    if (sensor1 == 0 && sensor2 == 0 && sensor3 == 0 && sensor4 == 0) {
        Forward_right();
       Forward_left();
       if(marker==2){
            Stop_left();
        Stop_right();
       }
        
    } else if (sensor1 == 1 && sensor2 == 0 && sensor3 == 0 && sensor4 == 1) {
        Forward_right();
        Forward_left();
    } else if (sensor1 == 0 && sensor2 == 1 && sensor3 == 1 && sensor4 == 1) {
        Stop_left();
        Forward_right();
    } else if (sensor1 == 0 && sensor2 == 0 && sensor3 == 1 && sensor4 == 1) {
        Stop_left();
        Forward_right();
    } else if (sensor1 == 0 && sensor2 == 0 && sensor3 == 0 && sensor4 == 1) {
        Stop_left();
        Forward_right();
    } else if (sensor1 == 1 && sensor2 == 1 && sensor3 == 1 && sensor4 == 0) {
        Forward_left();
        Stop_right();
    } else if (sensor1 == 1 && sensor2 == 1 && sensor3 == 0 && sensor4 == 0) {
        Forward_left();
        Stop_right();
    } else if (sensor1 == 1 && sensor2 == 0 && sensor3 == 0 && sensor4 == 0) {
        Forward_left();
        Stop_right();
 } else if (sensor1 == 1 && sensor2 == 1 && sensor3 == 1 && sensor4 == 1) {
        Reverse_right();
       Reverse_left();
  }
//    else if(marker == 2 && sensor1 ==0 && sensor2 == 0 && sensor3 ==0 && sensor4 ==0 ){
//        Stop_left();
//        Stop_right();
//    }




    IFS0bits.T3IF = 0; // clear interrupt flag
    IEC0bits.T3IE = 1; // enable interrupt
}

void Forward_right() {
    OC4RS = PR3 / 20;
}

void Forward_left() {
    OC5RS = PR3 / 10;
}

void Reverse_right() {
    OC4RS = PR3 / 10;
}

void Reverse_left() {
    OC5RS = PR3 / 20;
}

void Stop_right() {
    OC4RS = PR3 * 3 / 40;
}

void Stop_left() {
    OC5RS = PR3 * 3 / 40;
}

void delay_ms(int milliseconds) { // function to help with debounce
    int i;
    for (i = 0; i < milliseconds * LOOPS_NEEDED_TO_DELAY_ONE_MS; i++) {
    }
}