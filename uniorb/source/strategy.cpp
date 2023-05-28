#include "../include/strategy.hpp"

namespace uniorb {

strategy::strategy(const std::string & Name) : _name(Name) {}

std::string strategy::get_name() const {
    return _name;
}

}