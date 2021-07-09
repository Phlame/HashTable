#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define BENCHMARK

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

// Waits for enter to continue
void waitForEnter()
{
    getchar();
}

float startTime; // Start time of timer
// Starts timer
void startTimer()
{
    // Record start time
    startTime = (float)clock() / CLOCKS_PER_SEC;
}

// Ends timer and returns elapsed time in ms
float endTimer()
{
    // Record end time and calculate delta time
    float endTime = (float)clock() / CLOCKS_PER_SEC;
    float deltaTime = endTime - startTime;

    // Return delta time
    return deltaTime * 1e3;
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
                                     "Ahmed", "Nasser", "Ehab", "Sherif", "Samy"};

    str firstName = FIRST_NAMES[rand() % (sizeof(FIRST_NAMES) / sizeof(str))];
    size_t firstNameLength = strlen(firstName);
    str lastName = LAST_NAMES[rand() % (sizeof(LAST_NAMES) / sizeof(str))];
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

// ####################### Hashing #####################

// Computes hash of student name
size_t hashStudentName(const char *studentName)
{
    size_t hash = 0;
    for (size_t i = 0; i < strlen(studentName); i++)
        hash += (size_t)tolower(studentName[i]);
    return hash;
}

// Compares a student by name
int compareStudentName(const StudentInfo *student, const char *studentName)
{
    char nameLower[STR_SIZ];
    char studentNameLower[STR_SIZ];

    // Convert studentName to lowercase
    strcpy(studentNameLower, studentName);
    toLowerStr(studentNameLower);

    // Convert student name to lowercase
    strcpy(nameLower, student->name);
    toLowerStr(nameLower);

    // Check student name
    return strcmp(nameLower, studentNameLower) == 0;
}

// Computes hash of student birth date
size_t hashStudentBirthDate(const Date *birthDate)
{
    size_t hash = birthDate->day + birthDate->month * 100 + birthDate->year * 10000;
    return hash;
}

// Compares a student by birth date
int compareStudentBirthDate(const StudentInfo *student, const Date *birthDate)
{
    // Check student birth date
    return student->birthDate.day == birthDate->day &&
           student->birthDate.month == birthDate->month &&
           student->birthDate.year == birthDate->year;
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
    size_t size;           // List size (nodes count)
} StudentList;

// Initializes list
void initList(StudentList *students)
{
    // Empty list
    students->head = NULL;
    students->size = 0;
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
    students->size++;
}

// Searches by name (lowercase) for student info in list
const StudentListNode *findList(StudentList *students, const char *studentName)
{
    // Get first node
    StudentListNode *current = students->head;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Check student name
        if (compareStudentName(&current->student, studentName))
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
    // Check if list is empty
    if (students->head == NULL)
        return 0;

    // Check head
    if (compareStudentName(&students->head->student, studentName))
    {
        StudentListNode *temp = students->head;
        students->head = students->head->next;
        students->size--;
        free(temp);
        return 1;
    }

    // Get second node
    StudentListNode *prev = students->head;
    StudentListNode *current = students->head->next;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Check student name
        if (compareStudentName(&current->student, studentName))
        {
            prev->next = current->next;
            students->size--;
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
    openTable->size = tableSize == 0 ? 1 : tableSize;
    openTable->table = (StudentList *)malloc(openTable->size * sizeof(StudentList));

    // Initialize lists
    for (size_t i = 0; i < openTable->size; i++)
        initList(&openTable->table[i]);
}

// Adds student info to open hash
void insertOpenHash(OpenHash *openTable, const StudentInfo *student)
{
    // Get index using student name
    size_t index = hashStudentName(student->name) % openTable->size;

    // Get students list
    StudentList *students = &openTable->table[index];

    // Add student to students list
    insertList(students, student);
}

// Searches by name for student info in open hash
const StudentInfo *findOpenHash(OpenHash *openTable, const char *studentName)
{
    // Get index using student name
    size_t index = hashStudentName(studentName) % openTable->size;

    // Get students list
    StudentList *students = &openTable->table[index];

    // Search student in students list
    const StudentListNode *listNode = findList(students, studentName);
    if (listNode != NULL)
        return &listNode->student;

    // Couldn't find student info with that name
    return NULL;
}

// Removes student info with name from open hash
int removeOpenHash(OpenHash *openTable, const char *studentName)
{
    // Get index using student name
    size_t index = hashStudentName(studentName) % openTable->size;

    // Get students list
    StudentList *students = &openTable->table[index];

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

// ################### Closed Hashing ##################

// Represents closed hash table node
typedef struct ClosedHashNode
{
    StudentInfo student; // Node data (student info)
    int empty;           // Is this node empty
    int removed;         // Has this node been removed
} ClosedHashNode;

// Contains closed hash table info
typedef struct ClosedHash
{
    size_t size;           // Hash table size
    ClosedHashNode *table; // Points to first node (contiguous)
} ClosedHash;

// Defines proping function notaion
typedef size_t (*PropingFunc)(size_t);

// Resolves proping to linear proping
size_t closedHashLinear(size_t i) { return i; }

// Resolves proping to quadratic proping
size_t closedHashQuadratic(size_t i) { return i * i; }

// Initializes closed hash
void initClosedHash(ClosedHash *closedTable, unsigned int tableSize)
{
    // Initialize table
    closedTable->size = tableSize == 0 ? 1 : tableSize;
    closedTable->table = (ClosedHashNode *)malloc(closedTable->size * sizeof(ClosedHashNode));

    // Initialize nodes
    for (size_t i = 0; i < closedTable->size; i++)
    {
        closedTable->table[i].empty = 1;
        closedTable->table[i].removed = 0;
    }
}

// Adds student info to closed hash
int insertClosedHash(ClosedHash *closedTable, const StudentInfo *student, PropingFunc proping)
{
    // Get index using student birth date
    size_t index = hashStudentBirthDate(&student->birthDate) % closedTable->size;

    size_t i = 0;
    size_t prop = proping(i);
    size_t current = index;

    // Iterate over table linearly until you find empty cell
    while (prop < closedTable->size)
    {
        if (closedTable->table[current].empty)
        {
            // Found empty cell; Insert student
            closedTable->table[current].student = *student;
            closedTable->table[current].empty = 0;
            closedTable->table[current].removed = 0;
            return 1;
        }

        // Get next node
        i++;
        prop = proping(i);
        current = (index + prop) % closedTable->size;
    }

    // Couldn't insert (table is full based on proping)
    return 0;
}

// Gets closed hash node with student birth date
ClosedHashNode *_getClosedHashNode(ClosedHash *closedTable, const Date *birthDate, PropingFunc proping)
{
    // Get index using student birth date
    size_t index = hashStudentBirthDate(birthDate) % closedTable->size;

    size_t i = 0;
    size_t prop = proping(i);
    size_t current = index;

    // Iterate over table linearly until you find student
    while (prop < closedTable->size)
    {
        if (!closedTable->table[current].empty)
        {
            // Check student
            if (compareStudentBirthDate(&closedTable->table[current].student, birthDate))
                return &closedTable->table[current];
        }
        else if (!closedTable->table[current].removed)
        {
            // Reached empty cell; Couldn't find student info with that birth date
            return NULL;
        }

        // Get next node
        i++;
        prop = proping(i);
        current = (index + prop) % closedTable->size;
    }

    // Couldn't find student info with that birth date
    return NULL;
}

// Searches by birth date for student info in closed hash
const StudentInfo *findClosedHash(ClosedHash *closedTable, const Date *birthDate, PropingFunc proping)
{
    // Get closed hash node with student birth date
    ClosedHashNode *node = _getClosedHashNode(closedTable, birthDate, proping);
    if (node != NULL)
        return &node->student;

    // Couldn't find student info with that birth date
    return NULL;
}

// Removes student info with birth date from closed hash
int removeClosedHash(ClosedHash *closedTable, const Date *birthDate, PropingFunc proping)
{
    // Get closed hash node with student birth date
    ClosedHashNode *node = _getClosedHashNode(closedTable, birthDate, proping);
    if (node != NULL)
    {
        // Remove node
        node->empty = 1;
        node->removed = 1;
        return 1;
    }

    // Couldn't find student info with that birth date
    return 0;
}

// Prints all students in closed hash
void printClosedHash(ClosedHash *closedTable)
{
    // Print table header
    printStudentTableHeaderWithHash();

    size_t hash;
    size_t prevHash;

    // Print table
    for (size_t i = 0; i < closedTable->size; i++)
    {
        // Get closed hash node
        ClosedHashNode node = closedTable->table[i];

        // Check if node is empty
        if (!node.empty)
        {
            // Compute hash using student birth date
            hash = hashStudentBirthDate(&node.student.birthDate) % closedTable->size;

            // Print student based on previous hash
            if (hash != prevHash || i == 0)
                printStudentTableWithHash(&node.student, hash);
            else
                printStudentTable(&node.student);
        }
        else
        {
            // Set hash to index
            hash = i;

            // Print empty student based on previous hash
            if (hash != prevHash || i == 0)
                printf("| %6lu |        |                          |              |         |\n", hash);
            else
                printf("|        |        |                          |              |         |\n");
        }

        // Save hash
        prevHash = hash;
    }

    // Print table footer
    printStudentTableFooterWithHash();
}

// Destroys closed hash
void destroyClosedHash(ClosedHash *closedTable)
{
    // Save table
    ClosedHashNode *temp = closedTable->table;

    // Reset closed hash
    closedTable->table = NULL;
    closedTable->size = 0;

    // Remove table
    free(temp);
}

// #################### Benchmark ###################

// Runs different tests on an open hash table
void benchmarkOpenHash(unsigned int N)
{
    StudentInfo openHashStudent;

    // Create open hash
    OpenHash openHash;
    initOpenHash(&openHash, N);

    // Speed benchmark
    startTimer();
    // Add random students to open hash
    for (size_t i = 0; i < 100000; i++)
    {
        fillRandomStudent(&openHashStudent);
        insertOpenHash(&openHash, &openHashStudent);
    }
    printf("Insert 100K students (avg): %f ms\n", endTimer() / 100000);

    // Memory size benchmark
    unsigned int structSize = sizeof(OpenHash) + openHash.size * sizeof(StudentList);
    for (size_t i = 0; i < openHash.size; i++)
        structSize += openHash.table[i].size * sizeof(StudentListNode);

    printf("Size after 100K students: %u Bytes\n", structSize);

    // Destroy open hash
    destroyOpenHash(&openHash);
}

// Runs different tests on an closed hash table
void benchmarkClosedHash(unsigned int N, PropingFunc proping)
{
    StudentInfo closedHashStudent;

    // Create closed hash
    ClosedHash closedHash;
    initClosedHash(&closedHash, N);

    // Speed benchmark
    startTimer();
    // Add random students to open hash
    for (size_t i = 0; i < 100000; i++)
    {
        fillRandomStudent(&closedHashStudent);
        insertClosedHash(&closedHash, &closedHashStudent, proping);
    }
    printf("Insert 100K students (avg): %f ms\n", endTimer() / 100000);

    // Memory size benchmark
    unsigned int structSize = sizeof(ClosedHash) + closedHash.size * sizeof(ClosedHashNode);
    printf("Size after 100K students: %u Bytes\n", structSize);

    // Destroy closed hash
    destroyClosedHash(&closedHash);
}

int main()
{
    // Set random seed based on time
    srand(time(NULL));

    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // ############# Benchmark ##############

#ifdef BENCHMARK
    printf("######### Open Hash Benchmark #########\n\n");
    printf("Small: [N = 100K]\n");
    printf("-----------------\n");
    benchmarkOpenHash(100000);
    printf("\nMedium: [N = 1M]\n");
    printf("----------------\n");
    benchmarkOpenHash(1000000);
    printf("\nLarge: [N = 10M]\n");
    printf("----------------\n");
    benchmarkOpenHash(10000000);

    printf("\n####### Closed Hash Benchmark #######\n\n");
    printf("Small: [N = 100K]\n");
    printf("-----------------\n");
    benchmarkClosedHash(100000, closedHashLinear);
    printf("\nMedium: [N = 1M]\n");
    printf("----------------\n");
    benchmarkClosedHash(1000000, closedHashLinear);
    printf("\nLarge: [N = 10M]\n");
    printf("----------------\n");
    benchmarkClosedHash(10000000, closedHashLinear);

    printf("\n");
#endif

    // ########################################

    printf("Press ENTER to start open hash demo...");
    waitForEnter();

    // ########### Open Hash Demo ###########
    printf("########### Open Hash Demo ############\n\n");

    // Get open hash size
    unsigned int N;
    printf("Enter N (open hash size): ");
    scanf("%u", &N);
    flushInput();
    printf("\n");

    // Create open hash
    OpenHash openHash;
    initOpenHash(&openHash, N);

    // Get open hash random students
    unsigned int M;
    printf("Enter M (random students count): ");
    scanf("%u", &M);
    flushInput();
    printf("\n");

    // Add random students to open hash
    for (size_t i = 0; i < M; i++)
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
    if (removeOpenHash(&openHash, openHashRemoveName))
        printf("Removed student.\n\n");
    else
        printf("Couldn't remove student with that name.\n\n");

    // Print open hash
    printOpenHash(&openHash);

    // Destroy open hash
    destroyOpenHash(&openHash);

    // ########################################

    printf("Press ENTER to start closed hash demo...");
    waitForEnter();

    // ########## Closed Hash Demo ##########
    printf("########## Closed Hash Demo ###########\n\n");

    // Get closed hash size
    printf("Enter N (closed hash size): ");
    scanf("%u", &N);
    flushInput();
    printf("\n");

    // Create closed hash
    ClosedHash closedHash;
    initClosedHash(&closedHash, N);

    // Get closed hash random students
    printf("Enter M (random students count): ");
    scanf("%u", &M);
    flushInput();
    printf("\n");

    // Add random students to closed hash
    for (size_t i = 0; i < M; i++)
    {
        StudentInfo closedHashStudent;
        fillRandomStudent(&closedHashStudent);
        insertClosedHash(&closedHash, &closedHashStudent, closedHashLinear);
    }

    // Print closed hash
    printClosedHash(&closedHash);

    // Insert student demo
    printf("[Insert Demo]\n");
    StudentInfo closedHashStudentInsert;
    fillStudent(&closedHashStudentInsert);
    if (!insertClosedHash(&closedHash, &closedHashStudentInsert, closedHashLinear))
        printf("Couldn't insert student.\n\n");

    // Print closed hash
    printClosedHash(&closedHash);

    // Find student demo
    printf("[Find Demo]\n");
    printf("Enter student birth date to find (Day Month Year): ");
    Date closedHashFindDate;
    scanf("%d", &closedHashFindDate.day);
    scanf("%d", &closedHashFindDate.month);
    scanf("%d", &closedHashFindDate.year);
    const StudentInfo *closedHashStudentFind = findClosedHash(&closedHash, &closedHashFindDate, closedHashLinear);
    if (closedHashStudentFind != NULL)
        printStudent(closedHashStudentFind);
    else
        printf("Couldn't find student with that name.\n\n");

    // Print closed hash
    printClosedHash(&closedHash);

    // Remove student demo
    printf("[Remove Demo]\n");
    printf("Enter student birth date to remove (Day Month Year): ");
    Date closedHashRemoveDate;
    scanf("%d", &closedHashRemoveDate.day);
    scanf("%d", &closedHashRemoveDate.month);
    scanf("%d", &closedHashRemoveDate.year);
    if (removeClosedHash(&closedHash, &closedHashRemoveDate, closedHashLinear))
        printf("Removed student.\n\n");
    else
        printf("Couldn't remove student with that name.\n\n");

    // Print closed hash
    printClosedHash(&closedHash);

    // Destroy closed hash
    destroyClosedHash(&closedHash);

    return 0;
}
