#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/utsname.h>
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
void Touch(char resztaStringa[]);
void Cat(char resztaStringa[]);
void Ls(char resztaStringa[]);
void Echo(char resztaStringa[]);
void Cd(char resztaStringa[]);
void History();

char history[25][25];
char sciezka[100];
char user[25];
extern char *cuserid(char *);
FILE *fp;

int i=0;

void KopiujDoSpacji(char* dest, char* src) {
  while (*src && *src != ' ') {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';
}

void KopiujPoSpacji(char* dest, char* src) {
  while (*src && *src != ' ') {
    src++;
  } if (*src) {
    src++;
  } while (*src) {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';
}

int main() {
    getcwd(sciezka, sizeof(sciezka));
    cuserid(user);
    char polecenie[100];
    char reszta[100];

    while(TRUE){
        char tekst[100];
        printf("[" CYN "%s" RESET ":" RED "%s" RESET "]\n" GRN "$ " RESET,user,sciezka);
        fgets(tekst, 100, stdin);
        tekst[strcspn(tekst, "\n")] = 0;
        strcpy(history[i], tekst);
        KopiujDoSpacji(polecenie, tekst);
        KopiujPoSpacji(reszta, tekst);
        if (strcmp(polecenie, "help") == 0){
            Help();
        } else if (strcmp(polecenie, "touch") == 0){
            Touch(reszta);
        } else if (strcmp(polecenie, "cat") == 0){
            Cat(reszta);
            continue;
        } else if (strcmp(polecenie, "ls") == 0) {
            Ls(reszta);
        } else if (strcmp(polecenie, "echo") == 0){
            Echo(reszta);
        } else if (strcmp(polecenie, "cd") == 0){
            Cd(reszta);
        } else if (strcmp(polecenie, "history") == 0) {
            History(reszta);
        } else if (strcmp(polecenie, "exit") == 0){
            exit(1);
        } else if (strcmp(polecenie, "clear") == 0){
            printf("\e[1;1H\e[2J"); // Using REGEX
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                char *av[100];
                char *token = strtok(tekst, " ");
                int i = 0;
                while (token != NULL)
                {
                  av[i] = token;
                  token = strtok(NULL, " ");
                  i++;
                }
                av[i] = NULL;

                execvp(av[0], av);
                exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
                if (status != 0)
                {
                  printf("Error: błąd polecenia '%s'\n", tekst);
                }
              }
              else
              {
                perror("fork");
                exit(1);
              }
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
    printf("help, touch, cat, ls, echo, cd, history, exit, clear\n");
}

void Touch(char resztaStringa[]){
    fp = fopen(resztaStringa,"w");
}

void Cat(char resztaStringa[]){
    char a[50];
    char b[50];
    KopiujDoSpacji(a,resztaStringa);
    KopiujPoSpacji(b,resztaStringa);

    if (strcmp(a,">") == 0){
        fp = fopen(b, "w");
        FILE *plik = fopen(b, "a");
        char zdanie[50];
        while (TRUE){
            scanf("%s", zdanie);
            if (strcmp(zdanie, "EOF")==0) break;
            fprintf(plik,"%s\n", zdanie);
        }
        fclose(plik);
    } else if (strcmp(a,">>") == 0) {
        FILE *plik = fopen(b, "a");
        char zdanie[50];
        while (TRUE){
            scanf("%s", zdanie);
            if (strcmp(zdanie, "EOF")==0) break;
            fprintf(plik,"%s\n", zdanie);
        }
        fclose(plik);
    } else {
        int ret;
        ret=isDir(a);
        FILE *fptr;
        fptr = fopen(a, "r");
        if (fptr == NULL) {
            printf("cat: %s: No such file or directory\n",a);
        } else if (ret==0) {
            printf("cat: %s: Is a directory\n",a);
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
}

void Ls(char resztaStringa[]){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if ((d)&&((strcmp(resztaStringa,"-a")==0) || (strcmp(resztaStringa,"-all")==0))){
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    else if (d) {
        while ((dir = readdir(d)) != NULL) {
        if(dir->d_name[0] != '.') {
            printf("%s\n", dir->d_name);
        }
    }
    closedir(d);
    }
}

void Echo(char resztaStringa[]){
    printf("%s\n", resztaStringa);
}

void Cd(char resztaStringa[]){
    if (strcmp(resztaStringa, "~") == 0)chdir(getenv("HOME"));
    else if (strcmp(resztaStringa, "..") == 0) chdir("..");
    else if (chdir(resztaStringa) == -1)
        printf("cd: %s: No such file or directory\n",resztaStringa);
    else chdir(resztaStringa);
    getcwd(sciezka, sizeof(sciezka));
}

void History(){
    for (int k=0; k<i; k++) printf("%s\n",history[k]);
}
