const Radio = require('./radio');

let radio = new Radio();

radio.setPWR('high', 10, () => {
    // Aqui dentro
    radio.setCE('low', false, () => {

        radio.setCSN('low', false, () => {

            radio.execCommand(0x20, [0x7C], () => {

                radio.setCSN('high', false, () => {

                    radio.setCSN('low', false, () => {

                        radio.execCommand(0x00, [0xFF], (data) => {
                            console.log(data);
                        });

                    });

                });

            });

        });

    });
});
