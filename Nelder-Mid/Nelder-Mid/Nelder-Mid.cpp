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






void printSimplex(vector<point> vec)
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


/*  Получение координат новой отраженной точки
 *  Проверка, если функция в этой точке >= функции в старой точке, то значение не меняется
 *
 */

point reflection(vector<point> vec)
{
    point midPoint, refPoint;

    midPoint.init((vec[1].X1 + vec[2].X1) / 2, (vec[1].X2 + vec[2].X2) / 2, 0);

    refPoint.init( (2 * midPoint.X1 - vec[0].X1), (2 * midPoint.X2 - vec[0].X2), 0);

    return refPoint;
}

point expansion(vector<point> vec)
{
    point midPoint, expPoint;

    midPoint.init((vec[1].X1 + vec[2].X1) / 2, (vec[1].X2 + vec[2].X2) / 2, 0);

    expPoint.init((2* vec[0].X1 - midPoint.X1), (2 * vec[0].X2 - midPoint.X2), 0);

    return expPoint;
}

point contraction(vector<point> vec)
{
    point midPoint, contPoint;

    midPoint.init((vec[1].X1 + vec[2].X1) / 2, (vec[1].X2 + vec[2].X2) / 2, 0);

    contPoint.init(0.5 * (vec[0].X1 + midPoint.X1), 0.5 * (vec[0].X2 + midPoint.X2), 0);

    return contPoint;
}




/* Сортировка симплекса по убыванию значения функции
 *
 *
 */

vector<point> sortSimplexVector(vector<point> vec)
{
    vector<point> result = vec;
    vector<point>::iterator i = result.begin();
    point temp;

    if (F(result[0].X1, result[0].X2) < F(result[1].X1, result[1].X2))
    {
        temp = result[1];
        result[1] = result[0];
        result[0] = temp;
    }

    if (F(result[1].X1, result[1].X2) < F(result[2].X1, result[2].X2))
    {
        temp = result[2];
        result[2] = result[1];
        result[1] = temp;
    }

    if (F(result[0].X1, result[0].X2) < F(result[1].X1, result[1].X2))
    {
        temp = result[1];
        result[1] = result[0];
        result[0] = temp;
    }

    return result;
}

double countSimplexSpace(vector<point> vec)
{
    return abs( 0.5 * ((vec[0].X1 - vec[2].X1) * (vec[1].X2 - vec[2].X2) - (vec[1].X1 - vec[2].X1) * (vec[0].X2 - vec[2].X2)));
}

// Основной алгоритм

int main()
{


    setlocale(LC_ALL, "Russian");

    point p1, p2, p3, result, tempP;
    double curX1, curX2;
    vector<point> simplex;

    cout << "Введите начальное значение Х1: ";
    cin >> curX1;
    cout << "Введите начальное значение Х2: ";
    cin >> curX2;

    cout << "\nНачальная точка алгоритма: ( " << curX1 << " , " << curX2 << " )" << endl;
    cout << "Начальное значение длины ребра симплекса: " << l << endl;

    p1.init(curX1, curX2, 1);
    p2.countNewPoint(p1, 2);
    p3.countNewPoint(p1, 3);

    simplex.push_back(p1);
    simplex.push_back(p2);
    simplex.push_back(p3);
    


    /*
    p2.init(7, 2, 2);
    p1.init(4, 2, 1);
    p3.init(6, 8, 3);

    simplex.push_back(p2);
    simplex.push_back(p1);
    simplex.push_back(p3);

    printSimplex(simplex);

    //simplex = sortSimplexVector(simplex);

    result = contraction(simplex);

    out << result.X1 << " " << result.X2 << endl;

    result = reflection(simplex);

    simplex[0] = result;

    out << result.X1 << " " << result.X2 << endl;

    result = expansion(simplex);

    out << result.X1 << " " << result.X2 << endl;

    */
    
    printSimplex(simplex);
    
    out << countSimplexSpace(simplex) << endl;

    while (countSimplexSpace(simplex) > E)
    {
        simplex = sortSimplexVector(simplex);

        printSimplex(simplex);

        if (pointF(simplex[1]) > pointF(reflection(simplex)))
        {
            simplex[0] = reflection(simplex);

            if (pointF(simplex[2]) > pointF(expansion(simplex)))
            {
                simplex[0] = expansion(simplex);
            }
        }
        else
        {
            simplex[0] = contraction(simplex);
        }
    }

    





    out << "\nНайден локальный экстремум в точке ( " << simplex[2].X1 << " , " << simplex[2].X2 << " )" << endl;
    out << "Значение функции в этой точке: " << pointF(simplex[2]) << endl;
    out << "Площадь симплекса = " << countSimplexSpace(simplex) << endl;

    return 0;
}
