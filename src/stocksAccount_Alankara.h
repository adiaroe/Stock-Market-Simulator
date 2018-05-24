#pragma once
#ifndef STOCKSACCOUNT_ALANKARA_H
#define STOCKSACCOUNT_ALANKARA_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "Account_Alankara.h"
#include "stock_node.h"

using namespace std;

class Node;

class stocksAccount_Alankara : 	public Account_Alankara{

private:

	//variables used for file interactions: Result_1.txt, Result_2.txt and PFvalue.txt
	ifstream txtfile1, txtfile2, myfile;
	string line, comp_symbol, comp_PF, PF_ent_time[200];
	stringstream stream_read;
	double priceOfStock, priceOfStock_PF;
	map<string, double> store_txt1, store_txt2;
	map<string, int> PF_value;
	ifstream PF;
	int rand_file;
	string PF_line;
	
	//nodes for implementing Double LinkedList
	Node *prev_ptr = NULL;
	Node *head;
	Node *tail;

	double PFprice, PFarr[200];

	int PF_size;	//portfolio size
	int PF_i;			//portfolio iterator

	ofstream writeToPF, writeToBalance;
	
	string stockSymbol;
	int shares;
	double maxToSpend, minPrice, price;

public:

	stocksAccount_Alankara();	//ctor
	~stocksAccount_Alankara();	//dtor

	void getprevPF();			//get the previously stored portfolio
	void getPFvalue();			//get portfolio value from the file
	void storePF();				//to store the current portfolio to the file again
	void storePFvalue();		//to store the portfolio value and the corresponding timestamp
	void displayPF();			//display present portfolio
	void plotPF();		//portfolio graph

	void displayStock();		//display stock properties

	virtual double getBalance();//get the calling objects' cash balance
	virtual void setBalance();  //set the clling objects' cash balance

	void buyShares();		  // BUY shares
	void sellShares();				  // SELL shares
	
	bool sortList();		    //sort the linked list

	virtual void trans_history();    //print stock history

};

#endif
