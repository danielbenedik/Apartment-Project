#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define N 7
#define YEAR 2000
#define ONE 1


/*Struct for commandList*/
typedef struct listNodeCommands {
	int num;
	char* commandName;
	struct listNodeCommands* next;
	struct listNodeCommands* prev;
}listNodeCommands;

typedef struct listCommands
{
	listNodeCommands* head;
	listNodeCommands* tail;
}listCommands;

/*struct for Date*/
typedef struct NodeDate {
	short int day;
	short int month;
	short int year;
}NodeDate;

typedef struct listNodeApt
{
	unsigned short int AptCode;
	char* Aptaddress;
	int AptPrice;
	short int AptRooms;
	int print;
	NodeDate entryDate;
	NodeDate entryDataBase;

	struct listNodeApt* next;
	struct listNodeApt* prev;
}ListNodeApt;

typedef struct listApt
{
	ListNodeApt* head;
	ListNodeApt* tail;
}listApt;

//Primary functions
char* getStr();
void addToSave(char* str, char** short_term_history, listCommands* lstCommand);
void whatCommand(char*str, listApt *lst, char ** short_term_history, listCommands *command, unsigned short int *theMaxCodeOfApt);
// Sub functions
void removeAptListNode(listApt* aptList, ListNodeApt* nodeToRemove);
int howManyCommands(char** short_term_history, listCommands* lstCommand);
int checkCountInList(listCommands* lstCommand);
int charToNum(char *str);
void insertNodeToHead(char* command, listCommands* listCommand);
void insertNodeToTail(listCommands* lst, listNodeCommands* newTail);
long int fileSize(FILE * f);
unsigned short int codeToNum(char *str);

//Reseting Lists and Array functions
void makeEmptyList_Commands(listCommands* result);
void makeEmptyList_Apt(listApt* result);
void doNullForArr(char** arr);

//Adding apartments to apartment List functions
void add_To_LsApt(char* str, listApt* lsApt, unsigned short int code);
void location_in_the_list(listApt* lsApt, ListNodeApt* newApt);
//Arranging apartment List functions
void LinkBefore(ListNodeApt* node, ListNodeApt* newNode);
void LinkAfter(ListNodeApt* node, ListNodeApt* newNode);
void SetOnlyListMember(listApt* lsApt, ListNodeApt* member);
unsigned short int checkCode(listApt *lst);
//time functions
NodeDate getDate(int days);
void checkDate(int date, listApt *lst);
void currentDate(short int* day, short int* month, short int* year);
//Recovery functions
char* command_by_num(char** short_term_history, listCommands* listCommands, int num);
void command_to_change(char** short_term_history, listCommands* listCommands, listApt* listApt, char* firstStr, char* secondStr, int num, unsigned short int *theMaxCodeOfApt);
void last_command(char** short_term_history, listCommands* listCommands, listApt* listApt, unsigned short int *theMaxCodeOfApt);
void short_history(listCommands *lst, char** str); //recovery #3
void history(listCommands *lst, char** str); //recovery #4
void connective_to_command_by_num_for_Recovery2(char** short_term_history, listCommands* listCommands, listApt* listApt, int num, unsigned short int *theMaxCodeOfApt);
char* part_from_str(char* str);
char* replace_Command(char* s, char* oldW, char* newW);
BOOL checkRecovery(char* str);

//Get-an-apt functions
void maximumPrice(int price, listApt *aptLst);
void minimumNumRooms(int num, listApt *aptList);
void maximumNumRooms(int num, listApt *lst);
void getApt(char *str, listApt *aptLst);
void enter(listApt *lst, int day);
void buyApt(char *str, listApt *lst);
void deleteApt(listApt *lst, char *str);
void sendNodeToRemove(listApt *aptList);
void printSwitchOFF(ListNodeApt *prnt);/*changes the specefic apartment print switch to FALSE*/
void SwitchON(listApt *lst);/*resets all the print switche to TRUE*/
void printCommandList(listCommands *lst);
void printCommandListRec(listNodeCommands *node);
/*print AptList from high to low*/
void printHightToLow(listApt *lst);
void printHightToLowRec(ListNodeApt *node);
/*prints AptList from the lowest price to the higest price*/
void printLowToHighPrice(listApt *lst);
void printLowToHighPriceRec(ListNodeApt *node);

// SAVE
void save(char** short_term_history, listCommands* listCommand, listApt* Aprts);
void saveStocks(char** short_term_history, listCommands* listCommand);
void addToStockOrList(char* command, char** short_term_history, listCommands* listCommand);
void saveApts(listApt * FullAprts);
void aptToBits(char * mem, ListNodeApt *apt);

// LOAD
void loadStocks(char** short_term_history, listCommands* listCommand);
void load(char** short_term_history, listCommands* listCommand, listApt* Aprts);
void update_list_commands(char** short_term_history, listCommands* lstCommand, int index);
void loadApts(listApt * FullAprts);
ListNodeApt * bitsToApt(char * mem);

// FREE
void free_list_apt(listApt* lst);
void free_commands(char** arr, listCommands* lst, int index);
void free_arry_commands(char** arr, int index);
void free_list_commands(listCommands* lst);



