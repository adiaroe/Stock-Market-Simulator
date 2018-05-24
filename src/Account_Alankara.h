#pragma once

#ifndef ACCOUNT_ALANKARA_H
#define ACCOUNT_ALANKARA_H

#include<iostream>

using namespace std;

class Account_Alankara{

protected:

	double cashBalance;

public:
	
	virtual double getBalance() = 0;		// get cash balance
	virtual void setBalance() = 0;			// set cash balance
	virtual void trans_history() = 0;	// Print history of transaction	

};

#endif

