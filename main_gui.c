#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <gtk/gtk.h>

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
Questions *questions;
int current_question_index = 0;
int score = 0;

GtkWidget *question_label;
GtkWidget *option_buttons[4];
GtkWidget *next_button;
GtkWidget *window;
char *username;
long long int user_id;

// Function prototypes
void on_start_button_clicked(GtkWidget *widget, gpointer data);
char* choose_quiz();
int Counting_Questions(char* filename);
void load_quiz(const char* file_name, Questions* question);
//void start_quiz();
void report(char* name, int id, int score, int questions_count, char* file_report_name, Questions* question, char* file_name);
void on_option_button_clicked(GtkWidget *widget, gpointer data);
void on_next_button_clicked(GtkWidget *widget, gpointer data);
void show_question(int index);

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Quiz App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid to organize widgets
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create and add the question label
    question_label = gtk_label_new(NULL);
    gtk_grid_attach(GTK_GRID(grid), question_label, 0, 0, 4, 1);

    // Create and add the option buttons
    for (int i = 0; i < 4; i++) {
        option_buttons[i] = gtk_button_new_with_label(NULL);
        gtk_grid_attach(GTK_GRID(grid), option_buttons[i], 0, i + 1, 4, 1);
        g_signal_connect(option_buttons[i], "clicked", G_CALLBACK(on_option_button_clicked), GINT_TO_POINTER(i));
    }

    // Create and add the next button
    next_button = gtk_button_new_with_label("Next");
    gtk_grid_attach(GTK_GRID(grid), next_button, 0, 5, 4, 1);
    g_signal_connect(next_button, "clicked", G_CALLBACK(on_next_button_clicked), NULL);

    // Create and add the name entry
    GtkWidget *name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), name_entry, 0, 6, 2, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Enter your name");

    // Create and add the ID entry
    GtkWidget *id_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), id_entry, 2, 6, 2, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Enter your ID");

    // Create and add the start button
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, 7, 4, 1);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void on_start_button_clicked(GtkWidget *widget, gpointer data) {
    // Get the name and ID from the entries
    const char *name = gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(window), 0, 6)));
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(window), 2, 6)));

    // Validate the name and ID
    if (strlen(name) == 0 || strlen(id_str) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Please enter both name and ID.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Convert the ID string to an integer
    user_id = strtol(id_str, NULL, 10);

    // Copy the name to the global variable
    username = strdup(name);

    // Initialize the quiz
    char* file_name = choose_quiz(); 
    if(file_name == NULL){
        printf("No quiz files found!\n");
        return;
    }

    int number_of_questions = Counting_Questions(file_name);
    if (number_of_questions == -1){
        printf("Invalid quiz file!\n");
        return;
    }

    if(number_of_questions > 0){
        questions = (Questions*)malloc(sizeof(Questions) * number_of_questions);
        load_quiz(file_name, questions); 
        show_question(current_question_index);
    } else {
        printf("No questions in the file!\n");
        return;
    }
}
char* choose_quiz() {
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
        } else if (answer == 'n'){
            return choose_quiz();
        }
    }
}

int Counting_Questions (char*filename) {
    FILE *file = fopen(filename,"r");
    if(file == NULL) {
        return -1;
    }
    char ch;
    size = 0;
    while ((ch = fgetc(file)) != EOF) {
        if(ch == '?'){
            size++;
        }
    }
    fclose(file);
    return size;
}

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

void show_question(int index) {
    if (index >= size) {
        // Show final score and report
        char report_file_name[100];
        snprintf(report_file_name, sizeof(report_file_name), "report_%s.txt", username);
        report(username, user_id, score, size, report_file_name, questions, "quiz_file");
        gtk_main_quit();
        return;
    }
    gtk_label_set_text(GTK_LABEL(question_label), questions[index].question);
    gtk_button_set_label(GTK_BUTTON(option_buttons[0]), questions[index].option1);
    gtk_button_set_label(GTK_BUTTON(option_buttons[1]), questions[index].option2);
    gtk_button_set_label(GTK_BUTTON(option_buttons[2]), questions[index].option3);
    gtk_button_set_label(GTK_BUTTON(option_buttons[3]), questions[index].option4);
}

void on_option_button_clicked(GtkWidget *widget, gpointer data) {
    int chosen_option = GPOINTER_TO_INT(data);
    char chosen_answer = 'A' + chosen_option;
    questions[current_question_index].answer = chosen_answer;
    if (chosen_answer == questions[current_question_index].correct_option) {
        score++;
    }
    gtk_widget_set_sensitive(next_button, TRUE);
}

void on_next_button_clicked(GtkWidget *widget, gpointer data) {
    current_question_index++;
    show_question(current_question_index);
    gtk_widget_set_sensitive(next_button, FALSE);
}

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
        fprintf(file, "Your Answer: %c\n\n", question[i].answer);
    }
    fprintf(file, "Score: %d/%d\n", score, questions_count);
    fclose(file);
}
