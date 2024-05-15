#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct {
    char question[300];
    char option1[300];
    char option2[300];
    char option3[300];
    char option4[300];
    char correct_option;
} Questions;


int load_quiz(const char* file_name, Questions** question);
int start_quiz(int size, Questions* question);
//int report();

int main(void){
    Questions* question = NULL;
    const char* file_name = "questions.txt";
    int questions_count = load_quiz(file_name, &question);

    //printf("%i\n", questions_count);
    if (questions_count == -1){
        printf("The File isn't on the correct path or name! (It should be questions.txt)\n");
    }

    if(questions_count > 0){
        start_quiz(questions_count, question);
    }

    free(question);
    return 0;
}

int load_quiz(const char* file_name, Questions** question){
    FILE* file = fopen(file_name, "r");
    if(!file){
        return -1;
    }
    char ch;
    int size = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '?') {
        size++;
        }
    }

    (*question) = malloc(sizeof(Questions) * size);

    rewind(file);
    for(int i = 0; i < size; i++){
        fscanf(file, "%[^\n]\n", (*question)[i].question);
        fscanf(file, "A) %[^\n]\n", (*question)[i].option1);
        fscanf(file, "B) %[^\n]\n", (*question)[i].option2);
        fscanf(file, "C) %[^\n]\n", (*question)[i].option3);
        fscanf(file, "D) %[^\n]\n", (*question)[i].option4);
        fscanf(file, " %c\n", &(*question)[i].correct_option);

    }
    fclose(file);
    return size;

}

int start_quiz(int size, Questions* question){
    int score = 0;
    char answer;

    for (int i = 0; i < size; i++){
        printf("Q: \n %s\n\n", question[i].question);
        printf(" A)%s\n B)%s\n C)%s\n D)%s\n\n", question[i].option1, question[i].option2, question[i].option3, question[i].option4);

        do{
            printf("Enter Your Answer(A, B, C, D):");
            scanf(" %c", &answer);
            answer = toupper(answer);
        }
        while(answer != 'A' && answer != 'B' && answer != 'C' && answer != 'D');

        //scanf(" %c", &answer);
        if(answer ==  question[i].correct_option){
            printf("Correct!!\n\n");
            score++;
        }
        else{
            printf("Incorrect! , The correct answer is %c\n\n", question[i].correct_option);
        }
    }
    printf("\nYour total score: %d/%d\n", score, size);
}