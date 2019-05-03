
//   Daniel Benedik- 203914510
//	 Adeena persoff- 312551781

#include "header.h"


void main(int argc, char** argv)
{
	/////     Defining structures of the program       /////
	listApt lsApt;

	listCommands lstCommand;

	char *short_term_history[N];

	////  variables reset  ////
	makeEmptyList_Commands(&lstCommand);
	makeEmptyList_Apt(&lsApt);
	doNullForArr(short_term_history);

	
	unsigned short int theMaxCodeOfApt;
	int countOfCommands;
	char *str;

	////  loading data from files  ////

	load(short_term_history, &lstCommand, &lsApt);

	theMaxCodeOfApt = checkCode(&lsApt); // get the last code of apt entered from apt List
	countOfCommands = howManyCommands(short_term_history, &lstCommand);// get the number of command that were entered last run time
	update_list_commands(short_term_history, &lstCommand, countOfCommands);

	////  start receiving information from users ////

	printf("Please enter one of the following commands:\nadd-an-apt, get-an-apt, buy-an-apt or delete-an-apt\n");
	printf("For reconstruction commands, please enter:\n!!, !num, history, short_history or !num^str1^str2 \n");
	printf("To exit, enter exit.\n");

	printf(">> ");
	str = getStr();

	while (strcmp(str, "exit") != 0)
	{
		if (str[0] != '!' && str[0] != 'h' && str[0] != 's')
			addToSave(str, short_term_history, &lstCommand);

		////    filtering command entered      ////
		whatCommand(str, &lsApt, short_term_history, &lstCommand, &theMaxCodeOfApt);

		printf(">> ");

		str = getStr(); // receiving next command from user
	}

	
	countOfCommands = howManyCommands(short_term_history, &lstCommand);

	save(short_term_history, &lstCommand, &lsApt); // saving data to files


	 ////    free apartment and command list an array      ////
	free(str);
	free_list_apt(&lsApt);
	free_commands(short_term_history, &lstCommand, countOfCommands);

	printf("Good Bye!\n");

	system("pause");

}
