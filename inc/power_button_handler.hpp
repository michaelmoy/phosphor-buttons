#pragma once

#include "config.h"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/bus/match.hpp>
#include <sdbusplus/message.hpp>

#include <string>

namespace phosphor::button
{

constexpr auto powerButtonInterface =
    "xyz.openbmc_project.Chassis.Buttons.Power";
namespace sdbusRule = sdbusplus::bus::match::rules;

/**
 * @class PowerButtonHandler
 *
 * Abstract base class for custom power button handlers.
 *
 * Calls a derived class's pressed() and released()
 * functions when the power button is pressed and
 * released.
 */
class PowerButtonHandler
{
  public:
    PowerButtonHandler(sdbusplus::bus_t& bus) :
        bus(bus),
        pressedMatch(bus,
                     sdbusRule::type::signal() + sdbusRule::member("Pressed") +
                         sdbusRule::path(POWER_DBUS_OBJECT_NAME) +
                         sdbusRule::interface(powerButtonInterface),
                     std::bind(&PowerButtonHandler::pressedHandler, this,
                               std::placeholders::_1)),
        releasedMatch(bus,
                      sdbusRule::type::signal() +
                          sdbusRule::member("Released") +
                          sdbusRule::path(POWER_DBUS_OBJECT_NAME) +
                          sdbusRule::interface(powerButtonInterface),
                      std::bind(&PowerButtonHandler::releasedHandler, this,
                                std::placeholders::_1))
    {}

    virtual ~PowerButtonHandler() = default;

    void pressedHandler(sdbusplus::message_t /* msg*/)
    {
        pressed();
    }

    void releasedHandler(sdbusplus::message_t msg)
    {
        auto time = msg.unpack<uint64_t>();
        released(time);
    }

    virtual void pressed() = 0;

    virtual void released(uint64_t pressTimeMS) = 0;

  protected:
    sdbusplus::bus_t& bus;

  private:
    sdbusplus::bus::match_t pressedMatch;
    sdbusplus::bus::match_t releasedMatch;
};

} // namespace phosphor::button
