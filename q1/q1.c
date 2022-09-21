#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024 

int main(int argc, char *argv[])
{

    if (argv[1] == NULL){
        printf("usage: phrases [-l] file\n");
        return 0;
    }

    FILE *file_pointer;

    if (strcmp(argv[1], "-l") != 0){
    if ((file_pointer = fopen(argv[1], "r")) == NULL) // se o ficheiro nao abrir direito da erro
    {
        return EXIT_FAILURE;
    }
    }

    else{
	    if ((file_pointer = fopen(argv[2], "r"))==NULL){
		    return EXIT_FAILURE;
	    }
    }

    char c, lastc;
    char text[MAX_SIZE] = "";

    int num = 0;

    while (1){
        c = getc(file_pointer);
        if(c == EOF){
            if(lastc != '.' && lastc != '!' && lastc != '?')
                num ++;
            break;
        }
        else if (c == '\n'){
            if (lastc != '.' && lastc != '!' && lastc != '?')
                strncat(text, &c, 1);
            continue;
        }
            
        strncat(text, &c, 1); //copia para uma string o conteudo do ficheiro de texto
        lastc = c;

        if (c == '.' || c == '!' || c == '?'){
            num ++;
            getc(file_pointer); //para eliminar o espaço que exite no início das frases
        }
    }

    fclose(file_pointer);

    

    if(strcmp(argv[1], "-l") == 0){
	    int j = 1;
	    printf("[%d] ", j);
        for (int i = 0; i < sizeof(text); i++)
        {
            printf("%c", text[i]); //dá print do conteudo do ficheiro
            if (text[i] == '.' || text[i] == '!' || text[i] == '?')
            {   
                printf("\n");
	            j++;
                if (j <= num)
                    printf("[%d] ", j);
            }
            
        }
        printf("\n");
    }
    else
        printf("\nNumber of phrases: %d\n\n", num);

    return EXIT_SUCCESS;
}