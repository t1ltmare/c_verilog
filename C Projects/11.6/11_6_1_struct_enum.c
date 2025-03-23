#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

// TODO 1: Узнать количество учеников                                   DONE
// TODO 2: Заполнить данные о каждом ученике из консоли                  DONE
// TODO 3: Подсчитать средний балл учеников                             DONE
// ?       Посчитать количество учеников в классе
// TODO 5: Ранжировать учеников в классе по успеваемости                 DONE
// TODO 6: Расситать зависимость между успеваемостью и полом ученика
// ?       Сделать ввод в консоль до команды stop

typedef struct
{
    char surn[MAX_NAME_LENGTH];     // Фамилия
    char name[MAX_NAME_LENGTH];     // Имя
    char patr[MAX_NAME_LENGTH];     // Отчество
    char gend[MAX_NAME_LENGTH];     // Пол
    int  grad;                      // Класс
    char subj[MAX_NAME_LENGTH];     // Предмет
    int  scor;                      // Оценка за урок
} puple;

void addPuple(puple * p)
{
    printf("\nAdding a new pupil\n");
    printf("Enter puple's surname\n");
    scanf("%s", p->surn);
    printf("Enter puple's name\n");
    scanf("%s", p->name);
    printf("Enter puple's patronymic\n");
    scanf("%s", p->patr);
    printf("Enter puple's gender\n");
    scanf("%s", p->gend);
    printf("Enter puple's grade\n");
    scanf("%d", &p->grad);
    printf("Enter puple's subject\n");
    scanf("%s", p->subj);
    printf("Enter puple's score\n");
    scanf("%d", &p->scor);
}

void prtPuple(puple * p)
{
    printf("The puple's surname %s\n", p -> surn);
    printf("The puple's name %s\n", p -> name);
    printf("The puple's patronymic %s\n", p -> patr);
    printf("The puple's gender %s\n", p -> gend);
    printf("The puple's grade %d\n", p -> grad);
    printf("The puple's subject %s\n", p-> subj);
    printf("The puple's score %d\n", p->scor);
}

void rankByScore(puple pupils[], int count)
{
    // Пузырьковая сортировка
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (pupils[j].scor < pupils[j + 1].scor) {
                puple temp = pupils[j];
                pupils[j] = pupils[j + 1];
                pupils[j + 1] = temp;
            }
        }
    }

    // Вывод результата
    printf("\nPupils ranked by score:\n");
    for (int i = 0; i < count; i++) {
        printf("Name: %s, Score: %d\n", pupils[i].name, pupils[i].scor);
    }
}

void calculateAverageScore(puple pupils[], int count)
{
    int totalScore = 0;
    for (int i = 0; i < count; i++) {
        totalScore += pupils[i].scor;
    }
    float averageScore = (float)totalScore / count;
    printf("\nAverage score of pupils: %.2f\n", averageScore);
}

void calculatePerformanceByGender(puple pupils[], int count)
{
    int maleScore = 0, femaleScore = 0;
    int maleCount = 0, femaleCount = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(pupils[i].gend, "male") == 0) {
            maleScore += pupils[i].scor;
            maleCount++;
        } else if (strcmp(pupils[i].gend, "female") == 0) {
            femaleScore += pupils[i].scor;
            femaleCount++;
        }
    }

    float malePerformance = (maleCount > 0) ? (float)maleScore / maleCount : 0;
    float femalePerformance = (femaleCount > 0) ? (float)femaleScore / femaleCount : 0;

    printf("\nPerformance by gender:\n");
    printf("Male performance: %.2f%%\n", malePerformance);
    printf("Female performance: %.2f%%\n", femalePerformance);
}

void journal()
{
    printf("How many pupils are in the journal?\n");
    int pupleCount;
    scanf("%d", &pupleCount);
    puple pupils[pupleCount];
    for (int i = 0; i < pupleCount; i++) {
        addPuple(&pupils[i]);
    }
    rankByScore(pupils, pupleCount);
    calculateAverageScore(pupils, pupleCount);
    calculatePerformanceByGender(pupils, pupleCount);
}

int main()
{
    journal();
    return 0;
}