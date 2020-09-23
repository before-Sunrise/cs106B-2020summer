#include "BankAccount.h"
#include "error.h"
using namespace std;

BankAccount::BankAccount(string name, double amount){
    if (amount < 0){
        error("That is an invalid amount of money!");
    }
    
    this->name = name;
    this->amount = amount;
}

void BankAccount::deposit(double depositAmount){
    if (depositAmount < 0){
        error("You can't deposit negative money!");
    }
    amount += depositAmount;
}

void BankAccount::withdraw(double withdrawlAmount){
    if (withdrawlAmount < 0){
        error("You can't withdraw negative money!");
    }
    if (withdrawlAmount > amount){
        error("Sorry, you don't have enough money in your account to do that!");
    }
    amount -= withdrawlAmount;
}

void BankAccount::transfer(double transferAmount, BankAccount& recipient){
    this->withdraw(transferAmount); // if we don't have enough money, this will error
    recipient.deposit(transferAmount);
}


/* These methods can be marked const because they don't modify the
 * contents of the bank account.
 */

double BankAccount::getAmount() const {
    return amount;
}

string BankAccount::getName() const {
    return name;
}
