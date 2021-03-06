/**
 * \file Cozir5Drv.h
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __Cozir5Drv__
#define __Cozir5Drv__

#include <iostream>
#include <fstream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "Device.h"
#include "DataManip.h"


class Cozir5Drv : public Device {
    
public:
    Cozir5Drv(std::string devfile);
    virtual std::string getValueAtIndex(int index);
    
    static const int NUM_VALUES = 1;
    
protected:
    
    virtual bool initialize();
    virtual std::string readValue0();
    
private:
    
    // Create an array of read functions, so that multiple functions can be easily called
    typedef std::string(Cozir5Drv::*readValueType)();
    readValueType readFunction[NUM_VALUES] = { &Cozir5Drv::readValue0 };
    
    static const int COZIR5_PPM_MULTIPLIER = 10;
    static const int COZIR5_READ_DELAY = 200; // milliseconds
    static const int COZIR5_NUM_RETRY = 10;

    bool readSerial();
    
    const char *serialDevice;
    char receiveBuffer[100];
    int serialFile;
    
};

#endif /* defined(__Cozir5Drv__) */
