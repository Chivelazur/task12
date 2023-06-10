#include "../include/propagator.hpp"
#include "../include/exporter.hpp"


#include <algorithm>
#include <set>
#include <iostream>

namespace uniorb {

void propagator::propagate(const settings & Settings, const std::vector<event> & Events, const strategy & Strategy, propagator_data & Data) const {
    // Обрабатываем первое событие.
    if (!Events.empty()) _process_event(Settings, Events[0], Data, Strategy);

    // Последовательно обрабатываем все остальные события
    for (size_t i = 1; i < Events.size(); ++i) {
        // Обновляем данные бортовой памяти согласно Алгоритму 1.
        auto Metrica = _update_storage(Events[i], Events[i-1], Data);
        Data.metrics.push_back({Events[i], Metrica});

        // Обрабатываем событие.
        _process_event(Settings, Events[i], Data, Strategy);
    }
}

void propagator::_process_event(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const {
    if (Event.type == station_entrance) {
        _process_event_station_entrance(Settings, Event, Data, Strategy);
    }
    else if (Event.type == station_exit) {
        _process_event_station_exit(Settings, Event, Data, Strategy);
    }
    else if (Event.type == russia_entrance) {
        _process_event_russia_entrance(Settings, Event, Data, Strategy);
    }
    else if (Event.type == russia_exit) {
        _process_event_russia_exit(Settings, Event, Data, Strategy);
    }
    // else if (Event.type == download_finish) {
    //     _process_event_download_finish(Settings, Event, Data, Strategy);
    // }
}

void propagator::_process_event_station_entrance(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const {
    // Синтаксический сахар
    auto & Station = Data.stations.at(Event.station_id);
    auto SatelliteID = Event.satellite_id;

    // Добавляем спутник в зону видимости станции.
    Station.visible_satellites.insert(SatelliteID);

    // Если спутник в данный момент скачивает данные на другую станцию, то обработка не требуется:
    if (Data.satellites.at(SatelliteID).is_loading()) return;

    // Получаем ID спутника, данные с которого должна скачивать станция (может быть 0, то есть не скачиваем).
    auto NewSatelliteID = Strategy.get_observed_satellite(Settings, Station.id, Data);

    // Если меняется ID спутника, то необходимо обновить сессии.
    if (NewSatelliteID != Station.satellite_id) {
        // Если станция наблюдала некий спутник, то необходимо завершить сессию с ним.
        if (Station.is_loading()) {
            // Этот некий спутник больше не скачивает данные и можем начать съемку
            auto & OldSatellite = Data.satellites.at(Station.satellite_id);
            OldSatellite.finish_session(Event.epoch);
            if (OldSatellite.is_camera()) OldSatellite.start_camera(Event.epoch);
            // Завершаем сессию
            Station.finish_session(Event.epoch);
        }

        // Если скачиваем данные с нового спутника (ID != 0), создаем новую сессию
        if (NewSatelliteID != 0) {
            // Создаем сессию у станции для нового спутника.
            Station.start_session(Event.epoch, NewSatelliteID);
            // Создаем сессию для спутника (важно - get_observed_satellite не возвращает спутники, которые уже скачивают)
            // И останавливаем запись, если она была.
            auto & NewSatellite = Data.satellites.at(NewSatelliteID);
            if (NewSatellite.is_camera()) NewSatellite.finish_camera(Event.epoch);
            NewSatellite.start_session(Event.epoch, Station.id);
        }
    }
}

void propagator::_process_event_station_exit(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const {
    // Синтаксический сахар
    auto & Station = Data.stations.at(Event.station_id);
    auto SatelliteID = Event.satellite_id;

    // Убираем спутник из зоны видимости станции.
    Station.visible_satellites.erase(SatelliteID);

    // Получаем ID спутника, данные с которого должна скачивать станция (может быть 0, то есть не скачиваем).
    auto NewSatelliteID = Strategy.get_observed_satellite(Settings, Station.id, Data);

    // Если меняется ID спутника, то необходимо обновить сессии.
    if (NewSatelliteID != Station.satellite_id) {
        // Если станция наблюдала некий спутник, то необходимо завершить сессию с ним.
        if (Station.is_loading()) {
            // Этот некий спутник больше не скачивает данные.
            auto & OldSatellite = Data.satellites.at(Station.satellite_id);
            OldSatellite.finish_session(Event.epoch);
            if (OldSatellite.is_camera()) OldSatellite.start_camera(Event.epoch);
            // Завершаем сессию
            Station.finish_session(Event.epoch);
        }

        // Если скачиваем данные с нового спутника (ID != 0), создаем новую сессию
        if (NewSatelliteID != 0) {
            // Создаем сессию у станции для нового спутника.
            Station.start_session(Event.epoch, NewSatelliteID);
            // Создаем сессию для спутника (важно - get_observed_satellite не возвращает спутники, которые уже скачивают)
            // И останавливаем запись, если она была.
            auto & NewSatellite = Data.satellites.at(NewSatelliteID);
            if (NewSatellite.is_camera()) NewSatellite.finish_camera(Event.epoch);
            NewSatellite.start_session(Event.epoch, Station.id);
        }
    }
}

void propagator::_process_event_russia_entrance(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const {
    auto & Satellite = Data.satellites.at(Event.satellite_id);
    // Помечаем, что летим над РФ
    Satellite.is_russia = true;
    // Если можем снимать - начинаем
    if (Satellite.is_camera()) Satellite.start_camera(Event.epoch);
}

void propagator::_process_event_russia_exit(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const {
    auto & Satellite = Data.satellites.at(Event.satellite_id);
    // Если вели съемку - завершаем
    if (Satellite.is_camera()) Satellite.finish_camera(Event.epoch);
    // Помечаем, что не летим над РФ
    Satellite.is_russia = false;
}

/*
void propagator::_process_event_download_finish(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const {
    // Синтаксический сахар
    auto & Station = Data.stations.at(Event.station_id);
    auto SatelliteID = Event.satellite_id;

    // Получаем ID спутника, данные с которого должна скачивать станция (он всегда есть на этом шаге).
    auto NewSatelliteID = Strategy.get_observed_satellite(Settings, Event.station_id, Data);

    // Если меняется ID спутника, то необходимо обновить сессии.
    if (NewSatelliteID != Station.satellite_id) {
        // Если станция наблюдала спутник, то необходимо завершить сессию с ним.
        if (Station.satellite_id != 0) {
            Station.sessions.back().end_date = Event.epoch;
            // Старый спутник больше не скачивает данные.
            Data.satellites.at(Station.satellite_id).station_id = 0;
            Data.satellites.at(Station.satellite_id).sessions.back().end_date = Event.epoch;
        }

        // Обновляем ID спутника, с которого скачивает данные станция (может быть 0 - данные не скачиваем)
        Station.satellite_id = NewSatelliteID;

        // Если скачиваем данные с нового спутника (ID != 0), создаем новую сессию
        if (NewSatelliteID != 0) {
            // Создаем сессию для нового спутника.
            Station.sessions.push_back(session(NewSatelliteID, Station.id, Event.epoch));
            // Указываем, что спутник теперь передает данные на эту станцию и создаем сессию у него.
            auto & Satellite = Data.satellites.at(NewSatelliteID);
            Satellite.station_id = Station.id;
            Satellite.sessions.push_back(session(NewSatelliteID, Station.id, Event.epoch));
        }
    }
}
*/

metrica propagator::_update_storage(const event & CurrentEvent, const event & PreviousEvent, propagator_data & Data) const {
    // Готовим выходные метрики
    auto Metrica = metrica();

    // Рассчитываем дельту времени между текущим и предыдущим событием в секундах.
    double T = (CurrentEvent.epoch - PreviousEvent.epoch) * 86400.;
    
    for (auto & KV : Data.satellites) {
        auto & Satellite = KV.second;

        auto StorageIncrease = Satellite.parameters.input_rate * T;
        if (Satellite.is_russia) {
            // Считаем теор. максимум
            Metrica.max_data += StorageIncrease;
        }

        // Для всех спутников, с которых не ведется скачка данных.
        if (Satellite.station_id == 0) {
            // Если спутник находится над территорией Россией, значит он мог вести съемку.
            if (Satellite.is_camera()) {
                // Добавляем занятый объем бортовой памяти
                Satellite.increase_storage(StorageIncrease);
                // Если память закончилась - завершаем съемку.
                if (Satellite.is_overloaded()) Satellite.finish_camera(CurrentEvent.epoch);
            }

            // Считаем число спутников, которые не скачивают данные.
            Metrica.satellite_free_count++;
        }

        // Для всех спутников, с которых ведется скачка данных.
        else {
            auto StorageDecrease = Satellite.parameters.output_rate * T;
            auto UsedStorage = Satellite.get_used_storage();

            // Убавляем занятый объем бортовой памяти
            Satellite.decrease_storage(StorageDecrease);

            // Добавляем скачанный объем в метрику.
            // Если скачали не все данные.
            if (UsedStorage > StorageDecrease) {
                // Обновляем сеансы.
                Data.stations.at(Satellite.station_id).sessions.back().passed_data += StorageDecrease;
                Data.satellites.at(Satellite.id).sessions.back().passed_data += StorageDecrease;
            }
            // Если скачали все данные.
            else {
                // Обновляем сеансы.
                Data.stations.at(Satellite.station_id).sessions.back().passed_data += UsedStorage;
                Data.satellites.at(Satellite.id).sessions.back().passed_data += UsedStorage;
                // Считаем количество спутников, с которых полностью скачали данные.
                Metrica.satellite_memory_empty_count++;
            }

            // Считаем число спутников, которые скачивают данные.
            Metrica.satellite_loading_count++;
        }

        // Считаем суммарный объем занятой бортовой памяти спутников, переданных данных и количество спутников с заполненной бортовой памятью
        Metrica.used_total += Satellite.get_used_storage();
        Metrica.passed_data += Satellite.get_transferred_data();
        if (Satellite.is_overloaded()) Metrica.satellite_overloaded_count++;
    }

    return Metrica;
}

std::optional<event> propagator::_predict_download_finish_event(const event & CurrentEvent, const event & NextEvent, propagator_data & Data) const {
    // Рассчитываем дельту времени между следующим и текущим событием в секундах.
    double T = (NextEvent.epoch - CurrentEvent.epoch) * 86400.;
    if (T == 0) return std::nullopt;
    double T0 = T;
    size_t SatelliteID = 0;

    for (auto & KV : Data.satellites) {
        auto & Satellite = KV.second;
        // Для всех спутников, с которых ведется скачка данных.        
        if (Satellite.station_id != 0) {
            auto StorageDecrease = Satellite.parameters.output_rate * T;
            auto UsedStorage = Satellite.get_used_storage();

            // Если скачаем больше, чем есть на спутнике, необходимо определить момент времени
            if (StorageDecrease > UsedStorage) {
                T0 = std::min(T0, T * UsedStorage / StorageDecrease);
                SatelliteID = Satellite.id;
            }
        }
    }

    // Если такое событие есть - возвращаем его.
    if (SatelliteID != 0) {
        auto Event = event();
        Event.epoch = CurrentEvent.epoch + T0 / 86400.;
        Event.type = download_finish;
        Event.satellite_id = SatelliteID;
        return Event;
    }

    // Если нет - не возвращаем.
    return std::nullopt;
}

}