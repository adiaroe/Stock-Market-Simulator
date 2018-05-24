#pragma once
#ifndef STOCK_NODE_H
#define STOCK_NODE_H

#include <iostream>
#include <string.h>
//#include "Account_Alankara.h"
#include "stocksAccount_Alankara.h"

using namespace std;

class Node {

	friend class stocksAccount_Alankara;

private:

	string symbol;		//stock symbol
	double priceOfShare;//share price
	int numOfShares;	//share count
	double currentPFval;//current portfolio value;
    double temPriceVal; //temporary variable

	Node *next;			//node's next pointer
	Node *prev;			//node's previous pointer

public:

	Node() {
		this->next = NULL;
		this->prev = NULL;
	}

};

#endif