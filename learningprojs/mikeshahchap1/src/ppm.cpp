//
// Created by jay shah on 14/02/25.
//

// implementations should always include their own interface files first! for consistency checking!
#include "ppm.h"

PPM::PPM(std::string filename) {
    printf("Constructor called!\n");
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


