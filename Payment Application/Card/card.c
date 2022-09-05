#ifndef _CARDC_
#define _CARDC_


#include "card.h"
#include <stdio.h>
#include "math.h"
#include "stdint.h"
#include "ctype.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
	int i, chn=0;
	printf("Please enter the name written on your card:\n");
	fflush(stdout);
	gets((char*)cardData->cardHolderName);
	for(i=0;cardData->cardHolderName[i]!='\0';i++)
	{
		chn++;
	}
	if(( chn >24 )||(chn == 0 )||(20 > chn)) return WRONG_NAME;

	else
		return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	printf("Please enter card's primary account number:\n");
	fflush(stdout);
	int i = 0, pan=0;
	gets((char*)cardData->primaryAccountNumber);
	for(i=0;cardData->primaryAccountNumber[i]!='\0';i++)
	{
		pan++;
	}
	if(( pan >19 )||(pan == 0 )||(16 > pan))
		return WRONG_PAN;
	else
		return OK;
}


EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	int i = 0, ced = 0;
	printf("Please enter card expiry date in the format 'MM/YY', e.g '05/25':\n");
	fflush(stdout);
	gets((char*)cardData->cardExpirationDate);
	for(i=0; cardData->cardExpirationDate[i]!='\0';i++)
	{
		ced++;
	}
	fflush(stdout);

	if(( ced != 5 )||(ced == 0 ))
		return WRONG_EXP_DATE;
	else {
		for(i=0; cardData->cardExpirationDate[i]!='\0';i++)
		{
			if(i!=2)
			{
				if (!isdigit(cardData->cardExpirationDate[i]))
					return  WRONG_EXP_DATE;
			}
		}
	}
	return OK;

}
#endif



