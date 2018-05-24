#pragma once

#ifndef BANKACCOUNT_ALANKARA_H
#define BANKACCOUNT_ALANKARA_H

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "Account_Alankara.h"

using namespace std;

class bankAccount_Alankara : public Account_Alankara{

private:

	double deposit, withdraw;			//variables for bank activity parameters
	
	//file accessing variables
	ofstream myfile;
	ofstream writeToBalance;
	ifstream myfile_read;


public:

	bankAccount_Alankara();				//ctor
	~bankAccount_Alankara();			//dtor

	virtual double getBalance();		//to get cash balance
	virtual void setBalance();			//to set cash balance
	void displayBalance();				//to display balance

	void depositCash(double);			//to deposit cash into the bank
	void withdrawCash(double);			//to withdraw cash from the bank

	virtual void trans_history();		//to print transaction history

};

#endif