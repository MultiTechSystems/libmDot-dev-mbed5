/**   __  ___     ____  _    ______        __     ____         __                  ____
 *   /  |/  /_ __/ / /_(_)__/_  __/__ ____/ /    / __/_ _____ / /____ __ _  ___   /  _/__  ____
 *  / /|_/ / // / / __/ /___// / / -_) __/ _ \  _\ \/ // (_-</ __/ -_)  ' \(_-<  _/ // _ \/ __/ __
 * /_/  /_/\_,_/_/\__/_/    /_/  \__/\__/_//_/ /___/\_, /___/\__/\__/_/_/_/___/ /___/_//_/\__/ /_/
 * Copyright (C) 2015 by Multi-Tech Systems        /___/
 *
 *
 * @author Jason Reiss
 * @date   10-31-2015
 * @brief  lora::MacEvents provides an interface for events from the Mac layer
 *
 * @details
 *
 */

#ifndef __LORA_MAC_EVENTS_H__
#define __LORA_MAC_EVENTS_H__

#include "Lora.h"

namespace lora {

    class MacEvents {

        public:
            virtual ~MacEvents() {};

            virtual void TxDone(uint8_t dr) = 0;
            virtual void TxTimeout(void) = 0;

            virtual void JoinAccept(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) = 0;
            virtual void JoinFailed(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) = 0;
            virtual void PacketRx(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr, lora::DownlinkControl ctrl, uint8_t slot, uint8_t retries=0) = 0;
            virtual void RxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr, lora::DownlinkControl ctrl, uint8_t slot) = 0;

            virtual void Pong(int16_t m_rssi, int8_t m_snr, int16_t s_rssi, int8_t s_snr) = 0;
            virtual void NetworkLinkCheck(int16_t m_rssi, int8_t m_snr, int8_t s_snr, uint8_t s_gateways) = 0;

            virtual void RxTimeout(uint8_t slot) = 0;
            virtual void RxError(uint8_t slot) = 0;

            virtual void MissedAck(uint8_t retries) = 0;

            virtual uint8_t MeasureBattery() = 0;

        private:


    };

}

#endif
