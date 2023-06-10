#include "../include/uniorb.hpp"
#include <iostream>
#include <filesystem>

using namespace uniorb;



void launch(const settings & Settings, const std::vector<event> & Events, const strategy & Strategy, propagator_data & Data) {
    propagator Propagator = propagator();

    // Считаем.
    std::cout << "Start computing with " << Strategy.get_name() << std::endl;
    clock_t timer = std::clock();
    Propagator.propagate(Settings, Events, Strategy, Data);
    std::cout << "Finish computing with " << Strategy.get_name() << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;

    // Выводим данные.
    auto Path = Settings.output_dir_path + "\\" + Strategy.get_name();
    std::filesystem::create_directory(Path);
    std::filesystem::create_directory(Path + "\\Ground");
    std::filesystem::create_directory(Path + "\\Drop");
    std::filesystem::create_directory(Path + "\\Camera");
    exporter::export_sessions(Path + "\\Ground", Data);
    exporter::export_drops(Path + "\\Drop", Data);
    exporter::export_camera(Path + "\\Camera", Data);
    exporter::export_metrics(Path, Data);
    exporter::export_satellite_stats(Path, Data);

    // Подводим итог по ключевым метрикам алгоритма - GB переданных данных и количество секунд до первого заполнения памяти спутника.
    double FirstMJD = Data.metrics.begin()->first.epoch;
    double LastMJD =  Data.metrics.back().first.epoch;
    for (auto & KV : Data.metrics) {
        if (KV.second.satellite_overloaded_count != 0) {
            LastMJD = KV.first.epoch;
            break;
        }
    }
    double MinutesToFill = (LastMJD - FirstMJD) * 1440.;
    double TransferredGB = Data.metrics.back().second.passed_data;
    std::cout << "Total score of " << Strategy.get_name() << std::endl;
    std::cout << "          Minutes to the first satellite overload = " << MinutesToFill << std::endl;
    std::cout << "                     Total transferred data in GB = " << TransferredGB << std::endl;
    std::cout << "                                              Sum = " << TransferredGB + MinutesToFill << std::endl;
}

/*
Вход:
    1 аргумент - полный путь к директории с файлами пролетов над Россией.
    2 аргумент - полный путь к директории с файлами видимостей спутников над станциями.
    3 аргумент - полный путь к директории с выходными файлами (Results).
*/
int main(int argc, char *argv[]) {
    // Проверка
    if (argc < 3)
    {
        std::cout << "Path to data directories are required.";
        return 1;
    }
    
    // Исходные параметры спутников
    auto KinosatParams = satellite_parameters();
    KinosatParams.input_rate = 0.5; // GB/s
    KinosatParams.output_rate = 0.125; // GB/s
    KinosatParams.storage = 1000; // 1 TB = 1000 GB

    auto ZorkiyParams = satellite_parameters();
    ZorkiyParams.input_rate = 0.5; // GB/s
    ZorkiyParams.output_rate = 0.03125; // GB/s
    ZorkiyParams.storage = 500; // 0.5 TB = 500 GB

    // Задаем параметры считывания settings:
    settings Settings = settings();
    Settings.russia_dir_path = argv[1];
    Settings.visibility_dir_path = argv[2];
    Settings.output_dir_path = argv[3];
    Settings.kinosat_parameters = KinosatParams;
    Settings.zorkiy_parameters = ZorkiyParams;

    // Считываем исходные данные
    auto Events = std::vector<event>();
    auto Satellites = std::vector<satellite>();
    auto Stations = std::vector<station>();
    importer::import_data(Settings, Events, Satellites, Stations);

    // Выводим для проверки массив всех событий, упорядоченный
    propagator_data Data = propagator_data(Satellites, Stations);
    exporter::export_events(Settings.output_dir_path, Events, Data);

    // Выбираем стратегию обработки событий и выполнем обработку.
    auto Strategy1 = strategy_1();
    Data = propagator_data(Satellites, Stations);
    launch(Settings, Events, Strategy1, Data);
    
    auto Strategy2 = strategy_2();
    Data = propagator_data(Satellites, Stations);
    launch(Settings, Events, Strategy2, Data);

    auto Strategy3 = strategy_3();
    Data = propagator_data(Satellites, Stations);
    launch(Settings, Events, Strategy3, Data);

    auto Strategy4 = strategy_4();
    Data = propagator_data(Satellites, Stations);
    launch(Settings, Events, Strategy4, Data);

    return 0;
}