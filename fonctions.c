#include "fonctions.h"
#include <stdlib.h>
#include <windows.h>
#include <mysql.h>
#include <time.h>
#include <locale.h>

int i, j, main_exit;

struct {
    char name[60];
    int acc_no ;
    char address[60];
    char citizenship[15];
    int phone;
    char acc_type[10];
    float amt;
    struct date dob;
    struct date deposit;
    struct date withdraw;
}add, upd, check, rem, transaction;
void func_return(){
printf("\n\n\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &main_exit);
        system("cls");
        if (main_exit == 1)
            menu();
        else
            close();
    }
void fordelay(int j) {
    int i, k;
    for (i = 0; i < j; i++)
        k = i;
}


MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;
void init_mysql() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(conn, "127.0.0.1", "root", "root", "bank_system", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(1);
    }
}

void create_table() {
    if (mysql_query(conn, "CREATE TABLE IF NOT EXISTS account ("
                   "acc_no INT PRIMARY KEY,"
                   "name VARCHAR(255),"
                   "dob DATE,"
                   "address VARCHAR(255),"
                   "citizenship VARCHAR(255),"
                   "phone INT,"
                   "acc_type VARCHAR(50),"
                   "amt FLOAT,"
                   "deposit DATE"
                   ");")) {
        fprintf(stderr, "Failed to create table: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    }
}

void close_mysql() {

    mysql_close(conn);
}



void new_acc() {
     init_mysql();
     create_table();
    setlocale(LC_ALL, "C");
     int cancel_option;

    printf("\nEnter the following details to create a new account:\n");

printf("account number : ");
    scanf("%d", &add.acc_no);

    printf("Enter the num : ");
    scanf("%s", add.name);
printf("\nEnter the date of birth (yyyy-mm-dd): ");
    scanf("%04d-%02d-%02d",&add.dob.year,&add.dob.month,&add.dob.day);
    printf("\nEnter the address: ");
    scanf("%s", add.address);
    printf("\nEnter the citizenship number: ");
    scanf("%s", add.citizenship);
    printf("\phone: ");
    scanf("%d", &add.phone);
     printf("\nType of account:\n\t#Saving\n\t#Current\n\t#Fixed1(for 1 year)\n\t#Fixed2(for 2 years)\n\t#Fixed3(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", add.acc_type);
    if (strcmp(add.acc_type, "Saving") != 0 &&
        strcmp(add.acc_type, "Current") != 0 &&
        strcmp(add.acc_type, "Fixed1") != 0 &&
        strcmp(add.acc_type, "Fixed2") != 0 &&
        strcmp(add.acc_type, "Fixed3") != 0) {
        fprintf(stderr, "\nInvalid account type. Please enter a valid account type.\n");
        close_mysql();
        exit(1);
    }
    printf("\nEnter the amount to deposit:$");
    scanf("%f", &add.amt);
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    add.deposit.day = tm_info.tm_mday;
    add.deposit.month = tm_info.tm_mon + 1;
    add.deposit.year = tm_info.tm_year + 1900;

 printf("\nDo you want to continue  the account creation?\n");
    printf("Enter 0 to cancel and return to the main menu 1 to contenu: ");
    int confirm;
    scanf("%d", &confirm);

    if (confirm == 0) {
        printf("Account creation canceled. Returning to the main menu.\n");
        menu();
        return;
    }
    char query[200];
    sprintf(query, "INSERT INTO account VALUES (%d, '%s', '%04d-%02d-%02d', '%s', '%s', %d, '%s', %f, '%04d-%02d-%02d')",
            add.acc_no, add.name, add.dob.year,add.dob.month,add.dob.day, add.address, add.citizenship, add.phone, add.acc_type, add.amt,add.deposit.year,add.deposit.month,add.deposit.day);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to insert Account : %s\n");
        close_mysql();
        exit(1);
    }
    printf("Account created successfully!\n");
    close_mysql();
    printf("\n\n\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &main_exit);
        system("cls");
        if (main_exit == 1)
            menu();
        else
            close();
}


void view_list() {
    init_mysql();
    MYSQL_RES* result;
    MYSQL_ROW row;
    if (mysql_query(conn, "SELECT * FROM account")) {
        fprintf(stderr, "Failed to execute query: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    }
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Failed to retrieve result set: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    } else {
        printf("\n======================================================================================================================================================================\n");
        printf("%-15s%-25s%-15s%-25s%-15s%-20s%-15s%-15s%-25s\n", "Account No", "Name", "Date of Birth", "Address", "Citizenship", "Phone", "Account Type", "Amount", "Deposit Date");
        printf("==========================================================================================================================================================================\n");
        while ((row = mysql_fetch_row(result)) != NULL) {
            printf("%-15s%-25s%-15s%-25s%-15s%-20s%-15s%-15s%-25s\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
        }
        printf("\n");
        mysql_free_result(result);
        close_mysql();
        printf("\n\n\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &main_exit);
        system("cls");
        if (main_exit == 1)
            menu();
        else
            close();
    }
}
void edit() {
    init_mysql();
    int acc_no_to_edit;
    printf("Enter the account number you want to edit: ");
    scanf("%d", &acc_no_to_edit);
    char query[100];
    sprintf(query, "SELECT * FROM account WHERE acc_no = %d", acc_no_to_edit);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to execute query: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    }
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Failed to retrieve result set: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    }
    if (mysql_num_rows(result) == 0) {
        printf("Account not found.\n");
        printf("\n\n\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &main_exit);
        system("cls");
        if (main_exit == 1)
            menu();
        else
            close();


    } else {
        MYSQL_ROW row = mysql_fetch_row(result);
        printf("\nCurrent Account Details:\n");
        printf("Account No: %s\n", row[0]);
        printf("Name: %s\n", row[1]);
        printf("Date of Birth: %s\n", row[2]);
        printf("Address: %s\n", row[3]);
        printf("Citizenship: %s\n", row[4]);
        printf("Phone: %s\n", row[5]);
        printf("Account Type: %s\n", row[6]);
        printf("Amount: %s\n", row[7]);
        printf("Deposit Date: %s\n", row[8]);
        printf("\nEnter new details:\n");
        printf("Enter the new name: ");
        scanf("%s", add.name);
        printf("Enter the new date of birth (yyyy-mm-dd): ");
        scanf("%04d-%02d-%02d", &add.dob.year, &add.dob.month, &add.dob.day);
        printf("Enter the new address: ");
        scanf("%s", add.address);
        printf("Enter the new citizenship number: ");
        scanf("%s", add.citizenship);
        printf("Enter the new phone number: ");
        scanf("%d", &add.phone);
        printf("Enter the new account type: ");
        scanf("%s", add.acc_type);
        sprintf(query, "UPDATE account SET name = '%s', dob = '%04d-%02d-%02d', address = '%s', citizenship = '%s', phone = %d, acc_type = '%s' WHERE acc_no = %d",
            add.name, add.dob.year, add.dob.month, add.dob.day, add.address, add.citizenship, add.phone, add.acc_type, acc_no_to_edit);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Failed to execute query: %s\n");
            close_mysql();
            exit(1);
        }
        printf("Account details updated successfully!\n");

 printf("\n\n\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &main_exit);
        system("cls");
        if (main_exit == 1)
            menu();
        else
            close();
    }
    mysql_free_result(result);
    close_mysql();
}
void transact(void) {
    init_mysql();
    int choice, test = 0;
    printf("Enter the account no. of the customer:");
    scanf("%d", &transaction.acc_no);
    char query[100];
    sprintf(query, "SELECT * FROM account WHERE acc_no = %d", transaction.acc_no);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to execute query: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    }
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Failed to retrieve result set: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);
    }
    if (mysql_num_rows(result) == 0) {
        printf("Account not found.\n");
       func_return();
    }
     else {
                setlocale(LC_ALL, "C");

        MYSQL_ROW row = mysql_fetch_row(result);

        printf("\nCurrent Account Details:\n");
        printf("Account No: %s\n", row[0]);
        printf("Name: %s\n", row[1]);
        printf("Date of Birth: %s\n", row[2]);
        printf("Address: %s\n", row[3]);
        printf("Citizenship: %s\n", row[4]);
        printf("Phone: %s\n", row[5]);
        printf("Account Type: %s\n", row[6]);
        printf("Amount: %s\n", row[7]);
        printf("Deposit Date: %s\n", row[8]);
        printf("\nDo you want to\n1.Deposit\n2.Withdraw?\nEnter your choice (1 for deposit and 2 for withdraw): ");
        scanf("%d", &choice);
        if (choice == 1) {
            printf("Enter the amount you want to deposit: $ ");
            scanf("%f", &transaction.amt);
            sprintf(query, "UPDATE account SET amt = amt + %.2f WHERE acc_no = %d", transaction.amt, transaction.acc_no);
            printf("\n\nDeposited successfully!");
           func_return();
        } else if (choice == 2) {
            printf("Enter the amount you want to withdraw: $ ");
            scanf("%f", &transaction.amt);
            sprintf(query, "UPDATE account SET amt = amt - %.2f WHERE acc_no = %d", transaction.amt, transaction.acc_no);
            printf("\n\nWithdrawn successfully!");
        } else {
            printf("\nInvalid choice!\n");
            close_mysql();
            return;
        }
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Failed to : %s\n", mysql_error(conn));
            close_mysql();
            exit(1);
        }
        test = 1;
    }
    mysql_free_result(result);
    close_mysql();
      func_return();
}
void erase() {
    init_mysql();
    int acc_no_to_delete;
    printf("Enter the account number you want to delete: ");
    scanf("%d", &acc_no_to_delete);

    char query[100];
    sprintf(query, "DELETE FROM account WHERE acc_no = %d", acc_no_to_delete);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to execute query: %s\n", mysql_error(conn));
        close_mysql();
        exit(1);

    }

    if (mysql_affected_rows(conn) > 0) {
        printf("Account deleted successfully from the database!\n");
        func_return();
    } else {
        printf("Account not found in the database.\n");
        func_return();
    }

    // Close MySQL connection
    close_mysql();
}

void see() {
    init_mysql();
    int choice;
    printf("\n\n\n\n\n\t\tEnter 1 to search by account number or 2 to search by account name: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int acc_no_to_search;
        printf("\nEnter the account number you want to search: ");
        scanf("%d", &acc_no_to_search);

        char query[100];
        sprintf(query, "SELECT * FROM account WHERE acc_no = %d", acc_no_to_search);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Failed to execute query: %s\n", mysql_error(conn));
            close_mysql();
            exit(1);
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result\n");
            close_mysql();
            exit(1);
        }

        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printAccountDetails(row);
            func_return();
        } else {
            printf("Account not found.\n");
            func_return();
        }

        mysql_free_result(result);
    } else if (choice == 2) {
        char acc_name_to_search[60];
        printf("Enter the account name you want to search: ");
        scanf("%s", acc_name_to_search);

        char query[100];
        sprintf(query, "SELECT * FROM account WHERE name = '%s'", acc_name_to_search);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Failed to execute query: %s\n", mysql_error(conn));
            close_mysql();
            exit(1);
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result set: %s\n", mysql_error(conn));
            close_mysql();
            exit(1);
        }

        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printAccountDetails(row);
            func_return();
        } else {
            printf("Account not found.\n");
            func_return();
        }

        mysql_free_result(result);
    } else {
        printf("Invalid choice!\n");
                    func_return();

    }

    close_mysql();
}

void printAccountDetails(MYSQL_ROW row) {
    printf("\nAccount Details:\n");
    printf("Account No: %s\n", row[0]);
    printf("Name: %s\n", row[1]);
    printf("Date of Birth: %s\n", row[2]);
    printf("Address: %s\n", row[3]);
    printf("Citizenship: %s\n", row[4]);
    printf("Phone: %s\n", row[5]);
    printf("Account Type: %s\n", row[6]);
    printf("Amount: %s\n", row[7]);
    printf("Deposit Date: %s\n", row[8]);
}


void menu(void) {
    int choice;
    system("cls");
    system("color 7");
    printf("\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t" ANSI_COLOR_CYN "BANKING MANAGEMENT SYSTEM");
    printf("\n\n\n\t\t\t\t\t\t\t" ANSI_COLOR_BLU "**************** WELCOME TO THE MAIN MENU ****************");
    printf("\n\n\n\t\t\t\t\t\t\t" ANSI_COLOR_GRN "[1] Create a new account\n\n\t\t\t\t\t\t\t"ANSI_COLOR_MAG"[2] Update information of an existing account\n\n\t\t\t\t\t\t\t"ANSI_COLOR_GRN"[3] Transactions\n\n\t\t\t\t\t\t\t"ANSI_COLOR_YEL

    "[4] Check the details of an existing account\n\n\t\t\t\t\t\t\t"ANSI_COLOR_RED"[5] Remove an existing account\n\n\t\t\t\t\t\t\t"ANSI_COLOR_YEL"[6] View the customer's list\n\n\t\t\t\t\t\t\t"ANSI_COLOR_BLU"[7]Exit\n\n\n\n\t\t\t\t\t\t\t "ANSI_COLOR_CYN" Enter your choice:" );
    scanf("%d", &choice);
    system("cls");
    switch (choice) {
        case 1:
            new_acc();
            break;
        case 2:
            edit();
            break;
        case 3:
            transact();
            break;
        case 4:
             see();
            break;
        case 5:

            erase();
            break;
        case 6:
            view_list();
            break;
        case 7:
            close();
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
    }

}
