
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

struct Transaction {
    int id;
    string date;
    string category;
    double amount;
    Transaction* next;

    Transaction(int tid, string tdate, string tcategory, double tamount)
        : id(tid), date(tdate), category(tcategory), amount(tamount), next(nullptr) {}
};

struct Customer {
    int id;
    string name;
    double creditLimit;
    double balance;
    int rewardPoints;
    Transaction* transactions;
    Customer* next;

    Customer(int cid, string cname, double cLimit)
        : id(cid), name(cname), creditLimit(cLimit), balance(0.0), rewardPoints(0), transactions(nullptr), next(nullptr) {}
};

class CreditCardManagement {
private:
    Customer* head;
    int transactionCount;

    void addTransaction(Customer* customer, Transaction* transaction) {
        transaction->next = customer->transactions;
        customer->transactions = transaction;
    }

    void displayTransactions(Transaction* transaction) {
        while (transaction) {
            double gst = calculateGST(transaction->category, transaction->amount);
            cout << "Transaction ID: " << transaction->id
                 << ", Date: " << transaction->date
                 << ", Category: " << transaction->category
                 << ", Amount: ₹" << fixed << setprecision(2) << transaction->amount
                 << ", GST: ₹" << gst << endl;
            transaction = transaction->next;
        }
    }

    double calculateGST(string category, double amount) {
        if (category == "Electronics" || category == "Dining") {
            return amount * 0.18; // 18% GST
        }
        return 0.0;
    }

public:
    CreditCardManagement() : head(nullptr), transactionCount(0) {}

    void addCustomer(int id, string name, double creditLimit) {
        Customer* newCustomer = new Customer(id, name, creditLimit);
        newCustomer->next = head;
        head = newCustomer;
        cout << "Customer added successfully.\n";
    }

    void addTransaction(int customerId, string date, string category, double amount) {
        Customer* customer = findCustomer(customerId);
        if (!customer) {
            cout << "Customer not found.\n";
            return;
        }
        if (customer->balance + amount > customer->creditLimit) {
            cout << "Transaction denied: Credit limit exceeded.\n";
            return;
        }
        customer->balance += amount;
        customer->rewardPoints += static_cast<int>(amount / 100); // Earn 1 reward point per ₹100 spent
        Transaction* newTransaction = new Transaction(++transactionCount, date, category, amount);
        addTransaction(customer, newTransaction);
        cout << "Transaction added successfully. Reward Points Earned: " << static_cast<int>(amount / 100) << "\n";
    }

    Customer* findCustomer(int id) {
        Customer* temp = head;
        while (temp) {
            if (temp->id == id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void viewCustomer(int id) {
        Customer* customer = findCustomer(id);
        if (!customer) {
            cout << "Customer not found.\n";
            return;
        }
        cout << "Customer ID: " << customer->id << ", Name: " << customer->name
             << ", Credit Limit: ₹" << fixed << setprecision(2) << customer->creditLimit
             << ", Balance: ₹" << customer->balance
             << ", Reward Points: " << customer->rewardPoints << endl;

        if (customer->transactions) {
            cout << "Transactions:\n";
            displayTransactions(customer->transactions);
        } else {
            cout << "No transactions found.\n";
        }
    }

    void exportStatement(int id) {
        Customer* customer = findCustomer(id);
        if (!customer) {
            cout << "Customer not found.\n";
            return;
        }
        ofstream file("Monthly_Statement_" + to_string(id) + ".txt");
        if (!file) {
            cout << "Error creating file.\n";
            return;
        }
        file << "Monthly Statement for Customer ID: " << customer->id
             << ", Name: " << customer->name << "\n"
             << "--------------------------------------------\n";
        Transaction* transaction = customer->transactions;
        while (transaction) {
            double gst = calculateGST(transaction->category, transaction->amount);
            file << "Transaction ID: " << transaction->id
                 << ", Date: " << transaction->date
                 << ", Category: " << transaction->category
                 << ", Amount: ₹" << fixed << setprecision(2) << transaction->amount
                 << ", GST: ₹" << gst << "\n";
            transaction = transaction->next;
        }
        file << "--------------------------------------------\n";
        file << "Total Balance: ₹" << fixed << setprecision(2) << customer->balance << "\n";
        file << "Reward Points: " << customer->rewardPoints << "\n";
        file.close();
        cout << "Monthly statement exported successfully.\n";
    }
};

int main() {
    CreditCardManagement system;

    system.addCustomer(1, "Arjun Sharma", 200000.0);
    system.addCustomer(2, "Sneha Gupta", 150000.0);

    system.addTransaction(1, "2024-11-01", "Groceries", 5000.0);
    system.addTransaction(1, "2024-11-02", "Electronics", 60000.0);
    system.addTransaction(2, "2024-11-05", "Dining", 2500.0);

    cout << "\nCustomer Details:\n";
    system.viewCustomer(1);

    cout << "\nExporting Statement:\n";
    system.exportStatement(1);

    return 0;
}
