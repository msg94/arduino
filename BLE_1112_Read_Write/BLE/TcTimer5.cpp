#include "TcTimer5.h"

volatile unsigned int pMsCount[_TC_TIMER5_MAX+8];
voidFuncPtr TcTimer5_callBack = NULL;

void TcTimer5::begin()
{
	int i;
	for(i = 0; i < _TC_TIMER5_MAX; i++)
	{
		pMsCount[i] = 0;
	}
	tcConfigure();
}

void TcTimer5::start(voidFuncPtr func)
{
	TcTimer5_callBack = func;
	tcStartCounter();
}

void TcTimer5::stop()
{
	tcDisable();
}

unsigned int TcTimer5::get(unsigned int select)
{
	if(select < _TC_TIMER5_MAX)
	{
		return pMsCount[select];
	}
	return 0;
}

unsigned int TcTimer5::reset(unsigned int select)
{
	if(select < _TC_TIMER5_MAX)
	{
		pMsCount[select] = 0;
	}
	return 0;
}

unsigned int TcTimer5::check(unsigned int select, unsigned int count)
{
	if(select < _TC_TIMER5_MAX)
	{
		if(pMsCount[select] >= count)
		{
			pMsCount[select] = 0;
			return count;
		}
	}
	return 0;
}

void TC5_Handler(void)
{
	int i;
	for(i = 0; i < _TC_TIMER5_MAX; i++)
	{
		if(pMsCount[i] < 0xFFFFFF){ pMsCount[i]++; };
	}
	if(TcTimer5_callBack != NULL){ TcTimer5_callBack(); }
	TC5->COUNT16.INTFLAG.bit.MC0 = 1;
}

void TcTimer5::tcConfigure()
{
	// Enable GCLK for TCC2 and TC5 (timer counter input clock)
	GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
	while (GCLK->STATUS.bit.SYNCBUSY);

	tcReset(); //reset TC5

	// Set Timer counter Mode to 16 bits
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
	// Set TC5 mode as match frequency
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
	//set prescaler and enable TC5
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64 | TC_CTRLA_ENABLE;
	//set TC5 timer counter based off of the system clock and the user defined sample rate or waveform
	TC5->COUNT16.CC[0].reg = (uint16_t) ((SystemCoreClock / 64) / 1000 - 1);
	while (tcIsSyncing());

	// Configure interrupt request
	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_SetPriority(TC5_IRQn, 0);
	NVIC_EnableIRQ(TC5_IRQn);

	// Enable the TC5 interrupt request
	TC5->COUNT16.INTENSET.bit.MC0 = 1;
	while (tcIsSyncing()); //wait until TC5 is done syncing 
}


//Function that is used to check if TC5 is done syncing
//returns true when it is done syncing
bool TcTimer5::tcIsSyncing()
{
	return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

//This function enables TC5 and waits for it to be ready
void TcTimer5::tcStartCounter()
{
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
	while (tcIsSyncing()); //wait until snyc'd
}

//Reset TC5 
void TcTimer5::tcReset()
{
	TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while (tcIsSyncing());
	while (TC5->COUNT16.CTRLA.bit.SWRST);
}

//disable TC5
void TcTimer5::tcDisable()
{
	TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
	while (tcIsSyncing());
}
