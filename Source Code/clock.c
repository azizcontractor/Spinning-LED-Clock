/*
 * File:   clock.c
 * Author: Aziz Contractor
 *
 * Created on April 9, 2016, 10:56 PM
 */


#include "xc.h"
#include "p24fv32ka302.h"
#define FCY 16000000UL
#include "libpic30.h"
#pragma config FWDTEN = OFF
#pragma config FNOSC = FRCPLL

int chars[38][7] = {{0x0000,0x7C00,0xA200,0x9200,0x8A00,0x7C00,0x0000},//0
                {0x0000,0x0000,0x8000,0xFE00,0x8400,0x0000,0x0000},//1
                {0x0000,0x8C00,0x9200,0xA200,0xC200,0x8400,0x0000},//2
                {0x0000,0x6C00,0x9200,0x9200,0x9200,0x4400,0x0000},//3
                {0x0000,0x2000,0xFE00,0x2400,0x2800,0x3000,0x0000},//4
                {0x0000,0x7200,0x8A00,0x8A00,0x8A00,0x4E00,0x0000},//5
                {0x0000,0x6000,0x9200,0x9200,0x9400,0x7800,0x0000},//6
                {0x0000,0x0600,0x0A00,0x1200,0xE200,0x0200,0x0000},//7
                {0x0000,0x6C00,0x9200,0x9200,0x9200,0x6C00,0x0000},//8
                {0x0000,0x3C00,0x5200,0x9200,0x9200,0x0C00,0x0000},//9
                {0x4400},//:
                {0x0000,0xFC00,0x1200,0x1200,0x1200,0xFC00,0x0000},//A
                {0x0000,0x6C00,0x9200,0x9200,0x9200,0xFE00,0x0000},//B
                {0x0000,0x4400,0x8200,0x8200,0x8200,0x7C00,0x0000},//C
                {0x0000,0x7C00,0x8200,0x8200,0x8200,0xFE00,0x0000},//D
                {0x0000,0x8200,0x9200,0x9200,0x9200,0xFE00,0x0000},//E
                {0x0000,0x0200,0x1200,0x1200,0x1200,0xFE00,0x0000},//F
                {0x0000,0xE400,0xA200,0xA200,0x8200,0x7C00,0x0000},//G
                {0x0000,0xFE00,0x1000,0x1000,0x1000,0xFE00,0x0000},//H
                {0x0000,0x0000,0x8200,0xFE00,0x8200,0x0000,0x0000},//I
                {0x0000,0x7E00,0x8000,0x8000,0x8000,0x4000,0x0000},//J
                {0x0000,0x8200,0x4400,0x2800,0x1000,0xFE00,0x0000},//K
                {0x0000,0x8000,0x8000,0x8000,0x8000,0xFE00,0x0000},//L
                {0x0000,0xFE00,0x0400,0x1800,0x0400,0xFE00,0x0000},//M
                {0x0000,0xFE00,0x2000,0x1000,0x0800,0xFE00,0x0000},//N
                {0x0000,0x7C00,0x8200,0x8200,0x8200,0x7C00,0x0000},//O
                {0x0000,0x0C00,0x1200,0x1200,0x1200,0xFE00,0x0000},//P
                {0x0000,0xBC00,0x4200,0xA200,0x8200,0x7C00,0x0000},//Q
                {0x0000,0x8C00,0x5200,0x3200,0x1200,0xFE00,0x0000},//R
                {0x0000,0x6400,0x9200,0x9200,0x9200,0x4C00,0x0000},//S
                {0x0000,0x0200,0x0200,0xFE00,0x0200,0x0200,0x0000},//T
                {0x0000,0x7E00,0x8000,0x8000,0x8000,0x7E00,0x0000},//U
                {0x0000,0x0E00,0x3000,0xC000,0x3000,0x0E00,0x0000},//V
                {0x0000,0xFE00,0x4000,0x3000,0x4000,0xFE00,0x0000},//W
                {0x0000,0xC600,0x2800,0x1000,0x2800,0xC600,0x0000},//X
                {0x0000,0x0600,0x0800,0xF000,0x0800,0x0600,0x0000},//Y
                {0x0000,0x8600,0x8A00,0x9200,0xA200,0xC200,0x0000},//Z
                {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}//' '
};
int digit[8];
int i = 0;
int j = 0;
int len;
int revs = 0;

void initRTCC(void);
void init(void);
void readRTCC(void);
void showWelcome(void);

/**************************************************/
//main method 
int main(void) {
    __delay_ms(8000);
    initRTCC();
    init();
    while(1);
    return 0;
}

/**************************************************/
//initialize timer and external interrupt
void init(void) {
    TRISB = 0x01FF;
    T2CON = 0x8000;
    PR2 = 0x0BB8;
    IEC0bits.INT0IE = 1;
    INTCON2bits.INT0EP = 1;
    INTCON1bits.NSTDIS = 1;
}

/**************************************************/
/*interrupt service routine for external interrupt
 occurs on each revolution of the motor*/
void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void) {
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 0;
    if(revs < 200){
        if (revs == 0 || revs == 50 || revs == 100 || revs == 150){
            showWelcome();
        }
    }
    else if(revs < 401)
        readRTCC();
    else {
        revs = 0;
        showWelcome();
    }
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    revs++;
    return;
}

/**************************************************/
/*interrupt service routine for timer 2 which handles
 displaying the characters in sequence*/
void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;
    if(i < 8){
        if(revs > 199 && (i == 2 || i == 5))
            len = 1;
        else
            len = 7;
        if (j < len){
            LATB = chars[digit[i]][j++];
        }
        else{
            i++;
            j = 0;
        }
    }
    else{
        i = 0;
        IEC0bits.T2IE = 0;
        IFS0bits.INT0IF = 0;
        IEC0bits.INT0IE = 1;
    }
}

/**************************************************/
/*reading the real time clock and calender for
 hours, mins, secs, placing them in array for
 display*/
void readRTCC(void){
    _RTCPTR = 1;
    int hours = RTCVAL & 0x00FF;
    digit[7] = hours >> 4;
    digit[6] = hours & 0xF;
    digit[5] = 0xA;
    digit[4] = RTCVAL >> 12;
    digit[3] = (RTCVAL >> 8) & 0xF;
    digit[2] = 0xA;
    digit[1] = (RTCVAL >> 4) & 0xF;
    digit[0] = RTCVAL & 0xF;
}

/**************************************************/
/*initialize real time clock and calender
 currently running from LPRC which is inaccurate
 will be modified to run from 50Hz PWM in the future*/
void initRTCC(void){   
    //_RTCWREN = 1;       // unlock setting
    asm volatile("push w7");
    asm volatile("push w8");
	asm volatile("disi	#5");
	asm volatile("mov	#0x55, w7");
	asm volatile("mov	w7, _NVMKEY");
	asm volatile("mov	#0xAA, w8");
	asm volatile("mov	w8, _NVMKEY");
    asm volatile("bset	_RCFGCAL, #13");    // RTCWREN =1;
	asm volatile("pop w8");
	asm volatile("pop w7");

    _RTCEN = 0; // disable the clock
    RTCPWC = 0x8400; // set clock to 50 KHz pin
    // set timer to 50 KHz
    //initPWM();
    // set 07/11/2011 MON 23:59:30
    _RTCPTR = 3;        // start the sequence
    RTCVAL = 0x16;    // YEAR
    RTCVAL = 0x0409;    // MONTH-1/DAY-1
    RTCVAL = 0x0620;    // WEEKDAY/HOURS
    RTCVAL = 0x5930;    // MINUTES/SECONDS
    
    RCFGCAL |= 0x007f; // add calibration
    
    // lock and enable 
    _RTCEN = 1;         // start the clock
    _RTCWREN = 0;       // lock settings
} // initRTCC

/**************************************************/
/*showing welcome to CSU the time is by placing 
 each word in the array depending on the revs count*/
void showWelcome(void){
    switch(revs){
        case 0:
            digit[7] = 37;
            digit[6] = 33;
            digit[5] = 15;
            digit[4] = 22;
            digit[3] = 13;
            digit[2] = 25;
            digit[1] = 23;
            digit[0] = 15;
            break;
        case 50:
            digit[7] = 37;
            digit[6] = 30;
            digit[5] = 25;
            digit[4] = 37;
            digit[3] = 13;
            digit[2] = 29;
            digit[1] = 31;
            digit[0] = 37;
            break;
        case 100:
            digit[7] = 30;
            digit[6] = 18;
            digit[5] = 15;
            digit[4] = 37;
            digit[3] = 30;
            digit[2] = 19;
            digit[1] = 23;
            digit[0] = 15;
            break;
        case 150:
            digit[7] = 37;
            digit[6] = 37;
            digit[5] = 37;
            digit[4] = 19;
            digit[3] = 29;
            digit[2] = 37;
            digit[1] = 37;
            digit[0] = 37;
            break;
    }
}
