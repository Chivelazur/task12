#pragma once

#include "event.hpp"
#include "propagator_data.hpp"

namespace uniorb {

// Базовый абстрактный класс для стратегий выбора спутника, с которого нужна скачивать данные.
class strategy {
public:
    strategy(const std::string & Name);
    std::string get_name() const;

    // Метод возвращет ID спутника, который выбран для скачки данных станцией.
    // Если станция не скачивает данные со спутников - возвращает 0.
    virtual size_t get_observed_satellite(size_t StationID, propagator_data & Data) const = 0;

private:
    std::string _name;
    
};

}