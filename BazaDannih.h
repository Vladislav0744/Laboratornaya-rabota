#ifndef BAZA_DANNIH_H
#define BAZA_DANNIH_H

#include <vector>
#include <string>
#include <utility>
#include "OtdelenieStuff.h"

using namespace std;

/**
 * @brief Главный класс управления данными
 *
 * Хранит все отделения и все посылки, обеспечивает их взаимодействие.
 * Реализует загрузку и сохранение в файлы.
 */
class Holder {
public:
    /**
     * @brief Добавить новое отделение
     * @param o Отделение для добавления
     */
    void dobavitOtdel(Otdel o);
    
    /**
     * @brief Удалить отделение по ID
     * @param id ID удаляемого отделения
     * @return true если отделение найдено и удалено
     * @details Все посылки, которые были в отделении или ехали в него,
     *          возвращаются отправителю
     */
    bool udalitOtdel(int id);
    
    /**
     * @brief Найти отделение по ID
     * @param id ID отделения
     * @return Указатель на отделение или nullptr
     */
    Otdel* naytiOtdel(int id);
    
    /**
     * @brief Получить все отделения
     * @return Вектор всех отделений
     */
    vector<Otdel> poluchitVse();

    /**
     * @brief Добавить новую посылку
     * @param p Посылка для добавления
     * @details Посылка помещается в отделение отправителя
     */
    void dobavitPosylku(Posylka p);
    
    /**
     * @brief Найти посылку по трек-номеру
     * @param tr Трек-номер
     * @return Указатель на посылку или nullptr
     */
    Posylka* naytiPosylku(string tr);

    /**
     * @brief Выдать посылку получателю
     * @param tr Трек-номер посылки
     * @param fio ФИО получателя (для проверки)
     * @return true если выдача успешна
     * @details Посылка должна быть доставлена в отделение,
     *          ФИО должно совпадать. После выдачи посылка удаляется из системы
     */
    bool vydat(string tr, string fio);
    
    /**
     * @brief Проматнуть время вперёд
     * @param dney Количество дней для прокрутки
     * @param speed Скорость доставки в км/день
     * @details Обновляет положение всех посылок в пути
     */
    void proknutVremya(int dney, double speed);
    
    /**
     * @brief Получить список посылок, которые едут в указанное отделение
     * @param id ID отделения назначения
     * @return Вектор пар (трек-номер, осталось дней)
     */
    vector<pair<string, int>> chtoEdetVOtdel(int id);

    /**
     * @brief Загрузить данные из файлов
     * @param file Путь к файлу с отделениями
     * @return true если загрузка успешна (или файл не найден - новый запуск)
     * @details Отделения загружаются из file, посылки из parcels.txt
     */
    bool zagruzit(string file);
    
    /**
     * @brief Сохранить данные в файлы
     * @param file Путь для сохранения отделений
     * @return true если сохранение успешно
     * @details Отделения сохраняются в file, посылки в parcels.txt
     */
    bool sohranit(string file);
    
    /**
     * @brief Проверить существует ли отделение с таким ID
     * @param id ID для проверки
     * @return true если существует, false если нет
     */
    bool proveritDublikatOtdela(int id);
    
    /**
     * @brief Проверить существует ли отделение с таким ID
     * @param id ID для проверки
     * @return true если существует, false если нет
     */
    bool proveritSuschestvuetOtdel(int id);

private:
    vector<Otdel> otdeli;        ///< Список всех отделений
    vector<Posylka> vsePosylki;  ///< Список всех активных посылок
    
    /**
     * @brief Рассчитать расстояние между двумя отделениями
     * @param a ID первого отделения
     * @param b ID второго отделения
     * @return Расстояние в километрах
     */
    double rasstoyanie(int a, int b);
    
    /**
     * @brief Пересчитать положение посылок после прокрутки времени
     * @param dney Количество прошедших дней
     * @param sk Скорость доставки
     */
    void peresmotretPosylki(int dney, double sk);
};

#endif
