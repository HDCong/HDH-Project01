#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define LINE_MAX 80
#define TOK_BUF 45

char* getLine(){
    char* line = malloc(sizeof(char) * LINE_MAX);
    char c;
    int bufSize = LINE_MAX;
    int i = 0;

    while(1)
    {
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

void executingCommand(char** args, int block){
    pid_t pid, wpid;
    int status;
    int result;

    pid = fork();

    if(pid < 0){
        perror("osh");
    }
    else if(pid == 0){
        result = execvp(args[0], args);
        if (result == -1)
            perror("osh");
        exit(result);
    }
    
    if(block)
    {
       result = waitpid(pid, &status, 0);
    }
    else
    {
        printf("pid = %d\n", pid);
    }
    
}


void historyFeature(char *historyCmd){
    if(!historyCmd){
        printf("No commands in history.\n");
    }
    else{
        printf("%s\n",historyCmd);
    }
}

int check_ampersand(char** args)
{
    int i = 0;
    for ( i ; args[i] != NULL; i++)
    {
        if(args[i][0] == '&')
        {
            args[i] = NULL;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]){
    int status = 1;
    int block;
    char **args = NULL;
    char *historyCommand = NULL;
    while(status == 1){
        printf("osh> ");
        char *input_line = getLine();
        args = getTokens(input_line);
        
        if (!strcmp(args[0], "exit")){
            status = 0;
            exit(0);
        }
            

        // check for an ampersand
        block = (check_ampersand(args) == 0);

        if(!strcmp(args[0],"!!")){
            historyFeature(historyCommand);
        }
        else {
            free(historyCommand);
            historyCommand = malloc(sizeof(char) * (strlen(input_line)+1));
            strcpy(historyCommand, input_line);
        }
        
        executingCommand(args, block);
        free(input_line);
    }
    free(args);
    return 0;
}
