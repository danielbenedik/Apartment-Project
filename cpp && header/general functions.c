#include "header.h"


/*recieves input from user*/
char* getStr()
{
	char* str;
	char c;
	int pizSize = 20, strSize = 0;

	str = (char*)calloc(pizSize, sizeof(char));

	c = getchar();

	while (c != '\n')
	{
		str[strSize] = c;
		strSize++;

		if (pizSize == strSize)
		{
			pizSize *= 2;
			str = (char*)realloc(str, pizSize * sizeof(char));
		}
		c = getchar();

	}

	str = (char*)realloc(str, (strSize + ONE) * sizeof(char));
	str[strSize] = '\0';

	return str;
}


void addToSave(char* str, char** short_term_history, listCommands* lstCommand)
{
	int len, count = 0;
	len = strlen(str);

	if (short_term_history[N - 1] != NULL)// if the array is full
	{
		listNodeCommands* newNodCommand = (listNodeCommands*)malloc(sizeof(listNodeCommands));
		newNodCommand->commandName = short_term_history[6];// save the oldest command in a new node

		insertNodeToTail(lstCommand, newNodCommand); //insert new command node to command list
		for (int i = N - 1; i >= ONE; i--) //moving the array data one tray right
		{
			short_term_history[i] = short_term_history[i - 1];
		}

		short_term_history[0] = (char*)malloc((len + ONE) * sizeof(char)); // insert the latest command entered to the first place in command array

		strcpy(short_term_history[0], str);
	}
	else
	{
		if (short_term_history[0] == NULL)
		{
			short_term_history[0] = (char*)malloc((len + ONE) * sizeof(char));
			strcpy(short_term_history[0], str);
		}
		else
		{
			for (int i = 0; i < N; i++)// checks how many command are in the command array
				if (short_term_history[i] != NULL)
					count++;

			for (int i = count - 1; i >= 0; i--)
				short_term_history[i + 1] = short_term_history[i];

			short_term_history[0] = (char*)malloc((len + ONE) * sizeof(char));
			strcpy(short_term_history[0], str);
		}
	}
}

/*This function inquires what command was entered and leads the program in that direction*/
void whatCommand(char* str, listApt *lstApt, char ** short_term_history, listCommands *listcommand, unsigned short int *theMaxCodeOfApt)// שינוי
{
	int lenToKeep = 1, numCommandRes;

	char *strToNum, *commandRes, *specifiedCommand, *strForSend;

	specifiedCommand = strtok(str, "-");
	commandRes = strtok(NULL, " ");

	if (str[0] != '!' && str[0] != 'h' && str[0] != 's')
		lenToKeep += strlen(commandRes);

	/////find a the currect command//////
	if (!(strcmp(specifiedCommand, "get"))) 
	{
		getApt(commandRes + lenToKeep, lstApt);
	}
	else if (!(strcmp(specifiedCommand, "add")))
	{
		(*theMaxCodeOfApt)++;
		add_To_LsApt(commandRes + lenToKeep, lstApt, *theMaxCodeOfApt);

	}
	else if (!(strcmp(specifiedCommand, "delete")))
	{
		deleteApt(lstApt, commandRes + lenToKeep + 7);
	}
	else if (!(strcmp(specifiedCommand, "buy")))
	{
		buyApt(commandRes + lenToKeep, lstApt);
	}
	else if (!(strcmp(specifiedCommand, "!!"))) 
	{
		last_command(short_term_history, listcommand, lstApt, theMaxCodeOfApt);
	}

	else if (!(strcmp(specifiedCommand, "history"))) //recovery #4
	{
		history(listcommand, short_term_history);
	}
	else if (!(strcmp(specifiedCommand, "short_history")))//recovery #3
	{
		short_history(listcommand, short_term_history);
	}

	else
	{ ///////recovery #2 
		if (checkRecovery(specifiedCommand))
		{
			strToNum = strtok(specifiedCommand, "^");

			numCommandRes = charToNum(strToNum + ONE); 

			connective_to_command_by_num_for_Recovery2(short_term_history, listcommand, lstApt, numCommandRes, theMaxCodeOfApt);// שינוי
		}
		else /////recovery #5
		{
			strToNum = strtok(specifiedCommand, "^");

			numCommandRes = charToNum(strToNum + ONE); 

			strForSend = strtok(NULL, "^");
			char * second = strtok(NULL, "");

			command_to_change(short_term_history, listcommand, lstApt, strForSend, second, numCommandRes, theMaxCodeOfApt);//שינוי

		}

	}
}

unsigned short int codeToNum(char *str)
{
	int i = 0;
	unsigned short int num = 0;

	while (str[i] != '\0')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}

	return num;
}

////     The function removes the node from the list   ////
void removeAptListNode(listApt* aptList, ListNodeApt* nodeToRemove)
{
	ListNodeApt *tmp;

	if (aptList->head == aptList->tail) // if only one node 
		aptList->head = aptList->tail = NULL;

	else if (aptList->tail == nodeToRemove)//List is more than one node, the node to remove is the tail
	{	
		aptList->tail = nodeToRemove->prev;
		aptList->tail->next = NULL;
	}

	else if (aptList->head == nodeToRemove)//List is more than one node, the node to remove is the head
	{	
		aptList->head = nodeToRemove->next;
		aptList->head->prev = NULL;
	}

	else//List is more than one node, the node to remove is neither the head nor the tail
	{	
		tmp = nodeToRemove->next;
		tmp->prev = nodeToRemove->prev;

		tmp = nodeToRemove->prev;
		tmp->next = nodeToRemove->next;

	}

	free(nodeToRemove->Aptaddress);
	free(nodeToRemove);
}

////	returns how many commands were entered	////
int howManyCommands(char** short_term_history, listCommands* lstCommand)
{
	listNodeCommands* temp = lstCommand->head;
	int res = 0;

	if (short_term_history[N - 1] != NULL)
	{
		if (temp == NULL)
		{
			return N;
		}
		else
		{
			res = N;
			res += checkCountInList(lstCommand);

			return res;
		}
	}
	else 
	{
		for (int i = 0; i < 7; i++)
		{
			if (short_term_history[i] != NULL)
				res++;
		}
		return res;
	}
}

////	checks how many command are in command list		////
int checkCountInList(listCommands* lstCommand)
{
	listNodeCommands* temp = lstCommand->head;
	int res = 0;

	while (temp != NULL)
	{
		res++;
		temp = temp->next;
	}
	return res;
}



/*tranforms Character into an Integer*/
int charToNum(char *str)
{
	int i = 0, num = 0;

	while (str[i] != '\0')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}

	return num;
}

////	when we load data from file this function updates the command list with the correct command		////
void insertNodeToHead(char* command, listCommands* listCommand)
{
	listNodeCommands* newTail = (listNodeCommands*)malloc(sizeof(listNodeCommands));

	newTail->commandName = command;

	newTail->prev = NULL;

	if (listCommand->head == NULL)
	{
		listCommand->head = listCommand->tail = newTail;
		newTail->next = NULL;
	}
	else
	{
		listCommand->head->prev = newTail;
		newTail->next = listCommand->head;
		listCommand->head = newTail;
	}

}


/*isnerting new node into end of command list */
void insertNodeToTail(listCommands* lst, listNodeCommands* newTail)
{
	newTail->next = NULL;

	if (lst->head == NULL)
	{
		lst->head = lst->tail = newTail;
		newTail->num = ONE;
		newTail->prev = NULL;
		newTail->next = NULL;

	}
	else
	{
		lst->tail->next = newTail;
		newTail->num = lst->tail->num + ONE;
		newTail->prev = lst->tail;
		lst->tail = newTail;

	}
}

/* makes an empty Command list*/
void makeEmptyList_Commands(listCommands* result)
{
	result->head = result->tail = NULL;
}

/* makes an empty Apartment list*/
void makeEmptyList_Apt(listApt* result)
{
	result->head = result->tail = NULL;
}


void doNullForArr(char** arr)
{
	for (int i = 0; i < N; i++)
	{
		arr[i] = NULL;
	}
}

////	after loading apartments this function checks what is the highest apartment code to continue codes from that point	////
unsigned short int checkCode(listApt *lst)
{
	ListNodeApt *curr = lst->head;
	unsigned short int code = 0;
	while (curr != NULL)
	{
		if (code < curr->AptCode)
			code = curr->AptCode;

		curr = curr->next;
	}
	return code;
}