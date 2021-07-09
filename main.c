#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ###################### Utility ######################

#define STR_SIZ 25

// Flushes input stream (used after scanf and before getchar, gets)
void flushInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        /* ignore */;
}

// Accepts line from input stream
void inputLine(char *str)
{
    fgets(str, STR_SIZ, stdin);
    str[strcspn(str, "\n")] = '\0';
}

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
    char name[STR_SIZ];
    Date birthDate;
    int score;
} StudentInfo;

// Fills student info from input stream
void fillStudent(StudentInfo *student)
{
    printf("Enter ID: ");
    scanf("%d", &student->id);
    flushInput();

    printf("Enter Name: ");
    inputLine(student->name);

    printf("Enter Birth Date (Day Month Year): ");
    scanf("%d", &student->birthDate.day);
    scanf("%d", &student->birthDate.month);
    scanf("%d", &student->birthDate.year);

    printf("Enter Score: ");
    scanf("%d", &student->score);
    flushInput();
    printf("\n");
}

// Fills student info with random info
void fillRandomStudent(StudentInfo *student)
{
    typedef const char *str;
    static const str FIRST_NAMES[] = {"Adham", "Ali", "Ibrahim", "Fadi", "Nour",
                                      "Mahmoud", "Thabet", "Salah", "Ziad", "Sayed"};
    static const str LAST_NAMES[] = {"Mohamed", "Medhat", "Nader", "Omar", "Maged",
                                     "Ahmed", "Naser", "Ehab", "Sherif", "Samy"};

    str firstName = FIRST_NAMES[rand() % 3];
    size_t firstNameLength = strlen(firstName);
    str lastName = LAST_NAMES[rand() % 3];
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
void printStudentTableHeaderWithHash()
{
    printf("-----------------------------------------------------------------------\n");
    printf("|   Hash |     ID |           Name           |  Birth Date  |   Score |\n");
    printf("-----------------------------------------------------------------------\n");
}

// Prints student in tabular form
void printStudentTable(const StudentInfo *student)
{
    // Format birth date
    char birthDate[13];
    sprintf(birthDate, "%d/%d/%d", student->birthDate.day,
            student->birthDate.month, student->birthDate.year);

    // Print student in tabular form
    printf("|        | %6d | %-24s | %-12s | %6d%% |\n",
           student->id, student->name,
           birthDate, student->score);
}

// Prints student in tabular form with hash
void printStudentTableWithHash(const StudentInfo *student, size_t hash)
{
    // Format birth date
    char birthDate[13];
    sprintf(birthDate, "%d/%d/%d", student->birthDate.day,
            student->birthDate.month, student->birthDate.year);

    // Print student in tabular form
    printf("| %6lu | %6d | %-24s | %-12s | %6d%% |\n", hash,
           student->id, student->name,
           birthDate, student->score);
}

// Prints student in tabular form
void printStudentTableFooterWithHash()
{
    printf("-----------------------------------------------------------------------\n\n");
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
    printStudentTableHeaderWithHash();
    printStudentTableWithHash(&student1, 1);
    printStudentTableWithHash(&student2, 2);
    printStudentTableFooterWithHash();
    return 0;
}
