/**
 * @file BazaDannih.cpp
 * @brief Реализация класса Holder (хранилище данных)
 *
 * Содержит основную бизнес-логику программы:
 * - управление отделениями и посылками
 * - расчёт расстояний между отделениями
 * - прокрутка времени и перемещение посылок
 * - загрузка и сохранение данных
 */

#include "BazaDannih.h"
#include "VremyaMachinery.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/**
 * @brief Рассчитать расстояние между двумя отделениями
 * @param a ID первого отделения
 * @param b ID второго отделения
 * @return Расстояние в километрах
 *
 * @details Находит отделения по ID, использует их координаты
 *          и вызывает функцию rasst() из VremyaMachinery.
 *          Если хотя бы одно отделение не найдено, возвращает 0.
 *
 * @par Пример:
 * @code
 * // отделение 1: x=0, y=0
 * // отделение 2: x=3, y=4
 * rasstoyanie(1, 2); // возвращает 5
 * @endcode
 */
double Holder::rasstoyanie(int a, int b) {
    Otdel* o1 = nullptr;
    Otdel* o2 = nullptr;
    
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == a) o1 = &otdeli[i];
        if (otdeli[i].getId() == b) o2 = &otdeli[i];
    }
    
    if (o1 == nullptr || o2 == nullptr) return 0;
    
    double dx = o1->getX() - o2->getX();
    double dy = o1->getY() - o2->getY();
    return sqrt(dx*dx + dy*dy);
}

/**
 * @brief Проверить существует ли отделение с таким ID (для предотвращения дубликатов)
 * @param id ID для проверки
 * @return true если отделение уже существует, false если нет
 *
 * @details При обнаружении дубликата выводит сообщение об ошибке.
 *          Используется перед добавлением нового отделения.
 *
 * @par Пример:
 * @code
 * if (proveritDublikatOtdela(5)) {
 *     return; // отделение с ID 5 уже есть
 * }
 * @endcode
 */
bool Holder::proveritDublikatOtdela(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            cout << "Ошибка: отделение с ID " << id << " уже существует!\n";
            return true;
        }
    }
    return false;
}

/**
 * @brief Проверить существует ли отделение с таким ID
 * @param id ID для проверки
 * @return true если отделение существует, false если нет
 *
 * @details При отсутствии отделения выводит сообщение об ошибке.
 *          Используется при добавлении посылки для проверки существования
 *          отделений отправителя и получателя.
 *
 * @par Пример:
 * @code
 * if (!proveritSuschestvuetOtdel(otkuda)) {
 *     return; // отделение отправителя не найдено
 * }
 * @endcode
 */
bool Holder::proveritSuschestvuetOtdel(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            return true;
        }
    }
    cout << "Ошибка: отделение с ID " << id << " не найдено!\n";
    return false;
}

/**
 * @brief Добавить новое отделение
 * @param o Отделение для добавления
 *
 * @details Просто добавляет отделение в конец вектора.
 * @note Проверка на дубликаты должна выполняться до вызова этого метода.
 */
void Holder::dobavitOtdel(Otdel o) {
    otdeli.push_back(o);
}

/**
 * @brief Удалить отделение по ID
 * @param id ID удаляемого отделения
 * @return true если отделение найдено и удалено, false если не найдено
 *
 * @details При удалении отделения выполняются следующие шаги:
 *          1. Находит отделение по ID
 *          2. Возвращает все посылки, лежащие в отделении, обратно отправителям
 *          3. Возвращает все посылки, которые ехали в это отделение, обратно отправителям
 *          4. Удаляет отделение из списка
 *
 * @par Пример:
 * @code
 * if (udalitOtdel(3)) {
 *     cout << "Отделение 3 удалено\n";
 * }
 * @endcode
 */
bool Holder::udalitOtdel(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            
            cout << "\n=== УДАЛЕНИЕ ОТДЕЛЕНИЯ " << id << " ===\n";
            
            vector<Posylka> tmp = otdeli[i].vse();
            for (int j = 0; j < tmp.size(); j++) {
                int otkuda = tmp[j].getOtkuda();
                Otdel* from = naytiOtdel(otkuda);
                if (from != nullptr) {
                    tmp[j].setGde(otkuda);
                    tmp[j].setDney(0);
                    from->polozhit(tmp[j]);
                    
                    for (int k = 0; k < vsePosylki.size(); k++) {
                        if (vsePosylki[k].getTr() == tmp[j].getTr()) {
                            vsePosylki[k].setGde(otkuda);
                            vsePosylki[k].setDney(0);
                            break;
                        }
                    }
                    cout << "Посылка " << tmp[j].getTr() << " возвращена отправителю (лежала в отделении)\n";
                }
            }
            
            for (int j = 0; j < vsePosylki.size(); j++) {
                Posylka& p = vsePosylki[j];
                
                if (p.getKuda() == id && p.getGde() != id && p.getDney() > 0) {
                    int otkuda = p.getOtkuda();
                    Otdel* from = naytiOtdel(otkuda);
                    if (from != nullptr) {
                        p.setGde(otkuda);
                        p.setDney(0);
                        from->polozhit(p);
                        cout << "Посылка " << p.getTr() << " возвращена отправителю (была в пути)\n";
                    }
                }
            }
            
            otdeli.erase(otdeli.begin() + i);
            cout << "Отделение удалено\n";
            return true;
        }
    }
    return false;
}

/**
 * @brief Найти отделение по ID
 * @param id ID отделения
 * @return Указатель на отделение или nullptr если не найдено
 *
 * @details Линейный поиск по вектору отделений.
 * @note Возвращает указатель на элемент внутри вектора.
 *       При добавлении/удалении отделений указатель может стать недействительным.
 *
 * @par Пример:
 * @code
 * Otdel* o = naytiOtdel(5);
 * if (o != nullptr) {
 *     cout << o->getName();
 * }
 * @endcode
 */
Otdel* Holder::naytiOtdel(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            return &otdeli[i];
        }
    }
    return nullptr;
}

/**
 * @brief Получить все отделения
 * @return Вектор всех отделений (копия)
 */
vector<Otdel> Holder::poluchitVse() {
    return otdeli;
}

/**
 * @brief Добавить новую посылку
 * @param p Посылка для добавления
 *
 * @details Посылка добавляется в общий список всех посылок (vsePosylki)
 *          и помещается в отделение отправителя.
 *          Изначальный статус посылки: "ожидает отправки" (осталось дней = 0).
 *
 * @note Проверки на дубликаты и существование отделений должны выполняться
 *       до вызова этого метода.
 */
void Holder::dobavitPosylku(Posylka p) {
    vsePosylki.push_back(p);
    Otdel* start = naytiOtdel(p.getOtkuda());
    if (start != nullptr) {
        start->polozhit(p);
    }
}

/**
 * @brief Найти посылку по трек-номеру
 * @param tr Трек-номер
 * @return Указатель на посылку или nullptr если не найдена
 *
 * @details Линейный поиск по вектору всех активных посылок.
 * @note Поиск происходит только среди активных (не выданных) посылок.
 */
Posylka* Holder::naytiPosylku(string tr) {
    for (int i = 0; i < vsePosylki.size(); i++) {
        if (vsePosylki[i].getTr() == tr) {
            return &vsePosylki[i];
        }
    }
    return nullptr;
}

/**
 * @brief Выдать посылку получателю
 * @param tr Трек-номер посылки
 * @param fio ФИО получателя для проверки
 * @return true если выдача успешна, false в противном случае
 *
 * @details Условия успешной выдачи:
 *          - посылка существует в системе
 *          - посылка доставлена в отделение (getGde() == getKuda())
 *          - ФИО получателя совпадает с указанным в посылке
 *          - отделение назначения существует
 *
 * @post При успешной выдаче посылка полностью удаляется:
 *       - удаляется из списка посылок отделения
 *       - удаляется из общего списка vsePosylki
 *
 * @par Пример:
 * @code
 * if (vydat("TRACK001", "Иванов Иван")) {
 *     cout << "Посылка выдана\n";
 * }
 * @endcode
 */
bool Holder::vydat(string tr, string fio) {
    int index = -1;
    for (int i = 0; i < vsePosylki.size(); i++) {
        if (vsePosylki[i].getTr() == tr) {
            index = i;
            break;
        }
    }
    
    if (index == -1) return false;
    
    Posylka& p = vsePosylki[index];
    
    if (p.getGde() != p.getKuda()) return false;
    if (p.getPol() != fio) return false;
    
    Otdel* final = naytiOtdel(p.getGde());
    if (final == nullptr) return false;
    
    if (!final->zabrat(tr)) return false;
    
    vsePosylki.erase(vsePosylki.begin() + index);
    
    return true;
}

/**
 * @brief Пересчитать положение посылок после прокрутки времени
 * @param dney Количество прошедших дней
 * @param sk Скорость доставки в км/день
 *
 * @details Внутренний метод, вызываемый из proknutVremya().
 *          Для каждой посылки выполняется логика:
 *          - если уже доставлена (getGde() == getKuda()) - пропускаем
 *          - если в пути (getDney() > 0) - уменьшаем оставшиеся дни
 *          - если ожидает отправки (getDney() == 0) - начинаем путь:
 *            рассчитываем расстояние, вычисляем нужное количество дней,
 *            устанавливаем статус "в пути"
 *
 *          После обновления статусов:
 *          - очищаем все отделения
 *          - перераспределяем посылки по отделениям
 *            (посылки в пути не принадлежат ни одному отделению)
 */
void Holder::peresmotretPosylki(int dney, double sk) {
    for (int i = 0; i < vsePosylki.size(); i++) {
        Posylka& p = vsePosylki[i];
        
        if (p.getGde() == p.getKuda()) {
            continue;
        }
        
        if (p.getDney() > 0) {
           
            p.proshloDney(dney);
        } else {
            
            double dist = rasstoyanie(p.getGde(), p.getKuda());
            int needDays = VremyaMachinery::skolkoDney(dist, sk);
            if (needDays == 0 && dist > 0) needDays = 1;
            p.setDney(needDays);
            p.proshloDney(dney);
        }
    }
    
    for (int i = 0; i < otdeli.size(); i++) {
        otdeli[i].ochistit();
    }
    
    for (int i = 0; i < vsePosylki.size(); i++) {
        Posylka& p = vsePosylki[i];
        
        if (p.getDney() > 0 && p.getGde() != p.getKuda()) {
            continue;
        }
        
        Otdel* o = naytiOtdel(p.getGde());
        if (o != nullptr) {
            o->polozhit(p);
        }
    }
}

/**
 * @brief Проматнуть время вперёд
 * @param dney Количество дней для прокрутки
 * @param speed Скорость доставки в км/день
 *
 * @details Публичный метод, который вызывает внутреннюю функцию
 *          peresmotretPosylki() для обновления состояния всех посылок.
 *
 * @par Пример:
 * @code
 * proknutVremya(3, 450.0); // прошло 3 дня при скорости 450 км/день
 * @endcode
 */
void Holder::proknutVremya(int dney, double speed) {
    peresmotretPosylki(dney, speed);
}

/**
 * @brief Получить список посылок, которые едут в указанное отделение
 * @param id ID отделения назначения
 * @return Вектор пар (трек-номер, осталось дней до прибытия)
 *
 * @details Ищет посылки, удовлетворяющие всем условиям:
 *          - пункт назначения совпадает с id (p.getKuda() == id)
 *          - текущее положение не равно пункту назначения (p.getGde() != id)
 *          - количество оставшихся дней больше 0 (p.getDney() > 0)
 *
 * @par Пример:
 * @code
 * vector<pair<string, int>> edut = chtoEdetVOtdel(2);
 * for (auto& p : edut) {
 *     cout << p.first << " будет через " << p.second << " дн.\n";
 * }
 * @endcode
 */
vector<pair<string, int>> Holder::chtoEdetVOtdel(int id) {
    vector<pair<string, int>> result;
    
    for (int i = 0; i < vsePosylki.size(); i++) {
        Posylka& p = vsePosylki[i];
        
        if (p.getKuda() == id && p.getGde() != id && p.getDney() > 0) {
            result.push_back(make_pair(p.getTr(), p.getDney()));
        }
    }
    
    return result;
}

/**
 * @brief Загрузить данные из файлов
 * @param file Путь к файлу с отделениями
 * @return true если загрузка успешна (или файл не найден - новый запуск)
 *
 * @details Формат файла отделений (offices.txt):
 *          "ID X Y Название"
 *          Например: "1 0 0 Центральное"
 *
 *          Формат файла посылок (parcels.txt):
 *          "трек|отправитель|получатель|вес|откуда|куда|где|дней"
 *          Например: "TRACK001|Иванов|Петров|2.5|1|2|1|3"
 *
 *          Если файлы не найдены - программа начинает работу с пустой базой.
 *          Это нормальное поведение для первого запуска.
 */
bool Holder::zagruzit(string file) {
    ifstream f(file);
    
    if (!f.is_open()) {
        cout << "Файл " << file << " не найден. Начинаем с пустой базы.\n";
        return true;
    }
    
    otdeli.clear();
    vsePosylki.clear();
    
    string line;
    int officesLoaded = 0;
    
    while (getline(f, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        int id;
        double x, y;
        string name;
        
        ss >> id >> x >> y;
        getline(ss, name);
        
        if (!name.empty() && name[0] == ' ') {
            name = name.substr(1);
        }
        
        dobavitOtdel(Otdel(id, x, y, name));
        officesLoaded++;
    }
    
    f.close();
    
    cout << "Загружено отделений: " << officesLoaded << "\n";
    
    ifstream f2("parcels.txt");
    if (f2.is_open()) {
        int parcelsLoaded = 0;
        while (getline(f2, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string treker, otpr, pol;
            double ves;
            int otkuda, kuda, gde, dney;
            
            getline(ss, treker, '|');
            getline(ss, otpr, '|');
            getline(ss, pol, '|');
            ss >> ves;
            ss.ignore();
            ss >> otkuda;
            ss.ignore();
            ss >> kuda;
            ss.ignore();
            ss >> gde;
            ss.ignore();
            ss >> dney;
            
            Posylka p(treker, otpr, pol, ves, otkuda, kuda);
            p.setGde(gde);
            p.setDney(dney);
            vsePosylki.push_back(p);
            
            if (dney == 0 || gde == kuda) {
                Otdel* o = naytiOtdel(gde);
                if (o != nullptr) {
                    o->polozhit(p);
                }
            }
            parcelsLoaded++;
        }
        f2.close();
        cout << "Загружено посылок: " << parcelsLoaded << "\n";
    }
    
    return true;
}

/**
 * @brief Сохранить данные в файлы
 * @param file Путь для сохранения отделений
 * @return true если сохранение успешно, false при ошибке
 *
 * @details Отделения сохраняются в указанный файл в формате:
 *          "ID X Y Название"
 *
 *          Посылки сохраняются в файл parcels.txt в формате:
 *          "трек|отправитель|получатель|вес|откуда|куда|где|дней"
 *
 *          Сохраняются только активные (не выданные) посылки.
 *
 * @par Пример сохранённых данных:
 * @code
 * // offices.txt
 * 1 0 0 Москва
 * 2 500 0 Санкт-Петербург
 *
 * // parcels.txt
 * TRACK001|Иванов|Петров|2.5|1|2|2|0
 * @endcode
 */
bool Holder::sohranit(string file) {
    ofstream f(file);
    if (!f.is_open()) {
        cout << "Ошибка: не могу сохранить в " << file << "\n";
        return false;
    }
    
    for (int i = 0; i < otdeli.size(); i++) {
        f << otdeli[i].getId() << " "
          << otdeli[i].getX() << " "
          << otdeli[i].getY() << " "
          << otdeli[i].getName() << "\n";
    }
    f.close();
    
    ofstream f2("parcels.txt");
    if (!f2.is_open()) {
        cout << "Ошибка: не могу сохранить посылки\n";
        return false;
    }
    
    for (int i = 0; i < vsePosylki.size(); i++) {
        f2 << vsePosylki[i].getTr() << "|"
           << vsePosylki[i].getOtpr() << "|"
           << vsePosylki[i].getPol() << "|"
           << vsePosylki[i].getVes() << "|"
           << vsePosylki[i].getOtkuda() << "|"
           << vsePosylki[i].getKuda() << "|"
           << vsePosylki[i].getGde() << "|"
           << vsePosylki[i].getDney() << "\n";
    }
    f2.close();
    
    cout << "Данные сохранены\n";
    return true;
}
