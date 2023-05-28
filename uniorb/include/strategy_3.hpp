#pragma once

#include "strategy.hpp"

namespace uniorb {

// 
class strategy_3 : public strategy {
public:
    strategy_3();

    // Метод возвращет ID спутника, который выбран для скачки данных станцией.
    // Если станция не скачивает данные со спутников - возвращает 0.
    virtual size_t get_observed_satellite(const settings & Settings, size_t StationID, propagator_data & Data) const override;
};

}