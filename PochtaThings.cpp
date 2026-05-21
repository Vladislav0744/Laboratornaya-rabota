/**
 * @file PochtaThings.cpp
 * @brief Реализация класса Posylka (посылка)
 */

#include "PochtaThings.h"

/**
 * @brief Конструктор по умолчанию
 *
 * Инициализирует все поля значениями по умолчанию.
 * Трек-номер и ФИО - пустые строки.
 * Вес и ID отделений - 0 или -1.
 * Количество дней в пути = 0 (не в пути).
 */
Posylka::Posylka() {
    treker = "";
    otpravitel = "";
    poluchatel = "";
    ves = 0;
    izOtdela = -1;
    vOtdel = -1;
    seychasGde = -1;
    ostalosDney = 0;
}

/**
 * @brief Конструктор с параметрами
 * @param tr Трек-номер посылки
 * @param otpr ФИО отправителя
 * @param pol ФИО получателя
 * @param ves Вес в килограммах
 * @param otkuda ID отделения отправителя
 * @param kuda ID отделения получателя
 *
 * После создания посылка находится в отделении отправителя (seychasGde = otkuda).
 * Статус - "ожидает отправки" (ostalosDney = 0).
 */
Posylka::Posylka(std::string tr, std::string otpr, std::string pol,
               double ves, int otkuda, int kuda) {
    treker = tr;
    otpravitel = otpr;
    poluchatel = pol;
    this->ves = ves;
    izOtdela = otkuda;
    vOtdel = kuda;
    seychasGde = otkuda;
    ostalosDney = 0;
}

std::string Posylka::getTr() { return treker; }
std::string Posylka::getOtpr() { return otpravitel; }
std::string Posylka::getPol() { return poluchatel; }
double Posylka::getVes() { return ves; }
int Posylka::getOtkuda() { return izOtdela; }
int Posylka::getKuda() { return vOtdel; }
int Posylka::getGde() { return seychasGde; }
int Posylka::getDney() { return ostalosDney; }

void Posylka::setGde(int id) { seychasGde = id; }
void Posylka::setDney(int dn) { ostalosDney = dn; }

/**
 * @brief Уменьшить количество оставшихся дней в пути
 * @param dn Количество прошедших дней
 *
 * @details Если посылка в пути (ostalosDney > 0), уменьшает остаток.
 *          Если после уменьшения остаток становится <= 0,
 *          посылка считается доставленной:
 *          - ostalosDney = 0
 *          - seychasGde = vOtdel (перемещается в отделение получателя)
 */
void Posylka::proshloDney(int dn) {
    if (ostalosDney > 0) {
        ostalosDney = ostalosDney - dn;
        if (ostalosDney <= 0) {
            ostalosDney = 0;
            seychasGde = vOtdel;
        }
    }
}
