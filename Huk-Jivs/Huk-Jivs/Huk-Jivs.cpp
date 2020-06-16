/* Описание */

/*
 *          Методы оптимизации
 *          Реализация метода Хука-Дживса
 *          Значения начальной точки вводятся из консоли, вывод осуществляется в файл out.txt
 */

/* Библиотеки */

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Глобальные переменные

vector<double> stepVec = {1, 1};    // Шаги по каждой переменной
double curX1 = 1;                   // Начальное значение Х1
double curX2 = 1;                   // Начальное значение Х2
double E = 0.1;                     // Точность 
double curMinF;                     // Текущий минимум функции F
double gamma = 2;                   // Коэффициент дробления шага

int iterCounter = 1;

// Заданная функция

double F(double x1, double x2)
{
    return (33 * x1 * x1) + (53 * x1 * x2) + (33 * x2 * x2) - (6 * x1) + (17 * x2) + 7;
}


// Основной алгоритм

int main()
{
    ofstream out("out.txt");

    setlocale(LC_ALL, "Russian");

    cout << "Введите начальное значение Х1: ";
    cin >> curX1;
    cout << "Введите начальное значение Х2: ";
    cin >> curX2;

    cout << "\nНачальная точка алгоритма: ( " << curX1 << " , " << curX2 << " )" << endl;
    cout << "Начальное значение шага по каждой переменной: " << stepVec[0] << endl;
    cout << "Коэффициент дробления шага: " << gamma << endl << endl;


    curMinF = F(curX1, curX2);

    while ( (stepVec[0] >= E) && (stepVec[1] >= E) )
    {
        out << "Итерация " << iterCounter << endl;

        out << "\tТекущее значение минимума: " << curMinF << endl;




        out << "\t" << endl;

        if (F(curX1 + stepVec[0], curX2) < curMinF)
        {
            out << "\tF(X1 + step, X2) = " << F(curX1 + stepVec[0], curX2) << endl;

            curMinF = F(curX1 + stepVec[0], curX2);
            curX1 = curX1 + stepVec[0];

            out << "\tНовая точка: ( " << curX1 << " , " << curX2 << " )" << endl;
        }
        else
        {
            if (F(curX1 - stepVec[0], curX2) < curMinF)
            {
                out << "\tF(X1 - step, X2) = " << F(curX1 - stepVec[0], curX2) << endl;

                curMinF = F(curX1 - stepVec[0], curX2);
                curX1 = curX1 - stepVec[0];

                out << "\tНовая точка: ( " << curX1 << " , " << curX2 << " )" << endl;
            }
            else
            {
                stepVec[0] /= gamma;

                out << "\tПо Х1 не найдено новое значение. Новое значение шага: " << stepVec[0] << endl;
            }
        }
            

        if (F(curX1, curX2 + stepVec[1]) < curMinF)
        {
            out << "\tF(X1, X2 + step) = " << F(curX1, curX2 + stepVec[1]) << endl;

            curMinF = F(curX1, curX2 + stepVec[1]);
            curX2 = curX2 + stepVec[1];

            out << "\tНовая точка: ( " << curX1 << " , " << curX2 << " )" << endl;
        }
        else
        {
            if (F(curX1, curX2 - stepVec[1]) < curMinF)
            {
                out << "\tF(X1, X2 - step) = " << F(curX1, curX2 - stepVec[1]) << endl;

                curMinF = F(curX1, curX2 - stepVec[1]);
                curX2 = curX2 - stepVec[1];

                out << "\tНовая точка: ( " << curX1 << " , " << curX2 << " )" << endl;
            }
            else
            {
                stepVec[1] /= gamma;

                out << "\tПо Х2 не найдено новое значение. Новое значение шага: " << stepVec[1] << endl;
            }
        }
            

        iterCounter++;
        out << endl;
    }

    out << "\nНайден локальный экстремум в точке ( " << curX1 << " , " << curX2 << " )" << endl;
    out << "Значение функции в этой точке: " << F(curX1, curX2);

    return 0;
}
