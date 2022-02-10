#ifndef TcTimer5_h
#define TcTimer5_h

#include "Arduino.h"

#define _TC_TIMER5_MAX		8

namespace TcTimer5 {

	void begin();
	void start(voidFuncPtr func);
	void stop();
	unsigned int get(unsigned int select);
	unsigned int reset(unsigned int select);
	unsigned int check(unsigned int select, unsigned int count);

	void tcConfigure();
	bool tcIsSyncing();
	void tcStartCounter();
	void tcReset();
	void tcDisable();
};

#endif
