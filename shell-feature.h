//
//  shell-feature.h
//  HDH-Project01
//
//  Created by CongHoang on 10/1/19.
//  Copyright © 2019 CongHoang. All rights reserved.
//

#ifndef shell_feature_h
#define shell_feature_h

#include "define-value.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

// Support function
void showMessage(void);
int getLenOfArgs(char **args);
char* getLine(void);
char **getTokens(char *temp, bool *isRunBackground);
char **getPipeTokens(char *temp);
void deallocateMemory(char ** args);
bool isValidCommand(char *inputLine);

int check_redirect(char** args, char** direct);

// Feature
void executingCommand(char** args, int block, int redir, char* direct);
void historyFeature(char *historyCmd);
void communicationViaPipe(char **args);

#endif /* shell_feature_h */
