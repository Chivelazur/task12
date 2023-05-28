#pragma once

#include "propagator_data.hpp"
#include "settings.hpp"

namespace uniorb {

class exporter {
public:
    exporter() = delete;
    /*
    Вход:
        FullDitPath - полный путь к директории, куда запишутся файлы.
        Data - данные пропагатора.
    Выход:
        true - если выгрузка успешна.
        false - если ошибочна.
    Результат работы функции - файлы сеансов, созданные в директории Settings.output_dir_path
    */
    static bool export_sessions(const std::string & FullDirPath, const propagator_data & Data);

    /*
    Вход:
        FullDitPath - полный путь к директории, куда запишутся файлы.
        Data - данные пропагатора.
    Выход:
        true - если выгрузка успешна.
        false - если ошибочна.
    Результат работы функции - файл с контрольными метриками, созданные в директории Settings.output_dir_path
    */
    static bool export_metrics(const std::string & FullDirPath, const propagator_data & Data);

    /*
    Вход:
        FullDitPath - полный путь к директории, куда запишутся файлы.
        Data - данные пропагатора.
    Выход:
        true - если выгрузка успешна.
        false - если ошибочна.
    Результат работы функции - файл со статистикой переданного объема данных каждым спутником, созданные в директории Settings.output_dir_path
    */
    static bool export_satellite_stats(const std::string & FullDirPath, const propagator_data & Data);

    /*
    Вход:
        FullDitPath - полный путь к директории, куда запишутся файлы.
        Events - упорядоченный массив событий.
        Data - данные пропагатора.
    Выход:
        true - если выгрузка успешна.
        false - если ошибочна.
    Результат работы функции - файл с контрольными метриками, созданные в директории Settings.output_dir_path
    */
    static bool export_events(const std::string & FullDirPath, const std::vector<event> & Events, const propagator_data & Data);

    /* Формирует строку для вывода на основе данных события. */
    static std::string event_to_string(const event & Event, const propagator_data & Data);

};

}