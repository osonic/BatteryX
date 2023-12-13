//
// Created by Sonic on 5/7/15.
//

#include "dfft.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PI	M_PI	/* pi to machine precision, defined in math.h */
#define TWOPI	(2.0*PI)


dfft::dfft(int nn)
{
    m_nn = nn;
    int buffer_size = (int)((m_nn + 1) * sizeof(double) * 2);
    m_data = (double*)malloc(buffer_size);
    m_data[0] = 0;

}

dfft::~dfft()
{
    free(m_data);
}

void dfft::add_point(int ioffset, double real, double img)
{
    m_data[ioffset * 2 + 1] = real;
    m_data[ioffset * 2 + 2] = img;
}

void dfft::perform(int isign)
{
    int n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;

    n = m_nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            tempr = m_data[j];     m_data[j] = m_data[i];     m_data[i] = tempr;
            tempr = m_data[j+1]; m_data[j+1] = m_data[i+1]; m_data[i+1] = tempr;
        }
        m = n >> 1;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax = 2;
    while (n > mmax) {
        istep = 2*mmax;
        theta = TWOPI/(isign*mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j =i + mmax;
                tempr = wr*m_data[j]   - wi*m_data[j+1];
                tempi = wr*m_data[j+1] + wi*m_data[j];
                m_data[j]   = m_data[i]   - tempr;
                m_data[j+1] = m_data[i+1] - tempi;
                m_data[i] += tempr;
                m_data[i+1] += tempi;
            }
            wr = (wtemp = wr)*wpr - wi*wpi + wr;
            wi = wi*wpr + wtemp*wpi + wi;
        }
        mmax = istep;
    }
}
