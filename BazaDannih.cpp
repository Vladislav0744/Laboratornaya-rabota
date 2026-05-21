#include "BazaDannih.h"
#include "VremyaMachinery.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

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

bool Holder::proveritDublikatOtdela(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            cout << "Ошибка: отделение с ID " << id << " уже существует!\n";
            return true;
        }
    }
    return false;
}

bool Holder::proveritSuschestvuetOtdel(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            return true;
        }
    }
    cout << "Ошибка: отделение с ID " << id << " не найдено!\n";
    return false;
}

void Holder::dobavitOtdel(Otdel o) {
    otdeli.push_back(o);
}

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

Otdel* Holder::naytiOtdel(int id) {
    for (int i = 0; i < otdeli.size(); i++) {
        if (otdeli[i].getId() == id) {
            return &otdeli[i];
        }
    }
    return nullptr;
}

vector<Otdel> Holder::poluchitVse() {
    return otdeli;
}

void Holder::dobavitPosylku(Posylka p) {
    vsePosylki.push_back(p);
    Otdel* start = naytiOtdel(p.getOtkuda());
    if (start != nullptr) {
        start->polozhit(p);
    }
}

Posylka* Holder::naytiPosylku(string tr) {
    for (int i = 0; i < vsePosylki.size(); i++) {
        if (vsePosylki[i].getTr() == tr) {
            return &vsePosylki[i];
        }
    }
    return nullptr;
}

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

void Holder::proknutVremya(int dney, double speed) {
    peresmotretPosylki(dney, speed);
}

vector<pair<string, int>> Holder::chtoEdetVOtdel(int id) {
    vector<pair<string, int>> result;
    
    for (int i = 0; i < vsePosylki.size(); i++) {
        Posylka& p = vsePosylki[i];
        
        if (p.getKuda() == id &&
            p.getGde() != id &&
            p.getDney() > 0) {
            result.push_back(make_pair(p.getTr(), p.getDney()));
        }
    }
    
    return result;
}

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
