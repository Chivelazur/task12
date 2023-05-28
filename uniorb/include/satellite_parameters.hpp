#pragma once

namespace uniorb {

struct satellite_parameters {
public:
    // Скорость съемки, GB/s
    double input_rate;

    // Скорость передачи данных, GB/s
    double output_rate;
    
    // Объем бортовой памяти, GB
    double storage;
};

}