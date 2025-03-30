#pragma warning(disable:4996)
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

const int SUCCESS_RATE = 60; // вероятность 
const int TOTAL_STUDENTS = 10; // число студентов

using std::string;
using std::vector;
using std::ifstream;
using std::cout;

class QuadraticEquation {
public:
    double a, b, c;
    bool isMeaningful() const;
};

bool QuadraticEquation::isMeaningful() const {
    return !(a == 0 && b == 0 && c == 0);
}

class StudentResponse {
public:
    string name;
    vector<vector<double>> roots;
};

class PersonBase {
protected:
    QuadraticEquation getNextEquation(ifstream& input);
    vector<double> findRoots(const QuadraticEquation& eq);
};

QuadraticEquation PersonBase::getNextEquation(ifstream& input) {
    double coefficients[3];
    for (int i = 0; i < 3; i++) {
        input >> coefficients[i];
        if (input.fail()) {
            return { 0, 0, 0 };
        }
    }
    return { coefficients[0], coefficients[1], coefficients[2] };
}

vector<double> PersonBase::findRoots(const QuadraticEquation& eq) {
    vector<double> result;
    if (eq.a == 0) return result;

    double D = eq.b * eq.b - 4 * eq.a * eq.c;
    if (D > 0) {
        result.push_back((-eq.b - sqrt(D)) / (2 * eq.a));
        result.push_back((-eq.b + sqrt(D)) / (2 * eq.a));
    }
    else if (D == 0) {
        result.push_back(-eq.b / (2 * eq.a));
    }
    return result;
}

class Student : public PersonBase {
public:
    string name;
    virtual StudentResponse solveTask(const string& filename) = 0;
    virtual ~Student() = default;
};

class ExcellentStudent : public Student {
public:
    ExcellentStudent(const string& studentName) { name = studentName; }
    StudentResponse solveTask(const string& filename) override;
};

StudentResponse ExcellentStudent::solveTask(const string& filename) {
    StudentResponse response;
    response.name = name;

    ifstream input(filename);
    QuadraticEquation eq = getNextEquation(input);
    while (eq.isMeaningful()) {
        response.roots.push_back(findRoots(eq));
        eq = getNextEquation(input);
    }
    input.close();
    return response;
}

class RegularStudent : public Student {
public:
    RegularStudent(const string& studentName) { name = studentName; }
    StudentResponse solveTask(const string& filename) override;
};

StudentResponse RegularStudent::solveTask(const string& filename) {
    srand(time(nullptr));
    StudentResponse response;
    response.name = name;

    ifstream input(filename);
    QuadraticEquation eq = getNextEquation(input);
    while (eq.isMeaningful()) {
        if ((rand() % 100) < SUCCESS_RATE) {
            response.roots.push_back(findRoots(eq));
        }
        else {
            response.roots.push_back({ 0 });
        }
        eq = getNextEquation(input);
    }
    input.close();
    return response;
}

class WeakStudent : public Student {
public:
    WeakStudent(const string& studentName) { name = studentName; }
    StudentResponse solveTask(const string& filename) override;
};

StudentResponse WeakStudent::solveTask(const string& filename) {
    StudentResponse response;
    response.name = name;

    ifstream input(filename);
    QuadraticEquation eq = getNextEquation(input);
    while (eq.isMeaningful()) {
        response.roots.push_back({ 0 });
        eq = getNextEquation(input);
    }
    input.close();
    return response;
}

class GradingResult {
public:
    string studentName;
    int correctAnswers;
};

class Professor : public PersonBase {
private:
    vector<StudentResponse> submissions;
    vector<GradingResult> grades;
    vector<vector<double>> getCorrectSolutions(const string& filename);
public:
    void addSubmission(const StudentResponse& response);
    void clearSubmissions();
    void gradeSubmissions(const string& filename);
    int countEquations(const string& filename);
    void displayResults(const string& filename);
};

vector<vector<double>> Professor::getCorrectSolutions(const string& filename) {
    vector<vector<double>> solutions;
    ifstream input(filename);
    QuadraticEquation eq = getNextEquation(input);
    while (eq.isMeaningful()) {
        solutions.push_back(findRoots(eq));
        eq = getNextEquation(input);
    }
    input.close();
    return solutions;
}

void Professor::addSubmission(const StudentResponse& response) {
    submissions.push_back(response);
}

void Professor::clearSubmissions() {
    submissions.clear();
}

void Professor::gradeSubmissions(const string& filename) {
    grades.clear();
    vector<vector<double>> correctAnswers = getCorrectSolutions(filename);

    for (const auto& submission : submissions) {
        GradingResult result;
        result.studentName = submission.name;
        result.correctAnswers = 0;

        for (size_t i = 0; i < correctAnswers.size(); i++) {
            if (submission.roots[i] == correctAnswers[i]) {
                result.correctAnswers++;
            }
        }
        grades.push_back(result);
    }
}

int Professor::countEquations(const string& filename) {
    ifstream input(filename);
    int count = 0;
    QuadraticEquation eq = getNextEquation(input);
    while (eq.isMeaningful()) {
        count++;
        eq = getNextEquation(input);
    }
    input.close();
    return count;
}

void Professor::displayResults(const string& filename) {
    int totalEquations = countEquations(filename);
    int maxNameLength = 0;

    for (const auto& result : grades) {
        if (result.studentName.length() > maxNameLength) {
            maxNameLength = result.studentName.length();
        }
    }

    cout << std::setw(maxNameLength) << "Name" << " | Result\n";
    cout << string(maxNameLength + 10, '-') << "\n";

    for (const auto& result : grades) {
        cout << std::setw(maxNameLength) << result.studentName << " | "
            << result.correctAnswers << " / " << totalEquations << "\n"
            << string(maxNameLength + 10, '-') << "\n";
    }
}

vector<Student*> createStudentGroup(int count) {
    vector<Student*> group;
    srand(time(nullptr));
    for (int i = 0; i < count; i++) {
        string name = "Student" + std::to_string(i);
        switch (rand() % 3) {
        case 0: group.push_back(new ExcellentStudent(name)); break;
        case 1: group.push_back(new RegularStudent(name)); break;
        case 2: group.push_back(new WeakStudent(name)); break;
        }
    }
    return group;
}

int main() {
    const string taskFile = "eq_coeffs.txt";
    Professor teacher;
    vector<Student*> students = createStudentGroup(TOTAL_STUDENTS);

    for (auto student : students) {
        teacher.addSubmission(student->solveTask(taskFile));
    }

    teacher.gradeSubmissions(taskFile);
    teacher.displayResults(taskFile);

    for (auto student : students) {
        delete student;
    }

    return 0;
}
