
// Created by Gian-Luca Afting on 04.08.24.
//

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qobject.h>
#include "PathHelper.h"
#include <QVariant>
#include <sstream>


// Splittet einen String anhand eines chars.
std::vector<std::string> PathHelper::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::string PathHelper::join(const std::vector<std::string> &vec, char delimiter) {
    if (vec.empty()) return "";

    std::ostringstream oss;
    auto it = vec.begin();
    oss << *it++; // Output the first element

    while (it != vec.end()) {
        oss << delimiter << *it++;
    }

    return oss.str();
}

// Entfernt alles nach dem .
std::string PathHelper::removeFileType(const std::string &input) {
    return PathHelper::removeAfter(input, '.');
}

// Entfernt alles nach dem .
std::string PathHelper::removeAfter(const std::string &input, const char delimiter) {
    int dotIndex = input.find(delimiter);

    if (dotIndex != -1) {
        return input.substr(0, dotIndex);
    }

    return input;
}

// Exteract the file ending from the select option.
std::string PathHelper::extractFileEndingFromOption(const std::string &input) {
    std::size_t openParen = input.find('(');
    std::size_t closeParen = input.find(')');

    if (openParen != std::string::npos && closeParen != std::string::npos && openParen < closeParen) {
        std::string extensionPart = input.substr(openParen + 1, closeParen - openParen - 1);

        std::size_t dotPos = extensionPart.find('.');

        if (dotPos != std::string::npos) {
            return extensionPart.substr(dotPos);
        }
    }


    return "";
}

QString PathHelper::buildPath(const QModelIndex &index) {
    if(!index.isValid()) {
        return QString();
    }

    QString path;
    QModelIndex current = index;

    while(current.isValid()) {
        QString itemText = current.data(Qt::DisplayRole).toString();

        if(!path.isEmpty()) {
            path.prepend("/");
        }
        path.prepend(itemText);

        current = current.parent();
    }

    return path;
}

std::string PathHelper::getDirectoryPath(std::string path, char delimiter){
    std::vector<std::string> split = PathHelper::split(path, delimiter);
    if(split.back().find(".")){
        split.pop_back();
    }
    return PathHelper::join(split, delimiter);
}
