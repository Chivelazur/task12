#include "../include/event.hpp"

namespace uniorb {


bool event::compare_events(const event & A, const event & B) {
    return A.epoch < B.epoch;
}

}