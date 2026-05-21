#ifndef OTDELENIE_STUFF_H
#define OTDELENIE_STUFF_H

#include <string>
#include <vector>
#include "PochtaThings.h"

/**
 * @brief Класс, представляющий отделение почты
 *
 * Хранит информацию об отделении: ID, координаты, название,
 * а также список посылок, которые находятся в этом отделении.
 */
class Otdel {
public:
    /**
     * @brief Конструктор по умолчанию
     *
     * Создаёт пустое отделение с ID = -1
     */
    Otdel();
    
    /**
     * @brief Конструктор с параметрами
     * @param id Уникальный идентификатор отделения
     * @param x Координата X
     * @param y Координата Y
     * @param name Название отделения
     */
    Otdel(int id, double x, double y, std::string name);

    /// @return ID отделения
    int getId();
    
    /// @return Координата X
    double getX();
    
    /// @return Координата Y
    double getY();
    
    /// @return Название отделения
    std::string getName();

    /**
     * @brief Положить посылку в отделение
     * @param p Посылка для размещения
     */
    void polozhit(Posylka p);
    
    /**
     * @brief Забрать (удалить) посылку из отделения
     * @param tr Трек-номер посылки
     * @return true если посылка найдена и удалена, false если не найдена
     */
    bool zabrat(std::string tr);
    
    /**
     * @brief Найти посылку по трек-номеру
     * @param tr Трек-номер
     * @return Указатель на посылку или nullptr если не найдена
     */
    Posylka* nayti(std::string tr);
    
    /**
     * @brief Получить все посылки в отделении
     * @return Вектор всех посылок
     */
    std::vector<Posylka> vse();
    
    /**
     * @brief Очистить все посылки из отделения
     */
    void ochistit();

private:
    int nomer;                      ///< Уникальный ID отделения
    double x1;                      ///< Координата X
    double y1;                      ///< Координата Y
    std::string nazvanie;           ///< Название отделения
    std::vector<Posylka> spisok;    ///< Список посылок в отделении
};

#endif
