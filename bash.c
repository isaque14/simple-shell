#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define SIZE 1024
#define WORD 1000 
#define LIST 100

//PRINTS DE ENTRADA E HELP
void startup()
{
    char* user = getenv("USER");
    printf("\n\n\n**********************************\n");
    printf("\n\n\n    BEM VINDO AO MEU TERMINAL\n");

    printf("\n            %s",user);
    printf("\n\n\n***********************************\n");
}
void help()
{   
    printf("\n\n\n**********************************\n");
    printf("\n\n\n        MENU DE AJUDA\n");
    printf("\n\n COMANDOS:");
    printf("\n- cd");
    printf("\n- exit");
    printf("\n- help");
    printf("\n- Qualquer comando UNIX shell");

    printf("\n\n\n***********************************\n");
}

void showDir() {
   // char* user = getenv("USER");
    char* currentDir;
    if( (currentDir = getcwd(NULL, 0)) == NULL) {
        perror("failed to get current directory\n");
    }           
    getcwd(currentDir, sizeof(currentDir));
    //char* dir = strcat(user,"@");
    //dir = strcat(dir,currentDir);

    printf("\n%s@%s ",getenv("USER"),currentDir);
    free(currentDir);
}


int inputManager(char* input){

    char* temp;
    //char* dir = showDir();
   // printf("\n%s ",dir);
    temp = readline(">>> ");
    if(strlen(temp) != 0){
        add_history(temp);
        strcpy(input, temp);
        return 0;
    }
    else
        return 1;
}
int customCmdManager(char** input){

    int cmdTotal = 3;
    char* cmdList[cmdTotal];
    int cmd = 0;
    cmdList[0] = "exit";
    cmdList[1] = "help";
    cmdList[2] = "cd";
    
    for(int i = 0; i < cmdTotal; i++){
        if(strcmp(input[0], cmdList[i]) == 0){
            cmd = i+1;
        }
    }

    switch(cmd){
        case 1:
            exit(0);
        case 2:
            help();
            return 1;
        case 3:
            chdir(input[1]);
            return 1;
        default:
            break;
    }
    return 0;
}
void spaceInput(char* input, char** wordlist){
    for(int i = 0; i< LIST; i++){
        wordlist[i] = strsep(&input, " ");

         if (wordlist[i] == NULL) 
            break;

        if (strlen(wordlist[i]) == 0) 
            i--;
    }
}

int parseInput(char* input,  char** wordlist){

    spaceInput(input,wordlist);

    if(customCmdManager(wordlist))
        return 0;
    return 1;
}

void execCommands(char** wordlist){

    pid_t pid = fork();

    if(pid == -1){
        printf("\nFailed to create a child process");
        exit(1);
    }
    else if(pid == 0){
        if(execvp(wordlist[0], wordlist) < 0)
         perror("ERROR");
        exit(0);
    }
    else{
        wait(NULL);
        return;
    }
}



int main(void){
    char input[WORD], *wordlist[LIST];
    int execflag = 0;
    startup();
    while(1){
        showDir();
    
        if(inputManager(input))
            continue;

        execflag = parseInput(input, wordlist);
        if(execflag == 1)
            execCommands(wordlist);

    }
    return 0;
}