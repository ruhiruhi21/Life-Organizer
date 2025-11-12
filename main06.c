#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX 10
#define TODO_FILE "todo.txt"

// To-Do list variables
char session_todo[MAX][100];
int session_count = 0;

// ===== Save a new task to file =====
void save_todo(const char *task) {
    FILE *file = fopen(TODO_FILE, "a");
    if (file == NULL) {
        printf("Error: Could not save task to file.\n");
        return;
    }
    fprintf(file, "%s\n", task);
    fclose(file);
}
void view_saved_tasks() {
    FILE *file = fopen(TODO_FILE, "r");
    if (file == NULL) {
        printf("\nNo saved tasks found.\n");
        return;
    }

    char line[100];
    int count = 0;

    printf("\n====== All Stored Tasks ======\n");
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        printf("%d. %s\n", ++count, line);
    }

    if (count == 0)
        printf("No tasks found.\n");

    fclose(file);
}

// ===== 1. Clock =====
void show_clock() {
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);

    int hour = tm_info->tm_hour;
    int minute = tm_info->tm_min;
    int second = tm_info->tm_sec;
    int day = tm_info->tm_mday;
    int month = tm_info->tm_mon + 1;
    int year = tm_info->tm_year + 1900;

    printf("\n====== Digital Clock ======\n");
    printf("Current Time: %02d:%02d:%02d\n", hour, minute, second);
    printf("Current Date: %02d/%02d/%04d\n", day, month, year);
    printf("============================\n");
}

// ===== 2. Calendar =====
int get_day_of_week(int d, int m, int y) {
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int k = y % 100;
    int j = y / 100;
    int day = (d + 13*(m + 1)/5 + k + k/4 + j/4 + 5*j) % 7;
    return ((day + 6) % 7);
}

int get_days_in_month(int month, int year) {
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            return 29;
        else
            return 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else
        return 31;
}

void show_calendar() {
    int day, month, year;
    const char *day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    printf("\n====== Calendar ======\n");
    printf("Enter day, month, and year (e.g., 16 8 2025): ");
    scanf("%d %d %d", &day, &month, &year);

    int day_of_week_index = get_day_of_week(day, month, year);
    printf("Day of Week: %s, Month: %d, Year: %d\n", day_names[day_of_week_index], month, year);

    int start_day = get_day_of_week(1, month, year);
    int days = get_days_in_month(month, year);

    printf("\nSun Mon Tue Wed Thu Fri Sat\n");

    for (int i = 0; i < start_day; i++)
        printf("    ");

    for (int d = 1; d <= days; d++) {
        printf("%3d ", d);
        if ((start_day + d) % 7 == 0)
            printf("\n");
    }
    printf("\n");
}

// ===== 3. To-Do List =====
void todo_list() {
    int choice;
    char task[100];

    printf("\n====== To-Do List ======\n");

    while (1) {
        printf("\n1. Add Task (session only)\n");
        printf("2. View Current Session Tasks\n");
        printf("3. View All Stored Tasks (from file)\n");
        printf("4. Exit To-Do\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                if (session_count < MAX) {
                    printf("Enter task: ");
                    fgets(task, sizeof(task), stdin);
                    task[strcspn(task, "\n")] = '\0'; // remove newline
                    strcpy(session_todo[session_count], task);
                    session_count++;
                    save_todo(task); // save permanently
                    printf("Task added and saved.\n");
                } else {
                    printf("Session task list is full!\n");
                }
                break;

            case 2:
                printf("\n--- Current Session Tasks ---\n");
                if (session_count == 0)
                    printf("No tasks added in this session.\n");
                else {
                    for (int i = 0; i < session_count; i++)
                        printf("%d. %s\n", i + 1, session_todo[i]);
                }
                break;

            case 3:
                view_saved_tasks();
                break;

            case 4:
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
}

// ===== Main Menu =====
void menu() {
    int choice;
    while (1) {
        printf("\n========= MAIN MENU =========\n");
        printf("1. Clock\n");
        printf("2. Calendar\n");
        printf("3. To-Do List\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: show_clock(); break;
            case 2: show_calendar(); break;
            case 3: todo_list(); break;
            case 4: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

// ===== Main Function =====
int main() {
    menu();
    return 0;
}
