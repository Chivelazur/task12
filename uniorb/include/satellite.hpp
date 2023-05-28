#pragma once

#include <string>
#include "satellite_parameters.hpp"

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
    bool is_overloaded() const;
    double get_transferred_data() const;

private:
    // Автоматически присваемый ID при создании объекта. Начинается с 1.
    static size_t _id;

    // Объем использованной бортовой памяти в GB.
    double _used_storage;

    // Объем переданных данных в GB.
    double _transferred_data;
};

}