#ifndef MTSLOG_H
#define MTSLOG_H

#include <string>

inline const char* className(const std::string& prettyFunction)
{
    size_t colons = prettyFunction.find_last_of("::");
    if (colons == std::string::npos)
        return "";
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;

    return prettyFunction.substr(begin,end).c_str();
}

#define __CLASSNAME__ className(__PRETTY_FUNCTION__)


#ifdef MTS_DEBUG
#define logFatal(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::FATAL_LEVEL, "%s:%s:%d| [%s] " format "\r\n", __CLASSNAME__, __func__, __LINE__, mts::MTSLog::FATAL_LABEL, ##__VA_ARGS__)
#define logError(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::ERROR_LEVEL, "%s:%s:%d| [%s] " format "\r\n", __CLASSNAME__, __func__, __LINE__, mts::MTSLog::ERROR_LABEL, ##__VA_ARGS__)
#define logWarning(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::WARNING_LEVEL, "%s:%s:%d| [%s] " format "\r\n", __CLASSNAME__, __func__, __LINE__, mts::MTSLog::WARNING_LABEL, ##__VA_ARGS__)
#define logInfo(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::INFO_LEVEL, "%s:%s:%d| [%s] " format "\r\n", __CLASSNAME__, __func__, __LINE__, mts::MTSLog::INFO_LABEL, ##__VA_ARGS__)
#define logDebug(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::DEBUG_LEVEL, "%s:%s:%d| [%s] " format "\r\n", __CLASSNAME__, __func__, __LINE__, mts::MTSLog::DEBUG_LABEL, ##__VA_ARGS__)
#define logTrace(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::TRACE_LEVEL, "%s:%s:%d| [%s] " format "\r\n", __CLASSNAME__, __func__, __LINE__, mts::MTSLog::TRACE_LABEL, ##__VA_ARGS__)
#else
#define logFatal(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::FATAL_LEVEL, "[%s] " format "\r\n", mts::MTSLog::FATAL_LABEL, ##__VA_ARGS__)
#define logError(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::ERROR_LEVEL, "[%s] " format "\r\n", mts::MTSLog::ERROR_LABEL, ##__VA_ARGS__)
#define logWarning(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::WARNING_LEVEL, "[%s] " format "\r\n", mts::MTSLog::WARNING_LABEL, ##__VA_ARGS__)
#define logInfo(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::INFO_LEVEL, "[%s] " format "\r\n", mts::MTSLog::INFO_LABEL, ##__VA_ARGS__)
#define logDebug(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::DEBUG_LEVEL, "[%s] " format "\r\n", mts::MTSLog::DEBUG_LABEL, ##__VA_ARGS__)
#define logTrace(format, ...) \
    mts::MTSLog::printMessage(mts::MTSLog::TRACE_LEVEL, "[%s] " format "\r\n", mts::MTSLog::TRACE_LABEL, ##__VA_ARGS__)
#endif

namespace mts {

class MTSLog
{
public:

    /** Enum of log levels.
     */
    enum logLevel {
        NONE_LEVEL = 0,
        FATAL_LEVEL = 1,
        ERROR_LEVEL = 2,
        WARNING_LEVEL = 3,
        INFO_LEVEL = 4,
        DEBUG_LEVEL = 5,
        TRACE_LEVEL = 6
    };

    /** Print log message.
     */
    static void printMessage(int level, const char* format, ...);

    /** Determine if the given level is currently printable.
     */
    static bool printable(int level);

    /** Set log level
     * Messages with lower priority than the current level will not be printed.
     * If the level is set to NONE, no messages will print.
     */
    static void setLogLevel(int level);

    /** Get the current log level.
     */
    static int getLogLevel();

    /** Get string representation of the current log level.
     */
    static const char* getLogLevelString();

    static const char* NONE_LABEL;
    static const char* FATAL_LABEL;
    static const char* ERROR_LABEL;
    static const char* WARNING_LABEL;
    static const char* INFO_LABEL;
    static const char* DEBUG_LABEL;
    static const char* TRACE_LABEL;

private:

    /** Constructor
     */
    MTSLog();

    static int currentLevel;

};

}

#endif
