#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"

char currentUser[50];
char currentRole[20];

// LOGIN FUNCTION
int login() {
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];

    printf("USERNAME: ");
    scanf("%s", inUser);
    printf("PASSWORD: ");
    scanf("%s", inPass);

    FILE *fp = fopen("credentials.txt", "r");
    if (!fp) {
        printf("Error: credentials.txt file not found!\n");
        return 0;
    }

    while (fscanf(fp, "%49s %49s %19s", u, p, r) == 3) {
        if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0) {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


// ADD STUDENT
void addStudent() {
    int roll;
    char name[50], dept[50], phone[20];
    float mark;

    printf("Roll: ");
    scanf("%d", &roll);
    getchar();

    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Department: ");
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = 0;

    printf("Phone: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;

    printf("Mark: ");
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    if (!fp) {
        printf("File error!\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%s|%.2f\n", roll, name, dept, phone, mark);
    fclose(fp);

    printf("Student added successfully!\n");
}

// DISPLAY STUDENTS
void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file found!\n");
        return;
    }

    int roll;
    char name[50], dept[50], phone[20];
    float mark;

    printf("\nRoll\tName\t\tDepartment\tPhone\t\tMark\n");
    printf("---------------------------------------------------------------\n");

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f\n",
                  &roll, name, dept, phone, &mark) == 5) {
        printf("%d\t%-10s\t%-10s\t%-10s\t%.2f\n",
               roll, name, dept, phone, mark);
    }

    fclose(fp);
}

// SEARCH STUDENT
void searchStudent() {
    int find, roll;
    char name[50], dept[50], phone[20];
    float mark;
    int found = 0;

    printf("Enter roll to search: ");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No file!\n");
        return;
    }

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f\n",
                  &roll, name, dept, phone, &mark) == 5) {
        if (roll == find) {
            printf("\nFound Record:\n");
            printf("Roll: %d\nName: %s\nDept: %s\nPhone: %s\nMark: %.2f\n",
                   roll, name, dept, phone, mark);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Student not found!\n");
}

// UPDATE STUDENT
void updateStudent() {
    int updateRoll;
    printf("Enter roll to update: ");
    scanf("%d", &updateRoll);
    getchar();

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    int roll;
    char name[50], dept[50], phone[20];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f\n",
                  &roll, name, dept, phone, &mark) == 5) {

        if (roll == updateRoll) {
            found = 1;

            char newName[50], newDept[50], newPhone[20];
            float newMark;

            printf("New Name: ");
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = 0;

            printf("New Department: ");
            fgets(newDept, sizeof(newDept), stdin);
            newDept[strcspn(newDept, "\n")] = 0;

            printf("New Phone: ");
            fgets(newPhone, sizeof(newPhone), stdin);
            newPhone[strcspn(newPhone, "\n")] = 0;

            printf("New Mark: ");
            scanf("%f", &newMark);

            fprintf(temp, "%d|%s|%s|%s|%.2f\n",
                    roll, newName, newDept, newPhone, newMark);
        } else {
            fprintf(temp, "%d|%s|%s|%s|%.2f\n",
                    roll, name, dept, phone, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student updated successfully!\n");
    else printf("Roll not found!\n");
}

// DELETE STUDENT
void deleteStudent() {
    int delRoll;
    printf("Enter roll to delete: ");
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    int roll;
    char name[50], dept[50], phone[20];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f\n",
                  &roll, name, dept, phone, &mark) == 5) {
        if (roll != delRoll) {
            fprintf(temp, "%d|%s|%s|%s|%.2f\n",
                    roll, name, dept, phone, mark);
        } else found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student deleted successfully!\n");
    else printf("Roll not found!\n");
}

// MENUS
void adminMenu() {
    int c;
    while (1) {
        printf("\nADMIN MENU\n");
        printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n6.Logout\n");
        scanf("%d", &c);
        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else if (c == 5) deleteStudent();
        else return;
    }
}

void staffMenu() {
    int c;
    while (1) {
        printf("\nSTAFF MENU\n");
        printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Logout\n");
        scanf("%d", &c);
        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else return;
    }
}

void guestMenu() {
    int c;
    while (1) {
        printf("\nGUEST MENU\n");
        printf("1.Display\n2.Search\n3.Logout\n");
        scanf("%d", &c);
        if (c == 1) displayStudents();
        else if (c == 2) searchStudent();
        else return;
    }
}

// MAIN
int main() {
    if (!login()) {
        printf("Invalid login!\n");
        return 0;
    }

    printf("\nLogged in as: %s\n", currentRole);

    if (strcmp(currentRole, "admin") == 0) adminMenu();
    else if (strcmp(currentRole, "staff") == 0) staffMenu();
    else guestMenu();

    return 0;
}

