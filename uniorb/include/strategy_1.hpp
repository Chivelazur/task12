#pragma once

#include "strategy.hpp"

namespace uniorb {

// Скачиваем данные со спутника с максимально заполненным объемом бортовой памяти.
class strategy_1 : public strategy {
public:
    strategy_1();

    // Метод возвращет ID спутника, который выбран для скачки данных станцией.
    // Если станция не скачивает данные со спутников - возвращает 0.
    virtual size_t get_observed_satellite(size_t StationID, propagator_data & Data) const override;
};

}