/* Описание */

/*
 *          Методы оптимизации
 *          Реализация метода Простого симплекса
 *          Значения начальной точки вводятся из консоли, вывод осуществляется в файл out.txt
 */

 /* Библиотеки */

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <bitset>
#include <windows.h>

using namespace std;

// Глобальные переменные

int n = 2;                  // Количество переменных (Х1 и Х2)
double l = 2;               // Длина ребра симплекса
double curMaxF = -99999999;
int curIndex = 0;
int oldIndex = -1;


// Заданная функция

double F(double x1, double x2)
{
    return (33 * x1 * x1) + (53 * x1 * x2) + (33 * x2 * x2) - (6 * x1) + (17 * x2) + 7;
}

// Дополнительные функции


/*  Вычисление дополнительных точек симплекса
 *  double l        - Длина ребра симплекса
 *  int i           - Номер вершины симплекса (1, 2 или 3)
 *  int j           - Номер переменной (Х1 или Х2)
 *  double startX   - Первая точка симлекса
 */
double getNewX(int i, int j, double startX)
{
    if (i == j + 1)
    {
        return (startX + l * (sqrt(n + 1) - 1) / (n * sqrt(2)));
    }
    else
    {
        return (startX + l * (sqrt(n + 1) + n - 1) / (n * sqrt(2)));
    }
}



// Точка

struct point
{
    double X1;  // Значение первой координаты
    double X2;  // Значение второй координаты 
    int i;      // Номер точки

    void init(double x1, double x2, int I);
    void countNewPoint(point statrtPoint, int I);
};

void point::init(double x1, double x2, int I)
{
    X1 = x1;    
    X2 = x2;    
    i = I;
}

void point::countNewPoint(point statrtPoint, int I)
{
    X1 = getNewX(I, 1, statrtPoint.X1);
    X2 = getNewX(I, 2, statrtPoint.X2);
    i = I;
}


void printSimplex(vector<point> vec, ofstream& out)
{
    for (int i = 0; i < 3; i++)
    {
        out << "p" << vec[i].i << " = ( " << vec[i].X1 << " , " << vec[i].X2 << " )" << endl;
    }
    out << endl;
    for (int i = 0; i < 3; i++)
    {
        out << "F(p" << vec[i].i << ") = " << F(vec[i].X1, vec[i].X2) << endl;
    }
    out << endl;
}


vector<point> getMirrorSimplex(int curMaxIndex, vector<point> vec)
{
    double sum1 = 0, sum2 = 0;
    double newX1, newX2;
    vector<point> newVec;
    point newP;

    for (int i = 0; i < 3; i++)
    {
        if (i != curMaxIndex)
        {
            sum1 += vec[i].X1;
            sum2 += vec[i].X2;
        }
    }

    newX1 = sum1 - vec[curMaxIndex].X1;
    newX2 = sum2 - vec[curMaxIndex].X2;
    newP.init(newX1, newX2, curMaxIndex + 1);

    for (int i = 0; i < 3; i++)
    {
        if (i != curMaxIndex)
        {
            newVec.push_back(vec[i]);
        }
        else newVec.push_back(newP);
    }

    return newVec;
}

void updateMaxPoint(vector<point> vec)
{
    oldIndex = curIndex;
    curMaxF = -99999999;
    for (int i = 0; i < 3; i++)
    {
        if (F(vec[i].X1, vec[i].X2) > curMaxF)
        {
            curMaxF = F(vec[i].X1, vec[i].X2);
            curIndex = i;
        }
    }
}

void sortSimplexVector(vector<point> vec)
{
    if(F(vec[0].X1, vec[0].X2) > F(vec[1].X1, vec[1].X2)) 
}

// Основной алгоритм

int main()
{
    ofstream out("out.txt");

    setlocale(LC_ALL, "Russian");

    point p1, p2, p3;
    double curX1, curX2;
    vector<point> simplex;
    

    cout << "Введите начальное значение Х1: ";
    cin >> curX1;
    cout << "Введите начальное значение Х2: ";
    cin >> curX2;

    cout << "\nНачальная точка алгоритма: ( " << curX1 << " , " << curX2 << " )" << endl;
    cout << "Начальное значение длины ребра симплекса: " << l << endl;

    



    /*
    printSimplex(simplex, out);

    updateMaxPoint(simplex);

    out << "Current Maximum F = " << curMaxF << endl;
    out << "Current Index = " << curIndex + 1 << endl;

    simplex = getMirrorSimplex(curIndex, simplex);

    printSimplex(simplex, out);

    updateMaxPoint(simplex);

    out << "Current Maximum F = " << curMaxF << endl;
    out << "Current Index = " << curIndex + 1 << endl;

    */


    p1.init(curX1, curX2, 1);
    p2.countNewPoint(p1, 2);
    p3.countNewPoint(p1, 3);

    simplex.push_back(p1);
    simplex.push_back(p2);
    simplex.push_back(p3);

    while (oldIndex != curIndex)
    {
        printSimplex(simplex, out);

        updateMaxPoint(simplex);

        out << "Current Maximum F = " << curMaxF << endl;
        out << "Current Index = " << curIndex + 1 << endl << endl;

        simplex = getMirrorSimplex(curIndex, simplex);
    }



    /*
    out << "\nНайден локальный экстремум в точке ( " << curX1 << " , " << curX2 << " )" << endl;
    out << "Значение функции в этой точке: " << F(curX1, curX2);
    */


    return 0;
}
