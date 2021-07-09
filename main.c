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

// Converts string to lowercase
void toLowerStr(char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
        str[i] = (char)tolower(str[i]);
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

// #################### Linked List ####################

// Represents linked list node
typedef struct StudentListNode StudentListNode;
struct StudentListNode
{
    StudentInfo student;   // Node data (student info)
    StudentListNode *next; // Points to next node or end (null ptr)
};

// Contains linked list info
typedef struct StudentList
{
    StudentListNode *head; // Points to first node or end (null ptr)
} StudentList;

// Initializes list
void initList(StudentList *students)
{
    // Empty list
    students->head = NULL;
}

// Checks if list is empty
int isListEmpty(StudentList *students)
{
    // Check if list is empty
    return students->head == NULL;
}

// Adds student info to list (from head)
void insertList(StudentList *students, const StudentInfo *student)
{
    // Create new head which points to old head [which can be end (null ptr)]
    StudentListNode *newHead = (StudentListNode *)malloc(sizeof(StudentListNode));
    newHead->student = *student;
    newHead->next = students->head;

    // Point list head to newly created head
    students->head = newHead;
}

// Searches by name (lowercase) for student info in list
const StudentListNode *findList(StudentList *students, const char *studentName)
{
    char name[STR_SIZ];

    // Get first node
    StudentListNode *current = students->head;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Convert student name to lowercase
        strcpy(name, current->student.name);
        toLowerStr(name);

        // Check student name
        if (strcmp(name, studentName) == 0)
            return current;

        // Get next node
        current = current->next;
    }

    // Couldn't find student info with that name
    return NULL;
}

// Removes student info with name (lowercase) from list
int removeList(StudentList *students, const char *studentName)
{
    char name[STR_SIZ];

    // Check if list is empty
    if (students->head == NULL)
        return 0;

    // Convert head student name to lowercase
    strcpy(name, students->head->student.name);
    toLowerStr(name);
    
    // Check head
    if (strcmp(name, studentName) == 0)
    {
        StudentListNode *temp = students->head;
        students->head = students->head->next;
        free(temp);
        return 1;
    }

    // Get second node
    StudentListNode *prev = students->head;
    StudentListNode *current = students->head->next;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Convert student name to lowercase
        strcpy(name, current->student.name);
        toLowerStr(name);

        // Check student name
        if (strcmp(name, studentName) == 0)
        {
            prev->next = current->next;
            free(current);
            return 1;
        }

        // Get next node
        prev = current;
        current = current->next;
    }

    // Couldn't find student info with that name
    return 0;
}

void printListWithHash(StudentList *students, size_t hash)
{
    // Get first node
    size_t i = 0;
    StudentListNode *current = students->head;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Print student
        if (i == 0)
            printStudentTableWithHash(&current->student, hash);
        else
            printStudentTable(&current->student);

        // Get next node
        i++;
        current = current->next;
    }
}

// Destroys list
void destroyList(StudentList *students)
{
    // Get first node
    StudentListNode *temp = NULL;
    StudentListNode *current = students->head;

    // Reset list
    students->head = NULL;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Get next node
        temp = current;
        current = current->next;

        // Delete node
        free(temp);
    }
}

// ####################### Hashing #####################

// Computes hash of student name
size_t hashStudentName(const char *studentName)
{
    size_t hash = 0;
    for (size_t i = 0; i < strlen(studentName); i++)
        hash += (size_t)tolower(studentName[i]);
    return hash;
}

// #################### Open Hashing ###################

// Contains open hash table info
typedef struct OpenHash
{
    size_t size;        // Hash table size
    StudentList *table; // Points to first list of elements (contiguous)
} OpenHash;

// Initializes open hash
void initOpenHash(OpenHash *openTable, unsigned int tableSize)
{
    // Initialize table
    openTable->size = tableSize;
    openTable->table = (StudentList *)malloc(openTable->size * sizeof(StudentList));

    // Initialize lists
    for (size_t i = 0; i < openTable->size; i++)
        initList(&openTable->table[i]);
}

// Adds student info to open hash
void insertOpenHash(OpenHash *openTables, const StudentInfo *student)
{
    // Get index using student name
    size_t index = hashStudentName(student->name) % openTables->size;

    // Get students list
    StudentList *students = &openTables->table[index];

    // Add student to students list
    insertList(students, student);
}

// Searches by name for student info in open hash
const StudentInfo *findOpenHash(OpenHash *openTables, const char *studentName)
{
    // Get index using student name
    size_t index = hashStudentName(studentName) % openTables->size;

    // Get students list
    StudentList *students = &openTables->table[index];

    // Search student in students list
    const StudentListNode *listNode = findList(students, studentName);
    if (listNode != NULL)
        return &listNode->student;

    // Couldn't find student info with that name
    return NULL;
}

// Removes student info with name from open hash
int removeOpenHash(OpenHash *openTables, const char *studentName)
{
    // Get index using student name
    size_t index = hashStudentName(studentName) % openTables->size;

    // Get students list
    StudentList *students = &openTables->table[index];

    // Remove student from students list
    return removeList(students, studentName);
}

// Prints all students in open hash
void printOpenHash(OpenHash *openTable)
{
    // Print table header
    printStudentTableHeaderWithHash();

    // Print lists
    for (size_t i = 0; i < openTable->size; i++)
    {
        // Get students list
        StudentList *students = &openTable->table[i];

        // Print list
        if (!isListEmpty(students))
            printListWithHash(students, i);
        else
            printf("| %6lu |        |                          |              |         |\n", i);
    }

    // Print table footer
    printStudentTableFooterWithHash();
}

// Destroys open hash
void destroyOpenHash(OpenHash *openTable)
{
    // Destroy lists
    for (size_t i = 0; i < openTable->size; i++)
        destroyList(&openTable->table[i]);

    // Save table
    StudentList *temp = openTable->table;

    // Reset open hash
    openTable->table = NULL;
    openTable->size = 0;

    // Remove table
    free(temp);
}

int main()
{
    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // ########### Open Hash Demo ###########
    printf("########### Open Hash Demo ############\n\n");

    // Get open hash size
    int openHashSize;
    printf("Enter N (open hash size): ");
    scanf("%d", &openHashSize);
    flushInput();
    printf("\n");

    // Create open hash
    OpenHash openHash;
    initOpenHash(&openHash, openHashSize);

    // Add random students to open hash
    for (size_t i = 0; i < openHashSize; i++)
    {
        StudentInfo openHashStudent;
        fillRandomStudent(&openHashStudent);
        insertOpenHash(&openHash, &openHashStudent);
    }

    // Print open hash
    printOpenHash(&openHash);

    // Insert student demo
    printf("[Insert Demo]\n");
    StudentInfo openHashStudentInsert;
    fillStudent(&openHashStudentInsert);
    insertOpenHash(&openHash, &openHashStudentInsert);

    // Print open hash
    printOpenHash(&openHash);

    // Find student demo
    printf("[Find Demo]\n");
    printf("Enter student name to find: ");
    char openHashFindName[STR_SIZ];
    inputLine(openHashFindName);
    toLowerStr(openHashFindName);
    const StudentInfo *openHashStudentFind = findOpenHash(&openHash, openHashFindName);
    if (openHashStudentFind != NULL)
        printStudent(openHashStudentFind);
    else
        printf("Couldn't find student with that name.\n\n");

    // Print open hash
    printOpenHash(&openHash);

    // Remove student demo
    printf("[Remove Demo]\n");
    printf("Enter student name to remove: ");
    char openHashRemoveName[STR_SIZ];
    inputLine(openHashRemoveName);
    toLowerStr(openHashRemoveName);
    if (!removeOpenHash(&openHash, openHashRemoveName))
        printf("Couldn't find student with that name.\n\n");

    // Print open hash
    printOpenHash(&openHash);

    // Destroy open hash
    destroyOpenHash(&openHash);

    return 0;
}
