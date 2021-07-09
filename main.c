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

// ####################### Hashing #####################

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

int main()
{
    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // ########## Closed Hash Demo ##########
    printf("########## Closed Hash Demo ###########\n\n");

    // Get closed hash size
    unsigned int N;
    printf("Enter N (closed hash size): ");
    scanf("%u", &N);
    flushInput();
    printf("\n");

    // Create closed hash
    ClosedHash closedHash;
    initClosedHash(&closedHash, N);

    // Get closed hash random students
    unsigned int M;
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
    insertClosedHash(&closedHash, &closedHashStudentInsert, closedHashLinear);

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
        printf("Couldn't find student with that name.\n\n");

    // Print closed hash
    printClosedHash(&closedHash);

    // Destroy closed hash
    destroyClosedHash(&closedHash);

    return 0;
}
