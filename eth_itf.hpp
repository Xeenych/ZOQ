#pragma once

#include "w5500_driver.hpp"

template class <driver_t>
class eth_itf {
  public:
    struct settings_t {
        uint8_t shar[6];  // mac
        uint8_t sipr[4];  // ip
        uint8_t subr[4];  // subnet
        uint8_t gar[4];   // gateway
    };
    enum class state_t : uint8_t { W5500_NoChip, W5500_NoCable, W5500_Configured, W5500_Init };

    struct status_t {
        uint8_t HasChip : 1;
        uint8_t HasPhyLink : 1;
        uint8_t Is100Mbps : 1;
        uint8_t IsFullDuplex : 1;
    };

    eth_itf(driver_t& d, const settings_t& set);
    void SetSettings(const settings_t& settings);
    state_t Run();

  private:
    settings_t ReadSettings();
    void WriteSettings();
    driver_t& drv;
    settings_t settings;
};