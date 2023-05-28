#pragma once

#include <optional>
#include "settings.hpp"
#include "propagator_data.hpp"
#include "strategy.hpp"

namespace uniorb {

class propagator {
public:
    propagator() = default;
    void propagate(const settings & Settings, const std::vector<event> & Events, const strategy & Strategy, propagator_data & Data) const;

protected:
    void _process_event(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const;
    void _process_event_station_entrance(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const;
    void _process_event_station_exit(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const;
    void _process_event_russia_entrance(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const;
    void _process_event_russia_exit(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const;
    void _process_event_download_finish(const settings & Settings, const event & Event, propagator_data & Data, const strategy & Strategy) const;
    metrica _update_storage(const event & CurrentEvent, const event & PreviousEvent, propagator_data & Data) const;
    std::optional<event> _predict_download_finish_event(const event & CurrentEvent, const event & NextEvent, propagator_data & Data) const;

};

}