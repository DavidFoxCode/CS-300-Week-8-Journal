#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

struct Node {
    Course course;
    Node* left;
    Node* right;
};

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n\"");
    if (start == string::npos) {
        return "";
    }
    return text.substr(start, text.find_last_not_of(" \t\r\n\"") - start + 1);
}

string upper(string text) {
    for (char& c : text) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return text;
}

vector<string> split(const string& line) {
    vector<string> fields;
    stringstream stream(line);
    string field;
    while (getline(stream, field, ',')) {
        fields.push_back(trim(field));
    }
    return fields;
}

class CourseTree {
    Node* root = nullptr;
    int count = 0;

    static void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    static void printInOrder(Node* node) {
        if (node == nullptr) return;
        printInOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        printInOrder(node->right);
    }

    bool validate(Node* node) const {
        if (node == nullptr) return true;
        for (const string& prereq : node->course.prerequisites) {
            if (find(prereq) == nullptr) {
                cout << "Error: " << node->course.courseNumber << " lists prerequisite "
                     << prereq << ", which is not in the file." << endl;
                return false;
            }
        }
        return validate(node->left) && validate(node->right);
    }

public:
    ~CourseTree() { clear(); }

    void clear() {
        destroy(root);
        root = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool validate() const { return validate(root); }
    void printAll() const { printInOrder(root); }

    void insert(const Course& course) {
        Node* node = new Node{course, nullptr, nullptr};
        count++;
        if (root == nullptr) {
            root = node;
            return;
        }
        Node* current = root;
        while (true) {
            Node*& next = (course.courseNumber < current->course.courseNumber) ? current->left : current->right;
            if (next == nullptr) {
                next = node;
                return;
            }
            current = next;
        }
    }

    const Course* find(const string& courseNumber) const {
        Node* current = root;
        while (current != nullptr && current->course.courseNumber != courseNumber) {
            current = (courseNumber < current->course.courseNumber) ? current->left : current->right;
        }
        return current ? &current->course : nullptr;
    }
};

bool loadCourses(const string& fileName, CourseTree& tree) {
    ifstream file(fileName);
    if (!file) {
        cout << "Error: could not open \"" << fileName << "\"." << endl;
        return false;
    }

    tree.clear();
    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        if (lineNumber == 1 && line.rfind("\xEF\xBB\xBF", 0) == 0) {
            line.erase(0, 3);
        }
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = split(line);
        string error;
        if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
            error = "every course needs a course number and a title";
        } else if (tree.find(upper(fields[0])) != nullptr) {
            error = upper(fields[0]) + " is listed more than once";
        }
        if (!error.empty()) {
            cout << "Error on line " << lineNumber << ": " << error << "." << endl;
            tree.clear();
            return false;
        }

        Course course{upper(fields[0]), fields[1], {}};
        for (size_t i = 2; i < fields.size(); i++) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(upper(fields[i]));
            }
        }
        tree.insert(course);
    }

    if (tree.size() == 0) {
        cout << "Error: \"" << fileName << "\" doesn't have any courses in it." << endl;
        return false;
    }
    if (!tree.validate()) {
        tree.clear();
        return false;
    }

    cout << tree.size() << " courses loaded." << endl;
    return true;
}

void printCourse(const CourseTree& tree, const string& courseNumber) {
    const Course* course = tree.find(courseNumber);
    if (course == nullptr) {
        cout << courseNumber << " was not found in the course catalog." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;
    cout << "Prerequisites:" << (course->prerequisites.empty() ? " None" : "") << endl;
    for (const string& prereq : course->prerequisites) {
        const Course* prereqCourse = tree.find(prereq);
        cout << "  " << prereq << ", " << (prereqCourse ? prereqCourse->courseTitle : "unknown") << endl;
    }
}

string readLine() {
    string input;
    getline(cin, input);
    return trim(input);
}

int main() {
    CourseTree tree;
    string choice;
    cout << "Welcome to the course planner." << endl;

    while (choice != "9") {
        cout << "\n  1. Load Course Data\n  2. Display All Courses\n  3. Find a Course\n  9. Exit\n\n"
             << "What would you like to do? ";
        choice = readLine();
        if (!cin) break;

        if (choice == "1") {
            cout << "Enter the name of the course data file: ";
            string fileName = readLine();
            if (fileName.empty()) {
                cout << "Please enter a file name." << endl;
            } else {
                loadCourses(fileName, tree);
            }
        } else if ((choice == "2" || choice == "3") && tree.size() == 0) {
            cout << "Please load the course data first (option 1)." << endl;
        } else if (choice == "2") {
            cout << "Here is the course list:" << endl << endl;
            tree.printAll();
        } else if (choice == "3") {
            cout << "What course do you want to know about? ";
            string courseNumber = upper(readLine());
            if (courseNumber.empty()) {
                cout << "Please enter a course number." << endl;
            } else {
                printCourse(tree, courseNumber);
            }
        } else if (choice != "9") {
            if (!choice.empty()) {
                cout << "\"" << choice << "\" is not a valid option. ";
            }
            cout << "Please enter 1, 2, 3, or 9." << endl;
        }
    }

    cout << "Thank you for using the course planner!" << endl;
    return 0;
}
