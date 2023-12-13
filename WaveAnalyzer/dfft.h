//
// Created by Sonic on 5/7/15.
//



#ifndef WAVEANALYZER_DFFT_H
#define WAVEANALYZER_DFFT_H

class dfft {

private:
    double* m_data;
    int m_nn;

public:
    dfft(int m_nn);

    ~dfft();

    void add_point(int ioffset, double real, double img);
    void perform(int isign);

    double *get_data() const
    {
        return m_data;
    }

    int get_nn() const
    {
        return m_nn;
    }
};

#endif //WAVEANALYZER_DFFT_H
