/**********************************************************************
* COPYRIGHT 2016 MULTI-TECH SYSTEMS, INC.
*
* ALL RIGHTS RESERVED BY AND FOR THE EXCLUSIVE BENEFIT OF
* MULTI-TECH SYSTEMS, INC.
*
* MULTI-TECH SYSTEMS, INC. - CONFIDENTIAL AND PROPRIETARY
* INFORMATION AND/OR TRADE SECRET.
*
* NOTICE: ALL CODE, PROGRAM, INFORMATION, SCRIPT, INSTRUCTION,
* DATA, AND COMMENT HEREIN IS AND SHALL REMAIN THE CONFIDENTIAL
* INFORMATION AND PROPERTY OF MULTI-TECH SYSTEMS, INC.
* USE AND DISCLOSURE THEREOF, EXCEPT AS STRICTLY AUTHORIZED IN A
* WRITTEN AGREEMENT SIGNED BY MULTI-TECH SYSTEMS, INC. IS PROHIBITED.
*
***********************************************************************/

#include "ChannelPlan_US915.h"
#include "limits.h"

using namespace lora;

const uint8_t ChannelPlan_US915::US915_TX_POWERS[] = { 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10 };
const uint8_t ChannelPlan_US915::US915_RADIO_POWERS[] = { 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 18, 19, 19 };
const uint8_t ChannelPlan_US915::US915_MAX_PAYLOAD_SIZE[] =          { 11, 53, 126, 242, 242, 0, 0, 0, 53, 129, 242, 242, 242, 242, 0, 0 };
const uint8_t ChannelPlan_US915::US915_MAX_PAYLOAD_SIZE_REPEATER[] = { 11, 53, 126, 222, 222, 0, 0, 0, 33, 109, 222, 222, 222, 222, 0, 0 };

ChannelPlan_US915::ChannelPlan_US915()
:
  ChannelPlan(NULL, NULL)
{

}

ChannelPlan_US915::ChannelPlan_US915(Settings* settings)
:
  ChannelPlan(NULL, settings)
{

}

ChannelPlan_US915::ChannelPlan_US915(SxRadio* radio, Settings* settings)
:
  ChannelPlan(radio, settings)
{

}

ChannelPlan_US915::~ChannelPlan_US915() {

}

void ChannelPlan_US915::Init() {
    _plan = US915;
    _planName = "US915";

    _datarates.clear();
    _channels.clear();
    _dutyBands.clear();

    DutyBand band;

    band.Index = 0;
    band.DutyCycle = 0;

    Datarate dr;

    _maxTxPower = 30;
    _minTxPower = 0;

    _minFrequency = US915_FREQ_MIN;
    _maxFrequency = US915_FREQ_MAX;

    TX_POWERS = US915_TX_POWERS;
    RADIO_POWERS = US915_RADIO_POWERS;
    MAX_PAYLOAD_SIZE = US915_MAX_PAYLOAD_SIZE;
    MAX_PAYLOAD_SIZE_REPEATER = US915_MAX_PAYLOAD_SIZE_REPEATER;

    band.FrequencyMin = US915_FREQ_MIN;
    band.FrequencyMax = US915_FREQ_MAX;

    _freqUBase125k = US915_125K_FREQ_BASE;
    _freqUStep125k = US915_125K_FREQ_STEP;
    _freqUBase500k = US915_500K_FREQ_BASE;
    _freqUStep500k = US915_500K_FREQ_STEP;
    _freqDBase500k = US915_500K_DBASE;
    _freqDStep500k = US915_500K_DSTEP;
    GetSettings()->Session.Rx2Frequency = US915_500K_DBASE;

    _minDatarate = US915_MIN_DATARATE;
    _maxDatarate = US915_MAX_DATARATE;
    _minRx2Datarate = DR_8;
    _maxRx2Datarate = DR_13;
    _minDatarateOffset = US915_MIN_DATARATE_OFFSET;
    _maxDatarateOffset = US915_MAX_DATARATE_OFFSET;

    _numChans125k = US915_125K_NUM_CHANS;
    _numChans500k = US915_500K_NUM_CHANS;

    logInfo("Initialize channels...");

    SetNumberOfChannels(US915_125K_NUM_CHANS + US915_500K_NUM_CHANS, false);

    dr.SpreadingFactor = SF_10;

    logInfo("Initialize datarates...");

    // Add DR0-3
    while (dr.SpreadingFactor >= SF_7) {
        AddDatarate(-1, dr);
        dr.SpreadingFactor--;
        dr.Index++;
    }

    // Add DR4
    dr.SpreadingFactor = SF_8;
    dr.Bandwidth = BW_500;
    AddDatarate(-1, dr);
    dr.Index++;

    // Skip DR5-7 RFU
    dr.SpreadingFactor = SF_INVALID;
    AddDatarate(-1, dr), dr.Index++;
    AddDatarate(-1, dr), dr.Index++;
    AddDatarate(-1, dr), dr.Index++;

    band.PowerMax = 30;

    band.TimeOffEnd = 0;

    AddDutyBand(-1, band);

    GetSettings()->Session.Rx2DatarateIndex = DR_8;

    // Add DR8-13
    dr.SpreadingFactor = SF_12;
    while (dr.SpreadingFactor >= SF_7) {
        AddDatarate(-1, dr);
        dr.SpreadingFactor--;
        dr.Index++;
    }

    // Skip DR14-15 RFU
    dr.SpreadingFactor = SF_INVALID;
    AddDatarate(-1, dr), AddDatarate(-1, dr);

    GetSettings()->Session.TxDatarate = DR_0;
    GetSettings()->Session.TxPower = GetSettings()->Network.TxPower;

    SetFrequencySubBand(GetSettings()->Network.FrequencySubBand);

}

uint8_t ChannelPlan_US915::HandleJoinAccept(const uint8_t* buffer, uint8_t size) {

    if (size > 17) {
        // TODO: Handle future channel mask settings
    }

    return LORA_OK;
}

void ChannelPlan_US915::SetNumberOfChannels(uint8_t channels, bool resize) {
    uint8_t newsize = ((channels - 1) / CHAN_MASK_SIZE) + 1;

    if (resize) {
        _channels.resize(channels);
    }

    _channelMask.resize(newsize, 0x0);
    _numChans = channels;

}

bool ChannelPlan_US915::IsChannelEnabled(uint8_t channel) {
    uint8_t index = channel / CHAN_MASK_SIZE;
    uint8_t shift = channel % CHAN_MASK_SIZE;

    assert(index < _channelMask.size() * CHAN_MASK_SIZE);

    // cannot shift over 32 bits
    assert(shift < 32);

    // logDebug("index: %d shift %d cm: %04x bit: %04x enabled: %d", index, shift, _channelMask[index], (1 << shift), (_channelMask[index] & (1 << shift)) == (1 << shift));

    return (_channelMask[index] & (1 << shift)) == (1 << shift);
}

uint8_t ChannelPlan_US915::GetMinDatarate() {
    if (GetSettings()->Network.Mode == lora::PEER_TO_PEER)
        return 8;
    else
        return _minDatarate;
}

uint8_t ChannelPlan_US915::GetMaxDatarate() {
    if (GetSettings()->Network.Mode == lora::PEER_TO_PEER)
        return 13;
    else
        return _maxDatarate;
}

uint8_t ChannelPlan_US915::SetRx1Offset(uint8_t offset) {
    GetSettings()->Session.Rx1DatarateOffset = offset;
    return LORA_OK;
}

uint8_t ChannelPlan_US915::SetRx2Frequency(uint32_t freq) {
    GetSettings()->Session.Rx2Frequency = freq;
    return LORA_OK;
}

uint8_t ChannelPlan_US915::SetRx2DatarateIndex(uint8_t index) {
    GetSettings()->Session.Rx2DatarateIndex = index;
    return LORA_OK;
}

uint8_t ChannelPlan_US915::SetTxConfig() {
    logInfo("Configure radio for TX");

    uint8_t band = GetDutyBand(GetChannel(_txChannel).Frequency);
    Datarate txDr = GetDatarate(GetSettings()->Session.TxDatarate);
    int8_t max_pwr = _dutyBands[band].PowerMax;
    uint8_t chans_enabled = 0;

    int8_t pwr = 0;

    pwr = std::min < int8_t > (GetSettings()->Session.TxPower, max_pwr);

    // spec states that if < 50 125kHz channels are enabled, power is limited to 21dB conducted
    chans_enabled += CountBits(_channelMask[0]);
    chans_enabled += CountBits(_channelMask[1]);
    chans_enabled += CountBits(_channelMask[2]);
    chans_enabled += CountBits(_channelMask[3]);
    if (chans_enabled < 50 && pwr > 21) {
        pwr = 21;
    }

    if (pwr + GetSettings()->Network.AntennaGain >= max_pwr + 6 && GetSettings()->Network.AntennaGain > 6) {
        pwr -= (GetSettings()->Network.AntennaGain - 6);
    }

    for (int i = 20; i >= 0; i--) {
        if (RADIO_POWERS[i] <= pwr) {
            pwr = i;
            break;
        }
        if (i == 0) {
            pwr = i;
        }
    }

    logDebug("Session pwr: %d ant: %d max: %d", GetSettings()->Session.TxPower, GetSettings()->Network.AntennaGain, max_pwr);
    logDebug("Radio Power index: %d output: %d total: %d", pwr, RADIO_POWERS[pwr], RADIO_POWERS[pwr] + GetSettings()->Network.AntennaGain);

    uint32_t bw = txDr.Bandwidth;
    uint32_t sf = txDr.SpreadingFactor;
    uint8_t cr = txDr.Coderate;
    uint8_t pl = txDr.PreambleLength;
    uint16_t fdev = 0;
    bool crc = txDr.Crc;
    bool iq = txDr.TxIQ;

    if (GetSettings()->Network.DisableCRC == true)
        crc = false;

    SxRadio::RadioModems_t modem = SxRadio::MODEM_LORA;

    if (sf == SF_FSK) {
        modem = SxRadio::MODEM_FSK;
        sf = 50e3;
        fdev = 25e3;
        bw = 0;
    }

    GetRadio()->SetTxConfig(modem, pwr, fdev, bw, sf, cr, pl, false, crc, false, 0, iq, 3e3);

    logDebug("TX PWR: %u DR: %u SF: %u BW: %u CR: %u PL: %u CRC: %d IQ: %d", pwr, txDr.Index, sf, bw, cr, pl, crc, iq);

    return LORA_OK;
}

uint8_t ChannelPlan_US915::SetRxConfig(uint8_t window, bool continuous) {

    RxWindow rxw = GetRxWindow(window);
    GetRadio()->SetChannel(rxw.Frequency);

    Datarate rxDr = GetDatarate(rxw.DatarateIndex);
    uint32_t bw = rxDr.Bandwidth;
    uint32_t sf = rxDr.SpreadingFactor;
    uint8_t cr = rxDr.Coderate;
    uint8_t pl = rxDr.PreambleLength;
    uint16_t sto = rxDr.SymbolTimeout();
    uint32_t afc = 0;
    bool crc = false; // downlink does not use CRC according to LORAWAN

    if (GetSettings()->Network.DisableCRC == true)
        crc = false;

    Datarate txDr = GetDatarate(GetSettings()->Session.TxDatarate);
    bool iq = txDr.RxIQ;

    if (P2PEnabled()) {
        iq = txDr.TxIQ;
    }

    SxRadio::RadioModems_t modem = SxRadio::MODEM_LORA;

    if (sf == SF_FSK) {
        modem = SxRadio::MODEM_FSK;
        sf = 50e3;
        cr = 0;
        bw = 50e3;
        afc = 83333;
        iq = false;
        crc = true;  // FSK must use CRC
    }

    // Disable printf's to actually receive packets, printing to debug may mess up the timing
    // logTrace("Configure radio for RX%d on freq: %lu", window, rxw.Frequency);
    // logTrace("RX SF: %u BW: %u CR: %u PL: %u STO: %u CRC: %d IQ: %d", sf, bw, cr, pl, sto, crc, iq);

    GetRadio()->SetRxConfig(modem, bw, sf, cr, afc, pl, sto, false, 0, crc, false, 0, iq, continuous);

    return LORA_OK;
}

uint8_t ChannelPlan_US915::AddChannel(int8_t index, Channel channel) {
    logTrace("Add Channel %d : %lu : %02x %d", index, channel.Frequency, channel.DrRange.Value, _channels.size());

    assert(index < (int) _channels.size());

    if (index >= 0) {
        _channels[index] = channel;
    } else {
        _channels.push_back(channel);
    }

    return LORA_OK;
}

Channel ChannelPlan_US915::GetChannel(int8_t index) {
    Channel chan;
    memset(&chan, 0, sizeof(Channel));

    if (_channels.size() > 0) {
        chan = _channels[index];
    } else {
        if (index < 64) {
            chan.Index = index;
            chan.DrRange.Fields.Min = _minDatarate;
            chan.DrRange.Fields.Max = _maxDatarate - 1;
            chan.Frequency = _freqUBase125k + (_freqUStep125k * index);
        } else if (index < 72) {
            chan.Index = index;
            chan.DrRange.Fields.Min = _maxDatarate;
            chan.DrRange.Fields.Max = _maxDatarate;
            chan.Frequency = _freqUBase500k + (_freqUStep500k * (index - 64));
        }
    }

    return chan;
}

uint8_t ChannelPlan_US915::SetFrequencySubBand(uint8_t sub_band) {

    _txFrequencySubBand = sub_band;

    if (sub_band > 0) {
        SetChannelMask(0, 0x0000);
        SetChannelMask(1, 0x0000);
        SetChannelMask(2, 0x0000);
        SetChannelMask(3, 0x0000);
        SetChannelMask(4, 0x0000);
        SetChannelMask((sub_band - 1) / 2, (sub_band % 2) ? 0x00FF : 0xFF00);
        SetChannelMask(4, 1 << (sub_band - 1));
    } else {
        SetChannelMask(0, 0xFFFF);
        SetChannelMask(1, 0xFFFF);
        SetChannelMask(2, 0xFFFF);
        SetChannelMask(3, 0xFFFF);
        SetChannelMask(4, 0x00FF);
    }

    return LORA_OK;
}


void ChannelPlan_US915::LogRxWindow(uint8_t wnd) {

    RxWindow rxw = GetRxWindow(wnd);
    Datarate rxDr = GetDatarate(rxw.DatarateIndex);
    uint8_t bw = rxDr.Bandwidth;
    uint8_t sf = rxDr.SpreadingFactor;
    uint8_t cr = rxDr.Coderate;
    uint8_t pl = rxDr.PreambleLength;
    uint16_t sto = rxDr.SymbolTimeout();
    bool crc = false; // downlink does not use CRC according to LORAWAN
    bool iq = GetTxDatarate().RxIQ;

    logTrace("RX%d on freq: %lu", wnd, rxw.Frequency);
    logTrace("RX DR: %u SF: %u BW: %u CR: %u PL: %u STO: %u CRC: %d IQ: %d", rxDr.Index, sf, bw, cr, pl, sto, crc, iq);
}

RxWindow ChannelPlan_US915::GetRxWindow(uint8_t window) {
    RxWindow rxw;
    int index = 0;

    if (P2PEnabled()) {
        rxw.Frequency = GetSettings()->Network.TxFrequency;
        index = GetSettings()->Session.TxDatarate;
    } else {
        if (window == 1) {
            if (_txChannel < _numChans125k) {
                if (GetSettings()->Network.Mode == PUBLIC) {
                    rxw.Frequency = _freqDBase500k + (_txChannel % 8) * _freqDStep500k;
                } else {
                    rxw.Frequency = _freqDBase500k + (_txChannel / 8) * _freqDStep500k;
                }
            } else
                rxw.Frequency = _freqDBase500k + (_txChannel - _numChans125k) * _freqDStep500k;

            if (GetSettings()->Session.TxDatarate <= DR_4) {
                index = GetSettings()->Session.TxDatarate + 10 - GetSettings()->Session.Rx1DatarateOffset;

                if (index < DR_8)
                    index = DR_8;
                if (index > DR_13)
                    index = DR_13;
            } else if (GetSettings()->Session.TxDatarate >= DR_8) {
                index = GetSettings()->Session.TxDatarate - GetSettings()->Session.Rx1DatarateOffset;
                if (index < DR_8)
                    index = DR_8;
            }
        } else {
            if (GetSettings()->Network.Mode == PUBLIC) {
                rxw.Frequency = GetSettings()->Session.Rx2Frequency;
            } else {
                if (_txChannel < 64)
                    rxw.Frequency = _freqDBase500k + (_txChannel / 8) * _freqDStep500k;
                else
                    rxw.Frequency = _freqDBase500k + (_txChannel % 8) * _freqDStep500k;
            }
            index = GetSettings()->Session.Rx2DatarateIndex;
        }
    }

    rxw.DatarateIndex = index;

    return rxw;
}

uint8_t ChannelPlan_US915::HandleRxParamSetup(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status) {
    status = 0x07;
    int8_t datarate = 0;
    int8_t drOffset = 0;
    uint32_t freq = 0;

    drOffset = payload[index++];
    datarate = drOffset & 0x0F;
    drOffset = (drOffset >> 4) & 0x07;

    freq = payload[index++];
    freq |= payload[index++] << 8;
    freq |= payload[index++] << 16;
    freq *= 100;

    if (!CheckRfFrequency(freq)) {
        logInfo("Freq KO");
        status &= 0xFE; // Channel frequency KO
    }

    if (datarate < _minRx2Datarate || datarate > _maxRx2Datarate) {
        logInfo("DR KO");
        status &= 0xFD; // Datarate KO
    }

    if (drOffset < 0 || drOffset > _maxDatarateOffset) {
        logInfo("DR Offset KO");
        status &= 0xFB; // Rx1DrOffset range KO
    }

    if ((status & 0x07) == 0x07) {
        logInfo("RxParamSetup accepted Rx2DR: %d Rx2Freq: %d Rx1Offset: %d", datarate, freq, drOffset);
        SetRx2DatarateIndex(datarate);
        SetRx2Frequency(freq);
        SetRx1Offset(drOffset);
    } else {
        logInfo("RxParamSetup rejected Rx2DR: %d Rx2Freq: %d Rx1Offset: %d", datarate, freq, drOffset);
    }

    return LORA_OK;
}

uint8_t ChannelPlan_US915::HandleNewChannel(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status) {

    // Not Supported in US915
    status = 0;
    return LORA_OK;
}

uint8_t ChannelPlan_US915::HandlePingSlotChannelReq(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status) {

    lora::CopyFreqtoInt(payload + index, _beaconRxChannel.Frequency);
    index += 3;

    if (_beaconRxChannel.Frequency != 0) {
        _beaconRxChannel.DrRange.Value = payload[index];
    } else {
        // TODO: set to default beacon rx channel
    }

    status = 0x03;
    return LORA_OK;
}

uint8_t ChannelPlan_US915::HandleBeaconFrequencyReq(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status) {

    status = 0x03;
    Channel chParam;

    // Skip channel index
    index++;

    lora::CopyFreqtoInt(payload + index, chParam.Frequency);
    index += 3;
    chParam.DrRange.Value = payload[index++];

    if (!GetRadio()->CheckRfFrequency(chParam.Frequency)) {
        status &= 0xFE; // Channel frequency KO
    }

    if (chParam.DrRange.Fields.Min < chParam.DrRange.Fields.Max) {
        status &= 0xFD; // Datarate range KO
    } else if (chParam.DrRange.Fields.Min < _minDatarate || chParam.DrRange.Fields.Min > _maxDatarate) {
        status &= 0xFD; // Datarate range KO
    } else if (chParam.DrRange.Fields.Max < _minDatarate || chParam.DrRange.Fields.Max > _maxDatarate) {
        status &= 0xFD; // Datarate range KO
    }

    if ((status & 0x03) == 0x03) {
        _beaconChannel = chParam;
    }

    if (_beaconChannel.Frequency == 0) {
        // TODO: Set to default
    }

    status = 0x01;

    return LORA_OK;
}


uint8_t ChannelPlan_US915::HandleAdrCommand(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status) {

    uint8_t power = 0;
    uint8_t datarate = 0;
    uint16_t mask = 0;
    uint8_t ctrl = 0;
    uint8_t nbRep = 0;

    status = 0x07;
    datarate = payload[index++];
    power = datarate & 0x0F;
    datarate = (datarate >> 4) & 0x0F;

    mask = payload[index++];
    mask |= payload[index++] << 8;

    nbRep = payload[index++];
    ctrl = (nbRep >> 4) & 0x07;
    nbRep &= 0x0F;

    if (nbRep == 0) {
        nbRep = 1;
    }

    if (datarate > _maxDatarate) {
        status &= 0xFD; // Datarate KO
    }
    //
    // Remark MaxTxPower = 0 and MinTxPower = 10
    //
    if (power < 0 || power > 10) {
        status &= 0xFB; // TxPower KO
    }

    switch (ctrl) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            SetChannelMask(ctrl, mask);
            break;

        case 6:
            // enable all 125 kHz channels
            SetChannelMask(0, 0xFFFF);
            SetChannelMask(1, 0xFFFF);
            SetChannelMask(2, 0xFFFF);
            SetChannelMask(3, 0xFFFF);
            SetChannelMask(4, mask);
            break;

        case 7:
            // disable all 125 kHz channels
            SetChannelMask(0, 0x0);
            SetChannelMask(1, 0x0);
            SetChannelMask(2, 0x0);
            SetChannelMask(3, 0x0);
            SetChannelMask(4, mask);
            break;

        default:
            logWarning("rejecting RFU or unknown control value %d", ctrl);
            status &= 0xFE; // ChannelMask KO
            return LORA_ERROR;
    }

    if (GetSettings()->Network.ADREnabled) {
        GetSettings()->Session.TxDatarate = datarate;
        GetSettings()->Session.TxPower = TX_POWERS[power];
        GetSettings()->Session.Redundancy = nbRep;
    } else {
        logDebug("ADR is disabled, DR and Power not changed.");
        status &= 0xFB; // TxPower KO
        status &= 0xFD; // Datarate KO
    }

    logDebug("ADR DR: %u PWR: %u Ctrl: %02x Mask: %04x NbRep: %u Stat: %02x", datarate, power, ctrl, mask, nbRep, status);

    return LORA_OK;
}

uint8_t ChannelPlan_US915::ValidateAdrConfiguration() {
    uint8_t status = 0x07;
    uint8_t chans_enabled = 0;
    uint8_t datarate = GetSettings()->Session.TxDatarate;
    uint8_t power = GetSettings()->Session.TxPower;

    if (!GetSettings()->Network.ADREnabled) {
        logDebug("ADR disabled - no applied changes to validate");
        return status;
    }

    if (datarate > _maxDatarate) {
        logWarning("ADR Datarate KO - outside allowed range");
        status &= 0xFD; // Datarate KO
    }
    if (power < _minTxPower || power > _maxTxPower) {
        logWarning("ADR TX Power KO - outside allowed range");
        status &= 0xFB; // TxPower KO
    }
    
    // at least 2 125kHz channels must be enabled
    chans_enabled += CountBits(_channelMask[0]);
    chans_enabled += CountBits(_channelMask[1]);
    chans_enabled += CountBits(_channelMask[2]);
    chans_enabled += CountBits(_channelMask[3]);
    // Semtech reference (LoRaMac-node) enforces at least 2 channels
    if (chans_enabled < 2) {
        logWarning("ADR Channel Mask KO - at least 2 125kHz channels must be enabled");
        status &= 0xFE; // ChannelMask KO
    }
    
    // if TXDR == 4 (SF8@500kHz) at least 1 500kHz channel must be enabled
    if (datarate == DR_4 && (CountBits(_channelMask[4] & 0xFF) == 0)) {
        logWarning("ADR Datarate KO - DR4 requires at least 1 500kHz channel enabled");
        status &= 0xFD; // Datarate KO
    }

    return status;
}

uint32_t ChannelPlan_US915::GetTimeOffAir()
{
    if (GetSettings()->Test.DisableDutyCycle == lora::ON)
        return 0;

    uint32_t min = 0;
    uint32_t now = _dutyCycleTimer.read_ms();

    if (GetSettings()->Session.AggregatedTimeOffEnd > 0 && GetSettings()->Session.AggregatedTimeOffEnd > now) {
        min = std::max < uint32_t > (min, GetSettings()->Session.AggregatedTimeOffEnd - now);
    }

    now = time(NULL);
    uint32_t join_time = 0;

    if (GetSettings()->Session.JoinFirstAttempt != 0 && now < GetSettings()->Session.JoinTimeOffEnd) {
        join_time = (GetSettings()->Session.JoinTimeOffEnd - now) * 1000;
    }

    min = std::max < uint32_t > (join_time, min);

    return min;
}

std::vector<uint32_t> lora::ChannelPlan_US915::GetChannels() {
    std::vector < uint32_t > chans;

    if (GetSettings()->Network.FrequencySubBand > 0) {
        uint8_t chans_per_group = 8;
        size_t start = (GetSettings()->Network.FrequencySubBand - 1) * chans_per_group;
        for (int8_t i = start; i < int8_t(start + chans_per_group); i++) {
            chans.push_back(GetChannel(i).Frequency);
        }
        chans.push_back(GetChannel(_numChans125k + (GetSettings()->Network.FrequencySubBand - 1)).Frequency);
        chans.push_back(GetRxWindow(2).Frequency);
    } else {
        for (int8_t i = 0; i < _numChans; i++) {
            chans.push_back(GetChannel(i).Frequency);
        }
        chans.push_back(GetRxWindow(2).Frequency);
    }

    return chans;
}

std::vector<uint8_t> lora::ChannelPlan_US915::GetChannelRanges() {
    std::vector < uint8_t > ranges;

    if (GetSettings()->Network.FrequencySubBand > 0) {
        uint8_t chans_per_group = 8;
        size_t start = (GetSettings()->Network.FrequencySubBand - 1) * chans_per_group;
        for (int8_t i = start; i < int8_t(start + chans_per_group); i++) {
            ranges.push_back(GetChannel(i).DrRange.Value);
        }
        ranges.push_back(GetChannel(_numChans125k + (GetSettings()->Network.FrequencySubBand - 1)).DrRange.Value);
        ranges.push_back(GetRxWindow(2).DatarateIndex);
    } else {
        for (int8_t i = 0; i < _numChans; i++) {
            ranges.push_back(GetChannel(i).DrRange.Value);
        }
        ranges.push_back(GetRxWindow(2).DatarateIndex);
    }

    ranges.push_back(GetRxWindow(2).DatarateIndex);

    return ranges;

}

void lora::ChannelPlan_US915::EnableDefaultChannels() {
    SetFrequencySubBand(GetFrequencySubBand());
}

uint8_t ChannelPlan_US915::GetNextChannel()
{
    if (GetSettings()->Session.AggregatedTimeOffEnd != 0) {
        return LORA_AGGREGATED_DUTY_CYCLE;
    }

    if (P2PEnabled() || GetSettings()->Network.TxFrequency != 0) {
        logDebug("Using frequency %d", GetSettings()->Network.TxFrequency);

        if (GetSettings()->Test.DisableDutyCycle != lora::ON) {
            int8_t band = GetDutyBand(GetSettings()->Network.TxFrequency);
            logDebug("band: %d freq: %d", band, GetSettings()->Network.TxFrequency);
            if (band != -1 && _dutyBands[band].TimeOffEnd != 0) {
                return LORA_NO_CHANS_ENABLED;
            }
        }

        GetRadio()->SetChannel(GetSettings()->Network.TxFrequency);
        return LORA_OK;
    }

    uint8_t start = 0;
    uint8_t maxChannels = _numChans125k;
    uint8_t nbEnabledChannels = 0;
    uint8_t *enabledChannels = new uint8_t[maxChannels];

    if (GetTxDatarate().Bandwidth == BW_500) {
        maxChannels = _numChans500k;
        start = _numChans125k;
    }

// Search how many channels are enabled
    DatarateRange range;
    uint8_t dr_index = GetSettings()->Session.TxDatarate;
    uint32_t now = _dutyCycleTimer.read_ms();

    for (size_t i = 0; i < _dutyBands.size(); i++) {
        if (_dutyBands[i].TimeOffEnd < now || GetSettings()->Test.DisableDutyCycle == lora::ON) {
            _dutyBands[i].TimeOffEnd = 0;
        }
    }

    for (uint8_t i = start; i < start + maxChannels; i++) {
        range = GetChannel(i).DrRange;
        // logDebug("chan: %d freq: %d range:%02x", i, GetChannel(i).Frequency, range.Value);

        if (IsChannelEnabled(i) && (dr_index >= range.Fields.Min && dr_index <= range.Fields.Max)) {
            int8_t band = GetDutyBand(GetChannel(i).Frequency);
            // logDebug("band: %d freq: %d", band, _channels[i].Frequency);
            if (band != -1 && _dutyBands[band].TimeOffEnd == 0) {
                enabledChannels[nbEnabledChannels++] = i;
            }
        }
    }

    if (GetTxDatarate().Bandwidth == BW_500) {
        _dutyBands[0].PowerMax = 26;
    } else {
        if (nbEnabledChannels < 50)
            _dutyBands[0].PowerMax = 21;
        else
            _dutyBands[0].PowerMax = 30;
    }

    logTrace("Number of available channels: %d", nbEnabledChannels);

    uint32_t freq = 0;
    uint8_t sf = GetTxDatarate().SpreadingFactor;
    uint8_t bw = GetTxDatarate().Bandwidth;
    int16_t thres = DEFAULT_FREE_CHAN_RSSI_THRESHOLD;

    if (nbEnabledChannels == 0) {
        delete [] enabledChannels;
        return LORA_NO_CHANS_ENABLED;
    }

    if (GetSettings()->Network.CADEnabled) {
        // Search for free channel with ms timeout
        int16_t timeout = 10000;
        Timer tmr;
        tmr.start();

        for (uint8_t j = rand_r(0, nbEnabledChannels - 1); tmr.read_ms() < timeout; j++) {
            freq = GetChannel(enabledChannels[j]).Frequency;

            if (GetRadio()->IsChannelFree(SxRadio::MODEM_LORA, freq, sf, thres, bw)) {
                _txChannel = enabledChannels[j];
                break;
            }
        }
    } else {
        uint8_t j = rand_r(0, nbEnabledChannels - 1);
        _txChannel = enabledChannels[j];
        freq = GetChannel(_txChannel).Frequency;
    }

    assert(freq != 0);

    logDebug("Using channel %d : %d", _txChannel, freq);
    GetRadio()->SetChannel(freq);

    delete [] enabledChannels;
    return LORA_OK;
}

uint8_t lora::ChannelPlan_US915::GetJoinDatarate() {
    uint8_t dr = GetSettings()->Session.TxDatarate;

    if (GetSettings()->Test.DisableRandomJoinDatarate == lora::OFF) {
        static bool altDatarate = false;

        if (GetSettings()->Network.FrequencySubBand == 0) {
            static uint16_t used_bands_125k = 0;
            static uint16_t used_bands_500k = 0;
            uint8_t frequency_sub_band = 0;

            if (altDatarate) {
                // 500k channel
                if (CountBits(used_bands_500k) == 8) {
                    used_bands_500k = 0;
                }
                while ((frequency_sub_band = rand_r(1, 8)) && (used_bands_500k & (1 << (frequency_sub_band - 1))) != 0)
                    ;
                used_bands_500k |= (1 << (frequency_sub_band - 1));
            } else {
                // 125k channel
                if (CountBits(used_bands_125k) == 8) {
                    used_bands_125k = 0;
                }
                while ((frequency_sub_band = rand_r(1, 8)) && (used_bands_125k & (1 << (frequency_sub_band - 1))) != 0)
                    ;
                used_bands_125k |= (1 << (frequency_sub_band - 1));
            }

            logWarning("JoinDatarate setting frequency sub band to %d 125k: %04x 500k: %04x", frequency_sub_band, used_bands_125k, used_bands_500k);
            SetFrequencySubBand(frequency_sub_band);
        }

        if (altDatarate && CountBits(_channelMask[4] > 0)) {
            dr = lora::DR_4;
        } else {
            dr = lora::DR_0;
        }
        altDatarate = !altDatarate;
    }

    return dr;
}

uint8_t lora::ChannelPlan_US915::CalculateJoinBackoff(uint8_t size) {

    time_t now = time(NULL);
    uint32_t time_on_max = 0;
    static uint32_t time_off_max = 15;
    uint32_t rand_time_off = 0;
    static uint16_t join_cnt = 0;

    // TODO: calc time-off-max based on RTC time from JoinFirstAttempt, time-off-max is lost over sleep

    if ((time_t)GetSettings()->Session.JoinTimeOffEnd > now) {
        return LORA_JOIN_BACKOFF;
    }

    uint32_t secs_since_first_attempt = (now - GetSettings()->Session.JoinFirstAttempt);
    uint16_t hours_since_first_attempt = secs_since_first_attempt / (60 * 60);

    join_cnt = (join_cnt+1) % 16;

    if (GetSettings()->Session.JoinFirstAttempt == 0) {
        /* 1 % duty-cycle for first hour
         * 0.1 % next 10 hours
         * 0.01 % upto 24 hours         */
        GetSettings()->Session.JoinFirstAttempt = now;
        GetSettings()->Session.JoinTimeOnAir += GetTimeOnAir(size);
        GetSettings()->Session.JoinTimeOffEnd = now + rand_r(GetSettings()->Network.JoinDelay + 2, GetSettings()->Network.JoinDelay + 3);
    } else if (join_cnt == 0) {
        if (hours_since_first_attempt < 1) {
            time_on_max = 36000;
            rand_time_off = rand_r(time_off_max - 1, time_off_max + 1);
            // time off max 1 hour
            time_off_max = std::min < uint32_t > (time_off_max * 2, 60 * 60);

            if (GetSettings()->Session.JoinTimeOnAir < time_on_max) {
                GetSettings()->Session.JoinTimeOnAir += GetTimeOnAir(size);
                GetSettings()->Session.JoinTimeOffEnd = now + rand_time_off;
            } else {
                logWarning("Max time-on-air limit met for current join backoff period");
                GetSettings()->Session.JoinTimeOffEnd = GetSettings()->Session.JoinFirstAttempt + 60 * 60;
            }
        } else if (hours_since_first_attempt < 11) {
            if (GetSettings()->Session.JoinTimeOnAir < 36000) {
                GetSettings()->Session.JoinTimeOnAir = 36000;
            }
            time_on_max = 72000;
            rand_time_off = rand_r(time_off_max - 1, time_off_max + 1);
            // time off max 1 hour
            time_off_max = std::min < uint32_t > (time_off_max * 2, 60 * 60);

            if (GetSettings()->Session.JoinTimeOnAir < time_on_max) {
                GetSettings()->Session.JoinTimeOnAir += GetTimeOnAir(size);
                GetSettings()->Session.JoinTimeOffEnd = now + rand_time_off;
            } else {
                logWarning("Max time-on-air limit met for current join backoff period");
                GetSettings()->Session.JoinTimeOffEnd = GetSettings()->Session.JoinFirstAttempt + 11 * 60 * 60;
            }
        } else {
            if (GetSettings()->Session.JoinTimeOnAir < 72000) {
                GetSettings()->Session.JoinTimeOnAir = 72000;
            }
            uint32_t join_time = 2500;

            // 16 join attempts is ~2754 ms, check if this is the third of the 24 hour period

            time_on_max = 80700;
            time_off_max = 1 * 60 * 60; // 1 hour
            rand_time_off = rand_r(time_off_max - 1, time_off_max + 1);

            if (GetSettings()->Session.JoinTimeOnAir < time_on_max - join_time) {
                GetSettings()->Session.JoinTimeOnAir += GetTimeOnAir(size);
                GetSettings()->Session.JoinTimeOffEnd = now + rand_time_off;
            } else {
                logWarning("Max time-on-air limit met for current join backoff period");
                // Reset the join time on air and set end of restriction to the next 24 hour period
                GetSettings()->Session.JoinTimeOnAir = 72000;
                uint16_t days = (now - GetSettings()->Session.JoinFirstAttempt) / (24 * 60 * 60) + 1;
                logWarning("days : %d", days);
                GetSettings()->Session.JoinTimeOffEnd = GetSettings()->Session.JoinFirstAttempt + ((days * 24) + 11) * 60 * 60;
            }
        }

        logWarning("JoinBackoff: %lu seconds  Time On Air: %lu / %lu", GetSettings()->Session.JoinTimeOffEnd - now, GetSettings()->Session.JoinTimeOnAir, time_on_max);
    } else {
        GetSettings()->Session.JoinTimeOnAir += GetTimeOnAir(size);
        GetSettings()->Session.JoinTimeOffEnd = now + rand_r(GetSettings()->Network.JoinDelay + 2, GetSettings()->Network.JoinDelay + 3);
    }

    return LORA_OK;
}
