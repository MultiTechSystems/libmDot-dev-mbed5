#ifndef FOTA_H
#define FOTA_H
#include "mDot.h"
#ifdef FOTA
#include "FragmentationSession.h"
#endif
#include "MulticastGroup.h"

class Fota {

    public:
        Fota(mDot* dot);
        ~Fota();
      
        static Fota* getInstance(mDot* dot);
        static Fota* getInstance();
        void processCmd(uint8_t* payload, uint8_t port, uint8_t size);
        void reset();
        void enable(bool enabled);
        bool enable();
        void fixEventQueue();
        int32_t timeToStart();

    private:
        static void start();
        
        bool _enabled;
        Thread _send_thread;
        uint8_t* p[242];
        static Fota* _instance;
        mDot* _dot;
#ifdef FOTA
        FragmentationSession* _frag_session;
#endif
        MulticastGroup* _mc_group; 
};
#endif
