#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#define LINE_MAX 80
#define TOK_BUF 45

void msg_promt(){
    char buff[] = "osh> ";
    printf("%s", buff);
    fflush(stdout);
}

int getLenOfArgs(char **args){
    int count = 0;
    for(count; args[count] != NULL; count++);
    return count;
}


char* getLine(){
    char* line = malloc(sizeof(char) * LINE_MAX);
    char c;
    int bufSize = LINE_MAX;
    int i = 0;
    
    while(1){
        
        c = getchar();
        
        if ( c == EOF || c == '\n')
        {
            line[i] = '\0';
            return line;
        }
        else
        {
            line[i] = c;
        }
        
        i += 1;
        
        if (i >= bufSize)
        {
            bufSize += LINE_MAX;
            line = realloc(line, bufSize);
        }
    }
}

char **getTokens(char *temp, bool *isRunBackground) {
    char *string = malloc(sizeof(char) * (strlen(temp)+1));
    strcpy(string,temp);
    if (string[strlen(string)-1]=='&') {
        *isRunBackground=true;
        string[strlen(string)-1]='\0';
    }
    int bufsize = TOK_BUF, position = 0;
    char **tokens = (char**) malloc(bufsize * sizeof(char*));
    char *token;
    token = strtok(string, " |\t\r\n\a");
    while(token != NULL){
        tokens[position]= (char*)malloc(sizeof(char)* (strlen(token)+1));
        strcpy(tokens[position], token);
        position++;
        token = strtok(NULL, " |\t\r\n\a");
    }
    
    tokens[position] = NULL;
    
    return tokens;
}
void deallocateMemory(char ** args){
    int len = getLenOfArgs(args);
    for(int i=0;i< len;i++){
        free(args[i]);
    }
    free(args);
}
void excutingCommand(char** args){
    execvp(args[0], args);
    exit(EXIT_SUCCESS);
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
        msg_promt();
        char *input_line = getLine();
        bool isRunBackground = false;
        args = getTokens(input_line, &isRunBackground);
        if(args[0] == NULL){
            continue;
        }
        if (!strcmp(args[0], "exit"))
            status = 0;
        if(!strcmp(args[0],"!!")){
            historyFeature(historyCommand);
        }
        else {
            free(historyCommand);
            historyCommand = malloc(sizeof(char) * (strlen(input_line)+1));
            strcpy(historyCommand, input_line);
            pid_t pid;
            
            switch(pid = fork()) {
                case -1:
                    printf("Child process could not be created!\n");
                    exit(EXIT_FAILURE);
                    break;
                case 0:
                    excutingCommand(args);
                    break;
                default:
                    if(!isRunBackground)
                        wait(NULL);
                    break;
            }
        }
        
        free(input_line);
        deallocateMemory(args);
    }
    return 0;
}
