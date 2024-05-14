#include <stdio.h>
#include <stdlib.h>

// Define the structure before using it in any function prototypes.
typedef struct {
    char question[256]; // Using fixed-size arrays for simplicity
    char option1[100];
    char option2[100];
    char option3[100];
    char option4[100];
    char correct_answer;
} questions;

// Correct the function prototypes
int load_questions(const char* q_file_name, questions** question);
void quiz_start(questions *question, int size);
void results(const char *filename, int score, int total);

int main(void) {
    questions* question = NULL;
    const char* q_file_name = "questions.txt";
    int questions_count = load_questions(q_file_name, &question);

    if (questions_count == -1) {
        printf("ERROR! Failed to open file\n");
        return 1;
    }
    int score = 0;
    if (questions_count > 0) {
        quiz_start(question, questions_count);
        results("results.txt", score, questions_count);
    }

    free(question);
    return 0;
}

int load_questions(const char* q_file_name, questions** question) {
    FILE* file = fopen(q_file_name, "r");
    if (!file) {
        return -1;
    }

    int size;
    fscanf(file, "%d", &size);
    *question = malloc(sizeof(questions) * size);

    for (int i = 0; i < size; i++) {
        fscanf(file, " %[^\n]s", (*question)[i].question);
        fscanf(file, " A) %[^\n]s B) %[^\n]s C) %[^\n]s D) %[^\n]s %c",
               (*question)[i].option1,
               (*question)[i].option2,
               (*question)[i].option3,
               (*question)[i].option4,
               &(*question)[i].correct_answer);
    }

    fclose(file);
    return size;
}

void quiz_start(questions *question, int size) {
    int score = 0;
    char answer;

    for (int i = 0; i < size; i++) {
        printf("\nQ: %s\n", question[i].question);
        printf(" A) %s\n B) %s\n C) %s\n D) %s\n", question[i].option1, question[i].option2, question[i].option3, question[i].option4);
        printf("Enter your answer (A, B, C, D): ");
        scanf(" %c", &answer);

        if (answer == question[i].correct_answer) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer is %c.\n", question[i].correct_answer);
        }
    }

    printf("\nYour total score: %d/%d\n", score, size);
}

void results(const char *filename, int score, int total) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to save results");
        return;
    }

    fprintf(file, "Score: %d out of %d\n", score, total);
    fclose(file);
}
