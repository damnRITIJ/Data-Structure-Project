#include <iostream>
#include <string>
using namespace std;

struct Booking {
    string name;
    int row, seat;
    Booking* next;
    Booking(string n, int r, int s) : name(n), row(r), seat(s), next(NULL) {}
};

struct QNode {
    string name;
    QNode* next;
    QNode(string n) : name(n), next(NULL) {}
};

class Queue {
    QNode *front, *rear;
public:
    Queue() : front(NULL), rear(NULL) {}
    void enqueue(string name) {
        QNode* temp = new QNode(name);
        if (!rear) front = rear = temp;
        else {
            rear->next = temp;
            rear = temp;
        }
    }
    string dequeue() {
        if (!front) return "";
        QNode* temp = front;
        string name = temp->name;
        front = front->next;
        if (!front) rear = NULL;
        delete temp;
        return name;
    }
    bool isEmpty() { return front == NULL; }
    void display() {
        if (!front) { cout << "Waiting list empty.\n"; return; }
        cout << "Waiting List: ";
        for (QNode* temp = front; temp; temp = temp->next)
            cout << temp->name << " -> ";
        cout << "NULL\n";
    }
};

struct SNode {
    string name;
    int row, seat;
    SNode* next;
    SNode(string n, int r, int s) : name(n), row(r), seat(s), next(NULL) {}
};

class Stack {
    SNode* top;
public:
    Stack() : top(NULL) {}
    void push(string name, int row, int seat) {
        SNode* temp = new SNode(name, row, seat);
        temp->next = top;
        top = temp;
    }
    void pop() {
        if (!top) return;
        SNode* temp = top;
        top = top->next;
        delete temp;
    }
    bool isEmpty() { return top == NULL; }
    void displayTop() {
        if (top)
            cout << "Last cancelled booking: " << top->name << " (Seat: " << top->row << "," << top->seat << ")\n";
        else
            cout << "No recent cancellations.\n";
    }
};

struct TreeNode {
    string name;
    int row, seat;
    TreeNode *left, *right;
    TreeNode(string n, int r, int s) : name(n), row(r), seat(s), left(NULL), right(NULL) {}
};

class BST {
public:
    TreeNode* root = NULL;
    TreeNode* insert(TreeNode* root, string name, int row, int seat) {
        if (!root) return new TreeNode(name, row, seat);
        if (name < root->name)
            root->left = insert(root->left, name, row, seat);
        else
            root->right = insert(root->right, name, row, seat);
        return root;
    }

    void inorder(TreeNode* root) {
        if (!root) return;
        inorder(root->left);
        cout << root->name << " (" << root->row << "," << root->seat << ")\n";
        inorder(root->right);
    }

    bool search(TreeNode* root, string name) {
        if (!root) return false;
        if (root->name == name) {
            cout << "Found " << name << " at seat (" << root->row << "," << root->seat << ")\n";
            return true;
        }
        if (name < root->name) return search(root->left, name);
        else return search(root->right, name);
    }
};

class CinemaSystem {
    const static int rows = 3;
    const static int cols = 5;
    bool seats[rows][cols];
    Booking* head;
    Queue waitingList;
    Stack cancelStack;
    BST tree;
    int totalSeats;

public:
    CinemaSystem() {
        head = NULL;
        totalSeats = rows * cols;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                seats[i][j] = false;
    }

    void displaySeats() {
        cout << "\nSeat Layout (X = booked, O = free):\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++)
                cout << (seats[i][j] ? "X " : "O ");
            cout << endl;
        }
    }

    void bookSeat(string name) {
        bool booked = false;
        for (int i = 0; i < rows && !booked; i++) {
            for (int j = 0; j < cols && !booked; j++) {
                if (!seats[i][j]) {
                    seats[i][j] = true;
                    Booking* newB = new Booking(name, i, j);
                    newB->next = head;
                    head = newB;
                    tree.root = tree.insert(tree.root, name, i, j);
                    cout << "Seat booked for " << name << " at (" << i << "," << j << ")\n";
                    booked = true;
                }
            }
        }
        if (!booked) {
            cout << "All seats full. Added to waiting list.\n";
            waitingList.enqueue(name);
        }
    }

    void cancelBooking(string name) {
        Booking* curr = head, *prev = NULL;
        while (curr && curr->name != name) {
            prev = curr;
            curr = curr->next;
        }
        if (!curr) {
            cout << "No booking found for " << name << "\n";
            return;
        }

        seats[curr->row][curr->seat] = false;
        cancelStack.push(curr->name, curr->row, curr->seat);

        if (prev) prev->next = curr->next;
        else head = curr->next;
        delete curr;

        cout << "Booking cancelled for " << name << "\n";

        if (!waitingList.isEmpty()) {
            string nextCustomer = waitingList.dequeue();
            cout << "Booking seat for waiting customer: " << nextCustomer << "\n";
            bookSeat(nextCustomer);
        }
    }

    void searchBooking(string name) {
        if (!tree.search(tree.root, name))
            cout << "No booking found for " << name << endl;
    }

    void showRecentCancellation() {
        cancelStack.displayTop();
    }

    void showAllBookings() {
        cout << "\nAll Bookings (Alphabetical):\n";
        tree.inorder(tree.root);
    }
};

int main() {
    CinemaSystem cs;
    int choice;
    string name;

    while (true) {
        cout << "\n===== CINEMA BOOKING SYSTEM =====\n";
        cout << "1. Display Seats\n2. Book Seat\n3. Cancel Booking\n4. Search Booking\n5. Show All Bookings\n6. Show Recent Cancellation\n7. Show Waiting List\n8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cs.displaySeats();
                break;
            case 2:
                cout << "Enter name: ";
                cin >> name;
                cs.bookSeat(name);
                break;
            case 3:
                cout << "Enter name to cancel: ";
                cin >> name;
                cs.cancelBooking(name);
                break;
            case 4:
                cout << "Enter name to search: ";
                cin >> name;
                cs.searchBooking(name);
                break;
            case 5:
                cs.showAllBookings();
                break;
            case 6:
                cs.showRecentCancellation();
                break;
            case 7:
                cs.displaySeats();
                break;
            case 8:
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
}
