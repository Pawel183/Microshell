#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <ctype.h>

#define CYN   "\x1B[36m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define YEL   "\x1B[33m"
#define GRN   "\x1B[32m"
#define TRUE 1


int isDir(const char* fileName){
    struct stat path;
    stat(fileName, &path);
    return S_ISREG(path.st_mode);
}

void Help();
void Cat(char resztaStringa[]);
void Echo(char resztaStringa[]);
void Cd(char resztaStringa[]);
void History();

extern char *cuserid(char *);
FILE *fp;

void KopiujDoSpacji(char* dest, char* src) {
    while (*src && *src != ' ') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

void KopiujPoSpacji(char* dest, char* src) {
    while (*src && *src != ' ')
        src++;
    if (*src) {
    src++;
    } while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

char history[100][100];
char sciezka[100];
char user[100];
char poprzedniaSciezka[100];
int q=0;
int z=0;
int k=0;

int main() {
    cuserid(user);
    char polecenie[100];
    char reszta[100];
    char tekst[100];

    getcwd(poprzedniaSciezka, sizeof(poprzedniaSciezka));

    while(TRUE){
        getcwd(sciezka, sizeof(sciezka));
        if (k==0) {
            printf("[" CYN "%s" RESET ":" RED "%s" RESET "]\n" GRN "$ " RESET,user,sciezka);
        }
        fgets(tekst, 100, stdin);
        tekst[strcspn(tekst, "\n")] = 0;
        char tekstBezSpacji[100];
        int i=0,j=0;
        while(isspace(tekst[i])) {
            i++;
        }
        while(tekst[i] != '\0') {
            if(!isspace(tekst[i]) || (isspace(tekst[i]) && !isspace(tekst[i-1]))) {
                tekstBezSpacji[j] = tekst[i];
                j++;
            }
            i++;
        }
        tekstBezSpacji[j] = '\0';
        strcpy(tekst, tekstBezSpacji);
        strcpy(history[z], tekst);
        KopiujDoSpacji(polecenie, tekst);
        KopiujPoSpacji(reszta, tekst);
        if (strcmp(polecenie, "\0") == 0){
            k=0;
            continue;
        }


        if (strcmp(polecenie, "help") == 0){
            Help();
        } else if (strcmp(polecenie, "cat") == 0){
            Cat(reszta);
            continue;
        } else if (strcmp(polecenie, "cd") == 0){
            Cd(reszta);
        } else if (strcmp(polecenie, "history") == 0){
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
                while (token != NULL) {
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
                   printf("Error: błąd polecenia '%s'\n", tekst);
            } else {
                perror("fork");
                exit(1);
            }
        }
        z++;
    }
    return 0;
}

void Help(){
    printf(YEL "----------------------\n");
    printf("  Autor: P.Langowski  \n");
    printf("----------------------\n" RESET);
    printf("Obslugiwane funkcje:\n");
    printf("cat 'nazwa_pliku'\n");
    printf("cat > 'nazwa_pliku' (wpisac EOF, zeby wyjsc i zapisac plik)\n");
    printf("cat >> 'nazwa_pliku' (wpisac EOF, zeby wyjsc i dopisac do pliku)\n");
    printf("cd 'sciezka' , cd ..  ,  cd ~ \n");
    printf("history\n");
    printf("help, exit, clear\n");
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
        k=1;
        fclose(plik);
    } else if (strcmp(a,">>") == 0) {
        FILE *plik = fopen(b, "a");
        char zdanie[50];
        while (TRUE) {
            scanf("%s", zdanie);
            if (strcmp(zdanie, "EOF")==0) break;
            fprintf(plik,"%s\n", zdanie);
        }
        k=1;
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
            while (c != EOF) {
                printf ("%c", c);
                c = fgetc(fptr);
            }
            fclose(fptr);
        }
    }
}

void Cd(char resztaStringa[]){
    if (strcmp(resztaStringa, "~") == 0){
        getcwd(poprzedniaSciezka, sizeof(poprzedniaSciezka));
        chdir(getenv("HOME"));
        q++;
    } else if (strcmp(resztaStringa, "..") == 0) {
        getcwd(poprzedniaSciezka, sizeof(poprzedniaSciezka));
        chdir("..");
        q++;
    } else if (strcmp(resztaStringa, "-") == 0) {
        if (q==0) printf("bash: cd: OLDPWD not set\n");
        else {
            printf("%s\n", poprzedniaSciezka);
            chdir(poprzedniaSciezka);
        }
    } else if (chdir(resztaStringa) == -1) {
        getcwd(poprzedniaSciezka, sizeof(poprzedniaSciezka));
        printf("cd: %s: No such file or directory\n",resztaStringa);
        q++;
    } else {
        chdir(resztaStringa);
        q++;
    }
}

void History(){
    for (int k=0; k<z; k++) printf("%s\n",history[k]);
}
