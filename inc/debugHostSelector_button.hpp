
#pragma once
#include "button_factory.hpp"
#include "button_interface.hpp"
#include "common.hpp"
#include "gpio.hpp"
#include "xyz/openbmc_project/Chassis/Buttons/DebugHostSelector/server.hpp"
#include "xyz/openbmc_project/Chassis/Common/error.hpp"

#include <unistd.h>

#include <phosphor-logging/elog-errors.hpp>

static constexpr std::string_view DEBUG_SELECTOR_BUTTON =
    "DEBUG_SELECTOR_BUTTON";

class DebugHostSelector final : public sdbusplus::server::object_t<
                                    sdbusplus::xyz::openbmc_project::Chassis::
                                        Buttons::server::DebugHostSelector>,
                                public ButtonIface

{
  public:
    DebugHostSelector(sdbusplus::bus::bus& bus, const char* path,
                      EventPtr& event, buttonConfig& buttonCfg) :
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Chassis::
                                        Buttons::server::DebugHostSelector>(
            bus, path, action::defer_emit),
        ButtonIface(bus, event, buttonCfg)
    {

        init();
        emit_object_added();
    }

    ~DebugHostSelector()
    {
        deInit();
    }

    void simPress() override;
    void simRelease() override;
    void handleEvent(sd_event_source* es, int fd, uint32_t revents) override;

    static constexpr std::string_view getFormFactorName()
    {
        return DEBUG_SELECTOR_BUTTON;
    }

    static const char* getDbusObjectPath()
    {
        return DBG_HS_DBUS_OBJECT_NAME;
    }
};
