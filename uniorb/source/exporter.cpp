#include "../include/exporter.hpp"
#include "../include/date.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>

namespace uniorb {

bool exporter::export_sessions(const std::string & FullDirPath, const propagator_data & Data) {
    std::cout << "exporter::export_sessions. Start writing session files to " << FullDirPath << std::endl;
    clock_t timer = std::clock();

    // Записываем сессии по каждой станции
    for (const auto & KV : Data.stations) {
        auto & Station = KV.second;
        auto FullPath = FullDirPath + "\\Ground_" + Station.name + ".txt";
        auto Fout = std::ofstream(FullPath);

        if (!Fout.is_open() || !Fout.good()) {
            std::cout << "exporter::export_sessions. Unable to write file " << FullPath << std::endl;
            return false;
        }
        Fout << Station.name << std::endl;
        Fout << "-------------------------" << std::endl;
        Fout << "    Access    *     Start Time (UTCG)     *     Stop Time (UTCG)  *   Duration (s)        *      Sat name *   Data (Mbytes)" << std::endl;
        int i = 1;
        Fout.precision(3);
        for (const auto & Session : Station.sessions) {
            Fout << std::setfill(' ') << std::setw(10) << i++ << "   " 
                 << date(Session.start_date).get_date_2(3) << "   "
                 << date(Session.end_date).get_date_2(3) << "   "
                 << std::setfill(' ') << std::setw(15) << std::fixed << (Session.end_date - Session.start_date) * 86400. << "   "
                 << std::setfill(' ') << std::setw(20) << Data.satellites.at(Session.satellite_id).name << "   "
                 << std::setfill(' ') << std::setw(15) << std::fixed << Session.passed_data * 1000 << std::endl;
        }

        Fout.close();
    }
    
    std::cout << "exporter::export_sessions. Finish writing sessions files to " << FullDirPath << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

bool exporter::export_drops(const std::string & FullDirPath, const propagator_data & Data) {
    std::cout << "exporter::export_drops. Start writing drop files to " << FullDirPath << std::endl;
    clock_t timer = std::clock();

    // Записываем сессии по каждой станции
    for (const auto & KV : Data.satellites) {
        auto & Satellite = KV.second;
        auto FullPath = FullDirPath + "\\Drop_" + Satellite.name + ".txt";
        auto Fout = std::ofstream(FullPath);

        if (!Fout.is_open() || !Fout.good()) {
            std::cout << "exporter::export_drops. Unable to write file " << FullPath << std::endl;
            return false;
        }
        Fout << Satellite.name << std::endl;
        Fout << "-------------------------" << std::endl;
        Fout << "    Access    *     Start Time (UTCG)     *     Stop Time (UTCG)  *   Duration (s)        *       Station  *  Data (Mbytes)" << std::endl;
        int i = 1;
        Fout.precision(3);
        for (const auto & Session : Satellite.sessions) {
            Fout << std::setfill(' ') << std::setw(10) << i++ << "   " 
                 << date(Session.start_date).get_date_2(3) << "   "
                 << date(Session.end_date).get_date_2(3) << "   "
                 << std::setfill(' ') << std::setw(15) << std::fixed << (Session.end_date - Session.start_date) * 86400. << "   "
                 << std::setfill(' ') << std::setw(20) << Data.stations.at(Session.station_id).name << "   "
                 << std::setfill(' ') << std::setw(15) << std::fixed << Session.passed_data * 1000 << std::endl;
        }

        Fout.close();
    }
    
    std::cout << "exporter::export_drops. Finish writing drop files to " << FullDirPath << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

bool exporter::export_camera(const std::string & FullDirPath, const propagator_data & Data) {
    std::cout << "exporter::export_camera. Start writing camera files to " << FullDirPath << std::endl;
    clock_t timer = std::clock();

    // Записываем сессии по каждой станции
    for (const auto & KV : Data.satellites) {
        auto & Satellite = KV.second;
        auto FullPath = FullDirPath + "\\Camera_" + Satellite.name + ".txt";
        auto Fout = std::ofstream(FullPath);

        if (!Fout.is_open() || !Fout.good()) {
            std::cout << "exporter::export_drops. Unable to write file " << FullPath << std::endl;
            return false;
        }
        Fout << Satellite.name << std::endl;
        Fout << "-------------------------" << std::endl;
        Fout << "    Access    *     Start Time (UTCG)     *     Stop Time (UTCG)  *   Duration (s) *   Data (Mbytes)" << std::endl;
        int i = 1;
        Fout.precision(3);
        for (const auto & Session : Satellite.camera) {
            Fout << std::setfill(' ') << std::setw(10) << i++ << "   " 
                 << date(Session.start_date).get_date_2(3) << "   "
                 << date(Session.end_date).get_date_2(3) << "   "
                 << std::setfill(' ') << std::setw(15) << std::fixed << (Session.end_date - Session.start_date) * 86400. << "   "
                 << std::setfill(' ') << std::setw(15) << std::fixed << Session.passed_data * 1000 << std::endl;
        }

        Fout.close();
    }
    
    std::cout << "exporter::export_camera. Finish writing camera files to " << FullDirPath << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

bool exporter::export_metrics(const std::string & FullDirPath, const propagator_data & Data) {
    std::cout << "exporter::export_metrics. Start writing metrics to " << FullDirPath << std::endl;
    clock_t timer = std::clock();

    auto FullPath = FullDirPath + "\\metrics.txt";
    auto Fout = std::ofstream(FullDirPath + "\\metrics.txt");

    if (!Fout.is_open() || !Fout.good()) {
        std::cout << "exporter::export_metrics. Unable to write file " << FullPath << std::endl;
        return false;
    }
    double MaxData = 0;
    Fout << "                    Date             Satname               Event Loading sats    Free sats     Finished   Overloaded     Used, GB   Passed, GB      Max, GB" << std::endl;
    for (const auto & KV : Data.metrics) {
        auto & Event = KV.first;
        auto & Metrica = KV.second;
        MaxData += Metrica.max_data;
        Fout.precision(2);
        Fout << event_to_string(Event, Data) << "   "
             << std::setfill(' ') << std::setw(10) << Metrica.satellite_loading_count << "   "
             << std::setfill(' ') << std::setw(10) << Metrica.satellite_free_count  << "   "
             << std::setfill(' ') << std::setw(10) << Metrica.satellite_memory_empty_count << "   "
             << std::setfill(' ') << std::setw(10) << Metrica.satellite_overloaded_count << "   "
             << std::setfill(' ') << std::setw(10) << std::fixed << Metrica.used_total << "   "
             << std::setfill(' ') << std::setw(10) << std::fixed << Metrica.passed_data << "   " 
             << std::setfill(' ') << std::setw(10) << std::fixed << MaxData << "   " << std::endl;
    }

    Fout.close();
    std::cout << "exporter::export_metrics. Finish writing metrics to " << FullDirPath << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

bool exporter::export_events(const std::string & FullDirPath, const std::vector<event> & Events, const propagator_data & Data) {
    std::cout << "exporter::export_events. Start writing events to " << FullDirPath << std::endl;
    clock_t timer = std::clock();

    auto FullPath = FullDirPath + "\\events.txt";
    auto Fout = std::ofstream(FullDirPath + "\\events.txt");

    if (!Fout.is_open() || !Fout.good()) {
        std::cout << "exporter::export_events. Unable to write file " << FullPath << std::endl;
        return false;
    }

    for (const auto & Event : Events) {
        Fout << event_to_string(Event, Data) << std::endl;
    }

    Fout.close();
    std::cout << "exporter::export_events. Finish writing events to " << FullDirPath << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

bool exporter::export_satellite_stats(const std::string & FullDirPath, const propagator_data & Data) {
    std::cout << "exporter::export_satellite_stats. Start writing satellite stats to " << FullDirPath << std::endl;
    clock_t timer = std::clock();

    auto FullPath = FullDirPath + "\\satellite_stats.txt";
    auto Fout = std::ofstream(FullDirPath + "\\satellite_stats.txt");

    if (!Fout.is_open() || !Fout.good()) {
        std::cout << "exporter::export_satellite_stats. Unable to write file " << FullPath << std::endl;
        return false;
    }

    Fout << "       Satname        Passed, GB     Camera, GB        Max, GB      Overloads         Input, GB/s        Output, GB/s    Storage, GB" << std::endl;
    for (const auto & KV : Data.satellites) {
        auto & Satellite = KV.second;
        Fout.precision(2);
        Fout << Satellite.name << "   "
             << std::setfill(' ') << std::setw(15) << std::fixed << Satellite.get_transferred_data()
             << std::setfill(' ') << std::setw(15) << std::fixed << Satellite.get_camera_data()
             << std::setfill(' ') << std::setw(15) << std::fixed << Satellite.theormax_data
             << std::setfill(' ') << std::setw(15) << std::fixed << Satellite.get_overload_count();
        Fout.precision(5);
        Fout << std::setfill(' ') << std::setw(20) << std::fixed << Satellite.parameters.input_rate
             << std::setfill(' ') << std::setw(20) << std::fixed << Satellite.parameters.output_rate;
        Fout.precision(0);
             Fout << std::setfill(' ') << std::setw(15) << std::fixed << Satellite.parameters.storage << std::endl;
    }

    Fout.close();
    std::cout << "exporter::export_satellite_stats. Finish writing satellite stats to " << FullDirPath << " in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

std::string exporter::event_to_string(const event & Event, const propagator_data & Data) {
	std::stringstream str;
	str << date(Event.epoch).get_date_2(3) << std::setfill(' ') << std::setw(20) << Data.satellites.at(Event.satellite_id).name;

    if (Event.type == station_entrance) {
        str << std::setfill(' ') << std::setw(20) << "enter " + Data.stations.at(Event.station_id).name;
    }
    else if (Event.type == station_exit) {
        str << std::setfill(' ') << std::setw(20) << "exit " + Data.stations.at(Event.station_id).name;
    }
    else if (Event.type == russia_entrance) {
        str << std::setfill(' ') << std::setw(20) << "fly in Russia";
    }
    else if (Event.type == russia_exit) {
        str << std::setfill(' ') << std::setw(20) << "fly out Russia";
    }
    else if (Event.type == download_finish) {
        str << std::setfill(' ') << std::setw(20) << "finish download";
    }

    return str.str();
}

}