//
// Created by jay shah on 14/02/25.
//

// implementations should always include their own interface files first! for consistency checking!
#include "ppm.h"
#include<fstream>
#include <iostream>
#include <string>
#include<sstream>

PPM::PPM(std::string filename) {
    printf("Constructor called!\n");
    std::ifstream file(filename);
    std::cout << "Opening file " << filename << std::endl;
    if (!file.is_open()) {
        std::cout << "Could not open file " << filename << std::endl;
        return;
    }
    bool foundP3 = false;
    bool foundDimensions = false;
    bool foundRange = false;
    bool readingPixels = false;

    std::string line;
    while (std::getline(file, line)) {
        // skip lines with comments
        if (line[0] == '#') {
            continue;
        }
        // put actual line in a different stringstream
        std::stringstream sstrLine {line};
        std::string chunk;
        while (sstrLine >> chunk) {
            // processing all chunks in lines
            // state machine with if-else
            if (foundP3 == false && chunk == "P3") {
                std::cout << "Found P3" << std::endl;
                foundP3 = true;
            }
            else if (foundDimensions == false) {
                mWidth = std::stoi(chunk);
                sstrLine >> chunk;
                mHeight = std::stoi(chunk);
                foundDimensions = true;
            } else if (foundRange == false) {
                mMaxRange = std::stoi(chunk);
                foundRange = true;
            } else {
                mPixels.push_back(std::stoi(chunk));
                readingPixels = true;
            }

            // std::cout << chunk << std::endl;
        }
    }
}
PPM::~PPM() {
    printf("Destructor called!\n");
}

void PPM::darken() {
    printf("PPM Darken\n");
}

void PPM::lighten() {
    printf("PPM lightend\n");
}

void PPM::savePPM(std::string outFileName) {
    printf("Saving image to %s\n", outFileName.c_str());
}


