#include <msp430.h> 
#include "ledFunks.h"
#include "vuFunks.h"

/*
 * main.c
 */
#define NUM_SAMPS	20

int sample, sampFlag, ndx;
unsigned long int avg;


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

/*
 * 	-----------------	LED board initialization  ------------------------
 */

    P3DIR |= 0x03;		// Set P6.0 & P6.1 as outs
    P3DIR &= ~0xFC;		// init other pins as ins

    int i, sListFlag, coms[3] = {0x76, 0x00, 0x17};
    sListFlag = ledInit(coms);
    if(sListFlag)(P1OUT |= BIT0);	// indicate error
    else(P1OUT &= ~BIT0);


    // initialize display to dashes
    int disp[4];
    disp[0] = 0x81;
    disp[1] = 0x81;
    disp[2] = 0x81;
    disp[3] = 0x81;

    ledWrite(0x76,disp);
    __delay_cycles(0xffff);

    for(i=0; i<4; i++){
    	disp[i] = 0x80;
    }

    ledWrite(0x76,disp);


/*
 * 	-----------------	ADC initialization  ------------------------
 */

    ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
    ADC12MCTL0 = ADC12INCH_1;					// multiplex ADC to P6.1
    ADC12CTL1 = ADC12SHP;                     // Use sampling timer
    ADC12IE = 0x01;                           // Enable interrupt
    ADC12CTL0 |= ADC12ENC;					  // Enable encoding

    P1DIR |= BIT0;



//    ADC12CTL0 |= ADC12SC;
//    __bis_SR_register(GIE);        // LPM0 with interrupts enabled (CPUOFF waits for ADC)

/*
 * 	-----------------	Timer initialization  ------------------------
 */

    TA0CCTL0 = CCIE;
    TA0CCR0 = 1000;
    TA0CTL = TASSEL_2 + MC_1;	// SMCLK, upmode

/*
 * 	----------------   Other initialization -------------------------
 */

    unsigned int sampAry[NUM_SAMPS] = {0};
//    unsigned int avg=0;
    P4DIR |= BIT7;
    __bis_SR_register(GIE);        // enable interrupts



    while(1){

    	if(sampFlag == 1){

    		// rectify
    		if(sample<2000){
    			sample = (2000-sample) + sample;
    		}
    		sample -= 2000;

    		// copy and average
    		sampAry[ndx] = sample;		// copy sample val into array
    		ndx++;						// index for next sample
    		if(ndx>NUM_SAMPS)(ndx=0);

    		avg=0;
    		for(i=0; i<NUM_SAMPS; i++){
    			avg += sampAry[ndx];
    		}

    		avg = avg/NUM_SAMPS;

    		setDispAry(avg,disp);
//    		for(i=0; i<4; i++){
//    			disp[i] = 0x8A;
//    		}

    		__bic_SR_register(GIE);
    		ledWrite(0x76,disp);


    		P4OUT ^= BIT7;		// test if running
    		TA0CCTL0 = CCIE;	// restart interrupts
    		__bis_SR_register(GIE);		// reenable globals

    		sampFlag = 0;	// clear sample flag
    	}


    }


	return 0;
}


// ADC10 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	sample = ADC12MEM0;					// clrs IFG
	__bic_SR_register_on_exit(CPUOFF + GIE);	// comment if troublesome
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{

	__bic_SR_register(GIE);
	ADC12CTL0 |= ADC12SC;
	P1OUT ^= BIT0;
	TA0CCTL0 &= ~CCIE;						// disable timer interrupt to prevent second trigger
	sampFlag = 1;
	__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled (CPUOFF waits for ADC)

}






