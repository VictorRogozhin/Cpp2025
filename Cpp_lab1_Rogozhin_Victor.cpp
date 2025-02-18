#include "Header.h"

int main() {
    double a, b, c;
    double discriminant, root1, root2;

    inputCoefficients(a, b, c);

    discriminant = calculateDiscriminant(a, b, c);

    if (discriminant > 0) {
        calculateRealRoots(a, b, discriminant, root1, root2);
        printRealRoots(root1, root2);
    }
    else if (discriminant == 0) {
        root1 = -b / (2 * a);
        printSingleRoot(root1);
    }
    else {
        printComplexRootsMessage();
    }

    return 0;
}