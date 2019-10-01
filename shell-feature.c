#include "shell-feature.h"

void showMessage(){
    char buff[] = "osh> ";
    printf("%s", buff);
    fflush(stdout);
}
int getLenOfArgs(char **args){
    int count = 0;
    for(; args[count] != NULL; count++);
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
    token = strtok(string, " \t\r\n\a");
    while(token != NULL){
        tokens[position]= (char*)malloc(sizeof(char)* (strlen(token)+1));
        strcpy(tokens[position], token);
        position++;
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}
char **getPipeTokens(char *temp) {
    char *string = malloc(sizeof(char) * (strlen(temp)+1));
    strcpy(string,temp);
    int bufsize = TOK_BUF, position = 0;
    char **tokens = (char**) malloc(bufsize * sizeof(char*));
    char *token;
    token = strtok(string, "|\t\r\n\a");
    while(token != NULL){
        tokens[position]= (char*)malloc(sizeof(char)* (strlen(token)+1));
        strcpy(tokens[position], token);
        position++;
        token = strtok(NULL, "|\t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}
void executingCommand(char** args, int block, int redir, char* direct){
    pid_t pid;
    int status;
    int result;
    pid = fork();
    if(pid < 0){
        perror("osh");
    }
    else if(pid == 0){
        int fd;
        //printf("Yes yes yes %d\n", redir);
        if (direct)
            if(redir < 0)
            {
                printf("Redirect input");
                fd = open(direct, O_RDONLY);
                dup2(fd, STDIN_FILENO);
            }
            else if ( redir > 0)
            {
                fd = open(direct, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                dup2(fd, STDOUT_FILENO);
            }
        close(fd);
        free(direct);
        
        result = execvp(args[0], args);
        if (result == -1)
            perror("osh");
        exit(result);
    }
    if(!block){
        result = waitpid(pid, &status, 0);
    }
    else{
        printf("pid = %d\n", pid);
    }
}
void deallocateMemory(char ** args){
    int len = getLenOfArgs(args);
    for(int i=0;i< len;i++){
        free(args[i]);
    }
    free(args);
}
void historyFeature(char *historyCmd){
    if(!historyCmd){
        printf("No commands in history.\n");
    }
    else{
        printf("%s\n",historyCmd);
    }
}
// check_redirect > 0: for redirecting output
// check_redirect < 0: for redirecting input
// check_redirect = 0: not redirect
int check_redirect(char** args, char** direct){
    int i = 0;
    int res = 0;
    for( ; args[i] != NULL; i++){
        if (args[i][0] == '>')
            res = 1;
        if (args[i][0] == '<')
            res = -1;
        if (res != 0)
        {
            (*direct) = strdup(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            return res;
        }
    }
    return res;
}

void communicationViaPipe(char **args){ // args[0]: left command, args[1]: right command
    int pipefd[2];
    pid_t childpid1, childpid2;
    pipe(pipefd);
    childpid1= fork();
    if(childpid1==0)
    {
        close(pipefd[READ]);
        dup2(pipefd[WRITE],STDOUT_FILENO);
        bool flagTemp = false;
        char **argsLeftCommand = getTokens(args[0], &flagTemp);
        execvp(argsLeftCommand[0], argsLeftCommand);
    }
    childpid2=fork();
    if(childpid2==0)
    {
        close(pipefd[WRITE]);
        dup2(pipefd[READ],STDIN_FILENO);
        bool flagTemp = false;
        char **argsRightCommand = getTokens(args[1], &flagTemp);
        execvp(argsRightCommand[0], argsRightCommand);
    }
    close(pipefd[READ]);
    close(pipefd[WRITE]);
    
    waitpid(childpid1, NULL, 0);
    waitpid(childpid2, NULL, 0);
    
}
bool isValidCommand(char *inputLine){
    int check=0;
    for(int i =0 ;i <strlen(inputLine);i++)
    {
        char c= inputLine[i];
        if(c=='&' || c =='|' | c =='<' || c =='>')
            check++;
    }
    return check < 2;
}
