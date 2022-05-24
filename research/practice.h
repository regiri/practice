#ifndef PRACTICE_H_INCLUDED
#define PRACTICE_H_INCLUDED
#include <vector>
#include <string>
#include "plant.h"

using namespace std;

double new_control_action(double control_action, double control_deviation, double control_in, double tolerance, int control_channel);
void input_inital_data(
    int *num_measure_chan,
    int *num_control_chan,
    vector<double> &control_in,
    double*control_out,
    double *tolerance,
    double *timedelta,
    int *repeate_cnt,
    string filename = ""
);

double calculate_control_quality(
        Plant plant, double control_in, double control_out, int num_measure_chan,
        int num_control_chan, int repeate_cnt, double timedelta, double tolerance
);

#endif // PRACTICE_H_INCLUDED
