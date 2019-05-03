#include "header.h"

////	saves apartments and commands to files	////
void save(char** short_term_history, listCommands* listCommand, listApt* Aprts)
{
	saveApts(Aprts);
	saveStocks(short_term_history, listCommand);
	return;
}


void saveStocks(char** short_term_history, listCommands* listCommand)
{

	FILE* f = fopen("commands.txt", "w");
	int count = 0;

	listNodeCommands* temp = listCommand->tail;

	for (int i = 0; i < N; i++)
	{
		if (short_term_history[i] != NULL)
			count++;
	}

	for (int i = 0; i < count; i++)
	{
		fprintf(f, "%s\n", short_term_history[i]);
	}

	while (temp != NULL)
	{
		fprintf(f, "%s\n", temp->commandName);
		temp = temp->prev;
	}




	fclose(f);
}



//load
void loadStocks(char** short_term_history, listCommands* listCommand)
{
	FILE* f = fopen("commands.txt", "r");
	int  i, count = 0;
	char c;


	if (f == NULL)
		return;

	c = fgetc(f);

	while (c != EOF)
	{

		while (c != '\n')
		{
			count++;
			c = fgetc(f);
		}

	
		fseek(f, -1 * (count + 2), SEEK_CUR);

		char* temp = (char*)calloc((count + 1), sizeof(char));

		for (i = 0; i < count; i++)
		{
			temp[i] = fgetc(f);
		}

		addToStockOrList(temp, short_term_history, listCommand);

		c = fgetc(f);

		count = 0;
		c = fgetc(f);
	}
	fclose(f);
}


void addToStockOrList(char* command, char** short_term_history, listCommands* listCommand)
{
	int i = 0;
	if (short_term_history[N - 1] == NULL)
	{
		while (short_term_history[i] != NULL)
			i++;
		short_term_history[i] = command;
	}
	else
	{
		insertNodeToHead(command, listCommand);
	}
}

////	loading apartments and commands from files
void load(char** short_term_history, listCommands* listCommand, listApt* Aprts)
{

	loadApts(Aprts);
	loadStocks(short_term_history, listCommand);

	return;
}

////	update the commant index ////
void update_list_commands(char** short_term_history, listCommands* lstCommand, int index)
{
	listNodeCommands* temp = lstCommand->tail;

	if (index > N)
	{
		index -= N;
		while (temp != NULL)
		{
			temp->num = index;
			index--;
			temp = temp->prev;
		}
	}
}


void free_arry_commands(char** arr, int index)
{
	for (int i = 0; i < index; i++)
		free(arr[i]);
}

void free_commands(char** arr, listCommands* lst, int index)
{
	if (index > N)
	{
		free_arry_commands(arr, N);
		free_list_commands(lst);
	}
	else
	{
		free_arry_commands(arr, index);
	}

}

void free_list_commands(listCommands* lst)
{
	listNodeCommands* temp = lst->head;
	listNodeCommands* save;

	while (temp != NULL)
	{

		free(temp->commandName);// free the Aptaddress
		save = temp->next;
		free(temp);// free the nodapt

		temp = save;
	}

}

void free_list_apt(listApt* lst)
{
	ListNodeApt* temp = lst->head;
	ListNodeApt* save;

	while (temp != NULL)
	{

		free(temp->Aptaddress);// free the Aptaddress
		save = temp->next;
		free(temp);// free the nodapt

		temp = save;
	}

}

/*This function save the FullAprts to bin file*/
void saveApts(listApt * FullAprts)
{
	FILE* f = fopen("apartments.bin", "wb");
	ListNodeApt * curr = FullAprts->head;
	unsigned char mem[5];
	short int adrslen;

	while (curr != NULL)
	{
		adrslen = (short int)strlen(curr->Aptaddress);
		aptToBits(mem, curr); //bits that need to be save
		fwrite(mem, sizeof(char), 5, f);
		fwrite(&curr->AptCode, sizeof(short int), 1, f);
		fwrite(&adrslen, sizeof(short int), 1, f);
		fwrite(curr->Aptaddress, sizeof(char), adrslen, f);
		fwrite(&curr->AptPrice, sizeof(int), 1, f);
		curr = curr->next;
	}

	fclose(f);
}
/*This function load Aprts from file to Fullaprts*/
void loadApts(listApt * FullAprts)
{
	ListNodeApt * newApt;
	char mem[5];
	short int adrslen;
	FILE* f = fopen("apartments.bin", "rb");
	if (f == NULL)
		return;
	long int fsize = fileSize(f);
	while (ftell(f) < fsize)
	{
		fread(mem, sizeof(char), 5, f);
		newApt = bitsToApt(mem);
		fread(&newApt->AptCode, sizeof(short int), 1, f);
		fread(&adrslen, sizeof(short int), 1, f);
		newApt->Aptaddress = (char*)malloc(sizeof(char)*adrslen + 1);
		fread(newApt->Aptaddress, sizeof(char), adrslen, f);
		newApt->Aptaddress[adrslen] = '\0';
		fread(&newApt->AptPrice, sizeof(int), 1, f);
		location_in_the_list(FullAprts, newApt);
	}

	fclose(f);
}

void aptToBits(char * mem, ListNodeApt *apt)
/*This function turn Aprt into bits*/
{
	int i;
	for (i = 0; i < 5; i++)
		mem[i] = 0;

	//num of room 
	mem[0] = apt->AptRooms << 4;

	//enter date
	mem[0] = ((apt->entryDate.day >> 1) & 15) | mem[0];
	mem[1] = ((apt->entryDate.day << 7));
	mem[1] = ((apt->entryDate.month << 3) & 120) | mem[1];
	mem[1] = (apt->entryDate.year >> 4) & 7 | mem[1];
	mem[2] = (apt->entryDate.year << 4) & 240;

	//entry data base
	mem[3] = apt->entryDataBase.day << 3;
	mem[3] |= (apt->entryDataBase.month >> 1);// &7) | mem[3];
	mem[4] = apt->entryDataBase.month << 7;
	mem[4] |= apt->entryDataBase.year;
	return;
}
ListNodeApt * bitsToApt(char * mem)
/*This function turn bits into an aparment*/
{
	ListNodeApt* apt = (ListNodeApt*)malloc(sizeof(ListNodeApt));
	//rooms
	apt->AptRooms = (mem[0] >> 4) & 15;

	//enter date
	apt->entryDate.day = (mem[0] << 1) & 30 | (mem[1] >> 7) & 1;
	apt->entryDate.month = (mem[1] >> 3) & 15;
	apt->entryDate.year = (mem[1] << 4) & 112 | (mem[2] >> 4) & 15;

	//entry data base
	apt->entryDataBase.day = (mem[3] >> 3) & 15;
	apt->entryDataBase.month = (mem[3] << 1) & 14 | (mem[4] >> 7) & 1;
	apt->entryDataBase.year = mem[4] & 127;
	return apt;
}
/*This function calculate the file size*/
long int fileSize(FILE * f)
{
	long int res, temp;
	temp = ftell(f);
	fseek(f, 0, SEEK_END);
	res = ftell(f);
	fseek(f, temp, SEEK_SET);
	return res;
}

