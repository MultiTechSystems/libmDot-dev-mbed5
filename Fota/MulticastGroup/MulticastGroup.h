#ifndef MULTICASTGROUP_H
#define MULTICASTGROUP_H
#include "mDot.h"
#include "mbed.h"
#define GPS_EPOCH 315986400
#define MULTICAST_SESSIONS 3

class MulticastGroup {
    public:
        MulticastGroup(mDot* dot, std::vector<uint8_t>* ret, bool* filled);
        ~MulticastGroup();
        void reset();
        //void newTime();
        void processCmd(uint8_t* payload, uint8_t size);
        int32_t timeToStart();
        void fixEventQueue();

    private:
        enum McGroup {
            PACKAGE_VERSION_MC,
            STATUS,
            SETUP,
            DELETE,
            CLASS_C_SESSION,
            DATA_BLOCK_AUTH
        };

        typedef struct {
            bool valid;
            uint8_t dr;
            uint8_t fragGroup;
            uint16_t timeout;
            uint32_t tme;
            uint32_t freq;
            uint32_t addr;
            uint32_t max_frame_count;
            int32_t timetostart;
            int32_t class_c_end;
            int32_t class_c_start;
            time_t time_setup;
        } mcgroup;
        
        bool* _filled;
        uint8_t _groupId;
        uint8_t _ans;
        uint8_t _delay;
        uint8_t _token;
        uint8_t _dr;
        uint32_t _freq;
        uint32_t _frame_count;
        time_t _now;
        
        mDot* _dot; 
        Thread _event_thread;
        EventQueue _switch_class_queue;
        mcgroup _mcGroup[MULTICAST_SESSIONS];
        std::vector<uint8_t>* _ret;
        std::string _org_class;

        void setupClassC(uint8_t id);
        static void switchClass(uint32_t freq, uint8_t dr, std::string newClass);
};
#endif
