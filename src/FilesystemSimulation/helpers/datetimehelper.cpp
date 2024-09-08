#include "datetimehelper.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string DateTimeHelper::getCurrentDateTimeFormated() {
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* localTime = std::localtime(&currentTime);

    std::ostringstream oss;
    oss.str("");
    oss << std::put_time(localTime, "%d.%m.%Y %H:%M");

    std::string dateString = oss.str();

    return dateString;
}

long long  DateTimeHelper::getCurrentDateTimeMS() {

    auto now = std::chrono::system_clock::now();
    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    return ms;
}

std::string DateTimeHelper::msToTimestamp(long long ms) {
    std::chrono::milliseconds duration(ms);
    std::chrono::system_clock::time_point timePoint(duration);

    std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);
    std::tm* localTime = std::localtime(&timeT);

    auto msPart = duration.count() % 1000;

    std::ostringstream oss;
    oss << std::put_time(localTime, "%d.%m.%Y %H:%M");

    return oss.str();
}

long long DateTimeHelper::timestampToMs(const std::string& timestamp) {
    std::tm t = {};
    int milliseconds;

    std::istringstream iss(timestamp);
    iss >> std::get_time(&t, "%d.%m.%Y %H:%M");
    char dot;
    iss >> dot >> milliseconds;

    std::time_t timeT = std::mktime(&t);

    long long msSinceEpoch = static_cast<long long>(timeT) * 1000 + milliseconds;

    return msSinceEpoch;
}

