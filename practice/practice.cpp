#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <unistd.h>
#include "practice.h"

using namespace std;


void input_inital_data(
    int *num_measure_chan,
    int *num_control_chan,
    vector<double>&control_in,
    double *control_out,
    double *tolerance,
    double *timedelta,
    int *repeate_cnt,
    string filename
) {
    if (filename == "")
    {
        cout << "¬ведите номер опрашиваемого датчика: ";
        cin >> *num_measure_chan;
        cout << "¬ведите номер канала управлени€: ";
        cin >> *num_control_chan;
        double t;
        cout << "¬ведите нижний порог управл€ющего воздействи€: ";
        cin >> t;
        control_in.at(0) = t;
        cout << "¬ведите верний порог управл€ющего воздействи€: ";
        cin >> t;
        control_in[1] = t;
        cout << "¬ведите шаг управл€ющего воздействи€: ";
        cin >> t;
        control_in[2] = t;
        cout << "¬ведите желаемое значение по опрашиваемому датчику: ";
        cin >> *control_out;
        cout << "¬ведите допустимое отклонение: ";
        cin >> *tolerance;
        cout << "¬ведите интервал времени между опросами: ";
        cin >> *timedelta;
        cout << "¬ведите число опросов: ";
        cin >> *repeate_cnt;
    }
    else
    {
        ifstream input(filename);
        input >> *num_measure_chan >> *num_control_chan >> control_in[0] >> control_in[1] >> control_in[2] >> *control_out >> *tolerance >> *timedelta >> *repeate_cnt;
    }
}

double new_control_action(double control_action, double control_deviation, double control_in, double tolerance, int control_channel)
{
    double new_action;
    if (control_deviation <= tolerance && control_deviation >= -tolerance)
    {
        new_action = control_action;
    }
    else if (control_deviation < -tolerance)
    {
        new_action = control_action + control_in;
    }
    else
    {
        new_action = control_action - control_in;
    }
    switch (control_channel)
    {
    case 7:
        if (new_action > 70)
        {
            return 70;
        }
        else if (new_action < -70)
        {
            return -70;
        }
        else
        {
            return new_action;
        }
        break;
    case 8:
        if (new_action > 3)
        {
            return 3;
        }
        else if (new_action < -3)
        {
            return -3;
        }
        else
        {
            return new_action;
        }
        break;
    case 9:
        if (new_action > 5)
        {
            return 5;
        }
        else if (new_action < -5)
        {
            return -5;
        }
        else
        {
            return new_action;
        }
        break;
    case 10:
        if (new_action > 2.5)
        {
            return 2.5;
        }
        else if (new_action < -2.5)
        {
            return -2.5;
        }
        else
        {
            return new_action;
        }
        break;
    }
    return new_action;
}


double calculate_control_quality(
        Plant plant, double control_in, double control_out, int num_measure_chan,
        int num_control_chan, int repeate_cnt, double timedelta, double tolerance
) {
    double control_quality;
    vector <double> control_actions(repeate_cnt), control_deviations(repeate_cnt), results(repeate_cnt); // U[], D[], Y[]
    control_actions.at(0) = control_in;
    for (size_t i = 0; i < repeate_cnt - 1; i++)
    {
        plant_control(num_control_chan, control_actions[i], plant);

        sleep(timedelta);
        results[i] = plant_measure(num_measure_chan, plant);

        control_deviations[i] = results[i] - control_out;

        cerr << "¬оздействие " << control_actions[i] << "; »змерение " << results[i] << "; ќтклонение " << control_deviations[i] << endl;

        control_actions[i + 1] = new_control_action(control_actions[i], control_deviations[i], control_in, tolerance, num_control_chan);
    }
    plant_control(num_control_chan, control_actions.at(repeate_cnt - 1), plant);

    sleep(timedelta);
    results.at(repeate_cnt - 1) = plant_measure(num_measure_chan, plant);

    control_deviations.at(repeate_cnt - 1) = results.at(repeate_cnt - 1) - control_out;

    cerr << "¬оздействие " << control_actions.at(repeate_cnt - 1) << "; »змерение " << results.at(repeate_cnt - 1) << "; ќтклонение " << control_deviations.at(repeate_cnt - 1) << endl;

    for (size_t i = 0; i < repeate_cnt; i++)
    {
        control_quality += pow(control_deviations[i], 2);
    }
    control_quality = control_quality / repeate_cnt;

    cerr << control_quality;
    return control_quality;
}



