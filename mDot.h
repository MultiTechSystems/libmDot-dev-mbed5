// TODO: add license header

#ifndef MDOT_H
#define MDOT_H

#include "mbed.h"
#include "rtos.h"
#include "Mote.h"
#include <vector>
#include <map>
#include <string>

class mDotEvent;
class LoRaConfig;


class mDot {
        friend class mDotEvent;

    private:

        mDot(lora::ChannelPlan* plan);
        ~mDot();

        void initLora();

        void setLastError(const std::string& str);

        static bool validateBaudRate(const uint32_t& baud);
        static bool validateFrequencySubBand(const uint8_t& band);
        bool validateDataRate(const uint8_t& dr);

        int32_t joinBase(const uint32_t& retries);
        int32_t sendBase(const std::vector<uint8_t>& data, const bool& confirmed = false, const bool& blocking = true, const bool& highBw = false);
        void waitForPacket();
        void waitForLinkCheck();

        void setActivityLedState(const uint8_t& state);
        uint8_t getActivityLedState();

        void blinkActivityLed(void) {
            if (_activity_led) {
                int val = _activity_led->read();
                _activity_led->write(!val);
            }
        }

        mDot(const mDot&);
        mDot& operator=(const mDot&);

        uint32_t RTC_ReadBackupRegister(uint32_t RTC_BKP_DR);
        void RTC_WriteBackupRegister(uint32_t RTC_BKP_DR, uint32_t Data);

        void wakeup();

        void enterStopMode(const uint32_t& interval, const uint8_t& wakeup_mode = RTC_ALARM);
        void enterStandbyMode(const uint32_t& interval, const uint8_t& wakeup_mode = RTC_ALARM);

        static mDot* _instance;

        lora::Mote* _mote;
        LoRaConfig* _config;
        lora::Settings _settings;
        mDotEvent* _events;

        std::string _last_error;
        static const uint32_t _baud_rates[];
        uint8_t _activity_led_state;
        Ticker _tick;
        DigitalOut* _activity_led;
        bool _activity_led_enable;
        PinName _activity_led_pin;
        bool _activity_led_external;
        uint8_t _linkFailCount;
        uint8_t _class;
        InterruptIn* _wakeup;
        PinName _wakeup_pin;

        typedef enum {
            OFF,
            ON,
            BLINK,
        } state;

    public:

#if defined(TARGET_MTS_MDOT_F411RE)
        typedef enum {
            FM_APPEND = (1 << 0),
            FM_TRUNC = (1 << 1),
            FM_CREAT = (1 << 2),
            FM_RDONLY = (1 << 3),
            FM_WRONLY = (1 << 4),
            FM_RDWR = (FM_RDONLY | FM_WRONLY),
            FM_DIRECT = (1 << 5)
        } FileMode;
#endif /* TARGET_MTS_MDOT_F411RE */

        typedef enum {
            MDOT_OK = 0,
            MDOT_INVALID_PARAM = -1,
            MDOT_TX_ERROR = -2,
            MDOT_RX_ERROR = -3,
            MDOT_JOIN_ERROR = -4,
            MDOT_TIMEOUT = -5,
            MDOT_NOT_JOINED = -6,
            MDOT_ENCRYPTION_DISABLED = -7,
            MDOT_NO_FREE_CHAN = -8,
            MDOT_TEST_MODE = -9,
            MDOT_NO_ENABLED_CHAN = -10,
            MDOT_AGGREGATED_DUTY_CYCLE = -11,
            MDOT_MAX_PAYLOAD_EXCEEDED = -12,
            MDOT_LBT_CHANNEL_BUSY = -13,
            MDOT_ERROR = -1024,
        } mdot_ret_code;

        enum JoinMode {
            MANUAL = 0,
            OTA,
            AUTO_OTA,
            PEER_TO_PEER
        };

        enum Mode {
            COMMAND_MODE,
            SERIAL_MODE
        };

        enum RX_Output {
            HEXADECIMAL,
            BINARY
        };

        enum DataRates {
            DR0,
            DR1,
            DR2,
            DR3,
            DR4,
            DR5,
            DR6,
            DR7,
            DR8,
            DR9,
            DR10,
            DR11,
            DR12,
            DR13,
            DR14,
            DR15,
            SF_12 = 16,
            SF_11,
            SF_10,
            SF_9,
            SF_8,
            SF_7,
            SF_7H,
            SF_FSK
        };

        enum FrequencySubBands {
            FSB_ALL,
            FSB_1,
            FSB_2,
            FSB_3,
            FSB_4,
            FSB_5,
            FSB_6,
            FSB_7,
            FSB_8
        };

        enum JoinByteOrder {
            LSB,
            MSB
        };

        enum wakeup_mode {
            RTC_ALARM,
            INTERRUPT,
            RTC_ALARM_OR_INTERRUPT
        };

        enum UserBackupRegs {
            UBR0,
            UBR1,
            UBR2,
            UBR3,
            UBR4,
            UBR5,
            UBR6,
            UBR7,
            UBR8,
            UBR9
#if defined (TARGET_XDOT_L151CC)
           ,UBR10,
            UBR11,
            UBR12,
            UBR13,
            UBR14,
            UBR15,
            UBR16,
            UBR17,
            UBR18,
            UBR19,
            UBR20,
            UBR21
#endif /* TARGET_XDOT_L151CC */
        };

#if defined(TARGET_MTS_MDOT_F411RE)
        typedef struct {
                int16_t fd;
                char name[33];
                uint32_t size;
        } mdot_file;
#endif /* TARGET_MTS_MDOT_F411RE */

        typedef struct {
                uint32_t Up;
                uint32_t Down;
                uint32_t Joins;
                uint32_t JoinFails;
                uint32_t MissedAcks;
                uint32_t CRCErrors;
        } mdot_stats;

        typedef struct {
                int16_t last;
                int16_t min;
                int16_t max;
                int16_t avg;
        } rssi_stats;

        typedef struct {
                int16_t last;
                int16_t min;
                int16_t max;
                int16_t avg;
        } snr_stats;

        typedef struct {
                bool status;
                int32_t dBm;
                uint32_t gateways;
                std::vector<uint8_t> payload;
        } link_check;

        typedef struct {
                int32_t status;
                int16_t rssi;
                int16_t snr;
        } ping_response;

        static const uint8_t MaxLengths_915[];
        static const uint8_t MaxLengths_868[];

        static std::string JoinModeStr(uint8_t mode);
        static std::string ModeStr(uint8_t mode);
        static std::string RxOutputStr(uint8_t format);
        static std::string DataRateStr(uint8_t rate);
        static std::string FrequencyBandStr(uint8_t band);
        static std::string FrequencySubBandStr(uint8_t band);

#if defined(TARGET_MTS_MDOT_F411RE)
        uint32_t UserRegisters[10];
#else
        uint32_t UserRegisters[22];
#endif /* TARGET_MTS_MDOT_F411RE */

        /**
         *  Get a handle to the singleton object
         * @param plan the channel plan to use
         * @returns pointer to mDot object
         */
        static mDot* getInstance(lora::ChannelPlan* plan);

        void setEvents(mDotEvent* events);

        /**
         *
         * Get library version information
         * @returns string containing library version information
         */
        std::string getId();

        /**
         * Get MTS LoRa version information
         * @returns string containing MTS LoRa version information
         */
        std::string getMtsLoraId();

        /**
         * Perform a soft reset of the system
         */
        void resetCpu();

        /**
         * Reset config to factory default
         */
        void resetConfig();

        /**
         * Save config data to non volatile memory
         * @returns true if success, false if failure
         */
        bool saveConfig();

        /**
         * Set the log level for the library
         * options are:
         *  NONE_LEVEL - logging is off at this level
         *  FATAL_LEVEL - only critical errors will be reported
         *  ERROR_LEVEL
         *  WARNING_LEVEL
         *  INFO_LEVEL
         *  DEBUG_LEVEL
         *  TRACE_LEVEL - every detail will be reported
         * @param level the level to log at
         * @returns MDOT_OK if success
         */
        int32_t setLogLevel(const uint8_t& level);

        /**
         * Get the current log level for the library
         * @returns current log level
         */
        uint8_t getLogLevel();

        /**
         * Seed pseudo RNG in LoRaMac layer, uses random value from radio RSSI reading by default
         * @param seed for RNG
         */
        void seedRandom(uint32_t seed);


        uint8_t setChannelPlan(lora::ChannelPlan* plan);

        lora::Settings* getSettings();

        /**
         * Enable or disable the activity LED.
         * @param enable true to enable the LED, false to disable
         */
        void setActivityLedEnable(const bool& enable);

        /**
         * Find out if the activity LED is enabled
         * @returns true if activity LED is enabled, false if disabled
         */
        bool getActivityLedEnable();

        /**
         * Use a different pin for the activity LED.
         * The default is XBEE_RSSI.
         * @param pin the new pin to use
         */
        void setActivityLedPin(const PinName& pin);

        /**
         * Use an external DigitalOut object for the activity LED.
         * The pointer must stay valid!
         * @param pin the DigitalOut object to use
         */
        void setActivityLedPin(DigitalOut* pin);

        /**
         * Find out what pin the activity LED is on
         * @returns the pin the activity LED is using
         */
        PinName getActivityLedPin();

        /**
         * Returns boolean indicative of start-up from standby mode
         * @returns true if dot woke from standby
         */
        bool getStandbyFlag();

        std::vector<uint16_t> getChannelMask();

        int32_t setChannelMask(uint8_t offset, uint16_t mask);

        /**
         * Add a channel
         * @returns MDOT_OK
         */
        int32_t addChannel(uint8_t index, uint32_t frequency, uint8_t datarateRange);

        /**
         * Add a downlink channel
         * @returns MDOT_OK
         */
        int32_t addDownlinkChannel(uint8_t index, uint32_t frequency);

        /**
         * Get list of channel frequencies currently in use
         * @returns vector of channels currently in use
         */
        std::vector<uint32_t> getChannels();

        /**
         * Get list of downlink channel frequencies currently in use
         * @returns vector of channels currently in use
         */
        std::vector<uint32_t> getDownlinkChannels();

        /**
         * Get list of channel datarate ranges currently in use
         * @returns vector of datarate ranges currently in use
         */
        std::vector<uint8_t> getChannelRanges();

        /**
         * Get list of channel frequencies in config file to be used as session defaults
         * @returns vector of channels in config file
         */
        std::vector<uint32_t> getConfigChannels();

        /**
         * Get list of channel datarate ranges in config file to be used as session defaults
         * @returns vector of datarate ranges in config file
         */
        std::vector<uint8_t> getConfigChannelRanges();

        /**
         * Get default frequency band
         * @returns frequency band the device was manufactured for
         */
        uint8_t getDefaultFrequencyBand();

        /**
         * Set frequency sub band
         * only applicable if frequency band is set for United States (FB_915)
         * sub band 0 will allow the radio to use all 64 channels
         * sub band 1 - 8 will allow the radio to use the 8 channels in that sub band
         * for use with Conduit gateway and MTAC_LORA, use sub bands 1 - 8, not sub band 0
         * @param band the sub band to use (0 - 8)
         * @returns MDOT_OK if success
         */
        int32_t setFrequencySubBand(const uint8_t& band);

        /**
         * Get frequency sub band
         * @returns frequency sub band currently in use
         */
        uint8_t getFrequencySubBand();

        /**
         * Get frequency band
         * @returns frequency band (channel plan) currently in use
         */
        uint8_t getFrequencyBand();

        /**
         * Get channel plan name
         * @returns name of channel plan currently in use
         */
        std::string getChannelPlanName();

        /**
         * Get the datarate currently in use within the MAC layer
         * returns 0-15
         */
        uint8_t getSessionDataRate();


        /**
         * Get the current max EIRP used in the channel plan
         * May be changed by the network server
         * returns 0-36
         */
        uint8_t getSessionMaxEIRP();

        /**
         * Set the current max EIRP used in the channel plan
         * May be changed by the network server
         * accepts 0-36
         */
        void setSessionMaxEIRP(uint8_t max);

        /**
         * Get the current downlink dwell time used in the channel plan
         * May be changed by the network server
         * returns 0-1
         */
        uint8_t getSessionDownlinkDwelltime();

        /**
         * Set the current downlink dwell time used in the channel plan
         * May be changed by the network server
         * accepts 0-1
         */
        void setSessionDownlinkDwelltime(uint8_t dwell);

        /**
         * Get the current uplink dwell time used in the channel plan
         * May be changed by the network server
         * returns 0-1
         */
        uint8_t getSessionUplinkDwelltime();

        /**
         * Set the current uplink dwell time used in the channel plan
         * May be changed by the network server
         * accepts 0-1
         */
        void setSessionUplinkDwelltime(uint8_t dwell);

        /**
         * Set the current downlink dwell time used in the channel plan
         * May be changed by the network server
         * accepts 0-1
         */
        uint32_t getListenBeforeTalkTime(uint8_t ms);

        /**
         * Set the current downlink dwell time used in the channel plan
         * May be changed by the network server
         * accepts 0-1
         */
        void setListenBeforeTalkTime(uint32_t ms);

        /**
         * Enable/disable public network mode
         * JoinDelay will be set to (public: 5s, private: 1s) and
         * RxDelay will be set to 1s both can be adjusted afterwards
         * @param on should be true to enable public network mode
         * @returns MDOT_OK if success
         */
        int32_t setPublicNetwork(const bool& on);

        /**
         * Get public network mode
         * @returns true if public network mode is enabled
         */
        bool getPublicNetwork();

        /**
         * Get the device ID
         * @returns vector containing the device ID (size 8)
         */
        std::vector<uint8_t> getDeviceId();

        /**
         * Get the device port to be used for lora application data (1-223)
         *  @returns port
         */
        uint8_t getAppPort();

        /**
         * Set the device port to be used for lora application data (1-223)
         *  @returns MDOT_OK if success
         */
        int32_t setAppPort(uint8_t port);

        /**
         * Set the device class A, B or C
         *  @returns MDOT_OK if success
         */
        int32_t setClass(std::string newClass);

        /**
         * Get the device class A, B or C
         *  @returns MDOT_OK if success
         */
        std::string getClass();

        /**
         * Get the max packet length with current settings
         * @returns max packet length
         */
        uint8_t getMaxPacketLength();

        /**
         * Set network address
         * for use with MANUAL network join mode, will be assigned in OTA & AUTO_OTA modes
         * @param addr a vector of 4 bytes
         * @returns MDOT_OK if success
         */
        int32_t setNetworkAddress(const std::vector<uint8_t>& addr);

        /**
         * Get network address
         * @returns vector containing network address (size 4)
         */
        std::vector<uint8_t> getNetworkAddress();

        /**
         * Set network session key
         * for use with MANUAL network join mode, will be assigned in OTA & AUTO_OTA modes
         * @param key a vector of 16 bytes
         * @returns MDOT_OK if success
         */
        int32_t setNetworkSessionKey(const std::vector<uint8_t>& key);

        /**
         * Get network session key
         * @returns vector containing network session key (size 16)
         */
        std::vector<uint8_t> getNetworkSessionKey();

        /**
         * Set data session key
         * for use with MANUAL network join mode, will be assigned in OTA & AUTO_OTA modes
         * @param key a vector of 16 bytes
         * @returns MDOT_OK if success
         */
        int32_t setDataSessionKey(const std::vector<uint8_t>& key);

        /**
         * Get data session key
         * @returns vector containing data session key (size 16)
         */
        std::vector<uint8_t> getDataSessionKey();

        /**
         * Set network name
         * for use with OTA & AUTO_OTA network join modes
         * generates network ID (crc64 of name) automatically
         * @param name a string of of at least 8 bytes and no more than 128 bytes
         * @return MDOT_OK if success
         */
        int32_t setNetworkName(const std::string& name);

        /**
         * Get network name
         * @return string containing network name (size 8 to 128)
         */
        std::string getNetworkName();

        /**
         * Set network ID
         * for use with OTA & AUTO_OTA network join modes
         * setting network ID via this function sets network name to empty
         * @param id a vector of 8 bytes
         * @returns MDOT_OK if success
         */
        int32_t setNetworkId(const std::vector<uint8_t>& id);

        /**
         * Get network ID
         * @returns vector containing network ID (size 8)
         */
        std::vector<uint8_t> getNetworkId();

        /**
         * Set network passphrase
         * for use with OTA & AUTO_OTA network join modes
         * generates network key (cmac of passphrase) automatically
         * @param name a string of of at least 8 bytes and no more than 128 bytes
         * @return MDOT_OK if success
         */
        int32_t setNetworkPassphrase(const std::string& passphrase);

        /**
         * Get network passphrase
         * @return string containing network passphrase (size 8 to 128)
         */
        std::string getNetworkPassphrase();

        /**
         * Set network key
         * for use with OTA & AUTO_OTA network join modes
         * setting network key via this function sets network passphrase to empty
         * @param id a vector of 16 bytes
         * @returns MDOT_OK if success
         */
        int32_t setNetworkKey(const std::vector<uint8_t>& id);

        /**
         * Get network key
         * @returns a vector containing network key (size 16)
         */
        std::vector<uint8_t> getNetworkKey();

        /**
         * Set lorawan application EUI
         * equivalent to setNetworkId
         * @param eui application EUI (size 8)
         */
        int32_t setAppEUI(const uint8_t* eui);

        /**
         * Get lorawan application EUI
         * equivalent to getNetworkId
         * @returns vector containing application EUI (size 8)
         */
        const uint8_t* getAppEUI();

        /**
         * Set lorawan application key
         * equivalent to setNetworkKey
         * @param eui application key (size 16)
         */
        int32_t setAppKey(const uint8_t* key);

        /**
         * Set lorawan application key
         * equivalent to getNetworkKey
         * @returns eui application key (size 16)
         */
        const uint8_t* getAppKey();

        /**
         * Set join byte order
         * @param order 0:LSB 1:MSB
         */
        uint32_t setJoinByteOrder(uint8_t order);

        /**
         * Get join byte order
         * @returns byte order to use in joins 0:LSB 1:MSB
         */
        uint8_t getJoinByteOrder();

        /**
         * Attempt to join network
         * each attempt will be made with a random datarate up to the configured datarate
         * JoinRequest backoff between tries is enforced to 1% for 1st hour, 0.1% for 1-10 hours and 0.01% after 10 hours
         * Check getNextTxMs() for time until next join attempt can be made
         * @returns MDOT_OK if success
         */
        int32_t joinNetwork();

        /**
         * Attempts to join network once
         * @returns MDOT_OK if success
         */
        int32_t joinNetworkOnce();

        /**
         * Resets current network session, essentially disconnecting from the network
         * has no effect for MANUAL network join mode
         */
        void resetNetworkSession();

        /**
         * Restore saved network session from flash
         * has no effect for MANUAL network join mode
         */
        void restoreNetworkSession();

        /**
         * Save current network session to flash
         * has no effect for MANUAL network join mode
         */
        void saveNetworkSession();

        /**
         * Set number of times joining will retry each sub-band before changing
         * to the next subband in US915 and AU915
         * @param retries must be between 0 - 255
         * @returns MDOT_OK if success
         */
        int32_t setJoinRetries(const uint8_t& retries);

        /**
         * Get number of times joining will retry each sub-band
         * @returns join retries (0 - 255)
         */
        uint8_t getJoinRetries();

        /**
         * Set network join mode
         * MANUAL: set network address and session keys manually
         * OTA: User sets network name and passphrase, then attempts to join
         * AUTO_OTA: same as OTA, but network sessions can be saved and restored
         * @param mode MANUAL, OTA, or AUTO_OTA
         * @returns MDOT_OK if success
         */
        int32_t setJoinMode(const uint8_t& mode);

        /**
         * Get network join mode
         * @returns MANUAL, OTA, or AUTO_OTA
         */
        uint8_t getJoinMode();

        /**
         * Get network join status
         * @returns true if currently joined to network
         */
        bool getNetworkJoinStatus();

        /**
         * Do a network link check
         * application data may be returned in response to a network link check command
         * @returns link_check structure containing success, dBm above noise floor, gateways in range, and packet payload
         */
        link_check networkLinkCheck();

        /**
         * Set network link check count to perform automatic link checks every count packets
         * only applicable if ACKs are disabled
         * @param count must be between 0 - 255
         * @returns MDOT_OK if success
         */
        int32_t setLinkCheckCount(const uint8_t& count);

        /**
         * Get network link check count
         * @returns count (0 - 255)
         */
        uint8_t getLinkCheckCount();

        /**
         * Set network link check threshold, number of link check failures or missed acks to tolerate
         * before considering network connection lost
         * @pararm count must be between 0 - 255
         * @returns MDOT_OK if success
         */
        int32_t setLinkCheckThreshold(const uint8_t& count);

        /**
         * Get network link check threshold
         * @returns threshold (0 - 255)
         */
        uint8_t getLinkCheckThreshold();

        /**
         * Get/set number of failed link checks in the current session
         * @returns count (0 - 255)
         */
        uint8_t getLinkFailCount();
        int32_t setLinkFailCount(uint8_t count);

        /**
         * Set UpLinkCounter number of packets sent to the gateway during this network session (sequence number)
         * @returns MDOT_OK
         */
        int32_t setUpLinkCounter(uint32_t count);

        /**
         * Get UpLinkCounter
         * @returns number of packets sent to the gateway during this network session (sequence number)
         */
        uint32_t getUpLinkCounter();

        /**
         * Set UpLinkCounter number of packets sent by the gateway during this network session (sequence number)
         * @returns MDOT_OK
         */
        int32_t setDownLinkCounter(uint32_t count);

        /**
         * Get DownLinkCounter
         * @returns number of packets sent by the gateway during this network session (sequence number)
         */
        uint32_t getDownLinkCounter();

        /**
         * Enable/disable AES encryption
         * AES encryption must be enabled for use with Conduit gateway and MTAC_LORA card
         * @param on true for AES encryption to be enabled
         * @returns MDOT_OK if success
         */
        int32_t setAesEncryption(const bool& on);

        /**
         * Get AES encryption
         * @returns true if AES encryption is enabled
         */
        bool getAesEncryption();

        /**
         * Get RSSI stats
         * @returns rssi_stats struct containing last, min, max, and avg RSSI in dB
         */
        rssi_stats getRssiStats();

        /**
         * Get SNR stats
         * @returns snr_stats struct containing last, min, max, and avg SNR in cB
         */
        snr_stats getSnrStats();

        /**
         * Get ms until next free channel
         * only applicable for European models, US models return 0
         * @returns time (ms) until a channel is free to use for transmitting
         */
        uint32_t getNextTxMs();

        /**
         * Get join delay in seconds
         *  Public network defaults to 5 seconds
         *  Private network defaults to 1 second
         *  @returns number of seconds before join accept message is expected
         */
        uint8_t getJoinDelay();

        /**
         * Set join delay in seconds
         *  Public network defaults to 5 seconds
         *  Private network defaults to 1 second
         *  @param delay number of seconds before join accept message is expected
         *  @return MDOT_OK if success
         */
        uint32_t setJoinDelay(uint8_t delay);

        /**
         * Get join Rx1 datarate offset
         *  defaults to 0
         *  @returns offset
         */
        uint8_t getJoinRx1DataRateOffset();

        /**
         * Set join Rx1 datarate offset
         *  @param offset for datarate
         *  @return MDOT_OK if success
         */
        uint32_t setJoinRx1DataRateOffset(uint8_t offset);

        /**
         * Get join Rx2 datarate
         *  defaults to US:DR8, AU:DR8, EU:DR0
         *  @returns datarate
         */
        uint8_t getJoinRx2DataRate();

        /**
         * Set join Rx2 datarate
         *  @param datarate
         *  @return MDOT_OK if success
         */
        uint32_t setJoinRx2DataRate(uint8_t datarate);

        /**
         * Get join Rx2 frequency
         *  defaults US:923.3, AU:923.3, EU:869.525
         *  @returns frequency
         */
        uint32_t getJoinRx2Frequency();

        /**
         * Set join Rx2 frequency
         *  @param frequency
         *  @return MDOT_OK if success
         */
        uint32_t setJoinRx2Frequency(uint32_t frequency);

        /**
         * Get rx delay in seconds
         *  Defaults to 1 second
         *  @returns number of seconds before response message is expected
         */
        uint8_t getRxDelay();

        /**
         * Set rx delay in seconds
         *  Defaults to 1 second
         *  @param delay number of seconds before response message is expected
         *  @return MDOT_OK if success
         */
        uint32_t setRxDelay(uint8_t delay);

        /**
         * Get  preserve session to save network session info through reset or power down in AUTO_OTA mode
         *  Defaults to off
         *  @returns true if enabled
         */
        bool getPreserveSession();

        /**
         * Set preserve session to save network session info through reset or power down in AUTO_OTA mode
         *  Defaults to off
         *  @param enable
         *  @return MDOT_OK if success
         */
        uint32_t setPreserveSession(bool enable);

        /**
         * Get data pending
         * only valid after sending data to the gateway
         * @returns true if server has available packet(s)
         */
        bool getDataPending();

        /**
         * Get ack requested
         * only valid after sending data to the gateway
         * @returns true if server has requested ack
         */
        bool getAckRequested();

        /**
         * Get is transmitting indicator
         * @returns true if currently transmitting
         */
        bool getIsTransmitting();

        /**
         * Get is idle indicator
         * @returns true if not currently transmitting, waiting or receiving
         */
        bool getIsIdle();

        /**
         * Set TX data rate
         * data rates affect maximum payload size
         * @param dr SF_7 - SF_12|DR0-DR7 for Europe, SF_7 - SF_10 | DR0-DR4 for United States
         * @returns MDOT_OK if success
         */
        int32_t setTxDataRate(const uint8_t& dr);

        /**
         * Get TX data rate
         * @returns current TX data rate (DR0-DR15)
         */
        uint8_t getTxDataRate();

        /**
         * Get a random value from the radio based on RSSI
         *  @returns randome value
         */
        uint32_t getRadioRandom();

        /**
         * Get data rate spreading factor and bandwidth
         * EU868 Datarates
         * ---------------
         * DR0 - SF12BW125
         * DR1 - SF11BW125
         * DR2 - SF10BW125
         * DR3 - SF9BW125
         * DR4 - SF8BW125
         * DR5 - SF7BW125
         * DR6 - SF7BW250
         * DR7 - FSK
         *
         * US915 Datarates
         * ---------------
         * DR0 - SF10BW125
         * DR1 - SF9BW125
         * DR2 - SF8BW125
         * DR3 - SF7BW125
         * DR4 - SF8BW500
         *
         * AU915 Datarates
         * ---------------
         * DR0 - SF10BW125
         * DR1 - SF9BW125
         * DR2 - SF8BW125
         * DR3 - SF7BW125
         * DR4 - SF8BW500
         *
         * @returns spreading factor and bandwidth
         */
        std::string getDataRateDetails(uint8_t rate);
        std::string getDateRateDetails(uint8_t rate);


        /**
         * Set TX power output of radio before antenna gain, default: 14 dBm
         * actual output power may be limited by local regulations for the chosen frequency
         * power affects maximum range
         * @param power 2 dBm - 20 dBm
         * @returns MDOT_OK if success
         */
        int32_t setTxPower(const uint32_t& power);

        /**
         * Get TX power
         * @returns TX power (2 dBm - 20 dBm)
         */
        uint32_t getTxPower();

        /**
         * Get configured gain of installed antenna, default: +3 dBi
         * @returns gain of antenna in dBi
         */
        int8_t getAntennaGain();

        /**
         * Set configured gain of installed antenna, default: +3 dBi
         * @param gain -127 dBi - 128 dBi
         * @returns MDOT_OK if success
         */
        int32_t setAntennaGain(int8_t gain);

        /**
         * Enable/disable TX waiting for rx windows
         * when enabled, send calls will block until a packet is received or RX timeout
         * @param enable set to true if expecting responses to transmitted packets
         * @returns MDOT_OK if success
         */
        int32_t setTxWait(const bool& enable);

        /**
         * Get TX wait
         * @returns true if TX wait is enabled
         */
        bool getTxWait();

        /**
         * Cancel pending rx windows
         */
        void cancelRxWindow();

        /**
         * Get time on air
         * @returns the amount of time (in ms) it would take to send bytes bytes based on current configuration
         */
        uint32_t getTimeOnAir(uint8_t bytes);

        /**
         * Get min frequency
         * @returns minimum frequency based on current channel plan
         */
        uint32_t getMinFrequency();

        /**
         * Get max frequency
         * @returns maximum frequency based on current channel plan
         */
        uint32_t getMaxFrequency();

        /**
         * Get min datarate
         * @returns minimum datarate based on current channel plan
         */
        uint8_t getMinDatarate();

        /**
         * Get max datarate
         * @returns maximum datarate based on current channel plan
         */
        uint8_t getMaxDatarate();

        /**
         * Get min datarate offset
         * @returns minimum datarate offset based on current channel plan
         */
        uint8_t getMinDatarateOffset();

        /**
         * Get max datarate offset
         * @returns maximum datarate based on current channel plan
         */
        uint8_t getMaxDatarateOffset();

        /**
         * Get min datarate
         * @returns minimum datarate based on current channel plan
         */
        uint8_t getMinRx2Datarate();

        /**
         * Get max rx2 datarate
         * @returns maximum rx2 datarate based on current channel plan
         */
        uint8_t getMaxRx2Datarate();

        /**
         * Get max tx power
         * @returns maximum tx power based on current channel plan
         */
        uint8_t getMaxTxPower();

        /**
         * Get min tx power
         * @returns minimum tx power based on current channel plan
         */
        uint8_t getMinTxPower();

        /**
         *
         * get/set adaptive data rate
         * configure data rates and power levels based on signal to noise of packets received at gateway
         * true == adaptive data rate is on
         * set function returns MDOT_OK if success
         */
        int32_t setAdr(const bool& on);
        bool getAdr();

        /**
         * Set forward error correction bytes
         * @param bytes 1 - 4 bytes
         * @returns MDOT_OK if success
         */
        int32_t setFec(const uint8_t& bytes);

        /**
         * Get forward error correction bytes
         * @returns bytes (1 - 4)
         */
        uint8_t getFec();

        /**
         * Enable/disable CRC checking of packets
         * CRC checking must be enabled for use with Conduit gateway and MTAC_LORA card
         * @param on set to true to enable CRC checking
         * @returns MDOT_OK if success
         */
        int32_t setCrc(const bool& on);

        /**
         * Get CRC checking
         * @returns true if CRC checking is enabled
         */
        bool getCrc();

        /**
         * Set ack
         * @param retries 0 to disable acks, otherwise 1 - 8
         * @returns MDOT_OK if success
         */
        int32_t setAck(const uint8_t& retries);

        /**
         * Get ack
         * @returns 0 if acks are disabled, otherwise retries (1 - 8)
         */
        uint8_t getAck();

        /**
         * Set number of packet repeats for unconfirmed frames
         * @param repeat 0 or 1 for no repeats, otherwise 2-15
         * @returns MDOT_OK if success
         */
        int32_t setRepeat(const uint8_t& repeat);

        /**
         * Get number of packet repeats for unconfirmed frames
         * @returns 0 or 1 if no repeats, otherwise 2-15
         */
        uint8_t getRepeat();

        /**
         * Send data to the gateway
         * validates data size (based on spreading factor)
         * @param data a vector of up to 242 bytes (may be less based on spreading factor)
         * @returns MDOT_OK if packet was sent successfully (ACKs disabled), or if an ACK was received (ACKs enabled)
         */
        int32_t send(const std::vector<uint8_t>& data, const bool& blocking = true, const bool& highBw = false);

        /**
         * Inject mac command
         * @param data a vector containing mac commands
         * @returns MDOT_OK
         */
        int32_t injectMacCommand(const std::vector<uint8_t>& data);

        /**
         * Clear MAC command buffer to be sent in next uplink
         * @returns MDOT_OK
         */
        int32_t clearMacCommands();

        /**
         * Get MAC command buffer to be sent in next uplink
         * @returns command bytes
         */
        std::vector<uint8_t> getMacCommands();

        /**
         * Fetch data received from the gateway
         * this function only checks to see if a packet has been received - it does not open a receive window
         * send() must be called before recv()
         * @param data a vector to put the received data into
         * @returns MDOT_OK if packet was successfully received
         */
        int32_t recv(std::vector<uint8_t>& data);

        /**
         * Ping
         * status will be MDOT_OK if ping succeeded
         * @returns ping_response struct containing status, RSSI, and SNR
         */
        ping_response ping();

        /**
         * Get return code string
         * @returns string containing a description of the given error code
         */
        static std::string getReturnCodeString(const int32_t& code);

        /**
         * Get last error
         * @returns string explaining the last error that occured
         */
        std::string getLastError();

        /**
         * Go to sleep
         * @param interval the number of seconds to sleep before waking up if wakeup_mode == RTC_ALARM or RTC_ALARM_OR_INTERRUPT, else ignored
         * @param wakeup_mode RTC_ALARM, INTERRUPT, RTC_ALARM_OR_INTERRUPT
         *      if RTC_ALARM the real time clock is configured to wake the device up after the specified interval
         *      if INTERRUPT the device will wake up on the rising edge of the interrupt pin
         *      if RTC_ALARM_OR_INTERRUPT the device will wake on the first event to occur
         * @param deepsleep if true go into deep sleep mode (lowest power, all memory and registers are lost, peripherals turned off)
         *                  else go into sleep mode (low power, memory and registers are maintained, peripherals stay on)
         *
         * For the MDOT
         *      in sleep mode, the device can be woken up on an XBEE_DI (2-8) pin or by the RTC alarm
         *      in deepsleep mode, the device can only be woken up using the WKUP pin (PA0, XBEE_DIO7) or by the RTC alarm
         * For the XDOT
         *      in sleep mode, the device can be woken up on GPIO (0-3), UART1_RX, WAKE or by the RTC alarm
         *      in deepsleep mode, the device can only be woken up using the WKUP pin (PA0, WAKE) or by the RTC alarm
         */
        void sleep(const uint32_t& interval, const uint8_t& wakeup_mode = RTC_ALARM, const bool& deepsleep = true);

        /**
         * Set wake pin
         * @param pin the pin to use to wake the device from sleep mode
         *      For MDOT, XBEE_DI (2-8)
         *      For XDOT, GPIO (0-3), UART1_RX, or WAKE
         */
        void setWakePin(const PinName& pin);

        /**
         * Get wake pin
         * @returns the pin to use to wake the device from sleep mode
         *      For MDOT, XBEE_DI (2-8)
         *      For XDOT, GPIO (0-3), UART1_RX, or WAKE
         */
        PinName getWakePin();

        /**
         * Write data in a user backup register
         * @param register one of UBR0 through UBR9 for MDOT, one of UBR0 through UBR21 for XDOT
         * @param data user data to back up
         * @returns true if success
         */
        bool writeUserBackupRegister(uint32_t reg, uint32_t data);

        /**
         * Read data in a user backup register
         * @param register one of UBR0 through UBR9 for MDOT, one of UBR0 through UBR21 for XDOT
         * @param data gets set to content of register
         * @returns true if success
         */
        bool readUserBackupRegister(uint32_t reg, uint32_t& data);

        /**
         * Set LBT time in us
         * @param ms time in us
         * @returns true if success
         */
        bool setLbtTimeUs(uint16_t us);

        /**
         * Get LBT time in us
         * @returns LBT time in us
         */
        uint16_t getLbtTimeUs();

        /**
         * Set LBT threshold in dBm
         * @param rssi threshold in dBm
         * @returns true if success
         */
        bool setLbtThreshold(int8_t rssi);

        /**
         * Get LBT threshold in dBm
         * @returns LBT threshold in dBm
         */
        int8_t getLbtThreshold();

#if defined(TARGET_MTS_MDOT_F411RE)
        ///////////////////////////////////////////////////////////////////
        // Filesystem (Non Volatile Memory) Operation Functions for mDot //
        ///////////////////////////////////////////////////////////////////

        // Save user file data to flash
        // file - name of file max 30 chars
        // data - data of file
        // size - size of file
        // returns true if successful
        bool saveUserFile(const char* file, void* data, uint32_t size);

        // Append user file data to flash
        // file - name of file max 30 chars
        // data - data of file
        // size - size of file
        // returns true if successful
        bool appendUserFile(const char* file, void* data, uint32_t size);

        // Read user file data from flash
        // file - name of file max 30 chars
        // data - data of file
        // size - size of file
        // returns true if successful
        bool readUserFile(const char* file, void* data, uint32_t size);

        // Move a user file in flash
        // file     - name of file
        // new_name - new name of file
        // returns true if successful
        bool moveUserFile(const char* file, const char* new_name);

        // Delete user file data from flash
        // file - name of file max 30 chars
        // returns true if successful
        bool deleteUserFile(const char* file);

        // Open user file in flash, max of 4 files open concurrently
        // file - name of file max 30 chars
        // mode - combination of FM_APPEND | FM_TRUNC | FM_CREAT |
        //                       FM_RDONLY | FM_WRONLY | FM_RDWR | FM_DIRECT
        // returns - mdot_file struct, fd field will be a negative value if file could not be opened
        mDot::mdot_file openUserFile(const char* file, int mode);

        // Seek an open file
        // file - mdot file struct
        // offset - offset in bytes
        // whence - where offset is based SEEK_SET, SEEK_CUR, SEEK_END
        // returns true if successful
        bool seekUserFile(mDot::mdot_file& file, size_t offset, int whence);

        // Read bytes from open file
        // file - mdot file struct
        // data - mem location to store data
        // length - number of bytes to read
        // returns - number of bytes read, negative if error
        int readUserFile(mDot::mdot_file& file, void* data, size_t length);

        // Write bytes to open file
        // file - mdot file struct
        // data - data to write
        // length - number of bytes to write
        // returns - number of bytes written, negative if error
        int writeUserFile(mDot::mdot_file& file, void* data, size_t length);

        // Close open file
        // file - mdot file struct
        // returns true if successful
        bool closeUserFile(mDot::mdot_file& file);

        // List user files stored in flash
        std::vector<mDot::mdot_file> listUserFiles();

        // Move file into the firmware upgrade path to be flashed on next boot
        // file - name of file
        // returns true if successful
        bool moveUserFileToFirmwareUpgrade(const char* file);
#else
        ///////////////////////////////////////////////////////////////
        // EEPROM (Non Volatile Memory) Operation Functions for xDot //
        ///////////////////////////////////////////////////////////////
        
        // Write to EEPROM
        // addr - address to write to (0 - 0x17FF)
        // data - data to write
        // size - size of data
        // returns true if successful
        bool nvmWrite(uint16_t addr, void* data, uint16_t size);

        // Read from EEPROM
        // addr - address to read from (0 - 0x17FF)
        // data - buffer for data
        // size - size of buffer
        // returns true if successful
        bool nvmRead(uint16_t addr, void* data, uint16_t size);
#endif /* TARGET_MTS_MDOT_F411RE */

        // get current statistics
        // Join Attempts, Join Fails, Up Packets, Down Packets, Missed Acks
        mdot_stats getStats();

        // reset statistics
        // Join Attempts, Join Fails, Up Packets, Down Packets, Missed Acks
        void resetStats();

        // Convert pin number 2-8 to pin name DIO2-DI8
        static PinName pinNum2Name(uint8_t num);

        // Convert pin name DIO2-DI8 to pin number 2-8
        static uint8_t pinName2Num(PinName name);

        // Convert pin name DIO2-DI8 to string
        static std::string pinName2Str(PinName name);

        uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l);

        /*************************************************************************
         * The following functions are only used by the AT command application and
         * should not be used by standard applications consuming the mDot library
         ************************************************************************/

        // set/get configured baud rate for command port
        // only for use in conjunction with AT interface
        // set function returns MDOT_OK if success
        int32_t setBaud(const uint32_t& baud);
        uint32_t getBaud();

        // set/get baud rate for debug port
        // set function returns MDOT_OK if success
        int32_t setDebugBaud(const uint32_t& baud);
        uint32_t getDebugBaud();

        // set/get command terminal echo
        // set function returns MDOT_OK if success
        int32_t setEcho(const bool& on);
        bool getEcho();

        // set/get command terminal verbose mode
        // set function returns MDOT_OK if success
        int32_t setVerbose(const bool& on);
        bool getVerbose();

        // set/get startup mode
        // COMMAND_MODE (default), starts up ready to accept AT commands
        // SERIAL_MODE, read serial data and send it as LoRa packets
        // set function returns MDOT_OK if success
        int32_t setStartUpMode(const uint8_t& mode);
        uint8_t getStartUpMode();

        int32_t setRxDataRate(const uint8_t& dr);
        uint8_t getRxDataRate();

        // get/set TX/RX frequency
        // if frequency band == FB_868 (Europe), must be between 863000000 - 870000000
        // if frequency band == FB_915 (United States), must be between 902000000-928000000
        // if set to 0, device will hop frequencies
        // set function returns MDOT_OK if success
        int32_t setTxFrequency(const uint32_t& freq);
        uint32_t getTxFrequency();
        int32_t setRxFrequency(const uint32_t& freq);
        uint32_t getRxFrequency();

        // get/set TX/RX inverted
        // true == signal is inverted
        // set function returns MDOT_OK if success
        int32_t setTxInverted(const bool& on);
        bool getTxInverted();
        int32_t setRxInverted(const bool& on);
        bool getRxInverted();

        // get/set RX output mode
        // valid options are HEXADECIMAL and BINARY
        // set function returns MDOT_OK if success
        int32_t setRxOutput(const uint8_t& mode);
        uint8_t getRxOutput();

        // get/set serial wake interval
        // valid values are 2 s - INT_MAX (2147483647) s
        // set function returns MDOT_OK if success
        int32_t setWakeInterval(const uint32_t& interval);
        uint32_t getWakeInterval();

        // get/set serial wake delay
        // valid values are 2 ms - INT_MAX (2147483647) ms
        // set function returns MDOT_OK if success
        int32_t setWakeDelay(const uint32_t& delay);
        uint32_t getWakeDelay();

        // get/set serial receive timeout
        // valid values are 0 ms - 65000 ms
        // set function returns MDOT_OK if success
        int32_t setWakeTimeout(const uint16_t& timeout);
        uint16_t getWakeTimeout();

        // get/set serial wake mode
        // valid values are INTERRUPT or RTC_ALARM
        // set function returns MDOT_OK if success
        int32_t setWakeMode(const uint8_t& delay);
        uint8_t getWakeMode();

        // get/set serial flow control enabled
        // set function returns MDOT_OK if success
        int32_t setFlowControl(const bool& on);
        bool getFlowControl();

        // get/set serial clear on error
        // if enabled the data read from the serial port will be discarded if it cannot be sent or if the send fails
        // set function returns MDOT_OK if success
        int32_t setSerialClearOnError(const bool& on);
        bool getSerialClearOnError();

        // MTS_RADIO_DEBUG_COMMANDS

        /**
         * Disable Duty cycle
         * enables or disables the duty cycle limitations
         * **** ONLY TO BE USED FOR TESTINGS PURPOSES ****
         * **** ALL DEPLOYABLE CODE MUST ADHERE TO LOCAL REGULATIONS ****
         * **** THIS SETTING WILL NOT BE SAVED TO CONFIGURATION *****
         * @param val true to disable duty-cycle (default:false)
         */
        int32_t setDisableDutyCycle(bool val);

        /**
         * Disable Duty cycle
         * **** ONLY TO BE USED FOR TESTINGS PURPOSES ****
         * **** ALL DEPLOYABLE CODE MUST ADHERE TO LOCAL REGULATIONS ****
         * **** THIS SETTING WILL NOT BE SAVED TO CONFIGURATION *****
         * @return true if duty-cycle is disabled (default:false)
         */
        uint8_t getDisableDutyCycle();

        /**
         * LBT RSSI
         * @return the current RSSI on the configured frequency (SetTxFrequency) using configured LBT Time
         */
        int16_t lbtRssi();

        void openRxWindow(uint32_t timeout, uint8_t bandwidth = 0);
        void closeRxWindow();
        void sendContinuous(bool enable=true);
        int32_t setDeviceId(const std::vector<uint8_t>& id);
        int32_t setDefaultFrequencyBand(const uint8_t& band);
        bool saveProtectedConfig();
        void resetRadio();
        int32_t setRadioMode(const uint8_t& mode);
        std::map<uint8_t, uint8_t> dumpRegisters();
        void eraseFlash();

        // deprecated - use setWakeInterval
        int32_t setSerialWakeInterval(const uint32_t& interval);
        // deprecated - use getWakeInterval
        uint32_t getSerialWakeInterval();

        // deprecated - use setWakeDelay
        int32_t setSerialWakeDelay(const uint32_t& delay);
        // deprecated - use setWakeDelay
        uint32_t getSerialWakeDelay();

        // deprecated - use setWakeTimeout
        int32_t setSerialReceiveTimeout(const uint16_t& timeout);
        // deprecated - use getWakeTimeout
        uint16_t getSerialReceiveTimeout();

        void setWakeupCallback(void (*function)(void));

        template<typename T>
        void setWakeupCallback(T *object, void (T::*member)(void)) {
            _wakeup_callback.attach(object, member);
        }

    private:
        mdot_stats _stats;

        FunctionPointer _wakeup_callback;

        bool _standbyFlag;
        bool _testMode;
        uint8_t _savedPort;
        void handleTestModePacket();
        lora::ChannelPlan* _plan;
};

#endif
