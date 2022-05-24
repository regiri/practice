#include <iostream>
#include <vector>

#include "plant.h"
#include "practice.h"

using namespace std;



int main()
{
    setlocale(LC_ALL, "Russian");
    Plant plant;
    plant_init(plant);
    //вместо u0 запрашивать мин макс и шаг
    int num_measure_chan, num_control_chan, repeate_cnt; // M, L, N
    double control_out, tolerance, timedelta; // U0, Y0, E, T
    vector<double> control_in(3);

    input_inital_data(&num_measure_chan, &num_control_chan, control_in, &control_out, &tolerance, &timedelta, &repeate_cnt, "data.txt");

    calculate_control_quality(plant, control_in[0], control_out, num_measure_chan, num_control_chan, repeate_cnt, timedelta, tolerance);

    return 0;
}










