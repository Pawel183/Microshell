#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // cuserid, getcwd, chdir(getenv)
#include <dirent.h> // ls
#define CYN   "\x1B[36m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define YEL   "\x1B[33m"
#define GRN   "\x1B[32m"
#define TRUE 1

void Help();
void Pwd();
void Touch();
void Cat();
void Rm();
void Ls(int a);
void Echo();
void Cd();
void History();

char history[25][25];
char sciezka[100];
char user[25];

int i=0;

int main() {
    getcwd(sciezka, sizeof(sciezka));
    cuserid(user);

    while(TRUE){
        printf("[" CYN "%s" RESET ":" RED "%s" RESET "]\n" GRN "$ " RESET,user,sciezka);
        char polecenie[100];
        scanf("%s",polecenie);
        strcpy(history[i], polecenie);

        if (strcmp(polecenie, "help") == 0){
            Help();
        } else if (strcmp(polecenie, "pwd") == 0){
            Pwd(sciezka);
        } else if (strcmp(polecenie, "touch") == 0){
            Touch();
        } else if (strcmp(polecenie, "cat") == 0){
            Cat();
        } else if (strcmp(polecenie, "rm") == 0){
            Rm();
        } else if (strcmp(polecenie, "ls") == 0) {
            Ls(1);
        } else if (strcmp(polecenie, "echo") == 0){
            Echo();
        } else if (strcmp(polecenie, "cd") == 0){
            Cd(sciezka);
        } else if (strcmp(polecenie, "history") == 0) {
            History();
        } else if (strcmp(polecenie, "exit") == 0){
            exit(1);
        } else if (strcmp(polecenie, "clear") == 0){
            printf("\e[1;1H\e[2J"); // Using REGEX
        } else {
            printf("Blad polecenia: No such file or directory\n");
        }
        i++;
    }
   return 0;
}

void Help(){
    printf(YEL "----------------------\n");
    printf("  Autor: P.Langowski  \n");
    printf("----------------------\n" RESET);
    printf("Obslugiwane funkcje:\n");
    printf("help, pwd, touch, cat, rm, ls, echo, cd, history, exit, clear\n");
}

void Pwd(){
    getcwd(sciezka, sizeof(sciezka));
    printf("%s\n", sciezka);
}

void Touch(){
    char touch[50];
    scanf("%s",touch);
    strcat(strcat(history[i], " "), touch);
    FILE *fp;
    fp = fopen(touch, "w");
}

void Cat(){
    char cat[50];
    scanf("%s",cat);
    strcat(strcat(history[i], " "), cat);
    FILE *fptr;
    fptr = fopen(cat, "r");
    if (fptr == NULL) {
        printf("cat: %s: No such file or directory\n",cat);
    } else {
        char c;
        c = fgetc(fptr);
        while (c != EOF)
        {
            printf ("%c", c);
            c = fgetc(fptr);
        }
        fclose(fptr);
    }
}

void Rm(){
    char rm[50];
    scanf("%s", rm);
    strcat(strcat(history[i], " "), rm);
    if (remove(rm) != 0) printf("rm: cannot remove 'rm': No such file or directory\n");
}

void Ls(int a){
    DIR *d;  //directory stream
    struct dirent *dir;
    d = opendir(".");
    if ((d) && (a==0)) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    else if ((d) && (a!=0)) {
        while ((dir = readdir(d)) != NULL) {
        if(dir->d_name[0] != '.') {
            printf("%s\n", dir->d_name);
        }
    }
    closedir(d);
    }
}

void Echo(){
    char echo[50];
    scanf("%[^\n]",echo);
    strcat(history[i], echo);
    printf("%s\n", echo);
}

void Cd(){
char cd[50];
    scanf("%s",cd);
    strcat(strcat(history[i], " "), cd);
    if (strcmp(cd, "~") == 0) chdir(getenv("HOME"));
//    else if (chdir(x) == -1) printf("cd: %s: No such file or directory\n",x);
    else chdir(cd);
    getcwd(sciezka, sizeof(sciezka));
}

void History(){
    for (int k=0; k<i; k++) printf("%s\n",history[k]);
}


