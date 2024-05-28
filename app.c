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

    // Allocate memory for the user's name
    char* name = malloc(100); 

    // Prompt the user to enter their name
    printf("Enter your name: ");
    scanf("%s", name);

    // Variable to store the user's ID
    long long int id;

    // Loop until a valid integer ID is entered
    int read;
    do {
        printf("Enter your ID (As an Integer): ");
        read = scanf("%lli", &id);
        if (read!= 1){
            scanf("%*[^\n]");
        }
    } while( read!= 1);

    // Choose a quiz file for the user
    char* file_name = choose_quiz(); 

    // Check if a valid quiz file was chosen
    if(file_name == NULL){
        printf("There is no quiz files with the correct format on the program path (IT SHOULD BE STARTING WITH quiz_ AND.txt file)\n");
        return 1;
    }

    // Count the number of questions in the chosen quiz file
    int number_of_questions = Counting_Questions (file_name);

    // Check if the quiz file was found and read successfully
    if (number_of_questions == -1){
        printf("The file isn't on the correct path or name!\n");
        return 1;
    }

    // Check if there are any questions in the quiz file
    if(number_of_questions > 0){
        // Allocate memory for the quiz questions
        Questions question[size];

        // Load the quiz questions from the chosen quiz file
        load_quiz(file_name, question); 

        // Initialize the user's score
        int score = 0;

        // Run the quiz and update the user's score
        start_quiz(number_of_questions, question, &score); 

        // Generate the report file name
        char file_name_report[100] = "report_";
        char* file_report = strcat(file_name_report, name);
        char* file_report_name = strcat(file_report, ".txt");

        // Generate the full path for the report file
        char path[100] = "/home/kbassem/code/project/";
        char* file_report_path = strcat(path, file_report_name);

        // Generate and save the report
        report(name,id, score, number_of_questions, file_report_path, question, file_name);
    }
    else{
        printf("There is no questions on the file or it's not with the correct format (EVERY QUESTION SHOULD ENDS WITH A? AND THE OPTIONS ARE ON THE NEXT LINES!!!)\n");
        return 1;
    }

    // Free the allocated memory for the user's name
    free(name);

    return 0;
}

//Karim
char* choose_quiz(){
    DIR * dir; //define a pointer to a dir
    struct dirent* file_name_ptr;
    dir = opendir("."); //selects the directory that wants to access
    char* file_names[100]; //define an array of 100 file names
    int i = 0; 
    if(dir){ //checks if the program could access the directory

        while ((file_name_ptr = readdir(dir)) != NULL) { //loop till there is no files to read there names

            //if condition if the file name starts with quiz_ and ends with .txt if yes it append the files name on the array
            if (strstr(file_name_ptr->d_name, ".txt") && strstr(file_name_ptr->d_name, "quiz_")) { 
                file_names[i] = strdup(file_name_ptr->d_name);
                printf("%i) %s\n",i+1, file_names[i]); //prints to the user the file name and with the number of i+1 for more user friendly
                i++;
            }
        }
        closedir(dir); //close the directory to save memory
        if(i == 0){ //checks if the i = 0 then there is no files
            return NULL;
        }
        int choose;
        int read;
        do {
            printf("Choose one quiz you want to solve (1 to %d): ", i); //loop if the user puts an invalid number
            read = scanf("%i", &choose);
            if (read != 1){
                scanf("%*[^\n]");
            }
        } while (choose > i || choose <= 0);
        char* chosen_file = file_names[choose - 1]; //saves the choosen file
        char answer;
        do { //makes a loop till the user confirms what he have choosen
            printf("\nAre You Sure You Want To Solve The %s\n (Enter Y For Yes Or N for NO)", chosen_file);
            scanf(" %c", &answer);
            answer = tolower(answer);
        }while(answer != 'y' && answer != 'n');
        if (answer == 'y'){
            return chosen_file; //if the user confirms it return the chosen file name
        }
        else if (answer == 'n'){
            return choose_quiz(); //if not it returns the function from the beggining
        }
    }
}

//ramy
int Counting_Questions (char*filename) {
    //opens the file
    FILE *file = fopen(filename,"r");
    if(file == NULL) {
        return -1;//if can't open the file it returns -1 to the main
    }
    char ch;

    while ((ch = fgetc(file)) != EOF)//make a loop to reads all of the file char till the end of the file
    {
        if(ch == '?'){//check if the char is ? so this is the end of a question so increment the size by 1
            size++;
        }

    }
    fclose(file); //close the file to free the memory 
    return size; //return the number of questions to the main

}

//youssef
void load_quiz(const char* file_name, Questions* question){
    FILE* file = fopen(file_name, "r"); //opens the file

    for(int i = 0; i < size; i++){ //loop on the questions
    //save all of the questions elements on their places of variables
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
	for (int i = 0; i < size; i++) {// a loop on all of the questions
		printf("%s", question[i].question); //prints all of the questions
		printf("\nA)%s\nB)%s\nC)%s\nD)%s\n", question[i].option1, question[i].option2, question[i].option3, question[i].option4); //prints all of the options
	
		do { //checks if the user puts an invalid options it shows him the answer again
			printf("enter your answer: ");
			scanf(" %c", &question[i].answer);
            question[i].answer = toupper(question[i].answer); //make the answer not case sensitive
		} while (question[i].answer != 'A' && question[i].answer != 'B' && question[i].answer != 'C' && question[i].answer != 'D');
		
			if (question[i].answer == question[i].correct_option) { //checks if the answer that the user inputs is the same that the correct answer it increments the score by 1
				(*score)++; 
		}
	
	}
	printf("your score: %d/%d\n", *score, size); 
}

//mathew
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question, char* file_name) {

    FILE* file = fopen(file_report_name, "a"); //opens a new file with the path
    fprintf(file, "Name: %s\n", name); //prints the name of the user on the file
    fprintf(file, "ID: %d\n", id); //prints the id
    fprintf(file, "\n%s\n\n", file_name); //prints the name of teh quiz
    for (int i = 0; i < questions_count; i++) {
        fprintf(file, "%s\n", question[i].question); //prints the questions and the answers
        fprintf(file, "A) %s\n", question[i].option1);
        fprintf(file, "B) %s\n", question[i].option2);
        fprintf(file, "C) %s\n", question[i].option3);
        fprintf(file, "D) %s\n", question[i].option4);
        fprintf(file, "\nCorrect Answer: %c\n", question[i].correct_option); //prints the correct answer
        fprintf(file, "Your Answer: %c\n\n", question[i].answer); //prints the user answer
    }

    fprintf(file, "%f/%i\n", (float)score , questions_count); //prints the total socre of the user

    fclose(file);
}
