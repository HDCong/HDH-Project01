#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define LINE_MAX 80
#define TOK_BUF 45

char* getLine(){
    char *line = malloc(sizeof(char)*LINE_MAX);
    char c;
    int i;
    for(i = 0; i < LINE_MAX && (c = getchar()) != EOF && c != '\n'; i++) {
        line[i] = c;
    }
    line[i + 1] = '\0';
    return line;
}

char **getTokens(char *temp){
    char *string = malloc(sizeof(char) * (strlen(temp)+1));
    strcpy(string,temp);
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
void historyFeature(char *historyCmd){
    if(!historyCmd){
        printf("No commands in history.\n");
    }
    else{
        printf("%s\n",historyCmd);
    }
}

int main(){
    int status = 1;
    char **args = NULL;
    char *historyCommand = NULL;
    while(status == 1){
        printf("osh> ");
        char *input_line = getLine();
        args = getTokens(input_line);
        //pid_t pid = fork();

        // if(pid == -1){
        //     printf("Child process could not be createld!\n");
        // }
        // else if(pid == 0){
        //     excutingCommand(args);
        // }
        // else if(pid > 0){

        // }
        if(!strcmp(args[0],"!!")){
            historyFeature(historyCommand);
            
        }
        else {
            free(historyCommand);
            historyCommand = malloc(sizeof(char) * (strlen(input_line)+1));
            strcpy(historyCommand, input_line);
        }
        free(input_line);
    }
 
    return 0;
}
