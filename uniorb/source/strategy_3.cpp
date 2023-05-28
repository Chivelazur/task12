#include "../include/strategy_3.hpp"

namespace uniorb {

strategy_3::strategy_3() : strategy("strategy_3") {}

size_t strategy_3::get_observed_satellite(size_t StationID, propagator_data & Data) const {
    return 0;
}

}