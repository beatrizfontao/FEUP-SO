#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

#define READ_END 0
#define WRITE_END 1
#define LINESIZE 256
#define MAXSIZE 2048

struct pair{
    char first[15];
    char second[15];
};

void readFile(char *text, FILE *file_pointer) {
    char c;

    while (1){
        c = getc(file_pointer);
        if(c == EOF){
            c = '\n';
            strncat(text, &c, 1);
            break;
        }

        strncat(text, &c, 1); //copia para uma string o conteudo do ficheiro de texto
    }
    fclose(file_pointer);
}


void string_replace(char *haystack, char *needle, char *replacement) {
    char* find = strstr(haystack,needle);
    if(find == NULL)
        return;
    memmove(find + strlen(replacement),
            find + strlen(needle),
            strlen(haystack) - strlen(needle) + 1);

    memcpy(find, replacement, strlen(replacement));
}

void readCipher(FILE *file_pointer_c, struct pair *cypher) {
    char c;
    char temp[15] = "";
    int count = 0;
    while (1){
        c = getc(file_pointer_c);
        if(c == EOF){
            break;
        }
        if(c == ' '){
            strcpy(cypher[count].first,temp);
            strcpy(temp,"");
        }
        else if(c == '\n'){
            strcpy(cypher[count].second,temp);
            strcpy(temp,"");
            count++;
        } else
        strncat(temp, &c, 1); //copia para uma string o conteudo do ficheiro de texto
    }
    fclose(file_pointer_c);
}

int countCyphers(FILE *file_pointer_c){
    char c;
    int count = 0;
    while (1){
        c = getc(file_pointer_c);
        if(c == EOF){
            break;
        }
        if(c == '\n'){
            count++;
        }
    }
    fclose(file_pointer_c);
    return count;
}

int main(int argc, char *argv[]) {
    int nbytes, fd[2], fd2[2];
    pid_t pid;
    char text[MAXSIZE];
    strcpy(text, "");
    char buffer[LINESIZE];
    FILE *file_pointer_c = NULL, *file_pointer_c2 = NULL;

    if ((file_pointer_c = fopen("cypher.txt", "r"))==NULL){
        printf("Cant read file\n");
        return EXIT_FAILURE;
    }

    if ((file_pointer_c2 = fopen("cypher.txt", "r"))==NULL){
        printf("Cant read file\n");
        return EXIT_FAILURE;
    }

    if (pipe(fd) < 0) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd2) < 0) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) { 
/* parent */
        while(fgets(buffer,LINESIZE, stdin)){
            strcat(text, buffer);
        }
        close(fd[READ_END]);

        if ((nbytes = write(fd[WRITE_END], text, strlen(text))) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }
        close(fd[WRITE_END]);

/* wait for child and exit */
        if (waitpid(pid, NULL, 0) < 0) {
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
        }

        close(fd2[WRITE_END]);

        if ((nbytes = read(fd2[READ_END], text, MAXSIZE)) < 0) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
        close(fd2[READ_END]);
/* write message from parent */
        write(STDOUT_FILENO, text, nbytes);

        exit(EXIT_SUCCESS);
    } else {
/* child */
        close(fd[WRITE_END]);
        if ((nbytes = read(fd[READ_END], text, MAXSIZE)) < 0) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
        close(fd[READ_END]);

        int n = countCyphers(file_pointer_c2);
        struct pair cypher[n];
        readCipher(file_pointer_c, cypher);
        
        //Deciphering
        char temp[15];
        strcpy(temp, "");
        for(int i = 0; text[i] != '\0'; i++){
            if(isalpha(text[i]) == 0){
                if(temp[0] == '\0'){
                    strcpy(temp,"");
                    continue;
                }
                for(int j = 0; j < n; j++){
                    if(strcmp(temp, cypher[j].first) == 0){
                        string_replace(text, cypher[j].first, cypher[j].second);
                        break;
                    }
                    else if(strcmp(temp, cypher[j].second) == 0){
                        string_replace(text, cypher[j].second, cypher[j].first);
                        break;
                    }
                }
                strcpy(temp,"");
            }
            else strncat(temp, &text[i], 1);
        }

        close(fd2[READ_END]);

        if ((nbytes = write(fd2[WRITE_END], text, strlen(text))) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }
        close(fd2[WRITE_END]);
/* exit gracefully */
        exit(EXIT_SUCCESS);
    }
}

