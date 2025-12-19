#include <stdio.h>
#include <string.h> 
#include <time.h>   // for date handling
#include <stdlib.h> // for exit() function etc.

#define MAX_USERS 10
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_ENTRIES 100

struct User {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
};

// ------------------ GLOBALS ------------------
struct User users[MAX_USERS];
int userCount=0;    // how many entries in users are valid
char currentUser[MAX_USERNAME_LEN];
double expenses[MAX_ENTRIES];   // expense amounts
char expenseItem[MAX_ENTRIES][50];
char expenseCategory[MAX_ENTRIES][50];
char expenseDate[MAX_ENTRIES][11];  //  10 for DD/MM/YYYY + 1 for null terminator
int expenseCount=0; // how many expense entries are currrently stored

                                                                                        // CHECK 1(S)
// ------------------ UTILITIES ------------------
void clear_input(char str[]) {
    if (str[strlen(str)-1]=='\n')       // if string last element is an enter key
        str[strlen(str)-1]='\0';        // replace it by null charcter
}
                                                                                        // CHECK 1(E)

// ------------------ USER MANAGEMENT ------------------
void load_users() {
    FILE *file=fopen("users.txt", "r");  // "r" mode to read file
    if (!file) {            //there is no file, then create one with 3 default users    // CHECK 2(S)
        file=fopen("users.txt", "w");   // "w" mode to create file
        fprintf(file, "mahad,mahad123\nfaaiq,faaiq123\nareeb,areeb123\n");
        fclose(file);
        file=fopen("users.txt", "r");
    }
    userCount=0;    // reset user count                                                  // CHECK 2(E)

    //save the usernames and passwords into users array
    while (fscanf(file, "%[^,],%s\n", users[userCount].username, users[userCount].password)==2) // reads until a comma is found
    {
        userCount++;                                                                     // CHECK 3(S)
        if (userCount>=MAX_USERS) 
            break;
    }                                                                                    // CHECK 3(E)
    fclose(file);
}

void save_users(){
    //add users and passwords to file
    FILE *file=fopen("users.txt","w");
    for (int i=0; i<userCount; i++)
        fprintf(file,"%s,%s\n", users[i].username, users[i].password); // write username,password in file
    fclose(file);
}

int find_user_index(char *username) {
    //  Linear search for username in users array
    //  return the index of username, if no find then return -1
    for (int i=0; i<userCount; i++)
        if (strcmp(users[i].username, username) == 0)
            return i;
    return -1;
}

int sign_in() {
    char username[50], password[50];
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    clear_input(username);

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    clear_input(password);

    for (int i=0; i<userCount; i++) {
        if (strcmp(username, users[i].username)==0 && strcmp(password, users[i].password) == 0) {       // CHECK 4(S)
            strcpy(currentUser, username);
            return 1;       //sign in successful                                                        // CHECK 4(E)
        }
    }
    return 0; // one of the username or password is wrong
}

void sign_up() {
    if (userCount >= MAX_USERS) {                                                                        // CHECK 5(S)
        printf("User limit reached!\n");
        return;                                                                                          // CHECK 5(E)
    }

    char username[50], password[50];
    printf("Choose username: ");
    fgets(username, sizeof(username), stdin);
    clear_input(username);

    if (find_user_index(username) != -1) // checks find_user_index to avoid duplicates                   // CHECK 6(S)
    {
        printf("Username already exists!\n");
        return;                                                                                         // CHECK 6(E)
    }

    printf("Choose password: ");
    fgets(password, sizeof(password), stdin);
    clear_input(password);

    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    userCount++;
    save_users();
    printf("Signup successful! You are now logged in.\n");
    strcpy(currentUser, username);
}

// ------------------ EXPENSE / INCOME MANAGEMENT ------------------
void load_expenses() {
    char filename[100];
    sprintf(filename, "%s.csv", currentUser);   // formats into filename...
    FILE *file = fopen(filename, "r");
    expenseCount = 0;  // reset expense count
    if (!file) return;                                                                             // CHECK 7(S)
                                                                                                   // CHECK 7(E)
    char header[100]; // to skip header line
    fgets(header, sizeof(header), file);
    while (fscanf(file, "%[^,],%[^,],%lf,%[^\n]\n", expenseItem[expenseCount], expenseCategory[expenseCount], &expenses[expenseCount],
                  expenseDate[expenseCount]) == 4) // if 4 entries are read successfully
    {
        expenseCount++;
    }
    fclose(file);
}

void save_expenses() {
    char filename[100];
    sprintf(filename, "%s.csv", currentUser);
    FILE *file = fopen(filename, "w");
    fprintf(file, "Item,Category,Amount,Date\n");
    for (int i = 0; i < expenseCount; i++)
        fprintf(file, "%s,%s,%.2f,%s\n",
                expenseItem[i], expenseCategory[i], expenses[i], expenseDate[i]); // writes each expense entry separarated by commas
    fclose(file);
}

double get_user_income() // looks for a line username,income and returns 0 if not found
{
    FILE *file = fopen("all_incomes.csv", "r");
    if (!file) return 0;                                                                                // CHECK 8(S)
                                                                                                        // CHECK 8(E)
    char user[50]; double income; // buffers to read username and income amount
    char header[100];
    fgets(header, sizeof(header), file);
    while (fscanf(file, "%[^,],%lf\n", user, &income) == 2) {
        if (strcmp(user, currentUser) == 0) {
            fclose(file);
            return income;
        }
    }
    fclose(file);
    return 0;
}

void set_user_income(double total) // loops through all_incomes.csv and updates or adds the line for currentUser
{
    FILE *file = fopen("all_incomes.csv", "r");
    char temp[1000][100]; // to store all lines temporarily
    int count = 0, found = 0; // count of lines and found flag
    if (file) {
        while (fgets(temp[count], sizeof(temp[count]), file)) count++;
        fclose(file);
    }
    file = fopen("all_incomes.csv", "w");
    fprintf(file, "User,TotalIncome\n");
    for (int i = 1; i < count; i++) {
        char u[50]; double amt; // buffers to read username and amount
        if (sscanf(temp[i], "%[^,],%lf", u, &amt) == 2) // reads formatted data 'from' a string i.e. temp[i]
        {
            if (strcmp(u, currentUser) == 0) {
                fprintf(file, "%s,%.2f\n", u, total);
                found = 1; // marks that user was found and income was updated
            } else fprintf(file, "%s,%.2f\n", u, amt); // writes back unchanged line
        }
    }
    if (!found)
        fprintf(file, "%s,%.2f\n", currentUser, total); // adds new line with income if user not found
    fclose(file);
}

void get_date_input(char *date) {
    char input[20]; // buffer for user input
    printf("Enter date (DD/MM/YYYY) or 'T' for today: ");
    fgets(input, sizeof(input), stdin);
    clear_input(input);
    if (strcmp(input, "T") == 0 || strcmp(input, "t") == 0) {                                                           // CHECK 11(S)
        time_t t = time(NULL); // gets current time as seconds                                                          // CHECK 11(E)
        struct tm tm = *localtime(&t); // converts to local time structure (day, month, year, etc.)
        sprintf(date, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);  // formats date as DD/MM/YYYY
        // tm.tm_mday is day of month (1-31)
        // tm.tm_mon is month since January (0-11), so we add 1
        // tm.tm_year is years since 1900, so we add 1900
        // %02d means print as 2 digits with leading zero if needed, %04d means print as 4 digits with leading zeros if needed
    } else strcpy(date, input);
}

double calculate_total_expense() // sums up all expenses in expenses array
{
    double total = 0;
    for (int i = 0; i < expenseCount; i++)
        total += expenses[i];
    return total;
}

// ------------------ CORE FUNCTIONS ------------------
void add_income() {
    double amount;
    printf("Enter amount to add to total income: ");
    scanf("%lf", &amount);
    getchar(); // remove leftover newline

    double current = get_user_income(); // gets added income
    current += amount;
    set_user_income(current);
    printf("Income updated successfully! Total now: %.2f\n", current);
}

void add_expense() {
    if (expenseCount >= MAX_ENTRIES) 									// Check 9(S)
	{
		printf("\nExpense Limit Reached!\n");
		return;															// Check 9(E)
	}                                                                              

    printf("Enter item name: ");
    fgets(expenseItem[expenseCount], 50, stdin);
    clear_input(expenseItem[expenseCount]);

    printf("Enter category: ");
    fgets(expenseCategory[expenseCount], 50, stdin);
    clear_input(expenseCategory[expenseCount]);

    printf("Enter amount: ");
    scanf("%lf", &expenses[expenseCount]);
    getchar(); // remove leftover newline

    // Check if adding this expense exceeds income - if so, reject it
    double totalIncome = get_user_income();
    double totalExpense = calculate_total_expense();
    if (totalExpense + expenses[expenseCount] > totalIncome) {                                               // CHECK 10(S)
        printf("You cannot spend more than your income! Need %.2f more.\n",
               (totalExpense + expenses[expenseCount]) - totalIncome);
        return;                                                                                              // CHECK 10(E)
    }

    get_date_input(expenseDate[expenseCount]);
    expenseCount++;
    save_expenses();
    printf("Expense added.\n");
}

void delete_last_expense() {
    if (expenseCount == 0) {                                                                                 // CHECK 12(S)
        printf("No expenses to delete.\n");
        return;                                                                                              // CHECK 12(E)
    }
    expenseCount--; // simply reduce count to "remove" last expense
    save_expenses();
    printf("Last expense deleted.\n");
}

void delete_account() {
    printf("Are you sure you want to delete your account? (y/n): ");
    char ch = getchar();
    getchar(); // remove leftover newline
    if (ch != 'y' && ch != 'Y') return;                                                                     // CHECK 13(S)
                                                                                                            // CHECK 13(E)

    int idx = find_user_index(currentUser); // find index of current user
    if (idx == -1) return;
    for (int i = idx; i < userCount - 1; i++) // shift users left to delete
        users[i] = users[i + 1];
    userCount--;
    save_users();

    char filename[100];
    sprintf(filename, "%s.csv", currentUser);
    remove(filename); // defined in stdio.h to delete file

    // reads all_incomes.csv into lines array except currentUser, then writes back
    FILE *file = fopen("all_incomes.csv", "r");
    char lines[1000][100]; int count = 0;
    if (file) { while (fgets(lines[count], sizeof(lines[count]), file)) count++; fclose(file); }
    file = fopen("all_incomes.csv", "w");
    fprintf(file, "User,TotalIncome\n");
    for (int i = 1; i < count; i++) {
        char u[50]; double amt;
        if (sscanf(lines[i], "%[^,],%lf", u, &amt) == 2 && strcmp(u, currentUser) != 0)
            fprintf(file, "%s,%.2f\n", u, amt);
    }
    fclose(file);

    printf("Account deleted successfully.\n");
	printf("Please restart the application.\n");
	exit(0);
//	int main();
//	main();	causing undefined behaviour
}

// ------------------ VIEW & SUMMARY ------------------
void view_totals() {
    double inc = get_user_income();
    double exp = calculate_total_expense();
    printf("\nIncome: %.2f | Expense: %.2f | Balance: %.2f\n", inc, exp, inc - exp);
}

void generate_summary() {
    double inc = get_user_income(), exp = calculate_total_expense();
    printf("\nTotal Income: %.2f\nTotal Expense: %.2f\nBalance: %.2f\n", inc, exp, inc - exp);
    if (expenseCount == 0) { printf("No expenses.\n"); return; }

    // copies expenses to temporary arrays for sorting using bubble sort
    double sortedAmt[MAX_ENTRIES]; char sortedItem[MAX_ENTRIES][50];
    for (int i = 0; i < expenseCount; i++) {
        sortedAmt[i] = expenses[i];
        strcpy(sortedItem[i], expenseItem[i]);
    }
    for (int i = 0; i < expenseCount - 1; i++)
        for (int j = i + 1; j < expenseCount; j++)
            if (sortedAmt[j] > sortedAmt[i]) {
                double t = sortedAmt[i]; sortedAmt[i] = sortedAmt[j]; sortedAmt[j] = t; // swap expense amounts
                char s[50]; strcpy(s, sortedItem[i]); strcpy(sortedItem[i], sortedItem[j]); strcpy(sortedItem[j], s);
            }

    printf("\nTop 3 Items:\n");
    for (int i = 0; i < 3 && i < expenseCount; i++)
    {
        printf("%d. %s - %.2f\n", i + 1, sortedItem[i], sortedAmt[i]); // print top 3 items
	
	}
    // For each category, sum expenses and print percentage of total expense
    char cats[MAX_ENTRIES][50]; double catTotal[MAX_ENTRIES]; int catCount = 0;
    printf("\n\nCategory-wise Breakdown: (in order of addition)\n\n");
    for (int i = 0; i < expenseCount; i++) {
        int found = -1;
        for (int j = 0; j < catCount; j++)
            if (strcmp(expenseCategory[i], cats[j]) == 0) found = j;
        if (found != -1) catTotal[found] += expenses[i];
        else {
            strcpy(cats[catCount], expenseCategory[i]);
            catTotal[catCount++] = expenses[i];
        }
    }
    for (int i = 0; i < catCount; i++)
        printf("%s: %.2f (%.2f%%)\n", cats[i], catTotal[i], (catTotal[i] / exp) * 100); // print categories with totals and percentages
}

// ------------------ MAIN MENU ------------------
void main_menu() {
    int ch;
    while (1) {
        printf("\n--- Main Menu (%s) ---\n", currentUser);
        printf("1) Add Income\n2) Add Expense\n3) Delete Last Expense\n4) View Totals\n5) Summary\n6) Delete Account\n7) Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);
        getchar(); // remove leftover newline
        switch (ch) {
            case 1: add_income(); break;
            case 2: add_expense(); break;
            case 3: delete_last_expense(); break;
            case 4: view_totals(); break;
            case 5: generate_summary(); break;
            case 6: delete_account(); break;
            case 7: return;
            default: printf("Invalid choice.\n");
        }
    }
}

// ------------------ MAIN ------------------
int main() {
    load_users(); // load users from file
    while (1) {
        printf("\n========== EXPENSE TRACKER ==========\n");
        printf("1) Sign In\n2) Sign Up\n3) Exit\nEnter choice: ");
        int ch; 
        scanf("%d", &ch); 
        getchar();
        if (ch == 1) {
            if (sign_in()) {
                printf("Welcome %s!\n", currentUser);
                load_expenses();
                main_menu();
            } 
            else 
            printf("Invalid login. pls try again.\n");
        } 
        else if (ch == 2) {
            sign_up();
            load_expenses();
            main_menu();
        } 
        else if (ch == 3) {
            printf("Bye!\n");
            break;
        } 
        else 
        printf("Invalid.\n");
    }
    return 0;
}
