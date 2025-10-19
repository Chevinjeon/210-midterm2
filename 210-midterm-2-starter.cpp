#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList {
private:
    struct Node {
        string data;
        Node* prev;
        Node* next;
        Node(string val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(string value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(string value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(string v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(string v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }

    // Helper methods for coffee shop simulation
    bool isEmpty() {
        return head == nullptr;
    }

    int size() {
        int count = 0;
        Node* current = head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    string getFront() {
        if (head)
            return head->data;
        return "";
    }

    string getBack() {
        if (tail)
            return tail->data;
        return "";
    }

    string getRandomElement() {
        if (!head) return "";
        int sz = size();
        int randPos = rand() % sz;
        Node* current = head;
        for (int i = 0; i < randPos; i++) {
            current = current->next;
        }
        return current->data;
    }

    void printLine() {
        Node* current = head;
        while (current) {
            cout << "        " << current->data << endl;
            current = current->next;
        }
    }
};

int main() {
    srand(time(0));  // Seed random number generator
    
    // Load names from file
    vector<string> names;
    ifstream infile("names.txt");
    string name;
    while (getline(infile, name)) {
        names.push_back(name);
    }
    infile.close();

    if (names.empty()) {
        cout << "Error: Could not load names from file." << endl;
        return 1;
    }

    DoublyLinkedList coffeeLine;
    vector<string> usedNames;  // Track names already in use

    // Lambda to get a random unused name
    auto getRandomName = [&]() -> string {
        if (usedNames.size() >= names.size()) {
            // All names used, start reusing
            usedNames.clear();
        }
        string selectedName;
        do {
            selectedName = names[rand() % names.size()];
        } while (find(usedNames.begin(), usedNames.end(), selectedName) != usedNames.end());
        usedNames.push_back(selectedName);
        return selectedName;
    };

    // Store opens - add 5 customers
    cout << "Store opens:" << endl;
    for (int i = 0; i < 5; i++) {
        string customer = getRandomName();
        coffeeLine.push_back(customer);
        cout << "    " << customer << " joins the line" << endl;
    }
    
    cout << "    Resulting line:" << endl;
    if (!coffeeLine.isEmpty()) {
        coffeeLine.printLine();
    } else {
        cout << "        (empty)" << endl;
    }
    cout << endl;

    // Simulate 19 more time periods (total 20 including opening)
    for (int timeStep = 2; timeStep <= 20; timeStep++) {
        cout << "Time step #" << timeStep << ":" << endl;
        
        // 40% probability: Customer at front is served
        int prob = rand() % 100 + 1;
        if (prob <= 40 && !coffeeLine.isEmpty()) {
            string customer = coffeeLine.getFront();
            coffeeLine.pop_front();
            cout << "    " << customer << " is served" << endl;
            // Remove from used names
            usedNames.erase(remove(usedNames.begin(), usedNames.end(), customer), usedNames.end());
        }
        
        // 10% probability: Random customer in line leaves
        prob = rand() % 100 + 1;
        if (prob <= 10 && !coffeeLine.isEmpty() && coffeeLine.size() > 1) {
            string customer = coffeeLine.getRandomElement();
            if (customer != coffeeLine.getFront() && customer != coffeeLine.getBack()) {
                coffeeLine.delete_val(customer);
                cout << "    " << customer << " left the line" << endl;
                usedNames.erase(remove(usedNames.begin(), usedNames.end(), customer), usedNames.end());
            }
        }
        
        // 10% probability: VIP joins front of line
        prob = rand() % 100 + 1;
        if (prob <= 10) {
            string customer = getRandomName();
            coffeeLine.push_front(customer);
            cout << "    " << customer << " (VIP) joins the front of the line" << endl;
        }
        
        // 60% probability: New customer joins the end
        prob = rand() % 100 + 1;
        if (prob <= 60) {
            string customer = getRandomName();
            coffeeLine.push_back(customer);
            cout << "    " << customer << " joins the line" << endl;
        }
        
        // 20% probability: Customer at end leaves (frustrated)
        prob = rand() % 100 + 1;
        if (prob <= 20 && !coffeeLine.isEmpty()) {
            string customer = coffeeLine.getBack();
            coffeeLine.pop_back();
            cout << "    " << customer << " (at the rear) left the line" << endl;
            usedNames.erase(remove(usedNames.begin(), usedNames.end(), customer), usedNames.end());
        }
        
        // Display resulting line
        cout << "    Resulting line:" << endl;
        if (coffeeLine.isEmpty()) {
            cout << "        (empty)" << endl;
        } else {
            coffeeLine.printLine();
        }
        cout << endl;
    }
    
    return 0;
}
