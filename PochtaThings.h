#ifndef POCHTA_THINGS_H
#define POCHTA_THINGS_H

#include <string>

/**
 * @brief Класс, представляющий почтовую посылку
 *
 * Хранит информацию о посылке: трек-номер, отправителя, получателя,
 * вес, маршрут и текущее состояние доставки.
 */
class Posylka {
public:
    /**
     * @brief Конструктор по умолчанию
     *
     * Создаёт пустую посылку с неинициализированными полями.
     */
    Posylka();
    
    /**
     * @brief Конструктор с параметрами
     * @param tr Трек-номер посылки
     * @param otpr ФИО отправителя
     * @param pol ФИО получателя
     * @param ves Вес посылки в килограммах
     * @param otkuda ID отделения отправителя
     * @param kuda ID отделения получателя
     */
    Posylka(std::string tr, std::string otpr, std::string pol,
           double ves, int otkuda, int kuda);

    /// @return Трек-номер посылки
    std::string getTr();
    
    /// @return ФИО отправителя
    std::string getOtpr();
    
    /// @return ФИО получателя
    std::string getPol();
    
    /// @return Вес в килограммах
    double getVes();
    
    /// @return ID отделения отправителя
    int getOtkuda();
    
    /// @return ID отделения получателя
    int getKuda();
    
    /// @return ID отделения где сейчас находится посылка
    int getGde();
    
    /**
     * @brief Получить количество оставшихся дней в пути
     * @return 0 - если не в пути или доставлена, иначе >0
     */
    int getDney();

    /**
     * @brief Установить текущее отделение посылки
     * @param id ID отделения
     */
    void setGde(int id);
    
    /**
     * @brief Установить количество дней в пути
     * @param dn Количество дней (0 - не в пути)
     */
    void setDney(int dn);
    
    /**
     * @brief Уменьшить оставшиеся дни на указанное количество
     * @param dn Количество прошедших дней
     * @details Если после уменьшения дней становится <= 0,
     *          посылка считается доставленной в отделение получателя
     */
    void proshloDney(int dn);

private:
    std::string treker;      ///< Трек-номер посылки
    std::string otpravitel;  ///< ФИО отправителя
    std::string poluchatel;  ///< ФИО получателя
    double ves;              ///< Вес в килограммах
    int izOtdela;            ///< ID отделения отправителя
    int vOtdel;              ///< ID отделения получателя
    int seychasGde;          ///< ID текущего отделения (или откуда выехала)
    int ostalosDney;         ///< Осталось дней в пути (0 - не в пути)
};

#endif
