#include <iostream>
#include <cmath>
#include <string>

using namespace std;


double calculateDetonationSpeed(
    int a, int b, int c, int d,
    double Q_vv,
    double M_vv,
    double rho,
    string& error_msg 
) {
    error_msg.clear();
    
    if (M_vv <= 0) {
        error_msg = "Error: the molecular weight must be positive!";
        return -1;
    }

    // 1. Расчет кислородного баланса
    double KB = 1600.0 * (c - 2*a - b/2.0) / M_vv;
    KB = fabs(KB);

    // 2. Расчет Q_v
    double Q_v = 103.0 * (Q_vv - 0.3*(b + c + d)) / M_vv;

    // 3. Расчет фактора B
    double B = 103.0 * (a + b + c + d) / M_vv;

    // 4. Определение постоянной M
    double M;
    double condition = a + b/2.0;
    
    if (d < condition) M = 3.0;
    else if (d < 2*condition) M = 3.5;
    else M = 4.0;

    // 5. Проверка подкоренного выражения
    double sqrt_argument = 0.73*B - 0.24*KB - 0.0073*Q_v;
    
    if (sqrt_argument < 0) {
        error_msg = "Error: negative root expression (" 
                  + to_string(sqrt_argument) + "). The calculation is stopped.";
        return -1;
    }

    // 6. Расчет скорости
    double D1600 = sqrt(sqrt_argument);
    double Drho = D1600 + M*(rho*1000 - 1600)/1000;

    return Drho;
}

int main() {
    int a = 30, b = 70, c = 12, d = 66;
    double Q_vv = -3283.3;
    double M_vv = 119.82;
    double rho = 1.841;
    string error;

    double speed = calculateDetonationSpeed(a, b, c, d, Q_vv, M_vv, rho, error);

    if (!error.empty()) {
        cout << "[!] " << error << endl;
    } else if (speed > 0) {
        cout << "D: " << speed << " km/s" << endl;
    }

    return 0;
}