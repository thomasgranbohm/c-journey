#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ID_VALUE 10000

typedef struct
{
    int id;
    char holder_name[128];
    float balance;
} BankAccount;

typedef struct
{
    int amount;
    int capacity;
    BankAccount *accounts;
} Bank;

typedef enum
{
    NEW_ACCOUNT = 'n',
    CLOSE_ACCOUNT = 'c',
    VIEW_ACCOUNT = 'v',
    LIST_ACCOUNTS = 'l',
    DEPOSIT = 'd',
    WITHDRAW = 'w',
    TRANSFER = 't',
    QUIT = 'q',
} Command;

int get_account_id()
{
    char buffer[256];
    int account_id;
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &account_id) != 1 || account_id < 0 || account_id >= MAX_ID_VALUE)
    {
        printf("Please input a valid account number.\n");
        return -1;
    }

    return account_id;
}

int find_account_index(Bank *bank, int account_id)
{
    for (int i = 0; i < bank->amount; i++)
        if (bank->accounts[i].id == account_id)
            return i;

    return -1;
}

Bank *open_bank()
{
    Bank *bank = malloc(sizeof(Bank));
    if (!bank)
    {
        perror("bank malloc failed");
        exit(EXIT_FAILURE);
    }

    bank->amount = 0;
    bank->capacity = 1;

    BankAccount *accounts = malloc(sizeof(BankAccount) * bank->capacity);
    if (!accounts)
    {
        perror("accounts malloc failed");
        exit(EXIT_FAILURE);
    }

    bank->accounts = accounts;

    return bank;
}

void close_bank(Bank *bank)
{
    free(bank->accounts);
    free(bank);
}

void create_account(Bank *bank)
{
    char buffer[256];

    BankAccount account;
    do
    {
        account.id = rand() % MAX_ID_VALUE;
    } while (find_account_index(bank, account.id) != -1);

    printf("Enter account holder's name: ");
    fgets(buffer, sizeof(buffer), stdin);
    strncpy(account.holder_name, buffer, sizeof(account.holder_name));
    account.holder_name[strcspn(account.holder_name, "\n")] = '\0';

    if (account.holder_name[0] == '\0')
    {
        printf("Please input a valid name.\n");
        return;
    }

    printf("Enter initial deposit: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", &account.balance) != 1)
    {
        printf("Please input digits only.\n");
        return;
    }
    else if (account.balance < 0)
    {
        printf("Please input only positive integers.\n");
        return;
    }

    if (bank->amount == bank->capacity)
    {
        if (bank->capacity == 0)
            ++bank->capacity;
        else
            bank->capacity *= 2;

        BankAccount *new_accounts = realloc(bank->accounts, sizeof(BankAccount) * bank->capacity);

        if (!new_accounts)
        {
            perror("accounts realloc error");
            exit(EXIT_FAILURE);
        }

        bank->accounts = new_accounts;
    }

    bank->accounts[bank->amount] = account;
    bank->amount++;

    printf("Account created! Your account number is %04d.\n", account.id);
}

void close_account(Bank *bank)
{
    printf("Enter account number: ");
    int account_id = get_account_id();
    if (account_id == -1)
        return;

    int account_index = find_account_index(bank, account_id);
    if (account_index == -1)
    {
        printf("Could not find account #%04d. Please try again!\n", account_id);
        return;
    }

    memmove(&bank->accounts[account_index],
            &bank->accounts[account_index + 1],
            sizeof(BankAccount) * (bank->amount - account_index - 1));

    bank->amount--;

    printf("Successfully removed #%04d.\n", account_id);
}

void view_account(Bank *bank)
{
    printf("Enter account number: ");
    int account_id = get_account_id();
    if (account_id == -1)
        return;

    int account_index = find_account_index(bank, account_id);
    if (account_index == -1)
    {
        printf("Could not find account #%04d. Please try again!\n", account_id);
        return;
    }

    BankAccount account = bank->accounts[account_index];
    printf("--- Account details ---\n");
    printf("Account number: %04d\n", account.id);
    printf("Holder name: %s\n", account.holder_name);
    printf("Balance: $%.02f\n", account.balance);
}

void deposit_money(Bank *bank)
{
    printf("Enter account number: ");
    int account_id = get_account_id();
    if (account_id == -1)
        return;

    int account_index = find_account_index(bank, account_id);
    if (account_index == -1)
    {
        printf("Could not find account #%04d. Please try again!\n", account_id);
        return;
    }

    char buffer[256];
    float deposit_amount;
    printf("Enter deposit amount: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", &deposit_amount) != 1 || deposit_amount < 0)
    {
        printf("Please input a valid deposit amount.\n");
        return;
    }

    bank->accounts[account_index].balance += deposit_amount;
    printf(
        "Successfully deposited $%.02f from account #%04d. New balance is $%.02f!\n",
        deposit_amount,
        account_id,
        bank->accounts[account_index].balance);
}

void withdraw_money(Bank *bank)
{
    printf("Enter account number: ");
    int account_id = get_account_id();
    if (account_id == -1)
        return;

    int account_index = find_account_index(bank, account_id);
    if (account_index == -1)
    {
        printf("Could not find account #%04d. Please try again!\n", account_id);
        return;
    }

    char buffer[256];
    float withdrawal_amount;
    printf("Enter withdrawal amount: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", &withdrawal_amount) != 1 || withdrawal_amount < 0)
    {
        printf("Please input a valid withdrawal amount.\n");
        return;
    }
    else if (bank->accounts[account_index].balance < withdrawal_amount)
    {
        printf("Balance too low!\n");
        return;
    }

    bank->accounts[account_index].balance -= withdrawal_amount;
    printf("Successfully withdrew $%.02f from account #%04d. New balance is $%.02f!\n",
           withdrawal_amount,
           account_id,
           bank->accounts[account_index].balance);
}

void transfer_money(Bank *bank)
{
    printf("Enter sender account number: ");
    int sender_id = get_account_id();
    if (sender_id == -1)
        return;

    int sender_index = find_account_index(bank, sender_id);
    if (sender_index == -1)
    {
        printf("Could not find account #%04d. Please try again!\n", sender_id);
        return;
    }

    printf("Enter recipient account number: ");
    int recipient_id = get_account_id();
    if (recipient_id == -1)
        return;

    int recipient_index = find_account_index(bank, recipient_id);
    if (recipient_index == -1)
    {
        printf("Could not find account #%04d. Please try again!\n", recipient_id);
        return;
    }

    char buffer[256];
    float amount;
    printf("Enter withdrawal amount: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", &amount) != 1 || amount < 0)
    {
        printf("Please input a valid withdrawal amount.\n");
        return;
    }
    else if (bank->accounts[sender_index].balance < amount)
    {
        printf("Balance to low!\n");
        return;
    }

    bank->accounts[sender_index].balance -= amount;
    bank->accounts[recipient_index].balance += amount;

    printf("Successfully transfered $%.02f from #%04d to #%04d!\n",
           amount,
           sender_id,
           recipient_id);
}

void list_accounts(Bank *bank)
{
    if (bank->amount == 0)
    {
        printf("The bank has no open accounts.\n");
        return;
    }

    puts("---- List of accounts ----");
    for (int i = 0; i < bank->amount; i++)
    {
        BankAccount account = *(bank->accounts + i);
        printf("Account #%04d | Holder: %s | Balance: $%.02f\n", account.id, account.holder_name, account.balance);
    }
}

void save_bank(Bank *bank)
{
    FILE *file = fopen("bank.dat", "wb");
    if (!file)
    {
        perror("Failed to open file for writing");
        return;
    }

    fwrite(&bank->amount, sizeof(bank->amount), 1, file);
    fwrite(bank->accounts, sizeof(BankAccount), bank->amount, file);

    fclose(file);
}

void load_bank(Bank *bank)
{
    FILE *file = fopen("bank.dat", "rb");
    if (!file)
        return;

    int amount;
    if (fread(&amount, sizeof(bank->amount), 1, file) != 1)
    {
        perror("Failed to read number of accounts");
        fclose(file);
        return;
    }

    bank->accounts = malloc(sizeof(BankAccount) * amount);
    if (fread(bank->accounts, sizeof(BankAccount), amount, file) != (size_t)amount)
    {
        perror("Failed to read accounts");
        free(bank->accounts);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    bank->amount = amount;
    bank->capacity = amount;
}

int main()
{
    srand(time(NULL));

    Bank *bank = open_bank();
    load_bank(bank);

    printf("Welcome to the Bank!\n");
    printf("---------------------\n");

    char command;
    int flush;
    do
    {
        printf(" [%c] Create new account\n", NEW_ACCOUNT);
        printf(" [%c] Close account\n", CLOSE_ACCOUNT);
        printf(" [%c] View account\n", VIEW_ACCOUNT);
        printf(" [%c] List all accounts\n", LIST_ACCOUNTS);
        printf(" [%c] Deposit money\n", DEPOSIT);
        printf(" [%c] Withdraw money\n", WITHDRAW);
        printf(" [%c] Transfer money\n", TRANSFER);
        printf(" [%c] Quit\n", QUIT);
        printf("\n> ");
        command = getchar();

        while ((flush = getchar()) != '\n' && flush != EOF)
            ;

        switch (command)
        {
        case NEW_ACCOUNT:
            create_account(bank);
            break;
        case CLOSE_ACCOUNT:
            close_account(bank);
            break;
        case VIEW_ACCOUNT:
            view_account(bank);
            break;
        case LIST_ACCOUNTS:
            list_accounts(bank);
            break;
        case DEPOSIT:
            deposit_money(bank);
            break;
        case WITHDRAW:
            withdraw_money(bank);
            break;
        case TRANSFER:
            transfer_money(bank);
            break;
        case QUIT:
            break;
        default:
            printf("Command not found. Please try again!\n");
            break;
        }
    } while (command != QUIT);

    save_bank(bank);
    close_bank(bank);

    puts("Goodbye!");

    return 0;
}