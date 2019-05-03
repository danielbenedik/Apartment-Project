#include "header.h"

////   filters apartment by different data entered by users ////
void maximumPrice(int price, listApt *aptLst)
{
	ListNodeApt *curr;
	int currPrice;
	curr = aptLst->tail;

	while (curr != NULL)
	{
		currPrice = curr->AptPrice;
		if (price < currPrice)
		{
			printSwitchOFF(curr);
		}
		curr = curr->prev;
	}

}


void minimumNumRooms(int num, listApt *aptList)
{
	ListNodeApt *curr;

	int aptRoomNum;
	curr = aptList->tail;

	while (curr != NULL)
	{
		aptRoomNum = curr->AptRooms;
		if (num > aptRoomNum)
		{
			printSwitchOFF(curr);
		}
		curr = curr->prev;
	}
}


void maximumNumRooms(int num, listApt *lst)
{
	ListNodeApt *curr;
	int aptRoomNum;

	curr = lst->tail;

	while (curr != NULL)
	{
		aptRoomNum = curr->AptRooms;
		if (num < aptRoomNum)
		{
			printSwitchOFF(curr);
		}
		curr = curr->prev;
	}
}



/* Get an apartmet  - receives informartion wanted to be printed to screen and prints the information
of apartments the fit the  different descriptions by switching off the irrelevent print cursors to False
so when done going over entered command the function prints only apartment that their print cursor is True*/
void getApt(char *str, listApt *aptLst)
{
	char *command;
	int index = 0;
	int price;
	int minNumOfRooms;
	int maxNumOfRooms;
	int date;
	int daysBack;
	int if_printed = FALSE;
	int srtLength = strlen(str);


	command = strtok(str, " ");

	while (command != NULL)
	{
		if (!strcmp(command + 1, "MaximumPrice"))
		{
			price = charToNum(strtok(NULL, " "));
			maximumPrice(price, aptLst);
		}

		if (!strcmp(command + 1, "MinimumNumRooms"))
		{
			minNumOfRooms = charToNum(strtok(NULL, " "));
			minimumNumRooms(minNumOfRooms, aptLst);
		}
		if (!strcmp(command + 1, "MaximumNumRooms"))
		{
			maxNumOfRooms = charToNum(strtok(NULL, " "));
			maximumNumRooms(maxNumOfRooms, aptLst);
		}

		if (!strcmp(command + 1, "Date"))
		{
			date = charToNum(strtok(NULL, " "));
			checkDate(date, aptLst);

		}
		if (!strcmp(command + 1, "s"))
		{
			printLowToHighPrice(aptLst);
			if_printed = TRUE;
		}
		else if (!strcmp(command + 1, "sr"))
		{
			printHightToLow(aptLst);
			if_printed = TRUE;
		}
		if (!strcmp(command + 1, "Enter"))
		{
			daysBack = charToNum(strtok(NULL, " "));
			enter(aptLst, daysBack);
		}

		command = strtok(NULL, " ");
	}


	if (if_printed == FALSE)
		printLowToHighPrice(aptLst);

	SwitchON(aptLst); // reset print cursor of all apartments to TRUE

}

/*This function print the apartments that were entered in X amout of days*/
void enter(listApt *lst, int days)
{

	ListNodeApt *curr = lst->tail;

	NodeDate backDays = getDate(days);

	while (curr != NULL)
	{
		if (curr->entryDataBase.year < (backDays.year - 2000))
			printSwitchOFF(curr);
		else if (curr->entryDataBase.year == (backDays.year - 2000))
		{
			if (curr->entryDataBase.month < backDays.month)
				printSwitchOFF(curr);
			else if (curr->entryDataBase.month == backDays.month)
			{
				if (curr->entryDataBase.day < backDays.day)
					printSwitchOFF(curr);
			}
		}
		curr = curr->prev;
	}

}


/*This function buys an apartment and removes it from list by code received*/
void buyApt(char *str, listApt *lst)
{
	ListNodeApt *curr = lst->head;
	unsigned short int code = codeToNum(strtok(str, " "));

	while (curr != NULL)
	{
		if (curr->AptCode == code)
		{
			removeAptListNode(lst, curr);
			return;
		}
		curr = curr->next;
	}
}


/* Deletes the apartment the were entered to the list X days back */
void deleteApt(listApt *lst, char *str)
{
	SwitchON(lst);
	ListNodeApt *curr;
	curr = lst->head;
	int days = charToNum(str);

	NodeDate backDays = getDate(days);

	while (curr != NULL)
	{
		if (curr->entryDataBase.year < (backDays.year - 2000))
			printSwitchOFF(curr);
		else if (curr->entryDataBase.year == (backDays.year - 2000))
		{
			if (curr->entryDataBase.month < backDays.month)
				printSwitchOFF(curr);
			else if (curr->entryDataBase.month == backDays.month)
			{
				if (curr->entryDataBase.day < backDays.day)
					printSwitchOFF(curr);
			}
		}
		curr = curr->next;
	}

	sendNodeToRemove(lst);
}


void sendNodeToRemove(listApt *aptList)
{
	ListNodeApt *curr;
	curr = aptList->head;

	while (curr != NULL)
	{
		if (curr->print)
			removeAptListNode(aptList, curr);

		curr = curr->next;
	}

}


void printSwitchOFF(ListNodeApt *prnt)
{
	if ((prnt->print))
		prnt->print = FALSE;
}


void SwitchON(listApt *lst)
{
	ListNodeApt *curr;
	curr = lst->head;

	while (curr != NULL)
	{
		if (!(curr->print))
			curr->print = TRUE;
		curr = curr->next;
	}
}


/*print command List*/
void printCommandList(listCommands *lst)
{
	printCommandListRec(lst->head);
}


void  printCommandListRec(listNodeCommands *node)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		printf("%d: %s\n", node->num, node->commandName);
		printCommandListRec(node->next);
	}
}


/* print apartment list from high price to low price*/
void printHightToLow(listApt *lst)
{
	printHightToLowRec(lst->tail);
}


void printHightToLowRec(ListNodeApt *node)
{
	if (node == NULL)
	{
		printf("\n");
		return;
	}
	else if (node->print)
	{
		printf("Apt details:\n Code: %d \nAdress: %s\nNumber of rooms: %d \nPrice: %d \nEntry date: %d.%d.%d \nDatabase entry date: %d.%d.%d \n",
			node->AptCode, node->Aptaddress, node->AptRooms, node->AptPrice, node->entryDate.day, node->entryDate.month, node->entryDate.year+YEAR,
			node->entryDataBase.day, node->entryDataBase.month, node->entryDataBase.year+YEAR);
	}
	printHightToLowRec(node->prev);
}


/* print apartment list from low price to high price*/
void printLowToHighPrice(listApt *lst)
{
	printLowToHighPriceRec(lst->head);
}


void printLowToHighPriceRec(ListNodeApt *node)
{
	if (node == NULL)
	{
		printf("\n");
		return;
	}
	else if (node->print)
	{
		printf("Apt details:\nCode: %d \nAdress: %s\nNumber of rooms: %d \nPrice: %d \nEntry date: %d.%d.%d \nDatabase entry date: %d.%d.%d \n",
			node->AptCode, node->Aptaddress, node->AptRooms, node->AptPrice, node->entryDate.day, node->entryDate.month, node->entryDate.year+YEAR,
			node->entryDataBase.day, node->entryDataBase.month, node->entryDataBase.year+YEAR);
	}
	printLowToHighPriceRec(node->next);
}

