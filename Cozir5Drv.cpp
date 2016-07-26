/**
 * \file Cozir5Drv.cpp
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "Cozir5Drv.h"

Cozir5Drv::Cozir5Drv(std::string devfile) {
    
    ///dev/ttyO2
    this->serialDevice = devfile.c_str();
    
    if (initialize()) {
        this->active = true;
    }
    else {
        std::cerr << name << " did not initialize. " << name << " is inactive" << std::endl;
    }
    
}

std::string Cozir5Drv::getVersion() {
    return name + " " + version;
}

std::string Cozir5Drv::getDeviceName() {
    return name;
}

int Cozir5Drv::getNumValues() {
    return numValues;
}

std::string Cozir5Drv::getTypeAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueTypes[index];
}

std::string Cozir5Drv::getNameAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueNames[index];
}

bool Cozir5Drv::isActive() {
    return this->active;
}

std::string Cozir5Drv::getValueAtIndex(int index) {
    
    if (!this->active) {
        return "none";
    }

    if (index == 0) {
        return this->readValue0();
    }
    else {
        return "none";
    }
    
}

bool Cozir5Drv::initialize() {
    
    if ((this->serialFile = open(this->serialDevice, O_RDWR | O_NOCTTY | O_NDELAY)) < 0 ) {
        std::cerr << "COZIR5 : Failed to open serial device file " << this->serialDevice << std::endl;
        return false;
    }
    
    struct termios options;
    tcgetattr(this->serialFile, &options);
    
    // needed by the COZIR sensor only!
    cfmakeraw(&options);
    
    // Set up the communications options:
    // 9600 baud, 8-bit, enable receiver, no modem control lines
    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
    tcflush(this->serialFile, TCIFLUSH);             //discard file information not transmitted
    tcsetattr(this->serialFile, TCSANOW, &options);  //changes occur immmediately
    
    // Put the device into polling command mode
    write(this->serialFile, "K 2\r\n", 5);
    usleep(COZIR5_READ_DELAY * 1000);
    readSerial();
    
    if (strncmp(" K 00002", this->receiveBuffer, 8) != 0) {
        std::cerr << "COZIR5 Received '" << this->receiveBuffer << "' in response to 'K 2\\n'" << std::endl;
        return false;
    }
    
    return true;
}

std::string Cozir5Drv::readValue0() {
    
    if (!this->active) {
        return "none";
    }
    
    uint16_t co2 = 0;
    int badValCount = 0;
    bool validValue = false;
    
    write(this->serialFile, "Z\r\n",3);
    usleep(COZIR5_READ_DELAY * 1000);
    
    while (!validValue) {
        if (readSerial()) {
            
            std::string buff(this->receiveBuffer);
            
            std::string co2String = buff.substr(3,5);
            
            co2 = std::stoi(co2String, nullptr, 10);
            
            co2 *= COZIR5_PPM_MULTIPLIER;
            
            if ((co2 > 100) && (co2 < 50020)) {
                validValue = true;
            }
            else {
                std::cerr << "COZIR5 : value out range. Received : " << co2 << std::endl;
                badValCount++;
                usleep(COZIR5_READ_DELAY * 1000);
            }
        }
        else {
            std::cerr << "COZIR5 : Serial read error. " << std::endl;
            badValCount++;
            usleep(COZIR5_READ_DELAY * 1000);
        }
        
        // whew, we can't keep trying forever!
        if (badValCount > COZIR5_NUM_RETRY) {
            std::cerr << "COZIR5 : Number of retries exceeded. " << std::endl;
            break;
        }
    }
    
    // convert the float to a string with 1 decimal place
    return DataManip::dataToString(co2);
}

bool Cozir5Drv::readSerial() {
    
    long count;
    
    if ((count = read(this->serialFile, (void*)this->receiveBuffer, 100)) < 0) {
        return false;
    }
    else {
        this->receiveBuffer[count] = 0;
    }
    return true;
}






