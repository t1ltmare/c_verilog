#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define GNUPLOT_COMMAND "gnuplot -p"
#define DATA_FILE_BEFORE "sin_data_before.txt"
#define DATA_FILE_AFTER "sin_data_after.txt"
#define DATA_FILE_FILTER "sin_data_filter.txt"

void memAlloc(double **p, int n)
{
    *p = malloc(n * sizeof(double));
    if (*p == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
}

void fillArr(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        double x = (double)i / 10.0;
        p[i] = sin(x);
    }
}

void addNoise(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        int noise = rand() % 40;
        double variation = (double)(rand() % 20 - 10) / 100.0;
        if (noise == 19) variation += 0.8;
        else if (noise == 18) variation -= 0.8;
        p[i] += variation;
    }
}

void middleF(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        if ((p[i] <= p[i+1]) && (p[i] <= p[i+2]))
        {
            p[i] = (p[i+1] <= p[i+2]) ? p[i+1] : p[i+2];
        } else {
            if ((p[i+1] <= p[i]) && (p[i+1] <= p[i+2]))
            {
                p[i] = (p[i] <= p[i+2]) ? p[i] : p[i+2];
            } else p[i] = (p[i] <= p[i+1]) ? p[i] : p[i+1]; 
        }
    }
}

int main()
{
    //* Ввод размера массива из консоли, проверка ввода
    printf("Enter the array size\n");
    int size;
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Error: Invalid array size input.\n");
        return 1;
    }

    double *arr = NULL;
    srand(time(NULL));

    //* Наполнение массива чисел
    memAlloc(&arr, size);
    fillArr(arr, size);

    //* Объявление файла записи исходных данных. Заполнение файла для gnuplot отсчетами и исходными значениями
    FILE *dataFileBefore = fopen(DATA_FILE_BEFORE, "w");
    if (dataFileBefore == NULL)
    {
        perror("Error opening data file (before)");
        free(arr);    
        return 1;
    }
    for (int i = 0; i < size; i++)
    {
        double x = (double)i / 10.0; 
        fprintf(dataFileBefore, "%f %lf\n", x, arr[i]); 
    }
    fclose(dataFileBefore);
    printf("Data (before noise) saved to %s\n", DATA_FILE_BEFORE);

    //* Добавление шума
    addNoise(arr, size);

    //* Объявление файла записи исходных данных. Заполнение файла для gnuplot отсчетами и зашумленными значениями
    FILE *dataFileAfter = fopen(DATA_FILE_AFTER, "w");
    if (dataFileAfter == NULL)
    {
        perror("Error opening data file (after)");
        free(arr); 
        return 1;
    }
    for (int i = 0; i < size; i++)
    {
        double x = (double)i / 10.0; 
        fprintf(dataFileAfter, "%f %lf\n", x, arr[i]); 
    }
    fclose(dataFileAfter);
    printf("Data (after noise) saved to %s\n", DATA_FILE_AFTER);

    middleF(arr, size);

    //* Объявление файла записи исходных данных. Заполнение файла для gnuplot отсчетами и зашумленными значениями
    FILE *dataFileFilter = fopen(DATA_FILE_FILTER, "w");
    if (dataFileFilter == NULL)
    {
        perror("Error opening data file (after)");
        free(arr); 
        return 1;
    }
    for (int i = 0; i < size; i++)
    {
        double x = (double)i / 10.0; 
        fprintf(dataFileFilter, "%f %lf\n", x, arr[i]); 
    }
    fclose(dataFileFilter);
    printf("Data (after filter) saved to %s\n", DATA_FILE_FILTER);

    //* Вывод плоттера
    FILE *gnuplotPipe = popen(GNUPLOT_COMMAND, "w");
    if (gnuplotPipe == NULL)
    {
        perror("Error opening gnuplot pipe");
        free(arr); // Free memory before exiting
        return 1;
    }
    fprintf(gnuplotPipe, "set title 'Sine Wave: Before, After Noise, After Filter'\n");
    fprintf(gnuplotPipe, "set xlabel 'x (0.1 increments)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Value'\n");
    fprintf(gnuplotPipe, "plot '%s' with lines title 'Before Noise', '%s' with lines title 'After Noise', '%s' with lines title 'After Filter'\n", DATA_FILE_BEFORE, DATA_FILE_AFTER, DATA_FILE_FILTER);

    fflush(gnuplotPipe); 

    fprintf(gnuplotPipe, "pause 5\n"); 
    pclose(gnuplotPipe);

    free(arr);

    return 0;
}