/*
 * File:   newmainXC16.c
 * Author: Aziz
 *
 * Created on April 7, 2016, 8:21 PM
 */


#include "xc.h"
#include "p24fv32ka302.h"
#define FCY 16000000UL
#include "libpic30.h"

#pragma config FWDTEN = OFF 
#pragma config FNOSC = FRCPLL

/**************************************************/
/*PWM setup to control motor for spinning led*/
int main(void) {
    ANSBbits.ANSB14 = 0;//set digital input for momentary switch 1
    ANSBbits.ANSB12 = 0;//set digital input for momentary switch 2
    TRISB = 0xFF00;//configure pins
    OC1CON1 = 0x0000;//reset
    OC1CON2 = 0x0000;//reset
    
    OC1R = 0x02D0; //duty cycle = 45%
    OC1RS = 0x0640;//10KHz frequency
    
    OC1CON2 = 0x001F;//set OCSYNC to this OC module
    OC1CON1 = 0x1C06;//PWM source to system clock and start center aligned PWM
    
    INTCON2bits.INT1EP = 1;//rising edge priority for interrupt 1
    INTCON1bits.NSTDIS = 1;//disable nested interrupts
    INTCON2bits.INT2EP = 1;//rising edge priority for interrupt 2
    IFS1bits.INT1IF = 0;
    IFS1bits.INT2IF = 0;
    IEC1bits.INT1IE = 1;//clear flag line 35 and enable interrupt 1
    IEC1bits.INT2IE = 1;//clear flag line 36 and enable interrupt 2
    
    while(1);
    
}

/**************************************************/
/*ISR for momentary switch 1 that lowers PWM duty cycle
 thus slows motor*/
void __attribute__((interrupt,no_auto_psv)) _INT1Interrupt(void){
    IFS1bits.INT1IF = 0;
    if (OC1R > 0x0000)
        OC1R -= 0x00A0;
    return;
}

/**************************************************/
/*ISR for momentary switch 2 that increases PWM duty cycle
 thus speeds up motor*/
void __attribute__((interrupt,no_auto_psv)) _INT2Interrupt(void){
    IFS1bits.INT2IF = 0;
    if(OC1R < OC1RS)
        OC1R += 0x00A0;
    return;
}
