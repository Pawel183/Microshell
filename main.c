#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#define CYN   "\x1B[36m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define YEL   "\x1B[33m"
#define GRN   "\x1B[32m"
#define TRUE 1

#include <sys/types.h>
#include <sys/stat.h>
int isDir(const char* fileName)
{
    struct stat path;
    stat(fileName, &path);
    return S_ISREG(path.st_mode);
}

void Help();
void Pwd();
void Pwd_2();
void Touch();
int Cat();
void Rm();
void Ls(int a);
void Echo();
void Cd();
void History();

char history[25][25];
char sciezka[100];
char user[25];
extern char *cuserid(char *);
FILE *fp;

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
            Pwd_2();
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
            printf("\e[1;1H\e[2J");
        } else {
            pid_t pid;
            pid = fork();
            char x[50]="/bin/";
            strcat(x,polecenie);
            if (pid == 0) {
              execl(x, polecenie, NULL);
              exit(0);
            } else {
              wait(NULL);
            }
            //printf("Blad polecenia: No such file or directory\n");
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

void Pwd_2(){
    pid_t pid;
    pid = fork();
    if (pid == 0) {
      execl("/bin/pwd", "pwd", NULL);
      exit(0);
    } else {
      wait(NULL);
    }
}

void Touch(){
    char touch[50];
    scanf("%s",touch);
    strcat(strcat(history[i], " "), touch);
    fp = fopen(touch, "w");

}

int Cat(){
    char cat[50];
    scanf("%s",cat);
    strcat(strcat(history[i], " "), cat);
    if (strcmp(cat,">") == 0){
        char touch[50];
        scanf("%s",touch);
        strcat(strcat(history[i], " "), touch);
        fp = fopen(touch, "w");
        FILE *plik = fopen(touch, "a");
        char tekst[50];
        while (TRUE){
            scanf("%s", tekst);
            if (strcmp(tekst, "EOF")==0) break;
            fprintf(plik,"%s\n", tekst);
        }
        fclose(plik);
    } else if (strcmp(cat,">>") == 0) {
        char touch[50];
        scanf("%s",touch);
        strcat(strcat(history[i], " "), touch);
        FILE *plik = fopen(touch, "a");
        char tekst[50];
        while (TRUE){
            scanf("%s", tekst);
            if (strcmp(tekst, "EOF")==0) break;
            fprintf(plik,"%s\n", tekst);
        }
        fclose(plik);
    } else {
        int ret;
        ret=isDir(cat);
        FILE *fptr;
        fptr = fopen(cat, "r");
        if (fptr == NULL) {
            printf("cat: %s: No such file or directory\n",cat);
        } else if (ret==0) {
            printf("cat: %s: Is a directory\n",cat);
            return 0;
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
    return 0;
}

void Rm(){
    char rm[50];
    scanf("%s", rm);
    strcat(strcat(history[i], " "), rm);
    if (remove(rm) != 0) printf("rm: cannot remove 'rm': No such file or directory\n");
}

void Ls(int a){
    DIR *d;
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
    else if (chdir(cd) == -1) printf("cd: %s: No such file or directory\n",cd);
    else chdir(cd);
    getcwd(sciezka, sizeof(sciezka));
}

void History(){
    for (int k=0; k<i; k++) printf("%s\n",history[k]);
}
