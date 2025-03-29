#include <iostream>
#include <vector>
#include <cmath> 

using namespace std;

int main() {
    const double Isp = 126.82;       // Метеор говорит 126.82, но этот параметр настолько хаотично зависит от парамтров дивгателя, что я даже хз чё тут выбрать. 151.3 - это взято отсюда: https://www.nakka-rocketry.net/techsht-knsb.htm
    const int D = 4000;
    const double g = 9.81;
    const double Phi_0 = 0.98;
    const double Ro_v = 1.2255;
    const double Gamma_0 = 2640.0;  // Плотность АМГ6
    const double F = 2.13;          // Ну так как в случае разрыва этого всего наиболее вероятна форма 1:1:8 то берём это дело из методы 
    const double Q_TNT = 4612.0;    // МДж
    double MEmpty = 215.326643;     // gramm
    double c_x = 1.21;              // Я хз,  пока я не объявил её тут у меня нихера не работало.
    double M = 163.174322;          // gramm
    double Md = MEmpty - M;
    double Fuel_Mass = 937.0;       // gramm

    double TE = pow(Isp * g, 2) / (2000 * Q_TNT);
    double C = TE * Fuel_Mass;
    double Q = Md + M + C;
    double Alpha = C / Q;
    double Mu_d = Md / Q;
    double Ksi = Alpha / (1 - Mu_d);
    double V_0 = Phi_0 * (D / 2) * pow(Ksi / (2 - Ksi), 0.5);

    // Определение сопротивления возхдуха в зависимости от скорости оболочки
    if (V_0 <= 200) {
        c_x = 0.75;
    } else if (V_0 <= 300) {
        c_x = 0.94;
    } else if (V_0 <= 400) {
        c_x = 1.14;
    } else if (V_0 <= 500) {
        c_x = 1.21;
    } else if (V_0 <= 600) {
        c_x = 1.19;
    } else if (V_0 <= 800) {
        c_x = 1.14;
    } else if (V_0 <= 1000) {
        c_x = 1.12;
    } else if (V_0 <= 1500) {
        c_x = 1.08;
    } else { 
        c_x = 1.07;
    }

    cout << "m,I,V_ub" << endl; // Заголовок для CSV

    for (int m = 1; m <= 100; m++) { // масса осколка
        for (int I = 50; I <= 1000; I += 10) { // расстояние от эпицентра, всё тот же убойный интеграл
            double I_A_b = (Ro_v * c_x * F * I) / (2 * pow(Gamma_0, 2.0/3.0) * pow(m, 1.0/3.0)); //ХЗ, просото чтоб лучше читалось поделил на две части
            double V_ub = exp(log(V_0) - I_A_b);
            
            cout << m << "," << I << "," << V_ub << endl;
        }
    }

    return 0;
}