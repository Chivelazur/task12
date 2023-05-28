#pragma once

#include "strategy.hpp"

namespace uniorb {

// Скачиваем данные со спутника, который передает данные быстрее всего.
class strategy_2 : public strategy {
public:
    strategy_2();

    // Метод возвращет ID спутника, который выбран для скачки данных станцией.
    // Если станция не скачивает данные со спутников - возвращает 0.
    virtual size_t get_observed_satellite(const settings & Settings, size_t StationID, propagator_data & Data) const override;
};

}