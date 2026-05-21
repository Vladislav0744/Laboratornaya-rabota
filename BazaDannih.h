#ifndef BAZA_DANNIH_H
#define BAZA_DANNIH_H

#include <vector>
#include <string>
#include <utility>
#include "OtdelenieStuff.h"

using namespace std;

class Holder {
public:
    void dobavitOtdel(Otdel o);
    bool udalitOtdel(int id);
    Otdel* naytiOtdel(int id);
    vector<Otdel> poluchitVse();

    void dobavitPosylku(Posylka p);
    Posylka* naytiPosylku(string tr);

    bool vydat(string tr, string fio);
    void proknutVremya(int dney, double speed);
    
    vector<pair<string, int>> chtoEdetVOtdel(int id);

    bool zagruzit(string file);
    bool sohranit(string file);
    
    bool proveritDublikatOtdela(int id);
    bool proveritSuschestvuetOtdel(int id);

private:
    vector<Otdel> otdeli;
    vector<Posylka> vsePosylki;
    
    double rasstoyanie(int a, int b);
    void peresmotretPosylki(int dney, double sk);
};

#endif
