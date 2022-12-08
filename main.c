#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  //Biblioteka z pwd, whoami
#define CYN   "\x1B[36m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define GRN   "\x1B[32m"
#define TRUE 1

char Help();

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
            printf("%s", sciezka);
        } else if (strcmp(polecenie, "exit") == 0){
            exit(0);
        } else {
            printf("Unknown command");
        }

        printf("\n");
    }

   return 0;
}

char Help(){
    printf("----------------------\n");
    printf("  Autor: P.Langowski  \n");
    printf("----------------------\n");
    printf("Obslugiwane funkcje:\n");
    printf("help, pwd, cd ...");
}
