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
#include <ctime> 


using namespace std;

// Глобальные переменные

int n = 2;                  // Количество переменных (Х1 и Х2)
double l = 2;               // Длина ребра симплекса
double E = 0.01;
double curMaxF = -99999999;
int curIndex = 0;
int oldIndex = -1;

ofstream out("out.txt");


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

    void init(double x1, double x2);
};

void point::init(double x1, double x2)
{
    X1 = x1;
    X2 = x2;
}



// Заданная функция

double F(double x1, double x2)
{
    return (33 * x1 * x1) + (53 * x1 * x2) + (33 * x2 * x2) - (6 * x1) + (17 * x2) + 7;
}

double pointF(point p)
{
    return F(p.X1, p.X2);
}

// Дополнительные функции

vector<double> simpleVectorCreation(int n)
{
    vector<double> result;

    for (int i = 0; i < n; i++)
    {
        result.push_back((rand() % 200)/100.0 - 1);
    }

    return result;
}





// Основной алгоритм

int main()
{
    srand(time(NULL));

    setlocale(LC_ALL, "Russian");

    point p, newP;
    double curX1, curX2;
    vector<point> simplex;
    vector<double> randVecX1, randVecX2;
    int i = 0;
    double range = 2;
    int n = 50;


    

    cout << "Введите начальное значение Х1: ";
    cin >> curX1;
    cout << "Введите начальное значение Х2: ";
    cin >> curX2;

    cout << "\nНачальная точка алгоритма: ( " << curX1 << " , " << curX2 << " )" << endl;
    cout << "Начальное значение длины ребра симплекса: " << l << endl;

    p.init(curX1, curX2);

    out << "Начальная точка: ( " << p.X1 << " , " << p.X2 << " )" << endl;


    randVecX1 = simpleVectorCreation(n);
    randVecX2 = simpleVectorCreation(n);


    while (range > E)
    {
        newP.init(p.X1 + range * randVecX1[i], p.X2 + range * randVecX2[i]);
        out << "Новая точка: ( " << newP.X1 << " , " << newP.X2 << " )" << endl;

        if (pointF(newP) < pointF(p))
        {
            out << "Точка удачная. Смена точки.\n" << endl;

            p = newP;

            randVecX1 = simpleVectorCreation(n);
            randVecX2 = simpleVectorCreation(n);
        }
        else
        {
            if (i < n-1)
            {
                i++;
            }
            else
            {
                range /= 2;
                i = 0;
                out << "Радиус уменьшен вдвое\n" << endl;
            }
        }
    }

    





    





    out << "\nНайден локальный экстремум в точке ( " << p.X1 << " , " << p.X2 << " )" << endl;
    out << "Значение функции в этой точке: " << pointF(p) << endl;
    








    return 0;
}
