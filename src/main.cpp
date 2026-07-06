#define NOMINMAX

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <limits>
#include <filesystem>

struct User
{
    std::string username;
    std::string passwordHash;
};

struct Transaction
{
    int id = 0;
    std::string date;
    std::string type;
    std::string category;
    std::string description;
    double amount = 0.0;
};

struct FinancialSummary
{
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    double balance = 0.0;
};

void createDataFolder()
{
    try
    {
        std::filesystem::create_directories("data");
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cout << "Error: Could not create data folder." << std::endl;
        std::cout << error.what() << std::endl;
    }
}

std::string hashPassword(const std::string& password)
{
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

void displayMainMenu()
{
    std::cout << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "      Personal Finance Manager" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "1. Register" << std::endl;
    std::cout << "2. Login" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Please choose an option: ";
}

void displayFinanceMenu()
{
    std::cout << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "          Finance Menu" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "1. Add Transaction" << std::endl;
    std::cout << "2. Display All Transactions" << std::endl;
    std::cout << "3. Display Income Transactions" << std::endl;
    std::cout << "4. Display Expense Transactions" << std::endl;
    std::cout << "5. Search by Category" << std::endl;
    std::cout << "6. Show Financial Summary" << std::endl;
    std::cout << "7. Export Transactions to CSV" << std::endl;
    std::cout << "8. Save Summary Report" << std::endl;
    std::cout << "9. Logout" << std::endl;
    std::cout << "Please choose an option: ";
}

int getMenuChoice()
{
    int choice;

    if (!(std::cin >> choice))
    {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return choice;
}

bool usernameExists(const std::vector<User>& users, const std::string& username)
{
    for (const User& user : users)
    {
        if (user.username == username)
        {
            return true;
        }
    }

    return false;
}

void saveUsersToFile(const std::vector<User>& users, const std::string& filename)
{
    createDataFolder();

    std::ofstream file(filename);

    if (!file)
    {
        std::cout << "Error: Could not save users." << std::endl;
        return;
    }

    for (const User& user : users)
    {
        file << user.username << "|"
            << user.passwordHash << std::endl;
    }

    file.close();
}

void loadUsersFromFile(std::vector<User>& users, const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        return;
    }

    users.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        User user;

        std::getline(ss, user.username, '|');
        std::getline(ss, user.passwordHash, '|');

        if (!user.username.empty() && !user.passwordHash.empty())
        {
            users.push_back(user);
        }
    }

    file.close();
}

void registerUser(std::vector<User>& users)
{
    User newUser;
    std::string password;
    std::string confirmPassword;

    std::cout << "Enter username: ";
    std::getline(std::cin, newUser.username);

    if (newUser.username.empty())
    {
        std::cout << "Username cannot be empty." << std::endl;
        return;
    }

    if (usernameExists(users, newUser.username))
    {
        std::cout << "Username already exists." << std::endl;
        return;
    }

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    std::cout << "Confirm password: ";
    std::getline(std::cin, confirmPassword);

    if (password.empty())
    {
        std::cout << "Password cannot be empty." << std::endl;
        return;
    }

    if (password != confirmPassword)
    {
        std::cout << "Passwords do not match." << std::endl;
        return;
    }

    newUser.passwordHash = hashPassword(password);

    users.push_back(newUser);

    std::cout << "Registration successful." << std::endl;
}

bool loginUser(const std::vector<User>& users)
{
    std::string username;
    std::string password;

    std::cout << "Enter username: ";
    std::getline(std::cin, username);

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    std::string passwordHash = hashPassword(password);

    for (const User& user : users)
    {
        if (user.username == username && user.passwordHash == passwordHash)
        {
            std::cout << "Login successful. Welcome, " << username << "!" << std::endl;
            return true;
        }
    }

    std::cout << "Invalid username or password." << std::endl;
    return false;
}

int generateTransactionId(const std::vector<Transaction>& transactions)
{
    int maxId = 0;

    for (const Transaction& transaction : transactions)
    {
        if (transaction.id > maxId)
        {
            maxId = transaction.id;
        }
    }

    return maxId + 1;
}

void addTransaction(std::vector<Transaction>& transactions)
{
    Transaction transaction;

    transaction.id = generateTransactionId(transactions);

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::getline(std::cin, transaction.date);

    if (transaction.date.empty())
    {
        std::cout << "Date cannot be empty." << std::endl;
        return;
    }

    std::cout << "Enter type (Income/Expense): ";
    std::getline(std::cin, transaction.type);

    if (transaction.type != "Income" && transaction.type != "Expense")
    {
        std::cout << "Invalid type. Please enter Income or Expense." << std::endl;
        return;
    }

    std::cout << "Enter category: ";
    std::getline(std::cin, transaction.category);

    if (transaction.category.empty())
    {
        std::cout << "Category cannot be empty." << std::endl;
        return;
    }

    std::cout << "Enter description: ";
    std::getline(std::cin, transaction.description);

    if (transaction.description.empty())
    {
        transaction.description = "No description";
    }

    std::cout << "Enter amount: ";

    if (!(std::cin >> transaction.amount))
    {
        std::cout << "Invalid amount. Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (transaction.amount <= 0)
    {
        std::cout << "Amount must be greater than zero." << std::endl;
        return;
    }

    transactions.push_back(transaction);

    std::cout << "Transaction added successfully." << std::endl;
}

void displayTransaction(const Transaction& transaction)
{
    std::cout << "Transaction ID: " << transaction.id << std::endl;
    std::cout << "Date          : " << transaction.date << std::endl;
    std::cout << "Type          : " << transaction.type << std::endl;
    std::cout << "Category      : " << transaction.category << std::endl;
    std::cout << "Description   : " << transaction.description << std::endl;
    std::cout << "Amount        : "
        << std::fixed << std::setprecision(2)
        << transaction.amount << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

void displayAllTransactions(const std::vector<Transaction>& transactions)
{
    if (transactions.empty())
    {
        std::cout << "No transactions found." << std::endl;
        return;
    }

    std::cout << std::endl;
    std::cout << "========== All Transactions ==========" << std::endl;

    for (const Transaction& transaction : transactions)
    {
        displayTransaction(transaction);
    }
}

void displayTransactionsByType(
    const std::vector<Transaction>& transactions,
    const std::string& type)
{
    bool found = false;

    std::cout << std::endl;
    std::cout << "========== " << type << " Transactions ==========" << std::endl;

    for (const Transaction& transaction : transactions)
    {
        if (transaction.type == type)
        {
            displayTransaction(transaction);
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No " << type << " transactions found." << std::endl;
    }
}

void searchByCategory(const std::vector<Transaction>& transactions)
{
    std::string category;
    bool found = false;

    std::cout << "Enter category to search: ";
    std::getline(std::cin, category);

    std::cout << std::endl;
    std::cout << "========== Search Results ==========" << std::endl;

    for (const Transaction& transaction : transactions)
    {
        if (transaction.category == category)
        {
            displayTransaction(transaction);
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No transactions found for category: " << category << std::endl;
    }
}

FinancialSummary calculateFinancialSummary(const std::vector<Transaction>& transactions)
{
    FinancialSummary summary;

    for (const Transaction& transaction : transactions)
    {
        if (transaction.type == "Income")
        {
            summary.totalIncome += transaction.amount;
        }
        else if (transaction.type == "Expense")
        {
            summary.totalExpenses += transaction.amount;
        }
    }

    summary.balance = summary.totalIncome - summary.totalExpenses;

    return summary;
}

void displayFinancialSummary(const FinancialSummary& summary)
{
    std::cout << std::endl;
    std::cout << "========== Financial Summary ==========" << std::endl;
    std::cout << "Total Income  : "
        << std::fixed << std::setprecision(2)
        << summary.totalIncome << std::endl;
    std::cout << "Total Expenses: "
        << std::fixed << std::setprecision(2)
        << summary.totalExpenses << std::endl;
    std::cout << "Balance       : "
        << std::fixed << std::setprecision(2)
        << summary.balance << std::endl;
}

void saveTransactionsToFile(
    const std::vector<Transaction>& transactions,
    const std::string& filename)
{
    createDataFolder();

    std::ofstream file(filename);

    if (!file)
    {
        std::cout << "Error: Could not save transactions." << std::endl;
        return;
    }

    for (const Transaction& transaction : transactions)
    {
        file << transaction.id << "|"
            << transaction.date << "|"
            << transaction.type << "|"
            << transaction.category << "|"
            << transaction.description << "|"
            << transaction.amount << std::endl;
    }

    file.close();
}

void loadTransactionsFromFile(
    std::vector<Transaction>& transactions,
    const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        return;
    }

    transactions.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string idText;
        std::string amountText;
        Transaction transaction;

        std::getline(ss, idText, '|');
        std::getline(ss, transaction.date, '|');
        std::getline(ss, transaction.type, '|');
        std::getline(ss, transaction.category, '|');
        std::getline(ss, transaction.description, '|');
        std::getline(ss, amountText, '|');

        try
        {
            transaction.id = std::stoi(idText);
            transaction.amount = std::stod(amountText);

            if (!transaction.date.empty() &&
                !transaction.type.empty() &&
                !transaction.category.empty() &&
                transaction.amount > 0)
            {
                transactions.push_back(transaction);
            }
        }
        catch (const std::exception&)
        {
            std::cout << "Warning: Skipping invalid transaction record." << std::endl;
        }
    }

    file.close();
}

std::string escapeCsvField(const std::string& field)
{
    bool needsQuotes = field.find(',') != std::string::npos ||
        field.find('"') != std::string::npos ||
        field.find('\n') != std::string::npos;

    if (!needsQuotes)
    {
        return field;
    }

    std::string escaped = "\"";

    for (char ch : field)
    {
        if (ch == '"')
        {
            escaped += "\"\"";
        }
        else
        {
            escaped += ch;
        }
    }

    escaped += "\"";

    return escaped;
}

void exportTransactionsToCsv(
    const std::vector<Transaction>& transactions,
    const std::string& filename)
{
    createDataFolder();

    std::ofstream file(filename);

    if (!file)
    {
        std::cout << "Error: Could not export CSV file." << std::endl;
        return;
    }

    file << "ID,Date,Type,Category,Description,Amount" << std::endl;

    for (const Transaction& transaction : transactions)
    {
        file << transaction.id << ","
            << escapeCsvField(transaction.date) << ","
            << escapeCsvField(transaction.type) << ","
            << escapeCsvField(transaction.category) << ","
            << escapeCsvField(transaction.description) << ","
            << std::fixed << std::setprecision(2)
            << transaction.amount << std::endl;
    }

    file.close();

    std::cout << "Transactions exported to CSV successfully." << std::endl;
}

void saveSummaryReport(
    const std::vector<Transaction>& transactions,
    const std::string& filename)
{
    createDataFolder();

    FinancialSummary summary = calculateFinancialSummary(transactions);

    std::ofstream file(filename);

    if (!file)
    {
        std::cout << "Error: Could not save summary report." << std::endl;
        return;
    }

    file << "========== Personal Finance Summary ==========" << std::endl;
    file << "Total Income  : "
        << std::fixed << std::setprecision(2)
        << summary.totalIncome << std::endl;

    file << "Total Expenses: "
        << std::fixed << std::setprecision(2)
        << summary.totalExpenses << std::endl;

    file << "Balance       : "
        << std::fixed << std::setprecision(2)
        << summary.balance << std::endl;

    file << std::endl;
    file << "========== Transactions ==========" << std::endl;

    for (const Transaction& transaction : transactions)
    {
        file << transaction.id << " | "
            << transaction.date << " | "
            << transaction.type << " | "
            << transaction.category << " | "
            << transaction.description << " | "
            << std::fixed << std::setprecision(2)
            << transaction.amount << std::endl;
    }

    file.close();

    std::cout << "Summary report saved successfully." << std::endl;
}

void financeArea(
    std::vector<Transaction>& transactions,
    const std::string& transactionFile,
    const std::string& csvFile,
    const std::string& reportFile)
{
    while (true)
    {
        displayFinanceMenu();

        int choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            addTransaction(transactions);
            saveTransactionsToFile(transactions, transactionFile);
            break;

        case 2:
            displayAllTransactions(transactions);
            break;

        case 3:
            displayTransactionsByType(transactions, "Income");
            break;

        case 4:
            displayTransactionsByType(transactions, "Expense");
            break;

        case 5:
            searchByCategory(transactions);
            break;

        case 6:
        {
            FinancialSummary summary = calculateFinancialSummary(transactions);
            displayFinancialSummary(summary);
            break;
        }

        case 7:
            exportTransactionsToCsv(transactions, csvFile);
            break;

        case 8:
            saveSummaryReport(transactions, reportFile);
            break;

        case 9:
            std::cout << "You have logged out." << std::endl;
            return;

        default:
            std::cout << "Invalid option. Please choose again." << std::endl;
        }
    }
}

int main()
{
    createDataFolder();

    std::vector<User> users;
    std::vector<Transaction> transactions;

    const std::string userFile = "data/users.txt";
    const std::string transactionFile = "data/transactions.txt";
    const std::string csvFile = "data/transactions.csv";
    const std::string reportFile = "data/summary_report.txt";

    loadUsersFromFile(users, userFile);
    loadTransactionsFromFile(transactions, transactionFile);

    while (true)
    {
        displayMainMenu();

        int choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            registerUser(users);
            saveUsersToFile(users, userFile);
            break;

        case 2:
            if (loginUser(users))
            {
                financeArea(transactions, transactionFile, csvFile, reportFile);
            }
            break;

        case 3:
            saveUsersToFile(users, userFile);
            saveTransactionsToFile(transactions, transactionFile);
            std::cout << "Thank you for using the Personal Finance Manager." << std::endl;
            return 0;

        default:
            std::cout << "Invalid option. Please choose again." << std::endl;
        }
    }
}