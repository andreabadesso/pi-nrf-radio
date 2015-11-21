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

    /*
     * Blocks the thread for x ms
     *
     * @param {Number} block Time in microseconds to block the thread
     */
    block(us) {
        let start = process.hrtime();
        while(1) {
            let diff = process.hrtime(start);
            if (diff[0] * 1e9 + diff[1] >= us * 1e3) break;
        }

        console.log('Blocked for ' + us + ' us');
    }

    /*
     * Changes the state of the CE pin
     *
     * @param {String} state State to set the CE Pin ('high' or 'low')
     * @param {Boolean} block Time in microseconds to block the thread
     */
    setCE(state, block) {
        this.ce.mode(state);
        if (block) {
            this.block(block);
        }
    }

    /*
     * Executes a command on the radio via spi, returning its response
     *
     * @param {Number} cmd The command to execute on the radio
     * @param {Array} data Array of bytes with data to execute on the radio
     * @param {Function} cb The callback that will be executed on the spi response
     */
    execCommand(cmd, data, cb) {
        let writeBuf,
            readLen = 0;

        data.reverse();
        writeBuf = Buffer([cmd].concat(data));
        data.reverse();

        spi.transfer(writeBuf, readLen && readLen + 1, (err, data) => {
            if (err) {
                return cb(err);
            }

            return cb(null, data && Array.prototype.reverse.call(data.slice(1)));
        });
    }
}
