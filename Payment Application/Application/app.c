#ifndef _APP_C_

#define _APP_C_
#include "../Application/app.h"

void appStart(void){
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	ST_transaction_t transData;
	while (getCardHolderName(&cardData) == WRONG_NAME) {
		printf("\n\t<<<WRONG NAME>>>\n");
		printf("Re-enter:\n");

	}
	while (getCardPAN(&cardData) == WRONG_PAN) {
		printf("\n\t<<<WRONG PAN>>>\n");
		printf("Re-enter:\n");
	}
	while(getCardExpiryDate(&cardData) == WRONG_EXP_DATE) {
		printf("\n\t<<<WRONG EXPIRE DATE>>>\n");
		printf("Re-enter:\n");

	}
	while(getTransactionDate(&termData) == WRONG_DATE) {
		printf("\n\t<<<WRONG DATE>>>\n");
		printf("Re-enter:\n");
	}
	if (isCardExpired(cardData, termData) == EXPIRED_CARD) {
		printf("\n\n\t<<<DECLINED EXPIRED CARD>>>\n");
	}
	if (isCardExpired(cardData, termData) == OK_)
	{
		setMaxAmount(&termData);//set maximum amount allowed for transaction

		//If card is not expired check if amount is valid
		if (getTransactionAmount(&termData) == INVALID_AMOUNT)
		{
			printf("\n\t<<<DECLINED AMOUNT EXCEEDING LIMIT>>>\n");
		}
		else if (isBelowMaxAmount(&termData)==EXCEED_MAX_AMOUNT) //transaction amount exceeds limit
		{
			printf("\n\t<<<EXCEED MAX AMOUNT>>>\n");
		}
		else{
			transData.cardHolderData = cardData;
			transData.terminalData = termData;
			transData.transState=recieveTransactionData(&transData);
			//Check if account is valid
			if (isValidAccount(&cardData) == ACCOUNT_NOT_FOUND )
			{
				printf("\n\t<<<DECLINED INAVLID ACCOUNT>>>\n");
			}
			//account is valid
			else{
				//Check if the amount is available
				if(isAmountAvailable(&termData)==LOW_BALANCE)
				{
					printf("\n\t<<<DECLINED INSUFFECIENT FUNDS>>>\n");

				}
				else
				{
					saveTransaction(&transData);
					printData(&transData);
				}
			}
		}
	}
}

#endif
