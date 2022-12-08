#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define CYN   "\x1B[36m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define GRN   "\x1B[32m"
#define TRUE 1

void Help();

int main() {
    char sciezka[300];
    getcwd(sciezka, sizeof(sciezka));

    while(TRUE){
        printf("[" CYN "USER" RESET ":" RED "%s" RESET "]\n" GRN "$ " RESET,sciezka);
        char polecenie[100];
        scanf("%s",polecenie);
        if (strcmp(polecenie, "help") == 0){
            Help();
        } else if (strcmp(polecenie, "pwd") == 0){
            getcwd(sciezka, sizeof(sciezka));
            printf("%s\n", sciezka);
        } else if (strcmp(polecenie, "echo") == 0){
            char echo[50];
            scanf("%[^\n]",echo);
            printf("%s\n", echo);
        } else if (strcmp(polecenie, "cd") == 0){
            char cd[50];
            scanf("%s",cd);
            if (strcmp(cd, "~") == 0) chdir(getenv("HOME"));
//            else if (chdir(x) == -1) printf("cd: %s: No such file or directory\n",x);
            else chdir(cd);
            getcwd(sciezka, sizeof(sciezka));
        } else if (strcmp(polecenie, "exit") == 0){
            exit(1);
        } else if (strcmp(polecenie, "clear") == 0){
            system("clear");
        } else {
            printf("Unknown command\n");
        }
    }
   return 0;
}

void Help(){
    printf("----------------------\n");
    printf("  Autor: P.Langowski  \n");
    printf("----------------------\n");
    printf("Obslugiwane funkcje:\n");
    printf("help, echo, pwd, cd ... exit\n");
}
