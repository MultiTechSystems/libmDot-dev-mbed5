#ifndef _FRAGMENTATION_SESSION_H
#define _FRAGMENTATION_SESSION_H
#ifdef FOTA

#include "mbed.h"
#include "FragmentationMath.h"
#include "mDot.h"
#include "WriteFile.h"

#define MAX_PARITY 400
#define FRAG_OFFSET 3 
#define MULTICAST_SESSIONS 3

class FragmentationSession {

    public:
        FragmentationSession(mDot* dot, std::vector<uint8_t>* ret, bool* filled, uint32_t* delay);
        ~FragmentationSession();
        void processCmd(uint8_t* payload, uint8_t size);
        void reset();

    private:

        typedef struct {
            uint8_t  Padding;
            uint8_t  FragmentSize;
            uint16_t NumberOfFragments;
        } FragmentationSessionOpts_t;

        typedef struct {
            bool reset;
            bool failed;
            uint8_t fragMatrix;
            uint8_t blockAckDelay;
            uint8_t total_sessions;
            uint8_t mcGroupBitMask;
            uint8_t current_session;
            uint16_t index;
            uint16_t last_frag_num;
            uint32_t total_frags;
            FragmentationMath* math;
            FragmentationSessionOpts_t opts;
        } fragGroup;


        enum FragResult {
            FRAG_OK,
            FRAG_SIZE_INCORRECT,
            FRAG_FLASH_WRITE_ERROR,
            FRAG_NO_MEMORY,
            FRAG_COMPLETE
        };

        enum FragCmd {
            PACKAGE_VERSION_FRAG,
            FRAG_STATUS,
            FRAG_SESSION_SETUP,
            FRAG_SESSION_DELETE,
            DATA_FRAGMENT = 0x08,
            CHECKSUM = 0x80
        };

        void reset(uint16_t num);
        void upgradeFile(uint8_t fragIndex);
        bool process_frame(uint8_t fragIndex,uint16_t index, uint8_t buffer[], size_t size);
        
        mDot* _dot;
        WriteFile* _fh;
        fragGroup fg[MULTICAST_SESSIONS];

        uint8_t ans;
        bool* _filled;
        uint32_t* _delay;
        uint8_t fragIndex;
        std::vector<uint8_t>* _ret;
        std::string _org_class;
};
#endif
#endif // _FRAGMENTATION_SESSION_H
