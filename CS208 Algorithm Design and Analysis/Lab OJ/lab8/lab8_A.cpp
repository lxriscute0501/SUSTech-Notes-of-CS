#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;

void fft(vector<double>& real, vector<double>& imag, int n) {
    if (n == 1) return;

    vector<double> evenr(n / 2), eveni(n / 2);
    vector<double> oddr(n / 2), oddi(n / 2);

    for (int i = 0; i < n / 2; i++)
    {
        oddr[i] = real[2 * i + 1];
        oddi[i] = imag[2 * i + 1];
        evenr[i] = real[2 * i];
        eveni[i] = imag[2 * i];
    }

    fft(evenr, eveni, n / 2);
    fft(oddr, oddi, n / 2);

    double angle = 2 * M_PI / n;
    double wr = 1.0, wi = 0.0, coss = cos(angle), sinn = sin(angle);

    for (int i = 0; i < n / 2; i++)
    {
        double tempr = wr * oddr[i] - wi * oddi[i];
        double tempi = wr * oddi[i] + wi * oddr[i];

        real[i] = tempr + evenr[i];
        imag[i] = tempi + eveni[i];
        real[i + n / 2] = evenr[i] - tempr;
        imag[i + n / 2] = eveni[i] - tempi;

        double wrr = wr * coss - wi * sinn;
        double wii = wr * sinn + wi * coss;
        wr = wrr; wi = wii;
    }
}

int main()
{
    int n;
    cin >> n;
    int size = pow(2, n);
    vector<double> s(size), real(size, 0.0), imag(size, 0.0);

    for (int i = 0; i < size; i++)
    {
        cin >> s[i];
        real[i] = s[i];
    }

    fft(real, imag, size);

    for (int i = 0; i < size; ++i)
        cout << fixed << setprecision(10) << sqrt(real[i] * real[i] + imag[i] * imag[i]) << '\n';

    return 0;
}
