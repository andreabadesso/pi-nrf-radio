'use strict';

const   GPIO    = require('pi-gpio'),
        SPI     = require('pi-spi');

const   spiDev  = '/dev/spidev0.0';

const   pins    = {
    'CE': 11,
    'IRQ': 15,
    'CSN': 13,
    'LED': 12,
    'PWR': 7
};

class Radio {
    constructor() {
        this.spi = SPI.initialize(spiDev);
        this.ce = GPIO.connect(pins.CE);
        this.csn = GPIO.connect(pins.CSN);
        this.irq = GPIO.connect(pins.IRQ);
    }

    setCE(state, block) {
        ce.mode(state);
    }
}
