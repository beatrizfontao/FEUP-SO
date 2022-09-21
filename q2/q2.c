#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/mman.h>

void *create_shared_memory(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, protection, visibility, -1, 0);
}

int main(int argc, char *argv[])
{

    if(argc == 1){
        printf("Usage: addmx file1 file2\n");
        return EXIT_SUCCESS;
    }
    else if (argc != 3){
        printf("Wrong number of arguments.\nUsage: addmx file1 file2\n");
        return EXIT_FAILURE;
    }

    FILE *file_pointer1;
    FILE *file_pointer2;

    int m, n, m1, n1;

    if ((file_pointer1 = fopen(argv[1], "r")) == NULL) // se o ficheiro nao abrir direito da erro
    {
        return EXIT_FAILURE;
    }

    if ((file_pointer2 = fopen(argv[2], "r")) == NULL) // se o ficheiro nao abrir direito da erro
    {
        return EXIT_FAILURE;
    }

    // dimensoes da primeira matriz
    fscanf(file_pointer1, "%dx%d\n", &n, &m);
    // dimensoes da segunda matriz
    fscanf(file_pointer2, "%dx%d\n", &n1, &m1);

    if (n != n1 || m != m1)
    {
        printf("Matrix dimensions do not match\n");
        return EXIT_FAILURE;
    }

    int index = 0;
    int matrix1[n * m];
    int matrix2[n * m];

    // preencher as matrizes
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fscanf(file_pointer1, "%d", &matrix1[index]);
            fscanf(file_pointer2, "%d", &matrix2[index]);
            index++;
            if (j != m - 1)
            {
                fscanf(file_pointer1, " ");
                fscanf(file_pointer2, " ");
            }
        }
    }

    fclose(file_pointer1);
    fclose(file_pointer2);

    // criar 3 shared memories para cada matriz e para a soma
    int *shm_matrix1 = (int *)create_shared_memory(n * m * sizeof(int));
    int *shm_matrix2 = (int *)create_shared_memory(n * m * sizeof(int));
    int *shm_matrixsum = (int *)create_shared_memory(n * m * sizeof(int));

    /// copiar o conteudo das matrizes para a shared memory
    memcpy(shm_matrix1, matrix1, sizeof(matrix1));
    memcpy(shm_matrix2, matrix2, sizeof(matrix2));

    int column = -1; // coluna a ser atribuida a cada processo, começa a -1 para ser incrementada para 0 antes de começar

    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        column++;
        if (pid == 0) // verifica se é processo filho
        {
            for (int line = 0; line < m; line++)
            {
                shm_matrixsum[column + (n * line)] = shm_matrix1[column + (n * line)] + shm_matrix2[column + (n * line)];
            }
            exit(0);
        }
        wait(NULL); // processo pai espera
    }

    index = 0;
    printf("%dx%d\n", n, m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%d ", shm_matrixsum[index]);
            if (j == m - 1)
            {
                printf("\n");
            }
            index++;
        }
    }

    return EXIT_SUCCESS;
}
