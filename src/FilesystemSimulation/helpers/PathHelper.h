//
// Created by Gian-Luca Afting on 04.08.24.
//

#ifndef QTTEST_PATHHELPER_H
#define QTTEST_PATHHELPER_H


#include <QtCore/qobject.h>

class PathHelper {
public:
    static std::vector<std::string> split(const std::string &str, char delimiter);
    static std::string join(const std::vector<std::string> &vec, char delimiter);
    static std::string removeFileType(const std::string &input);
    static std::string removeAfter(const std::string &input, const char delimiter);
    static std::string extractFileEndingFromOption(const std::string &input);
    static QString buildPath(const QModelIndex &index);
    static std::string getDirectoryPath(const std::string path, char delimiter);
};


#endif //QTTEST_PATHHELPER_H
