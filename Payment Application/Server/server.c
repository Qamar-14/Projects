#ifndef _SERVERC_
#define _SERVERC_

#include "server.h"
#include <stdio.h>
#include <stdbool.h>

ST_accountsDB_t accountsDB[255] = {
		{  20000.0, "1234567891234567", },
		{  50000.0, "9872653461728839", },
		{  40000.0, "6873645738467382", },
		{  60000.0, "2013206333412314", },
};

ST_transaction_t trans[255];
static int index = 0;
static int transactionSequenceNumber = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData){
	if  (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	if  (isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	else transData->transState = APPROVED;
	//if (saveTransaction(transData) == SAVING_FAILED) return INTERNAL_SERVER_ERROR;
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData){
	//EN_serverError_t str;
	int i, j;
	//int count;
	bool valid= false;
	for (i=0;i<255;i++)
	{
		if (accountsDB[i].primaryAccountNumber[0] != '\0')
		{

			bool equal = true;
			//count =0;
			for (j=0;cardData->primaryAccountNumber[j]!='\0';j++)
			{
				if(accountsDB[i].primaryAccountNumber[j] != cardData->primaryAccountNumber[j])
				{
					equal = false;
					break;
				}
				//else count++;
			}
			if (equal) {
				valid = true;
				index = i;
				return OK__;
				break;
			}
			/*
			if (count>=16)
			{
				index = i;
				str = OK__;
				break;
			}*/
		}
		//else break;
	}
	return ACCOUNT_NOT_FOUND;

}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData){


	if (accountsDB[index].balance < termData->transAmount) return LOW_BALANCE;
	return OK;

}
EN_serverError_t saveTransaction(ST_transaction_t *transData){

	transactionSequenceNumber++;
	recieveTransactionData(transData);

	transData->terminalData.maxTransAmount = accountsDB[index].balance;
	accountsDB[index].balance -= transData->terminalData.transAmount;

	transData->transactionSequenceNumber =transactionSequenceNumber;

	if (transData->transState  == APPROVED)
	{
		trans[transactionSequenceNumber-1].cardHolderData = transData->cardHolderData;
		trans[transactionSequenceNumber-1].terminalData = transData->terminalData;
		trans[transactionSequenceNumber-1].transactionSequenceNumber =transData->transactionSequenceNumber;
		trans[transactionSequenceNumber-1].transState = APPROVED;

		return OK__;
	}
	else
	{
		transactionSequenceNumber--;
		return SAVING_FAILED;
	}
	if (transData->transState == DECLINED_STOLEN_CARD)
		printf("DECLINED STOLEN CARD");

	if (transData->transState == DECLINED_INSUFFECIENT_FUND)
		printf("DECLINED DUE TO INSUFFECIENT FUND");
}
EN_serverError_t printData(ST_transaction_t *transData){
	printf("--Transaction--\n");
	printf("\t-Card Holder Name: %s\n", transData->cardHolderData.cardHolderName);
	printf("\t-Primary Account Number : %s\n", transData->cardHolderData.primaryAccountNumber);
	printf("\t-Expiry date: %s\n", transData->cardHolderData.cardExpirationDate);
	printf("\t-Amount: %f\n", transData->terminalData.transAmount);
	printf("\t-Max Amount Allowed for Transaction: %f\n", transData->terminalData.maxTransAmount);
	printf("\t-Remaining Balance: %f\n", accountsDB[index].balance);

	printf("\t-Transaction State: ");


	if (transData->transState == APPROVED) printf("APPROVED\n");
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND) printf("DECLINED INSUFFECIENT FUND\n");
	else if (transData->transState == DECLINED_STOLEN_CARD) printf("DECLINED STOLEN CARD\n");

	printf("\t-Transaction Sequence Number: %d\n\n", transData->transactionSequenceNumber);
	return OK__;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
	if(transData->transactionSequenceNumber == transactionSequenceNumber)
		return OK__;
	else
		return TRANSACTION_NOT_FOUND;
}
#endif
