#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <sstream>

// Fonction utilitaire pour découper une description en lignes adaptées à une largeur donnée
inline std::vector<std::string> wrapText(const std::string& text, size_t lineWidth) {
    std::vector<std::string> lines;
    std::istringstream words(text);
    std::string word;
    std::string line;

    while (words >> word) {
        if (line.size() + word.size() + 1 > lineWidth) { // Si la ligne est trop longue
            lines.push_back(line);
            line.clear();
        }
        if (!line.empty()) {
            line += " ";
        }
        line += word;
    }
    if (!line.empty()) {
        lines.push_back(line);
    }
    return lines;
}

#endif // UTILITIES_H

