/**
 * @file OtdelenieStuff.cpp
 * @brief Реализация класса Otdel (отделение почты)
 */

#include "OtdelenieStuff.h"

/**
 * @brief Конструктор по умолчанию
 *
 * Создаёт пустое отделение с ID = -1, нулевыми координатами и пустым названием.
 */
Otdel::Otdel() {
    nomer = -1;
    x1 = 0;
    y1 = 0;
    nazvanie = "";
}

/**
 * @brief Конструктор с параметрами
 * @param id Уникальный идентификатор отделения
 * @param x Координата X
 * @param y Координата Y
 * @param name Название отделения
 */
Otdel::Otdel(int id, double x, double y, std::string name) {
    nomer = id;
    x1 = x;
    y1 = y;
    nazvanie = name;
}

int Otdel::getId() { return nomer; }
double Otdel::getX() { return x1; }
double Otdel::getY() { return y1; }
std::string Otdel::getName() { return nazvanie; }

/**
 * @brief Положить посылку в отделение
 * @param p Посылка для размещения
 *
 * Добавляет посылку в конец списка хранящихся в отделении.
 */
void Otdel::polozhit(Posylka p) {
    spisok.push_back(p);
}

/**
 * @brief Забрать посылку из отделения
 * @param tr Трек-номер посылки
 * @return true если посылка найдена и удалена, false если не найдена
 *
 * @details Перебирает список посылок в отделении.
 *          При нахождении совпадения по трек-номеру удаляет посылку из списка.
 *          Вектор сдвигается, порядок остальных посылок сохраняется.
 */
bool Otdel::zabrat(std::string tr) {
    for (int i = 0; i < spisok.size(); i++) {
        if (spisok[i].getTr() == tr) {
            spisok.erase(spisok.begin() + i);
            return true;
        }
    }
    return false;
}

/**
 * @brief Найти посылку по трек-номеру
 * @param tr Трек-номер
 * @return Указатель на посылку или nullptr если не найдена
 *
 * @note Возвращает указатель на элемент внутри вектора.
 *       Будьте осторожны - при модификации вектора указатель может стать недействительным.
 */
Posylka* Otdel::nayti(std::string tr) {
    for (int i = 0; i < spisok.size(); i++) {
        if (spisok[i].getTr() == tr) {
            return &spisok[i];
        }
    }
    return nullptr;
}

/**
 * @brief Получить все посылки в отделении
 * @return Копия вектора всех посылок
 *
 * @note Возвращается копия, а не ссылка, для защиты данных от случайного изменения.
 */
std::vector<Posylka> Otdel::vse() {
    return spisok;
}

/**
 * @brief Очистить все посылки из отделения
 *
 * Удаляет все элементы из вектора spisok.
 * Используется при пересчёте состояния после прокрутки времени.
 */
void Otdel::ochistit() {
    spisok.clear();
}
