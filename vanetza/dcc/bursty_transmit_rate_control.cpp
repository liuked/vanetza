#include "bursty_transmit_rate_control.hpp"
#include <stdexcept>

namespace vanetza
{
namespace dcc
{

BurstyTransmitRateControl::BurstyTransmitRateControl(const StateMachine& fsm, const Clock::time_point& clock) :
    m_burst_budget(clock), m_fsm_budget(fsm, clock), m_fsm(fsm)
{
}

Clock::duration BurstyTransmitRateControl::delay(Profile dp)
{
    Clock::duration delay = Clock::duration::max();

    switch (dp) {
        case Profile::DP0:
            delay = m_burst_budget.delay();
            break;
        case Profile::DP1:
        case Profile::DP2:
        case Profile::DP3:
            delay = m_fsm_budget.delay();
            break;
        default:
            throw std::invalid_argument("Invalid DCC Profile");
            break;
    };

    return delay;
}

Clock::duration BurstyTransmitRateControl::interval(Profile dp)
{
    Clock::duration interval = Clock::duration::max();

    switch (dp) {
        case Profile::DP0:
            interval = m_burst_budget.burst_period();
            break;
        case Profile::DP1:
        case Profile::DP2:
        case Profile::DP3:
            interval = m_fsm.transmission_interval();
            break;
        default:
            throw std::invalid_argument("Invalid DCC Profile");
            break;
    }

    return interval;
}

void BurstyTransmitRateControl::notify(Profile dp)
{
    switch (dp) {
        case Profile::DP0:
            m_burst_budget.notify();
            break;
        case Profile::DP1:
        case Profile::DP2:
        case Profile::DP3:
            m_fsm_budget.notify();
            break;
        default:
            throw std::invalid_argument("Invalid DCC Profile");
            break;
    };
}

} // namespace dcc
} // namespace vanetza