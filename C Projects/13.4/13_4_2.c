#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Function to allocate memory for an array
void memAlloc(double **p, int n)
{
    *p = malloc(n * sizeof(double));
    if (*p == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
}

// Function to fill an array with sine values, now using decimal increments
void fillArr(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        double x = (double)i / 10.0; // Calculate x as a multiple of 0.1
        p[i] = sin(x);
    }
}

void addNoise(double *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        double variation = (double)(rand() % 20 - 10) / 100.0;
        p[i] += variation;
    }
}

#define GNUPLOT_COMMAND "gnuplot -p"
#define DATA_FILE_BEFORE "sin_data_before.txt"
#define DATA_FILE_AFTER "sin_data_after.txt"

int main()
{
    printf("Enter the array size\n");
    int size;
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Error: Invalid array size input.\n");
        return 1;
    }

    double *arr = NULL;

    srand(time(NULL));

    memAlloc(&arr, size);
    fillArr(arr, size);

    // --- Data file generation BEFORE noise ---
    FILE *dataFileBefore = fopen(DATA_FILE_BEFORE, "w");
    if (dataFileBefore == NULL)
    {
        perror("Error opening data file (before)");
        free(arr); // Free memory before exiting
        return 1;
    }

    for (int i = 0; i < size; i++)
    {
        double x = (double)i / 10.0; // Calculate x for plotting purposes
        fprintf(dataFileBefore, "%f %lf\n", x, arr[i]); // x = 0.1*i, y = arr[i]
    }
    fclose(dataFileBefore);
    printf("Data (before noise) saved to %s\n", DATA_FILE_BEFORE);

    addNoise(arr, size);

    // --- Data file generation AFTER noise ---
    FILE *dataFileAfter = fopen(DATA_FILE_AFTER, "w");
    if (dataFileAfter == NULL)
    {
        perror("Error opening data file (after)");
        free(arr); // Free memory before exiting
        return 1;
    }

    for (int i = 0; i < size; i++)
    {
        double x = (double)i / 10.0; // Calculate x for plotting purposes
        fprintf(dataFileAfter, "%f %lf\n", x, arr[i]); // x = 0.1*i, y = arr[i]
    }
    fclose(dataFileAfter);
    printf("Data (after noise) saved to %s\n", DATA_FILE_AFTER);

    // --- Gnuplot plotting ---
    FILE *gnuplotPipe = popen(GNUPLOT_COMMAND, "w");
    if (gnuplotPipe == NULL)
    {
        perror("Error opening gnuplot pipe");
        free(arr); // Free memory before exiting
        return 1;
    }

    fprintf(gnuplotPipe, "set title 'Sine Wave: Before and After Noise (0.1 increments)'\n");
    fprintf(gnuplotPipe, "set xlabel 'x (0.1 increments)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Value'\n");
    fprintf(gnuplotPipe, "plot '%s' with lines title 'Before Noise', '%s' with lines title 'After Noise'\n", DATA_FILE_BEFORE, DATA_FILE_AFTER);

    fflush(gnuplotPipe); // Ensure commands are sent

    fprintf(gnuplotPipe, "pause 5\n"); // Keep plot open for 5 seconds, remove or adjust as needed
    pclose(gnuplotPipe);

    // Free the allocated memory
    free(arr);

    return 0;
}