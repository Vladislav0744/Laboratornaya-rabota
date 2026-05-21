#ifndef VREMYA_MACHINERY_H
#define VREMYA_MACHINERY_H

/**
 * @brief Пространство имён для функций расчёта времени и расстояний
 */
namespace VremyaMachinery {
    /**
     * @brief Рассчитать количество дней в пути
     * @param km Расстояние в километрах
     * @param kmDen Скорость в километрах в день
     * @return Количество дней (округляется вверх)
     * @details Пример: 500 км при скорости 450 км/день = 2 дня
     */
    int skolkoDney(double km, double kmDen);
    
    /**
     * @brief Рассчитать расстояние между двумя точками
     * @param x1 Координата X первой точки
     * @param y1 Координата Y первой точки
     * @param x2 Координата X второй точки
     * @param y2 Координата Y второй точки
     * @return Расстояние по формуле Евклида
     */
    double rasst(double x1, double y1, double x2, double y2);
}

#endif
