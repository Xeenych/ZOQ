#pragma once

namespace w5500 {

template <class driver_t>
class w5500_itf {
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

    inline w5500_itf(driver_t& d, const settings_t& set);
    void SetSettings(const settings_t& settings);
    void Run();

  private:
    settings_t ReadSettings();
    void WriteSettings();
    driver_t& drv;
    settings_t settings;
};

template <class driver_t>
w5500_itf<driver_t>::w5500_itf(driver_t& d, const settings_t& set) : drv(d), settings(set)
{
}

template <class driver_t>
w5500_itf<driver_t>::settings_t w5500_itf<driver_t>::ReadSettings()
{
    settings_t ret;
    ret.shar[0] = drv.ReadByte(W5500_ADR_SHAR + 0, W5500_BSB_COMMON);
    ret.shar[1] = drv.ReadByte(W5500_ADR_SHAR + 1, W5500_BSB_COMMON);
    ret.shar[2] = drv.ReadByte(W5500_ADR_SHAR + 2, W5500_BSB_COMMON);
    ret.shar[3] = drv.ReadByte(W5500_ADR_SHAR + 3, W5500_BSB_COMMON);
    ret.shar[4] = drv.ReadByte(W5500_ADR_SHAR + 4, W5500_BSB_COMMON);
    ret.shar[5] = drv.ReadByte(W5500_ADR_SHAR + 5, W5500_BSB_COMMON);

    ret.subr[0] = drv.ReadByte(W5500_ADR_SUBR + 0, W5500_BSB_COMMON);
    ret.subr[1] = drv.ReadByte(W5500_ADR_SUBR + 1, W5500_BSB_COMMON);
    ret.subr[2] = drv.ReadByte(W5500_ADR_SUBR + 2, W5500_BSB_COMMON);
    ret.subr[3] = drv.ReadByte(W5500_ADR_SUBR + 3, W5500_BSB_COMMON);

    ret.gar[0] = drv.ReadByte(W5500_ADR_GAR + 0, W5500_BSB_COMMON);
    ret.gar[1] = drv.ReadByte(W5500_ADR_GAR + 1, W5500_BSB_COMMON);
    ret.gar[2] = drv.ReadByte(W5500_ADR_GAR + 2, W5500_BSB_COMMON);
    ret.gar[3] = drv.ReadByte(W5500_ADR_GAR + 3, W5500_BSB_COMMON);

    ret.sipr[0] = drv.ReadByte(W5500_ADR_SIPR + 0, W5500_BSB_COMMON);
    ret.sipr[1] = drv.ReadByte(W5500_ADR_SIPR + 1, W5500_BSB_COMMON);
    ret.sipr[2] = drv.ReadByte(W5500_ADR_SIPR + 2, W5500_BSB_COMMON);
    ret.sipr[3] = drv.ReadByte(W5500_ADR_SIPR + 3, W5500_BSB_COMMON);
    return ret;
}

template <class driver_t>
void w5500_itf<driver_t>::SetSettings(const w5500_itf<driver_t>::settings_t& set)
{
    settings = set;
}

template <class driver_t>
void w5500_itf<driver_t>::WriteSettings()
{
    drv.WriteByte(W5500_ADR_SHAR + 0, W5500_BSB_COMMON, settings.shar[0]);
    drv.WriteByte(W5500_ADR_SHAR + 1, W5500_BSB_COMMON, settings.shar[1]);
    drv.WriteByte(W5500_ADR_SHAR + 2, W5500_BSB_COMMON, settings.shar[2]);
    drv.WriteByte(W5500_ADR_SHAR + 3, W5500_BSB_COMMON, settings.shar[3]);
    drv.WriteByte(W5500_ADR_SHAR + 4, W5500_BSB_COMMON, settings.shar[4]);
    drv.WriteByte(W5500_ADR_SHAR + 5, W5500_BSB_COMMON, settings.shar[5]);

    drv.WriteByte(W5500_ADR_SUBR + 0, W5500_BSB_COMMON, settings.subr[0]);
    drv.WriteByte(W5500_ADR_SUBR + 1, W5500_BSB_COMMON, settings.subr[1]);
    drv.WriteByte(W5500_ADR_SUBR + 2, W5500_BSB_COMMON, settings.subr[2]);
    drv.WriteByte(W5500_ADR_SUBR + 3, W5500_BSB_COMMON, settings.subr[3]);

    drv.WriteByte(W5500_ADR_GAR + 0, W5500_BSB_COMMON, settings.gar[0]);
    drv.WriteByte(W5500_ADR_GAR + 1, W5500_BSB_COMMON, settings.gar[1]);
    drv.WriteByte(W5500_ADR_GAR + 2, W5500_BSB_COMMON, settings.gar[2]);
    drv.WriteByte(W5500_ADR_GAR + 3, W5500_BSB_COMMON, settings.gar[3]);

    drv.WriteByte(W5500_ADR_SIPR + 0, W5500_BSB_COMMON, settings.sipr[0]);
    drv.WriteByte(W5500_ADR_SIPR + 1, W5500_BSB_COMMON, settings.sipr[1]);
    drv.WriteByte(W5500_ADR_SIPR + 2, W5500_BSB_COMMON, settings.sipr[2]);
    drv.WriteByte(W5500_ADR_SIPR + 3, W5500_BSB_COMMON, settings.sipr[3]);
}

template <class driver_t>
void w5500_itf<driver_t>::Run()
{
    uint8_t version;
    // проверка наличия чипа
    version = drv.ReadByte(W5500_ADR_VERSIONR, W5500_BSB_COMMON);

    status_t status;

    //status.HasChip = (version == 0x04) ? 1 : 0;

    WriteSettings();

    // проверка наличия провода
    uint8_t phycfgr = drv.ReadByte(W5500_ADR_PHYCFGR, W5500_BSB_COMMON);
    status.HasPhyLink = (phycfgr & 0x01 > 0) ? 1 : 0;
    status.Is100Mbps = (phycfgr & 0x02 > 0) ? 1 : 0;
    status.IsFullDuplex = (phycfgr & 0x04 > 0) ? 1 : 0;
    //return state_t::W5500_Configured;
}

}  // namespace w5500