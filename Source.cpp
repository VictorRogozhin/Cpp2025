#include "Header.h"


void inputCoefficients(double& a, double& b, double& c) {
    cout << "Enter coefficient a: ";
    cin >> a;
    cout << "Enter coefficient b: ";
    cin >> b;
    cout << "Enter coefficient c: ";
    cin >> c;
}


double calculateDiscriminant(double a, double b, double c) {
    return b * b - 4 * a * c;
}


void calculateRealRoots(double a, double b, double discriminant, double& root1, double& root2) {
    root1 = (-b + sqrt(discriminant)) / (2 * a);
    root2 = (-b - sqrt(discriminant)) / (2 * a);
}


void printRealRoots(double root1, double root2) {
    cout << "The equation has two real roots:" << endl;
    cout << fixed << setprecision(4) << "Root 1 = " << root1 << endl;
    cout << fixed << setprecision(4) << "Root 2 = " << root2 << endl;
}


void printSingleRoot(double root) {
    cout << "The equation has one real root (repeated root):" << endl;
    cout << fixed << setprecision(4) << "Root = " << root << endl;
}


void printComplexRootsMessage() {
    cout << "The equation has complex roots (no real roots)." << endl;
}