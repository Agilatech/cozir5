##Node addon for hardware COZIR5 CO2 sensor

#####This addon should work on any Linux platform, but has only been tested on BBB

###Install

```
npm install @agilatech/cozir5
```
```
git clone https://github.com/Agilatech/cozir5.git
node-gyp configure build
```

###Usage
#####Load the module and create and instance
```
const addon = require('cozir5');

// create an instance on the /dev/ttyO2 serial device file
const cozir5 = new addon.Cozir5('/dev/ttyO2');
```
#####Get basic device info
```
const name = cozir5.deviceName();  // returns string with name of device
const version = cozir5.deviceVersion(); // returns this software version
const active = cozir5.deviceActive(); // true if active, false if inactive
const numVals =  cozir5.deviceNumValues(); // returns the number of paramters sensed
```
####Get paramter info and values
```
const paramName0 = cozir5.nameAtIndex(0);
const paramType0 = cozir5.typeAtIndex(0);
const paramVal0  = cozir5.valueAtIndexSync(0);
```
####Asynchronous value collection is also available
```
cozir5.valueAtIndex(0, function(err, val) {
    if (err) {
        console.log(err);
    }
    else {
        console.log(`Asynchronous call return: ${val}`);
    }
});
```

###Dependencies
* node-gyp

