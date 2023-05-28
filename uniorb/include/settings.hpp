#pragma once

#include <string>
#include "satellite_parameters.hpp"

namespace uniorb {

struct settings {
public:
    // Полный путь к директории Russia2Constellation - директория с файлами пролетов спутников над территорией РФ.
    std::string russia_dir_path;

    // Полный путь к директории Facility2Constellation - директория с файлами видимостей спутников.
    std::string visibility_dir_path;
    
    // Полный путь к директории с выходными данными.
    std::string output_dir_path;
    
    // ТТХ спутника Зоркий
    satellite_parameters zorkiy_parameters;

    // ТТХ спутника Киноспутник
    satellite_parameters kinosat_parameters;
};

}