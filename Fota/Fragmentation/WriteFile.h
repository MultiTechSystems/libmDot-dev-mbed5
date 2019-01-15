#ifndef _WRITE_FILE_H
#define _WRITE_FILE_H
#include "mDot.h"
#ifdef FOTA
class WriteFile {
    public:
        WriteFile(mDot* dot);
        ~WriteFile();
        void writeFile(uint8_t* buffer, uint32_t size);
        void readFile(uint8_t* buffer, uint32_t size);
        void seekFile(uint32_t index);
        void createFile(uint16_t numOfFrags, uint8_t fragSize, uint8_t padding); 
        uint64_t completeFile(uint16_t numOfFrags, uint8_t padding, uint32_t total_frags);
        void cleanUp(bool complete);
        void reset();

    private:
        mDot* _dot;
        char* _temp;
        char* _firmware;
        uint8_t* _frag;
        uint8_t _padding;
        uint8_t _frag_size;
        uint16_t _num_frags;
        mDot::mdot_file _upgrade, _file;
}; 
#endif
#endif // WRITE_FILE_H
