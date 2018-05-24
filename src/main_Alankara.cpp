#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Account_Alankara.h"
#include "stocksAccount_Alankara.h"
#include "bankAccount_Alankara.h"
#include "stock_node.h"

using namespace std;

//main function starts here
int main() {

	//user choices
	int choice_1 = 0;//choice for main menu
	int choice_2 = 0;//choice for stocks account menu
	int choice_3 = 0;//choice for bank account menu

	bankAccount_Alankara bAccount;		//creating an instance of bank account
	stocksAccount_Alankara *sAccount = new stocksAccount_Alankara;	//creating an instance of stocks account

	cout << "Welcome to the Account Management System." << endl;

	while(choice_1 != 3){

		cout << "Please select the account to access: " << endl;
		cout << "1. Stock Portfolio Account" << endl;
		cout << "2. Bank Account" << endl;
		cout << "3. Exit" << endl;

		cin >> choice_1;

		cout << "Option " << choice_1 << endl;

		switch (choice_1){

		case 1:
			cout << "Stock Portfolio Account" << endl;
			while(choice_2 != 7){
		
				cout << "1. Display the price for a Stock Symbol" << endl;
				cout << "2. Display Current Portfolio" << endl;
				cout << "3. Buy Shares" << endl;
				cout << "4. Sell Shares" << endl;
				cout << "5. View a Graph for the Portfolio Value" << endl;
				cout << "6. View Transaction History" << endl;
				cout << "7. Return to the Previous Menu" << endl;
				cout << "Enter Your Choice : ";

				cin >> choice_2;

				cout << "Option " << choice_2 << endl;
				
				switch (choice_2) {
				//display stock prices
				case 1:
					sAccount->displayStock();
					break;
				//display portfolio
				case 2:
					sAccount->displayPF();
					break;
				//to buy particular share
				case 3:
					sAccount->buyShares();
					break;
				//to sell particular share
				case 4:
					sAccount->sellShares();
					break;
				//plot a matlab graph
				case 5:
					sAccount->plotPF();
					break;
				//print transaction history
				case 6:
					sAccount->trans_history();
					break;
				//exit
				case 7:
					break;
				default:
					cout << "Please Enter a Valid Option!" << endl;
				
				}
			
			}
			choice_2 = 0;
			break;

		case 2:
			while(choice_3 != 5) {

				cout << "Bank Account" << endl << endl;
				cout << "Please select an option:" << endl;
				cout << "1. View Account Balance" << endl;
				cout << "2. Deposit Money" << endl;
				cout << "3. Withdraw Money" << endl;
				cout << "4. Print out History" << endl;
				cout << "5. Return to Previous Menu" << endl;
				cout << "Please Enter Your Choice : " << endl;
				cin >> choice_3;
				cout << "Option " << choice_3 << endl;

				switch (choice_3) {
				//display balance in the bank account
				case 1:
					bAccount.displayBalance();
					break;
				//to deposit amount
				case 2:
					double depositAmount;
					cout << "Please select the amount you wish to deposit: ";
					cin >> depositAmount;
					bAccount.depositCash(depositAmount);
					break;
				//to withdraw amount
				case 3:
					double withdrawalAmount;
					cout << "Please enter the amount to be withdrawn from the bank account: ";
					cin >> withdrawalAmount;
					bAccount.withdrawCash(withdrawalAmount);
					break;
				//to print transaction history
				case 4:
					bAccount.trans_history();
					break;
				//to previous menu
				case 5:
					break;
				default:
					cout << "Please Enter Valid Option!" << endl;
				
				}
			
			}
			choice_3 = 0;
			break;

		case 3:
			sAccount->storePFvalue();		//store portfolio value and corresponding timestamp
			sAccount->storePF();				//store portfolio
			break;

		default:
			cout << "Please Enter Valid Option!" << endl;
		
		}
	
	}
	return 0;//for succesfull termination of main
} //End of Main