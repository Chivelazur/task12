#pragma once

#include <string>
#include <vector>
#include "satellite_parameters.hpp"
#include "session.hpp"

namespace uniorb {

struct satellite {
public:
    std::string name;
    size_t id;
    satellite_parameters parameters;

    // ID станции, на которую спутник скачивает данные (0 если не скачивает).
    size_t station_id = 0;
    // Признак, что спутник летит над Россией.
    bool is_russia = false;

    // Максимальный теоретический объем данных
    double theormax_data = 0;

    // Сеансы передачи данных на станции.
    std::vector<session> sessions = std::vector<session>();

    // Сеансы работы камеры.
    std::vector<session> camera = std::vector<session>();

    satellite(const std::string & Name, const satellite_parameters & Parameters);
    satellite(const satellite & Satellite);
    satellite(satellite && Satellite);
    satellite & operator=(const satellite & Satellite);
    satellite & operator=(satellite && Satellite);

    // Возвращает реально добавленный объем, с учетом, что есть лимит по памяти.
    double increase_storage(double Value);

    // Возвращает реально убавленный объем, с учетом, что не занятая память не может быть меньше 0.
    double decrease_storage(double Value);
    double get_used_storage() const;
    
    // Закончилась ли бортовая память
    bool is_overloaded() const;

    // Количество перезаполнений памяти
    size_t get_overload_count() const;

    // Объем переданных данных
    double get_transferred_data() const;

    // Объем снятых данных
    double get_camera_data() const;

    // Скачивает ли данные на станцию
    bool is_loading() const;

    // Ведет ли съемку
    bool is_camera() const;

    // Завершает текущий сеанс: ставит в 0 station_id, устанавливает конец сессии.
    // Пробует начать съемку, если есть память и летит над РФ.
    void finish_session(double EndMjd);

    // Стартует новый сеанс, если нет текущего.
    // Прерывает съемку, если она была.
    void start_session(double StartMjd, size_t StationID);

    // Начинает съемку, если память не переполнена и спутник летит над РФ.
    void start_camera(double StartMjd);

    // Завершает съемку, если память не переполнена и спутник летит над РФ.
    void finish_camera(double EndMjd);

private:
    // Автоматически присваемый ID при создании объекта. Начинается с 1.
    static size_t _id;

    // Объем использованной бортовой памяти в GB.
    double _used_storage;

    // Объем переданных данных в GB.
    double _transferred_data;

    // Объем отснятых данных в GB.
    double _camera_data;

    // Количество полных заполнений памяти
    size_t _overload_count;
};

}