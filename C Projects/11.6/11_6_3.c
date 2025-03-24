#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 50
#define NUM_EMPLOYEES 15

// TODO 15 сотрудников компании
// TODO ФИО, возраст, пол, должность
// TODO Посчитать средний и медианный возраст сотрудников по половой принадлежности
// TODO Посчитать количество мужчин и женщин
// TODO Посчитать средний и медианный возраст всех сотрудников

typedef struct 
{
    char fio[MAX_LENGTH];
    int age;
    char gen;
    char pos[MAX_LENGTH];
}employee;

void averAge(employee * p, char gen)
{
    int cnt = 0;
    int age = 0;
    for (int i = 0; i < NUM_EMPLOYEES; i++)
    {
        if (p[i].gen == gen) 
        { 
            age += p[i].age;
            cnt++;
        }
    }
    if (cnt > 0) 
    {
        printf("Number of %c employees is %d\n", gen, cnt);
        printf("Average age of %c employees is %d\n", gen, age/cnt);
    }
    else printf("No employees of gender %c found.\n", gen);
}

void mediAge(employee * p, char gen)
{
    int cnt = 0;
    int age = 0;
    int ages[NUM_EMPLOYEES];
    for (int i = 0; i < NUM_EMPLOYEES; i++)
    {
        if (p[i].gen == gen) 
        { 
            ages[cnt] = p[i].age;
            age += p[i].age;
            cnt++;
        }
    }
    for (int i = 0; i < cnt; i++)
    {
        for (int j = 0; j < cnt - i - 1; j++)
        {
            if (ages[j] > ages[j + 1])
            {
                int temp = ages[j];
                ages[j] = ages[j + 1];
                ages[j + 1] = temp;
            }
        }
    }
    printf("Median age of %c is %d\n", gen, ages[cnt/2]);
}

void main()
{
    employee employees[NUM_EMPLOYEES] = {
        {"Alice Smith", 32, 'F', "Software Engineer"},
        {"Bob Johnson", 45, 'M', "Project Manager"},
        {"Charlie Brown", 28, 'M', "Data Analyst"},
        {"Diana Davis", 35, 'F', "Marketing Manager"},
        {"Eve Wilson", 24, 'F', "Junior Developer"},
        {"Frank Garcia", 52, 'M', "Senior Architect"},
        {"Grace Rodriguez", 30, 'F', "UI/UX Designer"},
        {"Henry Martinez", 48, 'M', "Sales Director"},
        {"Ivy Anderson", 26, 'F', "Quality Assurance Tester"},
        {"Jack Taylor", 40, 'M', "Team Lead"},
        {"Kelly White", 33, 'F', "Business Analyst"},
        {"Liam Moore", 29, 'M', "System Administrator"},
        {"Mia Jackson", 37, 'F', "HR Manager"},
        {"Noah Harris", 43, 'M', "Finance Director"},
        {"Olivia Clark", 27, 'F', "Technical Writer"}
    };

    averAge(employees, 'M');
    mediAge(employees, 'M');
    averAge(employees, 'F');
    mediAge(employees, 'F');
}
