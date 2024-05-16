#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    char question[300];
    char option1[300];
    char option2[300];
    char option3[300];
    char option4[300];
    char correct_option;
    char answer;
} Questions;

int size;

int count_quetions(const char* file_name);
void load_quiz(const char* file_name, Questions* question);
int start_quiz(int size, Questions* question, int* score);
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question);

//karim
int main(void){

    char* name = malloc(100);
    printf("Enter your name: ");
    scanf("%s", name);
    int id;
    printf("enter your id: ");
    scanf("%i", &id);

    const char* file_name = "questions.txt";
    int questions_count = count_quetions(file_name);
    if (questions_count == -1){
        printf("The File isn't on the correct path or name! (It should be questions.txt)\n");
    }
    if(questions_count > 0){
        Questions question[size];
        load_quiz(file_name, question);
        int score = 0;
        start_quiz(questions_count, question, &score);
        char file_name_report[100] = "report_";
        char* file_report_name1 = strcat(file_name_report , name);
        char* file_report_name = strcat(file_report_name1 , ".txt");
        report(name ,id, score, questions_count, file_report_name, question);
    }

    return 0;
}

//ramy
int count_quetions(const char* file_name){
    FILE* file = fopen(file_name, "r");
    if(!file){
        return -1;
    }
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '?') {
        size++;
        }
    }
    rewind(file);
    fclose(file);
    return size;
}

//youssef
void load_quiz(const char* file_name, Questions* question){
    FILE* file = fopen(file_name, "r");

    for(int i = 0; i < size; i++){
        fscanf(file, "%[^\n]\n", question[i].question);
        fscanf(file, "A) %[^\n]\n", question[i].option1);
        fscanf(file, "B) %[^\n]\n", question[i].option2);
        fscanf(file, "C) %[^\n]\n", question[i].option3);
        fscanf(file, "D) %[^\n]\n", question[i].option4);
        fscanf(file, " %c\n", &question[i].correct_option);
    }
    fclose(file);
}

//wezza
int start_quiz(int size, Questions* question, int* score){

    for (int i = 0; i < size; i++){
        printf("Q: \n %s\n\n", question[i].question);
        printf(" A)%s\n B)%s\n C)%s\n D)%s\n\n", question[i].option1, question[i].option2, question[i].option3, question[i].option4);

        do{
            printf("Enter Your Answer(A, B, C, D):");
            scanf(" %c", &question[i].answer);
            question[i].answer = toupper(question[i].answer);
        }
        while(question[i].answer != 'A' && question[i].answer != 'B' && question[i].answer != 'C' && question[i].answer != 'D');

        if(question[i].answer ==  question[i].correct_option){
            (*score)++;
        }
    }
    printf("\nYour total score: %d/%d\n", *score, size);
}

//mathew
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question){
    FILE* file = fopen(file_report_name, "w");

    fprintf(file, "Name: %s\n ID: %i\n", name, id);

    for (int i = 0; i < size; i++){
        fprintf(file, "Q: \n %s\n\n", question[i].question);
        fprintf(file, " A)%s\n", question[i].option1);
        fprintf(file, " B)%s\n", question[i].option2);
        fprintf(file, " C)%s\n", question[i].option3);
        fprintf(file, " D)%s\n", question[i].option4);
        fprintf(file, " Correct answer: %c\n", question[i].correct_option);
        fprintf(file, " Your answer: %c\n", question[i].answer);
    }

    fprintf(file,"\n%i/%i\n\n",score, questions_count);
    fclose(file);
}
