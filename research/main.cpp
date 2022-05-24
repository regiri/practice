#include "plant.h"
#include "practice.h"
#include <discpp.h>
#include <cmath>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    // ������������� ��.
    Plant plant;
    plant_init(plant);

    // ��������� ����������������� ������.
    int num_measure_chan, num_control_chan, repeate_cnt; // M, L, N
    double control_out, tolerance, timedelta, control_quality = 0; // U0, Y0, E, T
    vector<double> control_in(3);

    input_inital_data(&num_measure_chan, &num_control_chan, control_in, &control_out, &tolerance, &timedelta, &repeate_cnt, "data.txt");
    double steps = (control_in[1] - control_in[0])/control_in[2] + 1;
    vector<double> xs(steps + 1);
    vector<double> ys(steps + 1);

    for (size_t i = 0; i < steps + 1; i++) {
        double control = control_in[0] + control_in[2] * i;
        xs[i] = control;
        ys[i] = calculate_control_quality(plant, control, control_out, num_measure_chan, num_control_chan, repeate_cnt, timedelta, tolerance);
    }

    // ������ ��� ��������� �������.
    Dislin g;

    // ������ ������, "cons" �������� ����� �� �����.
    g.metafl("PDF");
    // �������� ����� �������� ���� (�� ��������� ��������).
    g.scrmod("revers");
    //g.setfil("dislin.png");
    // �������������� ����������, ������ ���� ������ ������ ������� ���������.

    g.disini();
    // ������ ������� ����.
    g.name("Control in (U0)", "x");
    g.name("Control quality (Q)", "y");

    // ���������� ���� ����� ������� �� ��� �������: ���, ������ ����� �����.
    g.labdig(-1, "x");

    // ���������� ������������� ����� (����� �������) �� ����.
    g.ticks(1, "x");
    g.ticks(1, "y");

    // ������ ��������� �������.
    g.titlin("Plant parameter plot", 1);

    // ���������� ���� ��� ����� ��������, �������� � ������ �� 0 �� 1 (RGB).
    // �����: ��� ���������� ����� � ������ � 1, �. �. ���� ������-�����.
    const int background_color = g.intrgb(0.95, 0.95, 0.95);

    // ���������� ���� ������������ ���� ��� ������� ����.
    g.axsbgd(background_color);

    // ������ ������������ �����.
    g.graf(
        // ������� �������� �� ��� ������� (X).
        control_in[0], control_in[1],
        // ���������� �������� �� ��� ������� � ��������� �� ���.
        control_in[0], control_in[2],
        // ������� �������� �� ��� ������� (Y).
        650.0, 1100.0,
        // ���������� �������� �� ��� ������� � ��������� �� ���.
        650.0, 50.0);

    // ������ ������-����� ���� ��� ���������� ������.
    g.setrgb(0.7, 0.7, 0.7);
    // ������ ����� � ��������� ������ �� ����.
    g.grid(1, 1);

    // ������ ���� ��� ���������� ������ �� ����� �����������.
    // �����: �������� ����, ����������� � ����� (������ ��� ������ ����).
    g.color("fore");
    // ������ ���������.
    g.title();

    // ������ ������.
    g.color("red");
    g.curve(&xs[0], &ys[0], steps);

    // ��������� ������ ����������, ������ ���� ����� ���� ������� ���������.
    g.disfin();

    return 0;
}
