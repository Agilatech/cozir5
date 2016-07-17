/**
 * \file Cozir5Drv.h
 *
 *  Created by Scott Erholm on 6/14/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#ifndef __Cozir5Drv__
#define __Cozir5Drv__

#include <iostream>
#include <fstream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "DataManip.h"

#ifdef DEBUG
#  define DPRINT(x) do { std::cerr << x; std::cerr << std::endl; } while (0)
#else
#  define DPRINT(x) do {} while (0)
#endif


static const std::string name = "COZIR5";
static const std::string version = "0.9.0";

static const int numValues = 1;

static const std::string valueNames[numValues] = {"co2"};
static const std::string valueTypes[numValues] = {"integer"};

static const int COZIR5_PPM_MULTIPLIER = 10;
static const int COZIR5_READ_DELAY = 200; // milliseconds
static const int COZIR5_NUM_RETRY = 10;

class Cozir5Drv {
    
public:
    Cozir5Drv(std::string devfile);
    
    static std::string getVersion();
    static std::string getDeviceName();
    static int getNumValues();
    static std::string getTypeAtIndex(int index);
    static std::string getNameAtIndex(int index);
    
    bool isActive();
    std::string getValueAtIndex(int index);
    
protected:
    
    bool initialize();
    std::string readValue0();
    
private:
    
    bool readSerial();
    bool active = false;
    
    const char *serialDevice;
    char receiveBuffer[100];
    int serialFile;
    
};

#endif /* defined(__Cozir5Drv__) */
