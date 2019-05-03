#include "header.h"


/* Adds new apartment to apartment list*/
void add_To_LsApt(char* str, listApt* lsApt, unsigned short int code)
{
	int i = 0;
	ListNodeApt* newApt = (ListNodeApt*)malloc(sizeof(ListNodeApt));

	char del[2] = "\"";
	char* a = strtok(str + ONE, del);
	newApt->Aptaddress = part_from_str(a);

	sscanf(strtok(NULL, " "), "%d", &newApt->AptPrice);
	sscanf(strtok(NULL, " "), "%hd", &newApt->AptRooms);
	sscanf(strtok(NULL, " "), "%hd", &newApt->entryDate.day);
	sscanf(strtok(NULL, " "), "%hd", &newApt->entryDate.month);
	sscanf(strtok(NULL, " "), "%hd", &newApt->entryDate.year);


	currentDate(&(newApt->entryDataBase.day), &(newApt->entryDataBase.month), &(newApt->entryDataBase.year));

	newApt->entryDataBase.year -= 2000;
	newApt->AptCode = code;

	newApt->print = TRUE;

	location_in_the_list(lsApt, newApt);

}

////	sorts apartment list by price	////
void location_in_the_list(listApt* lsApt, ListNodeApt* newApt)
{
	ListNodeApt *temp1 = lsApt->head;
	if (temp1 == NULL)
	{
		SetOnlyListMember(lsApt, newApt);
		return;
	}
	else
	{
		while (temp1 != NULL)
		{
			if (newApt->AptPrice <= temp1->AptPrice)
			{
				if (temp1->prev == NULL)
				{
					lsApt->head = newApt;
				}

				LinkBefore(temp1, newApt);
				return;
			}
			temp1 = temp1->next;
		}
		LinkAfter(lsApt->tail, newApt);
		lsApt->tail = newApt;

	}
}


void LinkBefore(ListNodeApt* node, ListNodeApt* newNode)
{
	ListNodeApt* temp = node->prev;
	node->prev = newNode;

	if (NULL != temp)
		temp->next = newNode;

	newNode->prev = temp;
	newNode->next = node;

}


void LinkAfter(ListNodeApt* node, ListNodeApt* newNode)
{
	node->next = newNode;
	newNode->next = NULL;
	newNode->prev = node;
}


void SetOnlyListMember(listApt* lsApt, ListNodeApt* member)
{
	lsApt->head = member;
	lsApt->tail = member;
	member->next = NULL;
	member->prev = NULL;
}


/* recieves number of days back and builds a new date accordingly*/
NodeDate getDate(int days)
{
	NodeDate newDate;

	time_t currentDate;
	time(&currentDate);
	struct tm currentTime = *localtime(&currentDate);

	struct tm dateBack = currentTime;
	dateBack.tm_sec -= 60 * 60 * 24 * days;
	mktime(&dateBack);

	newDate.day = dateBack.tm_mday;
	newDate.month = dateBack.tm_mon + 1;
	newDate.year = dateBack.tm_year + 1900;

	return newDate;

}

////	get apartment to print till date entered	////
void checkDate(int date, listApt *lst)
{
	short int day;
	short int month;
	short int year;
	ListNodeApt *curr;

	curr = lst->tail;
	year = (date % 10000) - YEAR;
	date = date / 10000;
	month = date % 100;
	date = date / 100;
	day = date;

	while (curr != NULL)
	{
		if (curr->entryDate.year > year)
			printSwitchOFF(curr);
		else if (curr->entryDate.year == year && curr->entryDate.month > month)
			printSwitchOFF(curr);
		else if (curr->entryDate.year == year && curr->entryDate.month == month && curr->entryDate.day > day)
			printSwitchOFF(curr);

		curr = curr->prev;
	}
}


/* Finds current Date */
void currentDate(short int* day, short int* month, short int* year)
{
	time_t currentTime;
	time(&currentTime);

	struct tm *myTime = localtime(&currentTime);

	*day = myTime->tm_mday;
	*month = myTime->tm_mon + 1;
	*year = myTime->tm_year + 1900;
}


////	find the correct command by an index number entered	////
char* command_by_num(char** short_term_history, listCommands* listCommands, int num)
{
	int i = 0;
	listNodeCommands* temp;

	if (listCommands->tail == NULL) // if the list is empty
	{
		while (short_term_history[i] != NULL)
			i++;

		return short_term_history[i - num];
	}

	else
	{
		if (num > listCommands->tail->num) // if ther is list but the number is in the arr
			return (short_term_history[7 - (num - listCommands->tail->num)]);

		else // it in the list
		{
			temp = listCommands->head;

			while (temp != NULL)
			{
				if (temp->num == num)
					return temp->commandName;

				temp = temp->next;
			}
		}
	}
}


////  Recovery #5 change string entered to correct command by its index number	////
void command_to_change(char** short_term_history, listCommands* listCommands, listApt* listApt, char* firstStr, char* secondStr, int num, unsigned short int* theMaxCodeOfApt)
{
	char* commandToChange, *res;

	commandToChange = command_by_num(short_term_history, listCommands, num); 

	res = replace_Command(commandToChange, firstStr, secondStr);

	addToSave(res, short_term_history, listCommands);

	whatCommand(res, listApt, short_term_history, listCommands, theMaxCodeOfApt);

	free(res);
}

void last_command(char** short_term_history, listCommands* listCommands, listApt* listApt, unsigned short int* theMaxCodeOfApt)
{
	addToSave(short_term_history[0], short_term_history, listCommands);

	char* strForOneUse = (char*)malloc((strlen(short_term_history[0]) + 1) * sizeof(char));
	strcpy(strForOneUse, short_term_history[0]);

	whatCommand(strForOneUse, listApt, short_term_history, listCommands, theMaxCodeOfApt);
}


/*recovery #3*/
void short_history(listCommands *lst, char** str)
{
	int functionNum = 1;
	int i;

	if (str[N - 1] == NULL)
	{
		for (i = N - 1; i >= 0; i--)
		{
			if (str[i] != NULL)
				printf("%d: %s\n", functionNum++, str[i]);
		}
	}
	else if (lst->head == NULL)
	{
		for (i = N - 1; i >= 0; i--)
		{
			printf("%d: %s\n", functionNum++, str[i]);
		}
	}
	else
	{
		functionNum = lst->tail->num + 1;
		for (i = N - 1; i >= 0; i--)
		{
			printf("%d: %s\n", functionNum++, str[i]);
		}
	}

}


/*recovery #4*/
void history(listCommands *lst, char** str)
{
	int functionNum = 1;
	int i;
	if (str[N - 1] == NULL)
	{
		for (i = N - 1; i >= 0; i--)
		{
			if (str[i] != NULL)
				printf("%d: %s\n", functionNum++, str[i]);
		}
	}
	else if (lst->head == NULL)
	{
		for (i = N - 1; i >= 0; i--)
		{
			printf("%d: %s\n", functionNum++, str[i]);
		}
	}
	else
	{
		printCommandList(lst);
		functionNum = lst->tail->num + 1;
		for (i = N - 1; i >= 0; i--)
		{
			printf("%d: %s\n", functionNum++, str[i]);
		}
	}

}

////	recovery 2 - execues the command by number	////
void connective_to_command_by_num_for_Recovery2(char** short_term_history, listCommands* listCommands, listApt* listApt, int num, unsigned short int* theMaxCodeOfApt)
{
	char* resCommand = command_by_num(short_term_history, listCommands, num);
	addToSave(resCommand, short_term_history, listCommands);

	char* strForOneUse = (char*)malloc((strlen(resCommand) + 1) * sizeof(char));
	strcpy(strForOneUse, resCommand);

	whatCommand(strForOneUse, listApt, short_term_history, listCommands, theMaxCodeOfApt);

	free(strForOneUse);
}


char* part_from_str(char* str)
{
	int i = 0;
	char* res;

	strlen(str);

	res = (char*)calloc(strlen(str) + ONE, sizeof(char));


	while (str[i] != '\0')
	{
		res[i] = str[i];
		i++;
	}
	return res;
}


////	creates a new string for Rcovery 5	////
char* replace_Command(char* s, char* oldW, char* newW)
{
	char *result;
	int i, count = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	for (i = 0; s[i] != '\0'; i++)
	{
		if (strstr(&s[i], oldW) == &s[i])
		{
			count++;

			i += oldWlen - ONE;
		}
	}

	result = (char *)malloc(i + count * (newWlen - oldWlen) + ONE);

	i = 0;
	while (*s)
	{
		if (strstr(s, oldW) == s)
		{
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;
	}

	result[i] = '\0';

	return result;
}


/* specifies what recovery should the program be sent to*/
BOOL checkRecovery(char* str)
{
	char* resStr;
	char* saveCommend = (char*)malloc((strlen(str) + 1) * sizeof(char*));

	strcpy(saveCommend, str);

	resStr = strtok(saveCommend, "^");

	resStr = strtok(NULL, "^");

	free(saveCommend);

	if (!resStr) //if res is NULL
		return TRUE;
	else
		return FALSE;

}

