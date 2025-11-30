#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define FILENAME "bank_data.txt"

// Structure to store account information
struct Account {
    char name[50];
    int accountNumber;
    float balance;
};

// Global array to store all accounts and counter
struct Account accounts[MAX_ACCOUNTS];
int totalAccounts = 0;

// Function prototypes
void loadAccounts();
void saveAccounts();
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
int findAccount(int accNum);
void displayMenu();

int main() {
    int choice;
    
    // Load existing accounts from file at program start
    loadAccounts();
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                printf("\nThank you for using our banking system!\n");
                printf("All data has been saved successfully.\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
        
    } while(choice != 5);
    
    return 0;
}

void displayMenu() {
    system("clear || cls");  // Clear screen (works on both Unix and Windows)
    printf("\n========================================\n");
    printf("     BANKING MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("1. Create New Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("5. Exit\n");
    printf("========================================\n");
}

/**
 * FILE I/O: Load accounts from file
 * This function reads all account data from 'bank_data.txt' 
 * when the program starts, restoring previous session data
 */
void loadAccounts() {
    FILE *file;
    
    // Open file in read mode
    file = fopen(FILENAME, "r");
    
    // If file doesn't exist, start with empty accounts
    if(file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    
    // Read total number of accounts from first line
    fscanf(file, "%d", &totalAccounts);
    
    // Read each account's data line by line
    // Format: Name AccountNumber Balance
    for(int i = 0; i < totalAccounts; i++) {
        fscanf(file, "%s %d %f", 
               accounts[i].name, 
               &accounts[i].accountNumber, 
               &accounts[i].balance);
    }
    
    // Close the file after reading
    fclose(file);
    
    printf("Loaded %d account(s) from file.\n", totalAccounts);
}

/**
 * FILE I/O: Save accounts to file
 * This function writes all current account data to 'bank_data.txt'
 * after every transaction, ensuring data persistence
 */
void saveAccounts() {
    FILE *file;
    
    // Open file in write mode (overwrites existing content)
    file = fopen(FILENAME, "w");
    
    if(file == NULL) {
        printf("Error: Unable to save data to file!\n");
        return;
    }
    
    // Write total number of accounts as first line
    fprintf(file, "%d\n", totalAccounts);
    
    // Write each account's data on separate lines
    // Format: Name AccountNumber Balance
    for(int i = 0; i < totalAccounts; i++) {
        fprintf(file, "%s %d %.2f\n", 
                accounts[i].name, 
                accounts[i].accountNumber, 
                accounts[i].balance);
    }
    
    // Close the file to ensure data is written
    fclose(file);
}

void createAccount() {
    if(totalAccounts >= MAX_ACCOUNTS) {
        printf("\nError: Maximum account limit reached!\n");
        return;
    }
    
    struct Account newAcc;
    
    printf("\n--- Create New Account ---\n");
    printf("Enter Account Holder Name: ");
    scanf("%s", newAcc.name);
    
    printf("Enter Account Number: ");
    scanf("%d", &newAcc.accountNumber);
    
    // Check if account number already exists
    if(findAccount(newAcc.accountNumber) != -1) {
        printf("\nError: Account number already exists!\n");
        return;
    }
    
    printf("Enter Initial Deposit: $");
    scanf("%f", &newAcc.balance);
    
    if(newAcc.balance < 0) {
        printf("\nError: Initial deposit cannot be negative!\n");
        return;
    }
    
    // Add new account to array
    accounts[totalAccounts] = newAcc;
    totalAccounts++;
    
    // Save to file immediately
    saveAccounts();
    
    printf("\n✓ Account created successfully!\n");
    printf("Account Number: %d\n", newAcc.accountNumber);
    printf("Account Holder: %s\n", newAcc.name);
    printf("Balance: $%.2f\n", newAcc.balance);
}

void depositMoney() {
    int accNum;
    float amount;
    
    printf("\n--- Deposit Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    printf("Enter Amount to Deposit: $");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\nError: Deposit amount must be positive!\n");
        return;
    }
    
    // Add amount to balance
    accounts[index].balance += amount;
    
    // Save to file immediately
    saveAccounts();
    
    printf("\n✓ Deposit successful!\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Deposited: $%.2f\n", amount);
    printf("New Balance: $%.2f\n", accounts[index].balance);
}

void withdrawMoney() {
    int accNum;
    float amount;
    
    printf("\n--- Withdraw Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    printf("Current Balance: $%.2f\n", accounts[index].balance);
    printf("Enter Amount to Withdraw: $");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\nError: Withdrawal amount must be positive!\n");
        return;
    }
    
    // Check if sufficient balance exists
    if(amount > accounts[index].balance) {
        printf("\nError: Insufficient balance!\n");
        printf("Available Balance: $%.2f\n", accounts[index].balance);
        return;
    }
    
    // Subtract amount from balance
    accounts[index].balance -= amount;
    
    // Save to file immediately
    saveAccounts();
    
    printf("\n✓ Withdrawal successful!\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Withdrawn: $%.2f\n", amount);
    printf("Remaining Balance: $%.2f\n", accounts[index].balance);
}

void checkBalance() {
    int accNum;
    
    printf("\n--- Check Balance ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    printf("\n--- Account Details ---\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Current Balance: $%.2f\n", accounts[index].balance);
}

/**
 * Helper function to find an account by account number
 * Returns the index in the array, or -1 if not found
 */
int findAccount(int accNum) {
    for(int i = 0; i < totalAccounts; i++) {
        if(accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;  // Account not found
}