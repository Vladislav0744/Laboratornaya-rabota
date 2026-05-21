/**
 * @file main.cpp
 * @brief Главный файл программы "Почта России"
 *
 * Консольное приложение для управления почтовыми отправлениями.
 * Позволяет добавлять/удалять отделения, посылки, отслеживать их статус,
 * выдавать посылки получателям.
 *
 * @author Студент
 * @date 2026
 */

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <utility>
#include <fstream>
#include "BazaDannih.h"

using namespace std;

/// @brief Скорость доставки грузовика в километрах в день
double SKOROST = 450.0;

#ifdef _WIN32
    #include <direct.h>
    /**
     * @brief Создать папку (Windows)
     * @param path Путь к папке
     */
    void sozdatPapku(const string& path) {
        _mkdir(path.c_str());
    }
#else
    #include <sys/stat.h>
    /**
     * @brief Создать папку (Linux/macOS)
     * @param path Путь к папке
     */
    void sozdatPapku(const string& path) {
        mkdir(path.c_str(), 0777);
    }
#endif

/**
 * @brief Отобразить главное меню программы
 */
void pokazatMenu() {
    cout << "\n========================================\n";
    cout << "         ПОЧТА РОССИИ\n";
    cout << "========================================\n";
    cout << "1. Добавить отделение\n";
    cout << "2. Удалить отделение\n";
    cout << "3. Информация об отделении\n";
    cout << "4. Добавить посылку\n";
    cout << "5. Найти посылку по треку\n";
    cout << "6. Выдать посылку\n";
    cout << "7. Проматать время (дни)\n";
    cout << "0. Выход\n";
    cout << "> ";
}

/**
 * @brief Диалог добавления нового отделения
 * @param h Ссылка на объект базы данных
 */
void dobavitOtdel(Holder& h) {
    int id;
    double x, y;
    string name;
    
    cout << "ID отделения: "; cin >> id;
    
    if (h.proveritDublikatOtdela(id)) {
        cin.ignore();
        return;
    }
    
    cout << "Координата X: "; cin >> x;
    cout << "Координата Y: "; cin >> y;
    cin.ignore();
    cout << "Название отделения: "; getline(cin, name);
    
    h.dobavitOtdel(Otdel(id, x, y, name));
    cout << "Отделение добавлено\n";
}

/**
 * @brief Диалог удаления отделения
 * @param h Ссылка на объект базы данных
 */
void udalitOtdel(Holder& h) {
    int id;
    cout << "ID отделения: "; cin >> id;
    
    if (h.udalitOtdel(id)) {
        cout << "Отделение удалено. Посылки возвращены отправителям\n";
    } else {
        cout << "Отделение не найдено\n";
    }
}

/**
 * @brief Отображение информации об отделении
 * @param h Ссылка на объект базы данных
 * @details Выводит список посылок внутри отделения и список посылок в пути
 */
void posmotretOtdel(Holder& h) {
    int id;
    cout << "ID отделения: "; cin >> id;
    
    Otdel* o = h.naytiOtdel(id);
    if (o == nullptr) {
        cout << "Отделение не найдено\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "Отделение: " << o->getName() << "\n";
    cout << "ID: " << o->getId() << "\n";
    cout << "Координаты: (" << o->getX() << "; " << o->getY() << ")\n";
    cout << "========================================\n";
    
    vector<Posylka> tmp = o->vse();
    if (tmp.size() == 0) {
        cout << "\nПосылок внутри нет\n";
    } else {
        cout << "\n=== ПОСЫЛКИ ВНУТРИ ===\n";
        for (int i = 0; i < tmp.size(); i++) {
            cout << "   Трек: " << tmp[i].getTr()
                 << " | Получатель: " << tmp[i].getPol()
                 << " | Вес: " << tmp[i].getVes() << " кг\n";
        }
    }
    
    vector<pair<string, int>> edut = h.chtoEdetVOtdel(id);
    if (edut.size() == 0) {
        cout << "\n=== ПОСЫЛКИ В ПУТИ ===\n";
        cout << "   нет посылок в пути\n";
    } else {
        cout << "\n=== ПОСЫЛКИ КОТОРЫЕ ЕДУТ СЮДА ===\n";
        for (int i = 0; i < edut.size(); i++) {
            cout << "   Трек: " << edut[i].first
                 << " | Будет через: " << edut[i].second << " дн.\n";
        }
    }
}

/**
 * @brief Диалог добавления новой посылки
 * @param h Ссылка на объект базы данных
 */
void dobavitPosylku(Holder& h) {
    string treker, otpravitel, poluchatel;
    double ves;
    int otkuda, kuda;
    
    cout << "Трек-номер: "; cin >> treker;
    
    if (h.naytiPosylku(treker) != nullptr) {
        cout << "Посылка с таким трек-номером уже существует!\n";
        cin.ignore();
        return;
    }
    
    cin.ignore();
    cout << "Отправитель (ФИО): "; getline(cin, otpravitel);
    cout << "Получатель (ФИО): "; getline(cin, poluchatel);
    cout << "Вес (кг): "; cin >> ves;
    cout << "ID отделения отправителя: "; cin >> otkuda;
    cout << "ID отделения получателя: "; cin >> kuda;
    
    if (!h.proveritSuschestvuetOtdel(otkuda)) {
        return;
    }
    if (!h.proveritSuschestvuetOtdel(kuda)) {
        return;
    }
    
    if (otkuda == kuda) {
        cout << "Ошибка: отделение отправителя и получателя не могут совпадать!\n";
        return;
    }
    
    h.dobavitPosylku(Posylka(treker, otpravitel, poluchatel, ves, otkuda, kuda));
    cout << "Посылка добавлена\n";
}

/**
 * @brief Поиск посылки по трек-номеру
 * @param h Ссылка на объект базы данных
 */
void naytiPosylku(Holder& h) {
    string treker;
    cout << "Введите трек-номер: "; cin >> treker;
    
    Posylka* p = h.naytiPosylku(treker);
    if (p == nullptr) {
        cout << "Посылка не найдена\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "Трек-номер: " << p->getTr() << "\n";
    cout << "Отправитель: " << p->getOtpr() << "\n";
    cout << "Получатель: " << p->getPol() << "\n";
    cout << "Вес: " << p->getVes() << " кг\n";
    cout << "Откуда: отделение " << p->getOtkuda() << "\n";
    cout << "Куда: отделение " << p->getKuda() << "\n";
    cout << "----------------------------------------\n";
    
    if (p->getGde() == p->getKuda()) {
        cout << "СТАТУС: ДОСТАВЛЕНА\n";
        cout << "Находится в отделении " << p->getGde() << "\n";
    } else if (p->getDney() > 0) {
        cout << "СТАТУС: В ПУТИ\n";
        cout << "ОСТАЛОСЬ ДНЕЙ: " << p->getDney() << "\n";
        cout << "Сейчас находится между отделениями "
             << p->getGde() << " и " << p->getKuda() << "\n";
    } else {
        cout << "СТАТУС: ОЖИДАЕТ ОТПРАВКИ\n";
        cout << "Находится в отделении " << p->getGde() << "\n";
        cout << "Ещё не отправлена. Проматайте время чтобы отправить.\n";
    }
    cout << "========================================\n";
}

/**
 * @brief Диалог выдачи посылки получателю
 * @param h Ссылка на объект базы данных
 */
void vydatPosylku(Holder& h) {
    string treker, fio;
    cout << "Трек-номер: "; cin >> treker;
    cin.ignore();
    cout << "ФИО получателя: "; getline(cin, fio);
    
    if (h.vydat(treker, fio)) {
        cout << "Посылка выдана получателю!\n";
    } else {
        cout << "Ошибка выдачи. Проверьте:\n";
        cout << "  - правильность трек-номера\n";
        cout << "  - совпадение ФИО получателя\n";
        cout << "  - посылка должна быть доставлена в отделение\n";
    }
}

/**
 * @brief Прокрутка времени вперёд
 * @param h Ссылка на объект базы данных
 */
void prokrutka(Holder& h) {
    int dni;
    cout << "На сколько дней проматываем время? "; cin >> dni;
    
    if (dni <= 0) {
        cout << "Введите положительное число\n";
        return;
    }
    
    h.proknutVremya(dni, SKOROST);
    cout << "\nПрошло " << dni << " дней\n";
}

/**
 * @brief Главная функция программы
 * @return 0 при успешном завершении
 */
int main() {
    sozdatPapku("data");
    
    ofstream("data/offices.txt", ios::app);
    ofstream("parcels.txt", ios::app);
    
    Holder h;
    
    cout << "Загрузка данных...\n";
    h.zagruzit("data/offices.txt");
    
    int vibor;
    
    do {
        pokazatMenu();
        cin >> vibor;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(999, '\n');
            cout << "Ошибка: введите число\n";
            continue;
        }
        
        switch(vibor) {
            case 1:
                dobavitOtdel(h);
                break;
            case 2:
                udalitOtdel(h);
                break;
            case 3:
                posmotretOtdel(h);
                break;
            case 4:
                dobavitPosylku(h);
                break;
            case 5:
                naytiPosylku(h);
                break;
            case 6:
                vydatPosylku(h);
                break;
            case 7:
                prokrutka(h);
                break;
            case 0:
                cout << "\nСохраняем данные...\n";
                h.sohranit("data/offices.txt");
                cout << "До встречи!\n";
                break;
            default:
                cout << "Нет такого пункта меню\n";
        }
        
        if (vibor != 0) {
            cout << "\nНажмите Enter для продолжения...";
            cin.ignore(999, '\n');
            cin.get();
        }
        
    } while (vibor != 0);
    
    return 0;
}
