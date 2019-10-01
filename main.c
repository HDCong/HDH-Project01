#include "define-value.h"
#include "shell-feature.h"

int main(int argc, char* argv[]){
    int status = 1;
    char **args = NULL;
    char *historyCommand = NULL;
    while(status == 1){
        showMessage();
        char *input_line = getLine(), *direct = NULL;
        if(!isValidCommand(input_line))
        {
            printf("'%s': command not found\n", input_line);
            continue;
        }
        if(!strstr(input_line,"|")){
            // Check for &
            bool isRunBackground = false;
            args = getTokens(input_line, &isRunBackground);
            // Check for >, <
            int redir = check_redirect(args, &direct);
            if(args[0] == NULL){
                continue;
            }
            if (!strcmp(args[0], "exit")){
                status = 0;
                continue;
            }
            if(!strcmp(args[0],"!!")){
                historyFeature(historyCommand);
            }
            else {
                free(historyCommand);
                historyCommand = malloc(sizeof(char) * (strlen(input_line)+1));
                strcpy(historyCommand, input_line);
                executingCommand(args, isRunBackground, redir, direct);
            }
            
        }
        else{
            free(historyCommand);
            historyCommand = malloc(sizeof(char) * (strlen(input_line)+1));
            strcpy(historyCommand, input_line);
            args= getPipeTokens(input_line);
            communicationViaPipe(args);
        }
        free(input_line);
        deallocateMemory(args);
    }
    free(historyCommand);
    deallocateMemory(args);
    return 0;
}
