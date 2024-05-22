//Quiz App In C
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

//making a stuct to use it again on the code as a reference for the questions
typedef struct {
    char question[300];
    char option1[300];
    char option2[300];
    char option3[300];
    char option4[300];
    char correct_option;
    char answer;
} Questions;

//defining the size of the file and it's outside the main to be a global variable so we can use it on any function easly
int size;

//prototype of the functions that are at the button for better readability
int count_questions(char* file_name);
void load_quiz(const char* file_name, Questions* question);
int start_quiz(int size, Questions* question, int* score);
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question, char* file_name);
char* choose_quiz();

int main(void){
    
    //naming a variable for the name and the id of the user
    char* name = malloc(100); //malloc to dynamicly allocate memory for the variable
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

    char* file_name = choose_quiz(); //calling the function of choosing the quiz that the user wants to solve and save it with the file_name variable

    if(file_name == NULL){//checking if the choose quiz returns NULL that means that there is no quiz with the correct name on the path
        printf("There is no quiz files with the correct format on the program path (IT SHOULD BE STARTING WITH quiz_ AND .txt file)\n");
        return 1;
    }

    int questions_count = count_questions(file_name);//calling the function of the questions_count to count the number of questions on the file

    if (questions_count == -1){//checking if the function returns -1 that means that it couldn't open the file
        printf("The file isn't on the correct path or name!\n");
        return 1;
    }

    if(questions_count > 0){//checking if the number of questions is bigger that 0 that means that there is a questions on the file

        Questions question[size]; //declaring an array of the type Questions and the size that returns from the question_count

        load_quiz(file_name, question); //calling the function of load the quiz that read all of the questions with a specifi format

        int score = 0; //decalring a variablr named score to use it to know the score of the user on the quiz

        start_quiz(questions_count, question, &score); //calling the function tp start the quiz

        char file_name_report[100] = "report_"; //naming a varianle for a string and getting it with the prefix report to print it correctly

        char* file_report_name1 = strcat(file_name_report , name); //using this function to concatinate the string that is report_ and the name of the user

        char* file_report_name = strcat(file_report_name1 , ".txt"); //using the function to concatinate the string concatinated before with the .txt file extention

        report(name ,id, score, questions_count, file_report_name, question, file_name); //calling the function of the report to prints the report of the user an dhis exam on a .txt file

    }
    else{//this else to prints to the user that the file don't have the questions on the right format
        printf("There is no questions on the file or it's not with the correct format (EVERY QUESTION SHOULD ENDS WITH A ? AND THE OPTIONS ARE ON THE NEXT LINES!!!)\n");
        return 1;
    }

    return 0;
}

//Karim Bassem ID: 231000797
char* choose_quiz(){
    /*This function is to get a list with all of the files on the directory of the program and has the quiz_ and .txt on there name and puts 
    them on an array of 100 of char* (strings) and show them to user to choose from them what the user will solve*/

    DIR * dir; //declaring a pointer of a DIR (directory) that points to a specific directory

    struct dirent* file_name_ptr; //making another pointer that points to the files on the directory using the struct because it's somthing on the library

    dir = opendir(".");//pointing the pointer dir with the directory of the app that is "."

    char* file_names[100]; //decalring an array of char* to store on it the names of the files

    int i = 0; //decalring a variable i for the while loop

    if(dir){ //checking if the program could opens the directory where the program is

        while ((file_name_ptr = readdir(dir)) != NULL) { //making a loop and checks if the pointer that points to the file names isn't = to NULL "Not the end of teh directory"

            /*using the strstr function to check if the pointer that points towards
            a filename has the .txt on it and quiz_*/
            if (strstr(file_name_ptr->d_name, ".txt") && strstr(file_name_ptr->d_name, "quiz_")) {

                file_names[i] = strdup(file_name_ptr->d_name);//assigning the file_names[i] with the name of the file that the pointer points on it

                printf("%i) %s\n",i+1, file_names[i]); //print the list of the items and using i+1 for better UX

                i++; //incrementing the i on the array for a new file_name

            }
        }

        closedir(dir); //clsing the directory to save the memory

        if(i == 0){//checking if the i is still 0 and if yes it return NULL to the main
            return NULL;
        }

        int choose;//decalring a variable with what the user have choosen
        int read;
        //a do while loop to make sure that the user enters a valind number
        do {
            printf("Choose one quiz you want to solve (1 to %d): ", i);
            read = scanf("%i", &choose);
            if (read != 1){
                scanf("%*[^\n]");
            }
        } while (choose > i || choose <= 0);

        char* chosen_file = file_names[choose - 1];//assigning the user answer to a varable named chosen_file 

        char answer;//declaing a variable for the answer that the user will type if he is sure that this is the quiz that he wants to solve
        do {
            printf("\nAre You Sure You Want To Solve The %s\n (Enter Y For Yes Or N for NO)", chosen_file);
            scanf(" %c", &answer);
            answer = tolower(answer);//making the answer to a lower case to not be case sensitive
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
int count_questions(char* file_name){
    FILE* file = fopen(file_name, "r");
    size = 0;
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
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question, char* file_name){
    FILE* file = fopen(file_report_name, "a");

    fprintf(file, "Name: %s\nID: %i\n \t\t %s", name, id, file_name);

    for (int i = 0; i < size; i++){
        fprintf(file, "\nQ: \n%s\n\n", question[i].question);
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
