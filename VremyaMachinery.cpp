/**
 * @file VremyaMachinery.cpp
 * @brief Реализация математических функций для расчёта времени и расстояний
 */

#include "VremyaMachinery.h"
#include <cmath>

/**
 * @brief Рассчитать количество дней в пути
 * @param km Расстояние в километрах
 * @param kmDen Скорость в километрах в день
 * @return Количество дней (округляется вверх)
 *
 * @details Реализует формулу: ceil(расстояние / скорость)
 *          При скорости <= 0 возвращает 999999 (бесконечность)
 *
 * @par Пример:
 * @code
 * skolkoDney(500, 450);  // возвращает 2
 * skolkoDney(450, 450);  // возвращает 1
 * skolkoDney(100, 450);  // возвращает 1
 * @endcode
 */
int VremyaMachinery::skolkoDney(double km, double kmDen) {
    if (kmDen <= 0.001) return 999999;
    int res = (int)(km / kmDen);
    if (km / kmDen > res) res = res + 1;
    return res;
}

/**
 * @brief Рассчитать расстояние между двумя точками
 * @param x1 Координата X первой точки
 * @param y1 Координата Y первой точки
 * @param x2 Координата X второй точки
 * @param y2 Координата Y второй точки
 * @return Расстояние по формуле Евклида
 *
 * @details Формула: √((x2-x1)² + (y2-y1)²)
 *
 * @par Пример:
 * @code
 * rasst(0, 0, 3, 4);  // возвращает 5
 * @endcode
 */
double VremyaMachinery::rasst(double x1, double y1, double x2, double y2) {
    double a = x1 - x2;
    double b = y1 - y2;
    return sqrt(a*a + b*b);
}
