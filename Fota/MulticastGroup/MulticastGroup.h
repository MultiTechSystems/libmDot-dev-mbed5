/**********************************************************************
* COPYRIGHT 2018 MULTI-TECH SYSTEMS, INC.
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
        void setClockOffset(int32_t offset);

    private:
        enum MulticastCommands {
            PACKAGE_VERSION,
            STATUS,
            SETUP,
            DELETE,
            CLASS_C_SESSION,
            CLASS_B_SESSION_REQ
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
            int8_t periodicity;
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
        int32_t _clk_sync;

        mDot* _dot;
        Thread _event_thread;
        EventQueue _switch_class_queue;
        mcgroup _mcGroup[MULTICAST_SESSIONS];
        std::vector<uint8_t>* _ret;
        std::string _org_class;

        void setupClassB(uint8_t id);
        void setupClassC(uint8_t id);
        static void switchClass(uint32_t freq, uint8_t dr, std::string newClass);
};
#endif
