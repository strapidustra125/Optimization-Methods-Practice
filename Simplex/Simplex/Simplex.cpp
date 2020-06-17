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
double E = 0.1;
double curMaxF = -99999999;
int curIndex = 0;
int oldIndex = -1;

ofstream out("out.txt");


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
    newP.init(newX1, newX2, vec[curMaxIndex].i);

    if (F(newP.X1, newP.X2) >= F(vec[curMaxIndex].X1, vec[curMaxIndex].X2))
    {
        newP = vec[curMaxIndex];
    }

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


/* Пройти из текущей точки до минимальной с заданной длиной ребра симплекса
 * 
 * point start - начальная точка
 * 
 * Возвращает точку с минимальным значением функции из текущих
 */

point findCurMinSimplex(point start)
{
    point p1, p2, p3;
    vector<point> simplexNew, simplexCur;
    bool stopFlag = false;
    int vecPointer = 0;

    p1.init(start.X1, start.X2, 1);
    p2.countNewPoint(p1, 2);
    p3.countNewPoint(p1, 3);

    simplexCur.push_back(p1);
    simplexCur.push_back(p2);
    simplexCur.push_back(p3);

    //printSimplex(simplexCur);

    out << "Начало While... \n\n";

    while (!stopFlag)
    {
        out << "Текущий симплекс... \n\n";
        printSimplex(simplexCur);

        out << "Сортировка... \n\n";
        simplexCur = sortSimplexVector(simplexCur);

        printSimplex(simplexCur);

        out << "Отражение... \n\n";
        simplexNew = getMirrorSimplex(vecPointer, simplexCur);

        out << "Новый симплекс... \n\n";
        printSimplex(simplexNew);

        if (F(simplexNew[vecPointer].X1, simplexNew[vecPointer].X2) == F(simplexCur[vecPointer].X1, simplexCur[vecPointer].X2))
        {


            if (vecPointer != 2)
            {
                out << "Следующая точка... \n\n";
                vecPointer++;
            }
            else stopFlag = true;
        }
        else simplexCur = simplexNew;
    }

    return simplexCur[2];
}

// Основной алгоритм

int main()
{
    

    setlocale(LC_ALL, "Russian");

    point p1, p2, p3, newP;
    double curX1, curX2;
    vector<point> simplex;
    

    cout << "Введите начальное значение Х1: ";
    cin >> curX1;
    cout << "Введите начальное значение Х2: ";
    cin >> curX2;

    cout << "\nНачальная точка алгоритма: ( " << curX1 << " , " << curX2 << " )" << endl;
    cout << "Начальное значение длины ребра симплекса: " << l << endl;


    p1.init(curX1, curX2, 1);

    while (l > E)
    {
        newP = findCurMinSimplex(p1);

        l /= 2;

        p1.init(newP.X1, newP.X2, 1);
    }
    
    out << "\nНайден локальный экстремум в точке ( " << p1.X1 << " , " << p1.X2 << " )" << endl;
    out << "Значение функции в этой точке: " << F(p1.X1, p1.X2) << endl;
    out << "Длина ребра симплекса = " << l << endl;
    out << "Точность = " << E << endl;

    return 0;
}
