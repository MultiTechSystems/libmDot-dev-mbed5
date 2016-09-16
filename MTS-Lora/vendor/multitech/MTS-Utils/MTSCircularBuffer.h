#ifndef MTSCIRCULARBUFFER_H
#define MTSCIRCULARBUFFER_H

#include "Utils.h"

namespace mts
{

/** This class provides a circular byte buffer meant for temporary storage
* during IO transactions.  It contains many of the common methods you
* would expect from a circular buffer like read, write, and various
* methods for checking the size or status.  It should be noted that
* this class does not include any special code for thread safety like
* a lock.  In most cases this is not problematic, but is something
* to be aware of.
*/
class MTSCircularBuffer
{
public:
    /** Creates an MTSCircularBuffer object with the specified static size.
    *
    * @prarm bufferSize size of the buffer in bytes.
    */
    MTSCircularBuffer(int bufferSize);

    /** Destructs an MTSCircularBuffer object and frees all related resources.
    */
    ~MTSCircularBuffer();

    /** This method enables bulk reads from the buffer.  If more data is
    * requested then available it simply returns all remaining data within the
    * buffer.
    *
    * @param data the buffer where data read will be added to.
    * @param length the amount of data in bytes to be read into the buffer.
    * @returns the total number of bytes that were read.
    */
    int read(char* data, int length);

    /** This method reads a single byte from the buffer.
    *
    * @param data char where the read byte will be stored.
    * @returns 1 if byte is read or 0 if no bytes available.
    */
    int read(char& data);

    /** This method enables bulk writes to the buffer. If more data
    * is requested to be written then space available the method writes
    * as much data as possible and returns the actual amount written.
    *
    * @param data the byte array to be written.
    * @param length the length of data to be written from the data paramter.
    * @returns the number of bytes written to the buffer, which is 0 if
    * the buffer is full.
    */
    int write(const char* data, int length);

    /** This method writes a signle byte as a char to the buffer.
    *
    * @param data the byte to be written as a char.
    * @returns 1 if the byte was written or 0 if the buffer was full.
    */
    int write(char data);

    /** This method is used to setup a callback funtion when the buffer reaches
    * a certain threshold. The threshold condition is checked after every read
    * and write call is completed. The condition is made up of both a threshold
    * value and operator. An example that would trigger a callback is if the
    * threshold was 10, the operator GREATER, and there were 12 bytes added to an
    * empty buffer.
    *
    * @param tptr a pointer to the object to be called when the condition is met.
    * @param mptr a pointer to the function within the object to be called when
    * the condition is met.
    * @param threshold the value in bytes to be used as part of the condition.
    * @param op the operator to be used in conjunction with the threshold
    * as part of the condition.
    */
    template<typename T>
    void attach(T *tptr, void( T::*mptr)(void), int threshold, RelationalOperator op) {
        _threshold = threshold;
        _op = op;
        notify.attach(tptr, mptr);
    }

    /** This method is used to setup a callback funtion when the buffer reaches
    * a certain threshold. The threshold condition is checked after every read
    * and write call is completed. The condition is made up of both a threshold
    * value and operator. An example that would trigger a callback is if the
    * threshold was 10, the operator GREATER, and there were 12 bytes added to an
    * empty buffer.
    *
    * @param fptr a pointer to the static function to be called when the condition
    * is met.
    * @param threshold the value in bytes to be used as part of the condition.
    * @param op the operator to be used in conjunction with the threshold
    * as part of the condition.
    */
    void attach(void(*fptr)(void), int threshold, RelationalOperator op) {
        _threshold = threshold;
        _op = op;
        notify.attach(fptr);
    }

    /** This method returns the size of the storage space currently allocated for
    * the buffer. This value is equivalent to the one passed into the constructor.
    * This value is equal or greater than the size() of the buffer.
    *
    * @returns the allocated size of the buffer in bytes.
    */
    int capacity();

    /** This method returns the amount of space left for writing.
    *
    * @returns numbers of unused bytes in buffer.
    */
    int remaining();

    /** This method returns the number of bytes available for reading.
    *
    * @returns number of bytes currently in buffer.
    */
    int size();

    /** This method returns whether the buffer is full.
    *
    * @returns true if full, otherwise false.
    */
    bool isFull();

    /** This method returns whether the buffer is empty.
    *
    * @returns true if empty, otherwise false.
    */
    bool isEmpty();

    /** This method clears the buffer. This is done through
    * setting the internal read and write indexes to the same
    * value and is therefore not an expensive operation.
    */
    void clear();


private:
    int bufferSize; // total size of the buffer
    char* buffer; // internal byte buffer as a character buffer
    int readIndex; // read index for circular buffer
    int writeIndex; // write index for circular buffer
    int bytes; // available data
    FunctionPointer notify; // function pointer used for the internal callback notification
    int _threshold; // threshold for the notification
    RelationalOperator _op; // operator that determines the direction of the threshold
    void checkThreshold(); // private function that checks thresholds and processes notifications
};

}

#endif /* MTSCIRCULARBUFFER_H */
