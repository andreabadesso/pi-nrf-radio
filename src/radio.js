'use strict';

const   GPIO = require('pi-gpio'),
        SPI  = require('pi-spi');

const spiDev = '/dev/spidev0.0';

const ports = [{
    name: 'CE',
    pin: 11,
    mode: 'output'
}, {
    name: 'IRQ',
    pin: 15,
    mode: 'input'
}, {
    name: 'CSN',
    pin: 13,
    mode: 'output'
}, {
    name: 'LED',
    pin: 12,
    mode: 'output'
}, {
    name: 'PWR',
    pin: 7,
    mode: 'output'
}];

const   pins    = {
    'CE': 11,
    'IRQ': 15,
    'CSN': 13,
    'LED': 12,
    'PWR': 7
};

class Radio extends EventEmitter {

    constructor() {
        super();
        this.spi = SPI.initialize(spiDev);

        let openFns = ports.map((port) => {
            return (callback) => {
                GPIO.open(port, port.mode, callback);
            };
        });

        async.parallel(openFns, (err) => {
            if (err) {
                console.log(err);
                process.exit(1);
            }

            this.emit('ready');
            // Testing LED
            gpio.write(12, 1);
        });
    }

    /*
     * Blocks the thread for x ms
     *
     * @param {Number} block Time in microseconds to block the thread
     */
    block(us) {
        let start = process.hrtime();
        while (1) {
            let diff = process.hrtime(start);
            if (diff[0] * 1e9 + diff[1] >= us * 1e3) break;
        }

        console.log('Blocked for ' + us + ' us');
    }

    /*
     * Changes the state of the CE pin
     *
     * @param {String} state State to set the CE Pin (1 or 0)
     * @param {Boolean} block Time in microseconds to block the thread
     */
    setCE(state, block, cb) {
        GPIO.write(pins.CE, state, cb);
        if (block) {
            this.block(block);
        }
    }

    /*
     * Changes the state of the CSN pin
     *
     * @param {String} state State to set the CSN Pin (1 or 0)
     * @param {Boolean} block Time in microseconds to block the thread
     */
    setCSN(state, block, cb) {
        GPIO.write(pins.CSN, state, cb);
        if (block) {
            this.block(block);
        }
    }

    /*
     * Pulses the CSN by setting it to low immediately after setting it
     * to high
     */
    pulseCSN(block) {
        this.setCSN(1, () => {
            this.setCSN(0, block);
        });
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

        this.spi.transfer(writeBuf, readLen && readLen + 1, (err, data) => {
            if (err) {
                return cb(err);
            }

            return cb(null, data && Array.prototype.reverse.call(data.slice(1)));
        });
    }
}

module.exports = Radio;
