#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
#include <sstream>
#include <time.h>
#include <iomanip>
#include "stocksAccount_Alankara.h"
#include "stock_node.h"

// Library files for linking to MATLAB 
#pragma comment( lib, "libmx.lib" )	
#pragma comment( lib, "libmex.lib" )
#pragma comment( lib, "libeng.lib" )
#include <yvals.h>
#include <engine.h>
#if (_MSC_VER >= 1600)
#define __STDC_UTF_16__
#endif
#include "mex.h"


using namespace std;

//defining ctor
stocksAccount_Alankara::stocksAccount_Alankara(){
	
	//retrieving the given text file containing company symbols along with their stock values
	txtfile1.open("Result_1.txt");
	txtfile2.open("Result_2.txt");

	if (txtfile1.is_open() && txtfile2.is_open()){
		//run until the 'end of file' is not read
		while (!txtfile1.eof() && !txtfile2.eof()){
			//Result_1.txt
			getline(txtfile1, line);											//iteratively read lines from the Result_1.txt file
			stream_read << line;												//each line is stored in the stream
			stream_read >> comp_symbol >> priceOfStock;							//separating separate values into corresponding variables
			store_txt1.insert(pair<string, double>(comp_symbol, priceOfStock));	//inseting into a 2-D map
			stream_read.str("");					
			stream_read.clear();												
			//Now Result_2.txt
			getline(txtfile2, line);											//iteratively read lines from the Result_2.txt file
			stream_read << line;												//each line is stored in the stream 
			stream_read >> comp_symbol >> priceOfStock;							//separating values into corresponding variables
			store_txt2.insert(pair<string, double>(comp_symbol, priceOfStock));	//inseting into a 2-D map
			stream_read.str("");					
			stream_read.clear();													

		}

	}
	txtfile1.close();
	txtfile2.close();

	myfile.open("list_size.txt");	//latest portfolio file opened
	myfile >> PF_size;
	if (PF_size != 0)
		this->getprevPF(); 			//get values of previous portfolio
	myfile.close();
	this->getPFvalue();				//retrieval for plotting purpose

}

//declaring dtor
stocksAccount_Alankara::~stocksAccount_Alankara(){
}

//get the values from the previously stored portfolio file
void stocksAccount_Alankara::getprevPF() {

	PF.open("PF_file.txt");
	if (PF.is_open()) {
		//run until the 'end of file' is not read
		while (!PF.eof()) {

			getline(PF, line);												//iteratively read lines from the PF.txt file
			stream_read << line;											//each line is stored in the stream
			stream_read >> comp_PF >> priceOfStock_PF;						//separating values into corresponding variables
			PF_value.insert(pair<string, int>(comp_PF, priceOfStock_PF));	//inseting into a 2-D map
			stream_read.clear();

		}

	}
	PF.close();

	for (map<string, int>::iterator j = PF_value.begin(); j != PF_value.end(); ++j) { //iteratively going through the data structure
																					  //new node is declared for every iteration
		Node *present = new Node();
		present->symbol = j->first;
		present->numOfShares = j->second;

		//changes committed to the list
		if (prev_ptr != NULL) {

			present->prev = prev_ptr;
			prev_ptr->next = present;

		}
		else {

			present->prev = NULL;
			present->next = NULL;
			head = present;
			tail = present;

		}
		prev_ptr = present;
		tail = prev_ptr;

	}
	//creating a present interator node to compare with the argument node
	Node *prsnt = head;
	while (prsnt != NULL) {

		rand_file = rand() % 2;
		//considering Result_1.txt
		if (rand_file == 0) {

			for (map<string, double>::iterator i = store_txt1.begin(); i != store_txt1.end(); i++) {

				if (prsnt->symbol == i->first) {

					prsnt->temPriceVal = i->second;								 //retrieving that particular share price value
					prsnt->currentPFval = prsnt->numOfShares*prsnt->temPriceVal; // calculate total portfolio price value of this temporary node

				}

			}

		}
		//Now considering Result_2.txt
		else if (rand_file == 1) {

			for (map<string, double>::iterator j = store_txt2.begin(); j != store_txt2.end(); j++) {

				if (prsnt->symbol == j->first) {

					prsnt->temPriceVal = j->second;
					prsnt->currentPFval = prsnt->numOfShares*prsnt->temPriceVal;

				}

			}

		}
		PFprice = PFprice + prsnt->currentPFval;	//total potfolio price value
		prsnt = prsnt->next;						//now considering the next node
	}
	PFprice += cashBalance;							//update cash balance to the portfolio

}

//retrieving portfolio price value
void stocksAccount_Alankara::getPFvalue() {

	string var;//temp string for portfolio file
	
	myfile.open("PFvalue.txt");

	if (myfile.is_open()){

		while (!myfile.eof()){

			getline(myfile, PF_line);
			stream_read << PF_line;
			stream_read >> PFarr[PF_i] >> PF_ent_time[PF_i] >> var; //entering data into Portfolio array (PFarr) from each line from PFvalue.txt file
			if (PF_ent_time[PF_i] != ""){							// retrieve corresponding date in time_stamp
				//update the current time
				PF_ent_time[PF_i].append(" ");
				PF_ent_time[PF_i].append(var);						
			
			}
			PF_i++; // increment counter for both arrays.
			stream_read.clear();
		
		}
	
	}
	myfile.close();

}

void stocksAccount_Alankara::storePF(){

	Node *tempFile = head;
	writeToPF.open("PF_file.txt", ios_base::out);
	while (tempFile != NULL){

		writeToPF << tempFile->symbol << "\t" << tempFile->numOfShares << endl;
		tempFile = tempFile->next;

	}
	writeToPF.close();
	
	//store size after updation of portfolio file
	writeToPF.open("PF_file.txt");
	writeToPF << PF_size;
	writeToPF.close();

}

void stocksAccount_Alankara::storePFvalue(){

	//generating the current date and timestamp
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
	string str(buffer);

	if (PF_size == 0)
		PFprice = cashBalance;						//if portfolio file is empty, update it with the cash balance
	writeToPF.open("PFvalue.txt.", ios::app);
	writeToPF << PFprice << "  " << str << endl;
	writeToPF.close();

}

void stocksAccount_Alankara::displayPF()
{
	Node *prsnt = head;							//a temporary node for linkedlist traversal
	setBalance();								//making sure if latest value is set or not
	double stockValue = 0;						//initialise stock value
	double cashbalance = getBalance();			//initialise cash values

												//creating a present interator node to compare with the argument node
	while (prsnt != NULL) {

		rand_file = rand() % 2;
		//considering Result_1.txt
		if (rand_file == 0) {

			for (map<string, double>::iterator i = store_txt1.begin(); i != store_txt1.end(); i++) {

				if (prsnt->symbol == i->first) {

					prsnt->temPriceVal = i->second;								 //retrieving that particular share price value
					prsnt->currentPFval = prsnt->numOfShares*prsnt->temPriceVal; // calculate total portfolio price value of this temporary node

				}

			}

		}
		//Now considering Result_2.txt
		else if (rand_file == 1) {

			for (map<string, double>::iterator j = store_txt2.begin(); j != store_txt2.end(); j++) {

				if (prsnt->symbol == j->first) {

					prsnt->temPriceVal = j->second;
					prsnt->currentPFval = prsnt->numOfShares*prsnt->temPriceVal;

				}

			}

		}
		prsnt = prsnt->next;						//now considering the next node
	
	}
		
	this->sortList();												//sort the stock values

	if (PF_size > 0){

		cout << left << setw(20) << "Symbol";
		cout << left << setw(20) << "Shares";
		cout << left << setw(20) << "Price/Share($)";
		cout << left << setw(20) << "Total Value($)" << endl;
		
		prsnt = head;												//temp pointer for iterations

		while (prsnt != NULL)
		{
			cout << left << setw(20) << prsnt->symbol << endl;		//print company symbol
			cout << left << setw(20) << prsnt->numOfShares << endl;	//number of shares
			cout << left << setw(20) << prsnt->temPriceVal << endl;	//price per share
			cout << left << setw(20) << prsnt->currentPFval << endl;//value of the stock

			stockValue = stockValue + prsnt->currentPFval;			//store the value of stock
			prsnt = prsnt->next;									//iterate to next node
		}
		PFprice = cashbalance + stockValue;							//calculate the total value of portfolio
		cout << "Cash Balance  : $" << cashbalance << endl;
		cout << "Stock Balance : $" << stockValue << endl;
		cout << "Total Portfolio value is : $" << PFprice << endl; 

		PFarr[PF_i] = PFprice;										
		PF_i++;

	}

	else if (PF_size == 0){
	
		PFprice = cashbalance;
		cout << "No shares available to display." << endl;
	
	}

}

//get the calling objects' cash balance
double stocksAccount_Alankara::getBalance(){

	return cashBalance;

}

//set the calling objects' cash balance
void stocksAccount_Alankara::setBalance(){

	streampos length;
	myfile.open("balance_file.txt", ifstream::binary);
	if (myfile.is_open()){
		//get length of the file
		myfile.seekg(0, ifstream::end);
		length = myfile.tellg();
		myfile.close();
	
	}

	//found the latest cash balance in the file to update
	if (length){

		ifstream file1;
		file1.open("balance_file.txt", ifstream::in);
		file1 >> cashBalance;
		file1.close();
	
	}

	//if nothing is found update the file to carry an initial cash balance of $10,000
	else{

		double init_cashBalance = 10000.00;

		ofstream file2;										// output file
		file2.open("balance_file.txt", ofstream::out);
		file2 << init_cashBalance;
		file2.close();

		ifstream file3;										//open the balance sheet and get the cash balance
		file3.open("balance_file.txt", ifstream::in);
		file3 >> cashBalance;
		file3.close();
		
	}

}

void stocksAccount_Alankara::displayStock() {

	string stockSymbol;
	
	cout << "Enter Stock Symbol : " << endl;
	cin >> stockSymbol;
	
	bool flg = false;

	rand_file = rand() % 2;

	//considering Result_1.txt
	if (rand_file == 0){
	
		// iterating through map
		for (map<string, double>::iterator i = store_txt1.begin(); i != store_txt1.end(); i++)
		{
			// if symbol is found then display its stock price
			if (stockSymbol == i->first){		// compare the first stored value
				
				// print all the parameters
				cout << left << setw(10) << "Symbol";
				cout << left << setw(10) << "Price/Share" << endl;
				cout << left << setw(10) << i->first;
				cout << left << setw(10) << i->second << endl;
				flg = true;
			
			}
		
		}
		if (flg == false)
			cout << "Company Symbol not found in database!" << endl;
		return;
	}
	//now considering Result_2.txt
	else if (rand_file == 1){
	
		for (map<string, double>::iterator i = store_txt2.begin(); i != store_txt2.end(); i++){
			
			// if symbol is found then display its stock price
			if (stockSymbol == i->first){
			
				cout << left << setw(10) << "Symbol";
				cout << left << setw(10) << "Price/Share" << endl;
				cout << left << setw(10) << i->first;
				cout << left << setw(10) << i->second << endl;
				flg = true;
			
			}
		
		}

		//if company symbol not found
		if (flg == false)
			cout << "Company Symbol not found!\n";
		return;
	}

}

//buy shares
void stocksAccount_Alankara::buyShares(){

	cashBalance = getBalance();
	double priceAmount;
	cout << "Please enter the stock symbol you wish to purchase: ";
	cin >> stockSymbol;
	setBalance();
	bool flg2 = false, flg3 = false, flg4 = false, flg5 = false;
	double shareVal1, shareVal2;

	for (map<string, double>::iterator j = store_txt1.begin(); j != store_txt1.end(); j++){ // check in container if symbol present
	
		if (stockSymbol == j->first){

			shareVal1 = j->second;
			flg4 = true;
		
		}
	
	}

	for (map<string, double>::iterator j = store_txt2.begin(); j != store_txt2.end(); j++){

		if (stockSymbol == j->first){

			flg5 = true;
			shareVal2 = j->second;
		
		}
	
	}
	//If symbol entered is found
	if (flg4 == true || flg5 == true) {

		cout << "Please enter the number of shares : " << endl;
		cin >> shares;
		cout << "Please enter the maximum amount you are willing to pay per share: " << endl;
		cin >> maxToSpend;

		if (flg4 == true && flg5 == true) {
			//if the symbol found in both the files then choose any file randomly
			rand_file = rand() % 2;
			//if file 1 is chosen
			if (rand_file == 0) {

				if (maxToSpend >= shareVal1){ // check if value entered is more than expected
				
					priceAmount = shares * shareVal1;
					price = shareVal1;
				
				}
				else{

					cout << "ERROR: Maximum limit exceeded to buy the share!\n";
					return;
				
				}

			}
			//if file 2 is chosen
			if (rand_file == 1) {

				if (maxToSpend >= shareVal2) { // check if value entered is more than expected

					priceAmount = shares * shareVal2;
					price = shareVal2;

				}
				else {

					cout << "ERROR: Maximum limit exceeded to buy the share(s)!\n";
					return;

				}

			}

		}
		//if symbol only found in the 1st file
		if (flg4 == true && flg5 == false) {

			if (maxToSpend >= shareVal1) { // check if value entered is more than expected

				priceAmount = shares * shareVal1;
				price = shareVal1;

			}
			else {

				cout << "ERROR: Maximum limit exceeded to buy the share(s)!\n";
				return;

			}

		}
		//if symbol only found in the 2nd file
		if (flg4 == false && flg5 == true) {

			if (maxToSpend >= shareVal2) { // check if value entered is more than expected

				priceAmount = shares * shareVal2;
				price = shareVal2;

			}
			else {

				cout << "ERROR: Maximum limit exceeded to buy the share(s)!\n";
				return;

			}

		}
	
	}
	else {

		cout << "ERROR: The Symbol entered not found in the database available." << endl;
		return;

	}

	//check if fund suffice
	if (priceAmount > cashBalance) {

		cout << "Insufficient Funds!\a" << endl;
		return;
	
	}
	//if enough funds found, update the txt files and the linkedlist
	else {
		
		cashBalance = cashBalance - priceAmount;
		cout << "You have purchased " << shares << " shares of " << stockSymbol << " at " << price << " each for a total of $" << priceAmount;

		//write to a balance txt file.
		writeToBalance.open("balance_file.txt", ofstream::out);
		writeToBalance << cashBalance;
		writeToBalance.close();

		//store the current timestamp along with the purchase details for transaction history
		//generating the current date and timestamp
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
		string str(buffer);

		//write credit transaction to bank_transaction_history.txt file
		writeToBalance.open("bank_transaction_history.txt", ios::app);
		writeToBalance << left << setw(20) << "Credited to Stock Account.";
		writeToBalance << left << setw(1) << "$";
		writeToBalance << left << setw(15) << priceAmount;
		writeToBalance << right << setw(15) << str;
		writeToBalance << right << setw(5) << "$" << cashBalance << endl;
		writeToBalance.close();

		//write credit transaction to stock_transaction_history.txt file
		writeToBalance.open("stock_transaction_history.txt", ios::app);
		writeToBalance << left << setw(6) << "Buy";
		writeToBalance << left << setw(15) << stockSymbol;
		writeToBalance << left << setw(6) << shares;
		writeToBalance << left << setw(15) << price;
		writeToBalance << left << setw(10) << priceAmount;
		writeToBalance << left << setw(15) << str << endl;
		writeToBalance.close();
		//if list empty
		if (head == NULL && tail == NULL) {

			Node *n = NULL;
			//initializing head and tail values to the new node
			head = n;
			tail = n;

			n->symbol = stockSymbol;
			n->numOfShares = shares;
			n->priceOfShare = price;
		
		}
		//if entry of the recently bought symbol found in the list then update it
		else {
			
			Node *prsnt = head;
			while (prsnt != NULL) {
				//update share number 
				if (stockSymbol == prsnt->symbol) {

					prsnt->numOfShares = prsnt->numOfShares + shares;
					return;
				
				}
				prsnt = prsnt->next;
			
			}
			// add new node if share is not already present in the list
			Node *n = NULL, *temp = tail;
			tail->next = n;
			tail = n;
			tail->prev = temp;
			tail->symbol = stockSymbol;
			tail->numOfShares = shares;
			tail->priceOfShare = price;
		
		}
		PF_size++;
	
	}

}

//sell shares
void stocksAccount_Alankara::sellShares() {
	
	setBalance();
	cashBalance = getBalance();	

	cout << "Please enter the stock symbol you intend to sell: ";
	cin >> stockSymbol;

	bool flg1 = false, flg3 = false;

	Node *present = head;

	while (present != NULL){
		//check whether the portfolio has the symbol
		if (stockSymbol == present->symbol) {

			flg1 = true;
			break;
		
		}
		present = present->next;
	
	}

	//if symbol not present
	if (flg1 == false) {

		cout << "ERROR: The desired symbol is not available in the portfolio to sell!" << endl;
		return;
	
	}
	
	//if symbol present
	
	cout << "Please enter the number of shares of the symbol you intend to sell: ";
	cin >> shares;

	cout << "Please enter the minimum price at which you intend to sell: ";
	cin >> minPrice;

	rand_file = rand() % 2;

	if (PF_size > 0) {

		//initializing temporary node for traversal
		Node *prsnt = head;  

		while (prsnt != NULL) {
			//if the symbol found in the list and the corresponding number of shares are equal or greater than the number of shares intended to sell
			if (stockSymbol == prsnt->symbol && shares <= prsnt->numOfShares) {
			
				flg3 = true;
				prsnt->numOfShares = prsnt->numOfShares - shares;

				//price value from the Result_1.txt file
				if (rand_file == 0) {
				
					for (map<string, double>::iterator j = store_txt1.begin(); j != store_txt1.end(); j++) {

						if (stockSymbol == j->first) {
							//If the price per stock is lower than the amount the user is willing to sell for
							if (minPrice > (j->second)) {
								 
								cout << "ERROR: The price is lower than the current stock price hence it is not advisible to sell you shares." << endl;
								return;
							
							}
							price = j->second;
							break;
						
						}
					
					}
				
				}
				//price value from the Result_2.txt file
				else if (rand_file == 1) {

					for (map<string, double>::iterator j = store_txt2.begin(); j != store_txt2.end(); j++)
					{

						if (stockSymbol == j->first) {
							//If the price per stock is lower than the amount the user is willing to sell for
							if (minPrice > (j->second)) {

								cout << "ERROR: The price is lower than the current stock price hence it is not advisible to sell you shares." << endl;
								return;

							}
							price = j->second;
							break;
						}
					
					}
				
				}

				cout << "You sold " << shares << " shares of " << stockSymbol << " at rate of " << price << " per share with total amount of " << (shares*price) << endl;
				return;

				//generating the current date and timestamp
				time_t rawtime;
				struct tm timeinfo;
				char buffer[80];

				time(&rawtime);
				localtime_s(&timeinfo, &rawtime);

				strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
				string str(buffer);

				//Update the cash balance
				cashBalance = cashBalance + (shares*price);

				//adding the deposit transaction to bank_transaction_history.txt file
				writeToBalance.open("bank_transaction_history.txt", ios::app);
				writeToBalance << left << setw(30) << "Credited from Stock Account.";
				writeToBalance << left << setw(1) << "$";
				writeToBalance << left << setw(15) << shares * price;
				writeToBalance << right << setw(15) << str;
				writeToBalance << right << setw(5) << "$" << cashBalance << endl;
				writeToBalance.close();

				// write transaction to stock_transaction_history.txt file
				writeToBalance.open("stock_transaction_history.txt", ios::app);
				writeToBalance << left << setw(6) << "Sell";
				writeToBalance << left << setw(15) << stockSymbol;
				writeToBalance << left << setw(6) << shares;
				writeToBalance << left << setw(15) << price;
				writeToBalance << left << setw(10) << shares * price;
				writeToBalance << left << setw(15) << str << endl;
				writeToBalance.close();

				//update the balance sheet of that particular session
				writeToBalance.open("balance_file.txt", ios_base::out);
				writeToBalance << cashBalance;
				writeToBalance.close();

				//if shares of that particular symbol remains zero after selling then delete the node
				if (prsnt->numOfShares == 0) {
					//if head is to be deleted
					if (prsnt == head) {

						if (prsnt == tail) {

							head = NULL;
							tail = NULL;
							delete prsnt;
							PF_size--;
							return;

						}
						//passing the head to the next node
						Node *temp1 = prsnt->next;
						head = prsnt->next;
						temp1->prev = NULL;
						delete prsnt;
						PF_size--;
						return;
					
					}
					//if tail is to be deleted
					else if (prsnt == tail) {

						if (prsnt == head) {

							head = NULL;
							tail = NULL;
							delete prsnt;
							PF_size--;
							return;
						
						}
						//passing the tail to the previous node
						Node *temp2 = prsnt->prev;
						tail = prsnt->prev;
						temp2->next = NULL;
						delete prsnt;
						PF_size--;
						return;
					
					}
					//if node in between head and tail is to be deleted
					else {

						Node *temp3 = prsnt->prev;
						Node *temp4 = prsnt->next;
						temp3->next = prsnt->next;
						temp4->prev = prsnt->prev;
						delete prsnt;                     // delete current node.
						PF_size--;                         // update the size.
						return;
					
					}
				
				}
				
			}
			//search futher
			prsnt = prsnt->next;
		}
	}
	if (flg3 == false)	cout << "\n Number of shares out of range!\n";

}

//sorting the linked list using nodes
bool stocksAccount_Alankara::sortList() {
	//temporary head pointer for traversal
	Node *temp1 = head; 
	
	int coun = 0;
	//run the while loop till the head is null
	while (temp1 != NULL){
	
		rand_file = rand() % 2;
		
		//for Result_1.txt
		if (rand_file == 0) {

			for (map<string, double>::iterator j = store_txt1.begin(); j != store_txt1.end(); j++) {

				if (temp1->symbol == j->first) {

					temp1->temPriceVal = j->second;
					temp1->currentPFval = temp1->numOfShares*temp1->temPriceVal;
				
				}
			
			}
		
		}
		//now for Result_2.txt
		else if (rand_file == 1) {

			for (map<string, double>::iterator j = store_txt2.begin(); j != store_txt2.end(); j++) {

				if (temp1->symbol == j->first) {

					temp1->temPriceVal = j->second;
					temp1->currentPFval = temp1->numOfShares*temp1->temPriceVal;
				
				}
			
			}
		
		}
		temp1 = temp1->next;
	
	}
	
	//temporary head pointer for traversal
	Node *temp2 = head;

	if (head != NULL) {
	
		Node *prev = 0;
		Node *temp = NULL;
		int swapFlag = 0;

		//bubble sort on double linkedlist
		for (int i = 0; i < PF_size; i++) {
			//traverse until tail is reached
			while (temp2->next != 0) {
				//store next pointer to a temporary node
				temp = temp2->next;
				//swap if the value is greater than the subsequent
				if (temp2->currentPFval < temp->currentPFval) {

					temp->prev = temp2->prev;
					temp2->prev = temp;
					temp2->next = temp->next;
					swapFlag = 1;

					if (temp2->next != NULL)	temp2->next->prev = temp2;
					temp->next = temp2;   //now after swapping the current temp pointer must point to next node
					
					//now changing the current pointer according to some previous pointer conditions
					if (head == temp2)
						head = temp;
					if (temp2->next == 0)
						tail = temp2;
					if (prev != 0)				 
						prev->next = temp;
					prev = temp; 
					
				}
				else {
					//if there was no swapping then prev is made to point the current's next
					prev = temp2;
					temp2 = temp2->next; //moveing to the subsequent node
				
				}
			
			}
			if (swapFlag == 0) //no sorting required
				break;
			else {

				swapFlag = 0;
				prev = 0;
				temp2 = head;
			
			}
		
		}
	
	}
	//empty list
	else {
		
		cout << "ERROR: List is Empty!" << endl;
		return false;
	
	}
	return true;

}

//shows portfolio value variation using a graph plotted using MATLAB
void stocksAccount_Alankara::plotPF() {
	//engine pointer
	Engine *N;
	N = engOpen(NULL);
	//error in opening matlab engine
	if (N == NULL) {
		
		cout << "Error!!!" << endl;
		exit(1);
	
	}
	/*cout << "Potfolio Value" << setw(15) << "Date & Time" << endl;
	for (int i = 0; i < count; i++)
	{
	cout << port_array[i] << setw(30) << time_stamp[i] << endl;
	}*/
	// create mxArray and copy your c++ double array of portfolio value into it.
	cout << "Plotting MATLAB graph. Please wait..." << endl;
	mxArray *M;
	M = mxCreateDoubleMatrix(1, PF_i, mxREAL); // creat a matrix
	memcpy((void *)mxGetPr(M), (void *)PFarr, PF_i * sizeof(double)); // allocate memory space
	engPutVariable(N, "set", M);
	engEvalString(N, "plot(set);"); // plot the value
	engEvalString(N, "title('Change In Portfolio Value');");
	mxDestroyArray(M);// deallocate matrix M
}

//to print transasction history
void stocksAccount_Alankara::trans_history() {

	cout << left << setw(6) << "Transaction";
	cout << left << setw(15) << "Symbol";
	cout << left << setw(6) << "Shares";
	cout << left << setw(15) << "Price per Share($)";
	cout << left << setw(10) << "Value($)";
	cout << left << setw(15) << "Timestamp" << endl;

	myfile.open("stock_transaction_history.txt", ios::app); // open history file
	if (myfile.is_open()) {

		while (!myfile.eof()) {

			while (getline(myfile, PF_line))
				cout << PF_line << endl;
		}
	}
	myfile.close();  // close the file
}