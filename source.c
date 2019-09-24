#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define LINE_MAX 80
#define TOK_BUF 45

char* getLine(){
	char *line = malloc(sizeof(char)*80);
	char c;
	int i;

	for(i = 0; i < LINE_MAX && (c = getchar()) != EOF && c != '\n'; i++) {
		line[i] = c;
	}

	line[i + 1] = '\0';

	return line;
}

char **getTokens(char *string){
	int bufsize = TOK_BUF, position = 0;
	char **tokens = (char**) malloc(bufsize * sizeof(char*));
	char *token;

	token = strtok(string, " |\t\r\n\a");
	while(token != NULL){
		tokens[position] = token;
		position++;
		token = strtok(NULL, " |\t\r\n\a");
	}

	tokens[position] = NULL;

	return tokens;
}

void excutingCommand(char** args){

}


int main(){
	int status = 1;
	char **args = NULL;
	while(status == 1){
		printf("osh> ");
		char *input_line = getLine();
		args = getTokens(input_line);
		printf("%s\n", args[0]);
		//pid_t pid = fork();

		// if(pid == -1){
		// 	printf("Child process could not be created!\n");
		// }
		// else if(pid == 0){
		// 	excutingCommand(args);
		// }
		// else if(pid > 0){

		// }

		free(input_line);
	}
	return 0;
}