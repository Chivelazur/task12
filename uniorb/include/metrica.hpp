#pragma once

namespace uniorb {

struct metrica {
public:
    // Суммарный объем занятой бортовой памяти по всем спутникам.
    double used_total = 0;

    // Cуммарный объем данных, переданных со всех спутников на все станции.
    double passed_data = 0;

    // Количество спутников, у которых полностью заполнена память.
    int satellite_overloaded_count = 0;

    // Количество спутников, у которых полностью скачали все данные.
    int satellite_memory_empty_count = 0;

    // Количество спутников, с которых ведется скачка.
    int satellite_loading_count = 0;

    // Количество спутников, с которых не ведется скачка.
    int satellite_free_count = 0;
};

}