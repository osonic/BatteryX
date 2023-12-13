//
// Created by Sonic on 5/8/15.
//

#include "Render.h"
#include <math.h>
#include <stdio.h>

void render::perfom(int nn, double *data)
{
    int buffer_size = nn * 2;

    for(int i = 1; i < buffer_size + 1; i+=2)
    {
        printf("%4.2f ,", pow(data[i] * data[i] + data[i+1] * data[i+1], 0.5));
    }
/*
    for(int i = 1; i < buffer_size + 1; i+=2)
    {
        printf("%4.2f, %4.2fi |", data[i], data[i+1]);
    }
*/
    printf("\r\n");
}
