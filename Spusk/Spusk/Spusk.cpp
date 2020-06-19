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
double step = 0.01;

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

point findMinX1(int directionFlag, point p)
{
    point curP, nextP;

    curP = p;
    nextP.init(curP.X1 + step * directionFlag, curP.X2);

    while (pointF(curP) > pointF(nextP))
    {
        curP = nextP;
        nextP.init(curP.X1 + step * directionFlag, curP.X2);
    }

    return curP;
}

point findMinX2(int directionFlag, point p)
{
    point curP, nextP;

    curP = p;
    nextP.init(curP.X1, curP.X2 + step * directionFlag);

    while (pointF(curP) > pointF(nextP))
    {
        curP = nextP;
        nextP.init(curP.X1, curP.X2 + step * directionFlag);
    }

    return curP;
}





// Основной алгоритм

int main()
{
    srand(time(NULL));

    setlocale(LC_ALL, "Russian");

    point p;
    double curX1, curX2;




    cout << "Введите начальное значение Х1: ";
    cin >> curX1;
    cout << "Введите начальное значение Х2: ";
    cin >> curX2;

    cout << "\nНачальная точка алгоритма: ( " << curX1 << " , " << curX2 << " )" << endl;
    cout << "Начальное значение длины ребра симплекса: " << l << endl;

    p.init(curX1, curX2);

    out << "Начальная точка: ( " << p.X1 << " , " << p.X2 << " )" << endl;





    while ( (pointF(p) > F(p.X1 + step, p.X2)) || (pointF(p) > F(p.X1 - step, p.X2)) ||
            (pointF(p) > F(p.X1, p.X2 + step)) || (pointF(p) > F(p.X1, p.X2 - step)) )
    {
        if (pointF(p) > F(p.X1 + step, p.X2))
        {
            p = findMinX1(1, p);

            out << "Новая точка по Х1: ( " << p.X1 << " , " << p.X2 << " )" << endl;
        }
        else
        {
            if (pointF(p) > F(p.X1 - step, p.X2))
            {
                p = findMinX1(-1, p);

                out << "Новая точка по Х1: ( " << p.X1 << " , " << p.X2 << " )" << endl;
            }
        }



        if (pointF(p) > F(p.X1, p.X2 + step))
        {
            p = findMinX2(1, p);

            out << "Новая точка по Х2: ( " << p.X1 << " , " << p.X2 << " )" << endl;
        }
        else
        {
            if (pointF(p) > F(p.X1, p.X2 - step))
            {
                p = findMinX2(-1, p);

                out << "Новая точка по Х2: ( " << p.X1 << " , " << p.X2 << " )" << endl;
            }
        }
    }












    out << "\nНайден локальный экстремум в точке ( " << p.X1 << " , " << p.X2 << " )" << endl;
    out << "Значение функции в этой точке: " << pointF(p) << endl;









    return 0;
}
