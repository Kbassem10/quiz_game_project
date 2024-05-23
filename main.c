//Quiz App In C
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

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

//karim
char* choose_quiz();
//ramy
int Counting_Questions(char* filename);
//youssef
void load_quiz(const char* file_name, Questions* question);
//noureldine
int start_quiz(int size, Questions* question, int* score);
//mathew
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question, char* file_name);

//karim
int main(void){

    char* name = malloc(100); 
    printf("Enter your name: ");
    scanf("%s", name);
    long long int id;

        int read;
        do {
            printf("Enter your ID (As an Integer): ");
            read = scanf("%lli", &id);
            if (read != 1){
                scanf("%*[^\n]");
            }
        } while( read != 1);

    char* file_name = choose_quiz(); 
    if(file_name == NULL){
        printf("There is no quiz files with the correct format on the program path (IT SHOULD BE STARTING WITH quiz_ AND .txt file)\n");
        return 1;
    }
    int number_of_questions = Counting_Questions (file_name);

    if (number_of_questions == -1){
        printf("The file isn't on the correct path or name!\n");
        return 1;
    }

    if(number_of_questions > 0){
        Questions question[size];
        load_quiz(file_name, question); 
        int score = 0;
        start_quiz(number_of_questions, question, &score); 

        char file_name_report[100] = "report_";
        char* file_report = strcat(file_name_report , name);
        char* file_report_name = strcat(file_report , ".txt");

        char path[100] = "/home/kbassem10/code/project/";
        char* file_report_path = strcat(path, file_report_name);
        report(name ,id, score, number_of_questions, file_report_path, question, file_name);
    }
    else{
        printf("There is no questions on the file or it's not with the correct format (EVERY QUESTION SHOULD ENDS WITH A ? AND THE OPTIONS ARE ON THE NEXT LINES!!!)\n");
        return 1;
    }

    return 0;
}

//Karim BONUS
char* choose_quiz(){
    DIR * dir;
    struct dirent* file_name_ptr;
    dir = opendir(".");
    char* file_names[100];
    int i = 0;
    if(dir){ 
        while ((file_name_ptr = readdir(dir)) != NULL) {
            if (strstr(file_name_ptr->d_name, ".txt") && strstr(file_name_ptr->d_name, "quiz_")) {
                file_names[i] = strdup(file_name_ptr->d_name);
                printf("%i) %s\n",i+1, file_names[i]);
                i++;
            }
        }
        closedir(dir);
        if(i == 0){
            return NULL;
        }
        int choose;
        int read;
        do {
            printf("Choose one quiz you want to solve (1 to %d): ", i);
            read = scanf("%i", &choose);
            if (read != 1){
                scanf("%*[^\n]");
            }
        } while (choose > i || choose <= 0);
        char* chosen_file = file_names[choose - 1];
        char answer;
        do {
            printf("\nAre You Sure You Want To Solve The %s\n (Enter Y For Yes Or N for NO)", chosen_file);
            scanf(" %c", &answer);
            answer = tolower(answer);
        }while(answer != 'y' && answer != 'n');
        if (answer == 'y'){
            return chosen_file;
        }
        else if (answer == 'n'){
            return choose_quiz();
        }
    }
}

//ramy
int Counting_Questions (char*filename) {

    FILE *file = fopen(filename,"r");
    if(file == NULL) {
        return -1;
    }
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if(ch == '?'){
            size++;
        }

    }
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

//noureldine
int start_quiz(int size, Questions* question, int* score) {
	for (int i = 0; i < size; i++) {
		printf("%s", question[i].question);
		printf("\nA)%s\nB)%s\nC)%s\nD)%s\n", question[i].option1, question[i].option2, question[i].option3, question[i].option4);
	
		do {
			printf("enter your answer: ");
			scanf(" %c", &question[i].answer);
            question[i].answer = toupper(question[i].answer);
		} while (question[i].answer != 'A' && question[i].answer != 'B' && question[i].answer != 'C' && question[i].answer != 'D');
		
			if (question[i].answer == question[i].correct_option) {
				(*score)++;
		}
	
	}
	printf("your score: %d/%d\n", *score, size);
}

//mathew
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question, char* file_name) {

    FILE* file = fopen(file_report_name, "a");
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "ID: %d\n", id);
    fprintf(file, "\n%s\n\n", file_name);
    for (int i = 0; i < questions_count; i++) {
        fprintf(file, "%s\n", question[i].question);
        fprintf(file, "A) %s\n", question[i].option1);
        fprintf(file, "B) %s\n", question[i].option2);
        fprintf(file, "C) %s\n", question[i].option3);
        fprintf(file, "D) %s\n", question[i].option4);
        fprintf(file, "\nCorrect Answer: %c\n", question[i].correct_option);
        fprintf(file, "Your ANswer: %c\n\n", question[i].answer);
    }

    fprintf(file, "%f/%i\n", (float)score , questions_count);

    fclose(file);
}
