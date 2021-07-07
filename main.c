#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ###################### Student ######################

// Contains date info
typedef struct Date
{
    int day;
    int month;
    int year;
} Date;

// Contains student info
typedef struct StudentInfo
{
    int id;
    char name[25];
    Date birthDate;
    int score;
} StudentInfo;

// Fills student info from input stream
void fillStudent(StudentInfo *student)
{
    printf("Enter ID: ");
    scanf("%d", &student->id);

    // Flush input
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        /* ignore */;

    printf("Enter Name: ");
    fgets(student->name, sizeof(student->name), stdin);
    student->name[strcspn(student->name, "\n")] = '\0';

    printf("Enter Birth Date (Day Month Year): ");
    scanf("%d", &student->birthDate.day);
    scanf("%d", &student->birthDate.month);
    scanf("%d", &student->birthDate.year);

    printf("Enter Score: ");
    scanf("%d", &student->score);
    printf("\n");
}

// Fills student info with random info
void fillRandomStudent(StudentInfo *student)
{
    typedef const char *str;
    static const str firstNames[] = {"Adham", "Ali", "Ibrahim"};
    static const str lastNames[] = {"Mohamed", "Medhat", "Nader"};

    str firstName = firstNames[rand() % 3];
    size_t firstNameLength = strlen(firstName);
    str lastName = lastNames[rand() % 3];
    size_t lastNameLength = strlen(lastName);

    memcpy(student->name, firstName, firstNameLength * sizeof(char));
    student->name[firstNameLength] = ' ';
    memcpy(student->name + firstNameLength + 1, lastName, lastNameLength * sizeof(char));
    student->name[firstNameLength + lastNameLength + 1] = '\0';
    student->id = (rand() % 999999) + 1;
    student->birthDate.day = (rand() % 30) + 1;
    student->birthDate.month = (rand() % 12) + 1;
    student->birthDate.year = (rand() % 30) + 1980;
    student->score = (rand() % 50) + 50;
}

// Prints student info to output stream
void printStudent(const StudentInfo *student)
{
    printf("%d: %s %d/%d/%d %d%%\n\n", student->id, student->name,
           student->birthDate.day, student->birthDate.month,
           student->birthDate.year, student->score);
}

// Prints students table header
void printStudentTableHeader()
{
    printf("--------------------------------------------------------------\n");
    printf("|     ID |           Name           |  Birth Date  |   Score |\n");
    printf("--------------------------------------------------------------\n");
}

// Prints student in tabular form
void printStudentTable(const StudentInfo *student)
{
    // Format birth date
    char birthDate[13];
    sprintf(birthDate, "%d/%d/%d", student->birthDate.day,
            student->birthDate.month, student->birthDate.year);

    // Print student in tabular form
    printf("| %6d | %-24s | %-12s | %6d%% |\n",
           student->id, student->name,
           birthDate, student->score);
}

// Prints student in tabular form
void printStudentTableFooter()
{
    printf("--------------------------------------------------------------\n\n");
}

int main()
{
    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // Create student 1
    StudentInfo student1;
    fillStudent(&student1);
    printStudent(&student1);

    // Create student 2
    StudentInfo student2;
    fillStudent(&student2);
    printStudent(&student2);

    // Print student table
    printStudentTableHeader();
    printStudentTable(&student1);
    printStudentTable(&student2);
    printStudentTableFooter();
    return 0;
}
