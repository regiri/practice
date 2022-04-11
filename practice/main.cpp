#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <windows.h>
#include "plant.h"

using namespace std;

void input_inital_data(
    int *num_measure_chan,
    int *num_control_chan,
    double *control_in,
    double *control_out,
    double *tolerance,
    double *timedelta,
    int *repeate_cnt,
    string filename = ""
);

double new_control_action(double control_action, double control_deviation, double control_in, double tolerance, int control_channel);

int main()
{
    setlocale(LC_ALL, "Russian");
    Plant plant;
    plant_init(plant);

    int num_measure_chan, num_control_chan, repeate_cnt; // M, L, N
    double control_in, control_out, tolerance, timedelta, control_quality = 0; // U0, Y0, E, T

    input_inital_data(&num_measure_chan, &num_control_chan, &control_in, &control_out, &tolerance, &timedelta, &repeate_cnt, "data.txt");
    vector <double> control_actions(repeate_cnt), control_deviations(repeate_cnt), results(repeate_cnt); // U[], D[], Y[]

    control_actions.at(0) = control_in;
    for (size_t i = 0; i < repeate_cnt - 1; i++)
    {
        plant_control(num_control_chan, control_actions[i], plant);

        Sleep(timedelta);
        results[i] = plant_measure(num_measure_chan, plant);

        control_deviations[i] = results[i] - control_out;

        cout << "Воздействие " << control_actions[i] << "; Измерение " << results[i] << "; Отклонение " << control_deviations[i] << endl;

        control_actions[i + 1] = new_control_action(control_actions[i], control_deviations[i], control_in, tolerance, num_control_chan);
    }
    plant_control(num_control_chan, control_actions.at(repeate_cnt - 1), plant);

    Sleep(timedelta);
    results.at(repeate_cnt - 1) = plant_measure(num_measure_chan, plant);

    control_deviations.at(repeate_cnt - 1) = results.at(repeate_cnt - 1) - control_out;

    cout << "Воздействие " << control_actions.at(repeate_cnt - 1) << "; Измерение " << results.at(repeate_cnt - 1) << "; Отклонение " << control_deviations.at(repeate_cnt - 1) << endl;

    for (size_t i = 0; i < repeate_cnt; i++)
    {
        control_quality += pow(control_deviations[i], 2);
    }
    control_quality = control_quality / repeate_cnt;

    cout << control_quality;

    return 0;
}

void input_inital_data(
    int *num_measure_chan,
    int *num_control_chan,
    double *control_in,
    double *control_out,
    double *tolerance,
    double *timedelta,
    int *repeate_cnt,
    string filename
)
{
    if (filename == "")
    {
        cout << "Введите номер опрашиваемого датчика: ";
        cin >> *num_measure_chan;
        cout << "Введите номер канала управления: ";
        cin >> *num_control_chan;
        cout << "Введите величину управляющего воздействия: ";
        cin >> *control_in;
        cout << "Введите желаемое значение по опрашиваемому датчику: ";
        cin >> *control_out;
        cout << "Введите допустимое отклонение: ";
        cin >> *tolerance;
        cout << "Введите интервал времени между опросами: ";
        cin >> *timedelta;
        cout << "Введите число опросов: ";
        cin >> *repeate_cnt;
    }
    else
    {
        ifstream input(filename);
        input >> *num_measure_chan >> *num_control_chan >> *control_in >> *control_out >> *tolerance >> *timedelta >> *repeate_cnt;
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











