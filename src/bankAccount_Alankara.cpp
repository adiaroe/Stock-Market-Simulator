#include "bankAccount_Alankara.h"

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>

using namespace std;

//ctor
bankAccount_Alankara::bankAccount_Alankara(){

	setBalance();
	deposit = 0;
	withdraw = 0;

}

//dtor
bankAccount_Alankara::~bankAccount_Alankara(){
}

double bankAccount_Alankara::getBalance() {

	return cashBalance;

}

//to set the cash balance accordingly
void bankAccount_Alankara::setBalance() {

	streampos length;
	ifstream myfile1;
	//open the balance_file.txt file
	myfile1.open("balance_file.txt", ifstream::binary);
	if (myfile1.is_open()) {

		myfile1.seekg(0, ios::end);              // put console at end of file
		length = myfile1.tellg();				   // find the position of the cursor
		myfile1.close();                         // close file
	
	}
	//if nothing is found in the balance file then initialize it with the default value
	if (!length) {

		double defaultBalance = 10000.00;
		ofstream myfile2;
		myfile2.open("balance_file.txt", ios_base::out);
		myfile2 << defaultBalance;
		myfile2.close();

		ifstream myfile3("balance_file.txt", ios_base::in);
		myfile3 >> cashBalance;
		myfile3.close();
	
	}
	//if balance found in the file then read it and store it into the program session
	else {
	 
		ifstream myfile4;
		myfile4.open("balance_file.txt", ifstream::in);
		myfile4 >> cashBalance;
		myfile4.close();
	
	}

}

//display the most recently updated cash balance
void bankAccount_Alankara::displayBalance() {
	//set balance to the latest value
	setBalance();
	cout << "You have $" << getBalance() << " in your bank account." << endl;

}

//Deposit cash into your bank account
void bankAccount_Alankara::depositCash(double amount) {
	//get the amount and update the final cash balance
	//set balance to the latest value
	cashBalance = getBalance();
	deposit = amount;
	cashBalance = cashBalance + deposit;

	//generating the current date and timestamp
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
	string str(buffer);

	//logging in the deposit transaction into the bank_transaction_history.txt file
	myfile.open("bank_transaction_history.txt", ios::app);
	myfile << left << setw(30) << "Deposit";
	myfile << left << setw(1) << "$";
	myfile << left << setw(15) << deposit;
	myfile << right << setw(15) << str;
	myfile << right << setw(5) << "$" << cashBalance << endl;
	myfile.close();

	//log into the balance_file.txt
	writeToBalance.open("balance_file.txt", ios_base::out);
	writeToBalance << cashBalance;
	writeToBalance.close();
}

//withdraw cash from the bank account
void bankAccount_Alankara::withdrawCash(double amount) {
	//set balance to the latest value
	setBalance();
	cashBalance = getBalance();
	withdraw = amount;
	
	//continue withdrawal if cash balance is greater than zero
	if (cashBalance > 0) {
		//deny withdrawal if cash balance is less than withdrawal amount
		if (withdraw > cashBalance) {

			cout << "ERROR: Insufficient funds to withdraw cash!" << endl;
			return;
		
		}
		//sufficient funds
		else {

			cashBalance = cashBalance - withdraw;

			//generating the current date and timestamp
			time_t rawtime;
			struct tm timeinfo;
			char buffer[80];

			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);

			strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &timeinfo);
			string str(buffer);

			// write withdrawal transaction to file
			myfile.open("bank_transaction_history.txt", ios::app);
			myfile << left << setw(30) << "Withdrawal";
			myfile << left << setw(1) << "$";
			myfile << left << setw(15) << withdraw;
			myfile << right << setw(15) << str;
			myfile << right << setw(5) << "$" << cashBalance << endl;
			myfile.close();

			// write to a balance file
			writeToBalance.open("balance_file.txt", ios_base::out);
			writeToBalance << cashBalance;
			writeToBalance.close();

			return;
		
		}
	
	}
	//deny withdrawal if cash balance is equal to zero
	else {

		cout << "ERROR: Account is empty.";
	
	}

}

//to print transaction history
void bankAccount_Alankara::trans_history() {

	myfile_read.open("bank_transaction_history.txt", ios::app);
	cout << left << setw(30) << "Transaction";
	cout << left << setw(16) << "Amount";
	cout << left << setw(23) << "Timestamp";
	cout << right << setw(5) << "Balance" << endl;
	
	string line;
	if (myfile_read.is_open()) {
		//iterates until the end of file is reached
		while (!myfile_read.eof()) {

			while (getline(myfile_read, line))
				cout << line << endl;
		
		}
	
	}
	myfile_read.close();

}

