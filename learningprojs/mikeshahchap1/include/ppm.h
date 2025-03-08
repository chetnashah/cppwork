//
// Created by jay shah on 14/02/25.
//

#ifndef PPM_H
#define PPM_H

#include<string>
#include "pixel.h"
class PPM {
public:
    // constructor signature declaration mandatory in header files!
    PPM(std::string filename);

    ~PPM();

    void savePPM(std::string outFileName);
    void lighten();
    void darken();

private:
    std::vector<pixel> mPixels;
    int mWidth;
    int mHeight;
    int mMaxRange;
};



#endif //PPM_H
