/**   __  ___     ____  _    ______        __     ____         __                  ____
 *   /  |/  /_ __/ / /_(_)__/_  __/__ ____/ /    / __/_ _____ / /____ __ _  ___   /  _/__  ____
 *  / /|_/ / // / / __/ /___// / / -_) __/ _ \  _\ \/ // (_-</ __/ -_)  ' \(_-<  _/ // _ \/ __/ __
 * /_/  /_/\_,_/_/\__/_/    /_/  \__/\__/_//_/ /___/\_, /___/\__/\__/_/_/_/___/ /___/_//_/\__/ /_/
 * Copyright (C) 2015 by Multi-Tech Systems        /___/
 *
 *
 * @author Jason Reiss
 * @date   10-31-2015
 * @brief  lora::ChannelPlan provides an interface for LoRaWAN channel schemes
 *
 * @details
 *
 */

#ifndef __CHANNEL_PLAN_EU868_H__
#define __CHANNEL_PLAN_EU868_H__

#include "Lora.h"
#include "SxRadio.h"
#include <vector>
#include "ChannelPlan.h"

namespace lora {

    class ChannelPlan_EU868 : public lora::ChannelPlan {
        public:
            /**
             * ChannelPlan constructor
             * @param radio SxRadio object used to set Tx/Rx config
             * @param settings Settings object
             */
            ChannelPlan_EU868();
            ChannelPlan_EU868(Settings* settings);
            ChannelPlan_EU868(SxRadio* radio, Settings* settings);

            /**
             * ChannelPlan destructor
             */
            virtual ~ChannelPlan_EU868();

            /**
             * Initialize channels, datarates and duty cycle bands according to current channel plan in settings
             */
            virtual void Init();

            /**
             * Get the next channel to use to transmit
             * @return LORA_OK if channel was found
             * @return LORA_NO_CHANS_ENABLED
             */
            virtual uint8_t GetNextChannel();

            /**
             * Add a channel to the ChannelPlan
             * @param index of channel, use -1 to add to end
             * @param channel settings to add
             */
            virtual uint8_t AddChannel(int8_t index, Channel channel);

            /**
             * Get channel at index
             * @return Channel
             */
            virtual Channel GetChannel(int8_t index);

            /**
             * Get rx window settings for requested window
             * RX_1, RX_2, RX_BEACON, RX_SLOT
             * @param window
             * @return RxWindow
             */
            virtual RxWindow GetRxWindow(uint8_t window);

            /**
             * Get datarate to use on the join request
             * @return datarate index
             */
            virtual uint8_t GetJoinDatarate();

            /**
             * Calculate the next time a join request is possible
             * @param size of join frame
             * @returns LORA_OK
             */
            virtual uint8_t CalculateJoinBackoff(uint8_t size);

            /**
             * Get next channel and set the SxRadio tx config with current settings
             * @return LORA_OK
             */
            virtual uint8_t SetTxConfig();

            /**
             * Set the SxRadio rx config provided window
             * @param window to be opened
             * @param continuous keep window open
             * @return LORA_OK
             */
            virtual uint8_t SetRxConfig(uint8_t window, bool continuous);

            /**
             * Set frequency sub band if supported by plan
             * @param sub_band
             * @return LORA_OK
             */
            virtual uint8_t SetFrequencySubBand(uint8_t sub_band);

            /**
             * Get time on air with current settings
             * @param bytes number of bytes to be sent
             */
            virtual uint32_t GetTimeOnAir(uint8_t bytes);

            /**
             * Callback for ACK timeout event
             * @return LORA_OK
             */
            virtual uint8_t HandleAckTimeout();

            /**
             * Callback for Join Accept packet to load optional channels
             * @return LORA_OK
             */
            virtual uint8_t HandleJoinAccept(const uint8_t* buffer, uint8_t size);

            /**
             * Callback to for rx parameter setup ServerCommand
             * @param payload packet data
             * @param index of start of command buffer
             * @param size number of bytes in command buffer
             * @param[out] status to be returned in MoteCommand answer
             * @return LORA_OK
             */
            virtual uint8_t HandleRxParamSetup(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status);

            /**
             * Callback to for new channel ServerCommand
             * @param payload packet data
             * @param index of start of command buffer
             * @param size number of bytes in command buffer
             * @param[out] status to be returned in MoteCommand answer
             * @return LORA_OK
             */
            virtual uint8_t HandleNewChannel(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status);

            /**
             * Callback to for ping slot channel request ServerCommand
             * @param payload packet data
             * @param index of start of command buffer
             * @param size number of bytes in command buffer
             * @param[out] status to be returned in MoteCommand answer
             * @return LORA_OK
             */
            virtual uint8_t HandlePingSlotChannelReq(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status);

            /**
             * Callback to for beacon frequency request ServerCommand
             * @param payload packet data
             * @param index of start of command buffer
             * @param size number of bytes in command buffer
             * @param[out] status to be returned in MoteCommand answer
             * @return LORA_OK
             */
            virtual uint8_t HandleBeaconFrequencyReq(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status);

            /**
             * Callback to for adaptive datarate ServerCommand
             * @param payload packet data
             * @param index of start of command buffer
             * @param size number of bytes in command buffer
             * @param[out] status to be returned in MoteCommand answer
             * @return LORA_OK
             */
            virtual uint8_t HandleAdrCommand(const uint8_t* payload, uint8_t index, uint8_t size, uint8_t& status);

            /**
             * Validate the configuration after multiple ADR commands have been applied
             * @return status to be returned in MoteCommand answer
             */
            virtual uint8_t ValidateAdrConfiguration();

            /**
             * Update duty cycle with at given frequency and time on air
             * @param freq frequency
             * @param time_on_air_ms tx time on air
             */
            virtual void UpdateDutyCycle(uint32_t freq, uint32_t time_on_air_ms);

            /**
             * Get the time the radio must be off air to comply with regulations
             * Time to wait may be dependent on duty-cycle restrictions per channel
             * Or duty-cycle of join requests if OTAA is being attempted
             * @return ms of time to wait for next tx opportunity
             */
            virtual uint32_t GetTimeOffAir();

            /**
             * Get the channels in use by current channel plan
             * @return channel frequencies
             */
            virtual std::vector<uint32_t> GetChannels();

            /**
             * Get the channel datarate ranges in use by current channel plan
             * @return channel datarate ranges
             */
            virtual std::vector<uint8_t> GetChannelRanges();

            /**
             * Print log message for given rx window
             * @param wnd 1 or 2
             */
            virtual void LogRxWindow(uint8_t wnd);

            /**
             * Enable the default channels of the channel plan
             */
            virtual void EnableDefaultChannels();

        protected:

            static const uint8_t EU868_TX_POWERS[8];                    //!< List of available tx powers
            static const uint8_t EU868_RADIO_POWERS[21];                 //!< List of calibrated tx powers
            static const uint8_t EU868_MAX_PAYLOAD_SIZE[];              //!< List of max payload sizes for each datarate
            static const uint8_t EU868_MAX_PAYLOAD_SIZE_REPEATER[];     //!< List of repeater compatible max payload sizes for each datarate
    };
}

#endif //__CHANNEL_PLAN_EU868_H__
