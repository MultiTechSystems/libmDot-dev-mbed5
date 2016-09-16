#ifndef MTSTEXT_H
#define MTSTEXT_H

#include <string>
#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace mts
{

/** This class contains a number of static methods for manipulating strings and other
* text data.
*/
class Text
{
public:
    /** This static method can be used to pull out a string at the next line break. A
    * break can either be a newline '\n', carriage return '\r' or both.
    *
    * @param source the source string to look for the line break on.
    * @param start the start postion within the string to begin looking for the line
    * break.
    * @param cursor this value will be updated with the index for the next available character
    * after the line break. If a line break is not found returns -1.
    * @returns the string beginning with the start index up to including the line breaks.
    */
    static std::string getLine(const std::string& source, const size_t& start, size_t& cursor);

    /** This is a static method for splitting strings using a delimeter value.
    *
    * @param str the string to try and split.
    * @param delimiter the delimeter value to split on as a character.
    * @param limit the maximum number of splits. If equal to 0 it splits as amny times as possible.
    * The default is 0.
    * @returns an ordered vector of strings conatining the splits of the original string.
    */
    static std::vector<std::string> split(const std::string& str, char delimiter, int limit = 0);

    /** This is a static method for splitting strings using a delimeter value.
    *
    * @param str the string to try and split.
    * @param delimiter the delimeter value to split on as a string.
    * @param limit the maximum number of splits. If equal to 0 it splits as amny times as possible.
    * The default is 0.
    * @returns an ordered vector of strings conatining the splits of the original string.
    */
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter, int limit = 0);
    
    static std::string readString(char* index, int length);

    static std::string toUpper(const std::string str);

    static std::string float2String(double val, int precision);

    static std::string bin2hexString(const std::vector<uint8_t>& data, const char* delim = "", bool leadingZeros = false, bool bytePadding = true);

    static std::string bin2hexString(const uint8_t* data, const uint32_t len, const char* delim = "", bool leadingZeros = false, bool bytePadding = true);

    static std::string bin2base64(const std::vector<uint8_t>& data);

    static std::string bin2base64(const uint8_t* data, size_t size);

    static bool base642bin(const std::string in, std::vector<uint8_t>& out);

    static void ltrim(std::string& str, const char* args);

    static void rtrim(std::string& str, const char* args);

    static void trim(std::string& str, const char* args);

private:
    // Safety for class with only static methods
    Text();
    Text(const Text& other);
    Text& operator=(const Text& other);
};

}

#endif
