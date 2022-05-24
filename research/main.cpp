#include "plant.h"
#include "practice.h"
#include <discpp.h>
#include <cmath>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    // Инициализация ОУ.
    Plant plant;
    plant_init(plant);

    // Получение экспериментальных данных.
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

    // Объект для рисования графика.
    Dislin g;

    // Формат вывода, "cons" означает вывод на экран.
    g.metafl("PDF");
    // Включает режим светлого фона (по умолчанию наоборот).
    g.scrmod("revers");
    //g.setfil("dislin.png");
    // Инициализирует библиотеку, должно быть раньше других вызовов рисования.

    g.disini();
    // Задает подписи осей.
    g.name("Control in (U0)", "x");
    g.name("Control quality (Q)", "y");

    // Количество цифр после запятой по оси абсцисс: нет, только целая часть.
    g.labdig(-1, "x");

    // Количество промежуточных меток (между числами) на осях.
    g.ticks(1, "x");
    g.ticks(1, "y");

    // Задает заголовок графика.
    g.titlin("Plant parameter plot", 1);

    // Определяет цвет как смесь красного, зеленого и синего от 0 до 1 (RGB).
    // Здесь: все компоненты равны и близки к 1, т. е. цвет светло-серый.
    const int background_color = g.intrgb(0.95, 0.95, 0.95);

    // Использует ране определенный цвет для заливки фона.
    g.axsbgd(background_color);

    // Задает координатную сетку.
    g.graf(
        // Область значений по оси абсцисс (X).
        control_in[0], control_in[1],
        // Наименьшее значение на оси абсцисс и следующее за ним.
        control_in[0], control_in[2],
        // Область значений по оси ординат (Y).
        650.0, 1100.0,
        // Наименьшее значение на оси ординат и следующее за ним.
        650.0, 50.0);

    // Задает светло-серый цвет для следующего вызова.
    g.setrgb(0.7, 0.7, 0.7);
    // Рисует сетку с заданными шагами по осям.
    g.grid(1, 1);

    // Задает цвет для следующего вызова из числа стандартных.
    // Здесь: основной цвет, контрастный с фоном (черный для белого фона).
    g.color("fore");
    // Рисует заголовок.
    g.title();

    // Рисует данных.
    g.color("red");
    g.curve(&xs[0], &ys[0], steps);

    // Завершает работу библиотеки, должно быть после всех вызовов рисования.
    g.disfin();

    return 0;
}
