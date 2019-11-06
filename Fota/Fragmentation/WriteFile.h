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

#ifndef _WRITE_FILE_H
#define _WRITE_FILE_H
#include "mDot.h"
#ifdef FOTA
class WriteFile {
    public:
        WriteFile(mDot* dot);
        ~WriteFile();
        int writeFile(uint8_t* buffer, uint32_t size);
        int readFile(uint8_t* buffer, uint32_t size);
        int seekFile(uint32_t index);
        int createFile(uint16_t numOfFrags, uint8_t fragSize, uint8_t padding);
        uint64_t completeFile(uint16_t numOfFrags, uint8_t padding, uint32_t total_frags);
        void cleanUp(bool complete);
        void reset();

    private:
        mDot* _dot;
        char* _temp;
        char _firmware[2];
        uint8_t* _frag;
        uint8_t _padding;
        uint8_t _frag_size;
        uint16_t _num_frags;
        mDot::mdot_file _upgrade, _file;
};
#endif
#endif // WRITE_FILE_H
