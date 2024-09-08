#ifndef DATETIMEHELPER_H
#define DATETIMEHELPER_H

#include <string>

class DateTimeHelper {
public:
    static std::string getCurrentDateTimeFormated();
    static long long getCurrentDateTimeMS();
    static std::string msToTimestamp(long long ms);
    static long long timestampToMs(const std::string &timestamp);
};

#endif // DATETIMEHELPER_H
