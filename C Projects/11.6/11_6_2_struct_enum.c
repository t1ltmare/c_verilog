#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 50
#define MAX_AUTHORS 3

typedef struct {
    char authors[MAX_AUTHORS][MAX_NAME_LENGTH];
    char bookName[MAX_NAME_LENGTH];
    char ISBN[MAX_NAME_LENGTH];
    int year;
} Library;

void main()
{
    Library books[] = {
        {{"Ivanov I.I.", "Petrov P.P.", "Sidorov S.S."}, "Programming Basics", "978-5-123-45678-9", 2020},
        {{"Smirnov A.A."}, "Algorithms and Data Structures", "978-5-987-65432-1", 2018},
        {{"Kuznetsov V.V.", "Skillbox Expert"}, "Introduction to Machine Learning", "978-5-456-78901-2", 2022},
        {{"Lermontov M.Yu."}, "A Hero of Our Time", "978-5-789-01234-5", 1840},
        {{"Tolstoy L.N."}, "War and Peace", "978-5-234-56789-0", 1869},
        {{"Chekhov A.P."}, "The Cherry Orchard", "978-5-890-12345-6", 1904},
        {{"Skillbox Guide", "Expert"}, "Skillbox Guide to Success", "978-5-345-67890-1", 2024}
    };

    int numBooks = sizeof(books) / sizeof(books[0]);

    char keyword[MAX_NAME_LENGTH];
    printf("Enter a keyword to search in book titles: ");

    if (fgets(keyword, MAX_NAME_LENGTH, stdin) == NULL) {
        perror("Input reading error");
        exit(1);
    }

    keyword[strcspn(keyword, "\n")] = 0;

    int found = 0;
    printf("\nSearch results:\n");

    for (int i = 0; i < numBooks; i++) {
        if (strstr(books[i].bookName, keyword) != NULL) {
            printf("  Title: %s\n", books[i].bookName);
            printf("  Authors: ");
            for (int j = 0; j < MAX_AUTHORS; j++) {
                if (strlen(books[i].authors[j]) > 0) {
                    printf("%s", books[i].authors[j]);
                    if (j < MAX_AUTHORS - 1 && strlen(books[i].authors[j+1]) > 0) {
                         printf(", ");
                    }
                }
            }
            printf("\n");
            printf("  Year: %d\n", books[i].year);
            printf("\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No books found with the keyword '%s'.\n", keyword);
    }
}