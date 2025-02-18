#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Функция для ввода коэффициентов уравнения
void inputCoefficients(double& a, double& b, double& c);

// Функция для вычисления дискриминанта
double calculateDiscriminant(double a, double b, double c);

// Функция для вычисления действительных корней
void calculateRealRoots(double a, double b, double discriminant, double& root1, double& root2);

// Функция для вывода действительных корней
void printRealRoots(double root1, double root2);

// Функция для вывода кратного корня
void printSingleRoot(double root);

// Функция для вывода сообщения о комплексных корнях
void printComplexRootsMessage();
