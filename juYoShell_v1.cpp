#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include "Chronos.hpp"

//I wrote this line from inside of my program 
//holy shit i just compiled my program from within my program
//AND THEN RAN IT SDFIUHAOSJUHG QOEUYRG UOEIWYFGOUIQWHG  NOT OKAY!!!!!!!!!!!!

/* Enums */
enum { 
	JUYOSHELL_CMD_PTIME, JUYOSHELL_CMD_HISTORY, JUYOSHELL_CMD_EXIT,
	JUYOSHELL_CMD_HELP,
	JUYOSHELL_CMD_NULL, 
	JUYOSHELL_CMD_EXEC
};

/* Globals */
std::vector<char**> history;

/* Overloads */
std::ostream& operator<<(std::ostream& os, std::vector<char**> v) //history specific overload
{
	for (int i = 0; i < v.size(); ++i)
	{
		//v[i] is char**
		os << v.size() - i << ": "; //print ^ number
		
		int j = 0;
		while(v[i][j] != NULL)
		{
			os << v[i][j] << " "; //string together input line
			j++;
		}
		
		if(i < v.size()-1) //keep print out neat, no extra newline
			os << std::endl;	
	}
	return os;
}

/* Prototypes */
int shell_input_enum(char**);
bool shell_launcher(char**);

/* Functions */

	/* Input Functions */
int shell_input_enum(char** input)
{
	if( input[0][0] == '^' ) //check if ^n or ^ 
		return JUYOSHELL_CMD_HISTORY;
	else if( strcmp(input[0], "ptime") == 0) //check if ptime
		return JUYOSHELL_CMD_PTIME;
	else if( strcmp(input[0], "exit") == 0 ) //check if exit
		return JUYOSHELL_CMD_EXIT;
	else if( strcmp(input[0], "help") == 0 ) //check if help
		return JUYOSHELL_CMD_HELP;
	else if( strcmp(input[0], "") == 0 ) //check if null or ws
		return JUYOSHELL_CMD_NULL;
	else
		return JUYOSHELL_CMD_EXEC; //return results
}

char** shell_input_tokenizer(char* cstr) //the majority of this was lifted from an example because it was completely outside of my background knowledge
{
	int bufferSize = strlen(cstr);  //start the buffer at the length of the char*
	char ** input = (char**)malloc(bufferSize * sizeof(char*));	//alloc for execvp

  	if (!input) 
  	{ 
	    	fprintf(stderr, "juyoshell: allocation error\n");
    		exit(EXIT_FAILURE);
	}
	
	int pos = 0; //position in the char*
	cstr = strtok(cstr, " \n\r\t"); //tokenize by whitespace
	while (cstr != NULL) 
	{ 
		input[pos] = cstr; //pop token over
		pos++; //go to the next token

		if (pos >= bufferSize) //reallocate if needed
		{
			bufferSize += bufferSize; //increase buffer size
			input = (char**)realloc(input, bufferSize * sizeof(char*));
			if (!input)
			{
				fprintf(stderr, "juyoshell: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		
		cstr = strtok(NULL, " \n\r\t"); //null out what has already been processed
	}
	input[pos] = NULL; //?
	
	return input;
}

char** shell_input_parser() 
{
	std::string str;
	std::getline(std::cin, str); //grab line of input
	char* cstr = strdup(str.c_str()); //stick it in a cstring
	char** input = shell_input_tokenizer(cstr); //get execvp-ready tokens
	
	return input;
}

	/* Doing Actual Stuff Functions */
void shell_fork(char ** input)
{
	int success = 0;
		
	auto pid = fork(); 
	if( pid > 0 ) //parent, will wait while child still alive //TODO THIS IS WHAT SHOULD BE TIMED
	{
		//chronos.stopwatch(); //starts stopwatch on first call
		waitpid(pid, &success, 0); 
		if( WIFEXITED(success) == 1 ) printf("Parent Says Fork Succeeded!\n");
		else 			 printf("Parent Says Fork Failed!\n");
		//chronos.stopwatch(); //stops stopwatch on second call
	}
	else if( pid == 0 ) //child, will become the exec inputted if possible 
	{
		execvp(input[0], input); //replace with executable
		printf("Could not execute %s!\n", input[0]); //this line only executes if execvp fails
		exit(EXIT_FAILURE);
	}
	else //otherwise error forking //perror("hw5")
	{
		perror("juyoshell");
	}
}

void shell_cmd_ptime() //TODO
{
printf("Ptime!\n");
	//TODO
	//chronos.print_stopwatch();
}

void shell_history_log(char** input) 
{
	if( shell_input_enum(input) == JUYOSHELL_CMD_HISTORY )
		return;
	
	history.push_back(input);
}

void shell_cmd_history_print()
{
	std::cout << "History:\n" << history << std::endl;
}

void shell_cmd_history(char** input) 
{
//printf("History!\n");
	char* cmd = input[0];//grab the n history entry, if exists and not a history cmd
	cmd = strtok(cmd, "^ \n\r\t"); //should just be a number now
	
	if(cmd == NULL)
	{
		shell_cmd_history_print();
		return;
	}
	
	int n = atoi(cmd);
	if(n < 1)
		return;
//printf("Go back: %d\n",n);
	if( n >= history.size() )
		return;
	
	shell_launcher(history[history.size() - n] ); //re-call launcher with historical input
}

void shell_cmd_help()
{
	printf("\n");
	printf("  juyoshell, version 0.1.1-alpha\n");
	printf("  The following commands are defined.\n");
	printf("  -  exit \t kill the shell\n");
	printf("  -  ptime\t time spent executing (in milliseconds)\n");
	printf("  -  ^    \t list of input history with index\n");
	printf("  -  ^n  \t repeat input history at index n\n");
	printf("  -  help \t this help page\n");
	printf("\n");
}

bool shell_cmd(char** input)
{
	bool exit = false;
	switch( shell_input_enum(input) )
	{
		case JUYOSHELL_CMD_EXIT: 
			exit = true; //exit
			break;
		case JUYOSHELL_CMD_PTIME:
			shell_cmd_ptime();
			break; //good
		case JUYOSHELL_CMD_HISTORY: 
			shell_cmd_history(input);
			break; //good
		case JUYOSHELL_CMD_HELP:
			shell_cmd_help();
			break;
	}
	
	return (!exit); //flip for readability
}

bool shell_launcher(char** input) //TODO timing (chronos.stopwatch()?)
{
	bool result = true; //true=good, false=exit, 
	
	if( shell_input_enum(input) == JUYOSHELL_CMD_NULL ) //if empty
		return result; //nothing to do, don't push to history, just ask again
		
	if( shell_input_enum(input) == JUYOSHELL_CMD_EXEC ) //executable
		shell_fork(input); //shell fork
	else //must be shell command
		result = shell_cmd(input); //shell cmd function
	
	shell_history_log(input); //store input into history (even if faulure(and even if exit))
	return result;  //1=good, 0=exit, -1=execfailure, -2=forkfailure
}

	/* Shell Loop Function */
void shell_loop()
{
	bool cont = true;
	
	do{
		printf("[cmd]: "); //print prompt "[cmd]: "
		auto input = shell_input_parser(); //get input from input handler
		cont = shell_launcher(input); //forward input to launch handler
	} while (cont == true); //while not exited
}

/* Main */
int main(void)
{
	shell_loop();

printf("Done!\n");
}

