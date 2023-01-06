#include <Wire.h>

class I2C 
{
    public:
        I2C();
        void scan();
        void attach(TwoWire &wire);

        void write(uint8_t address, uint8_t reg, uint8_t data);
        void write(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length);

        uint8_t read_button_state(uint8_t number);
        
        uint8_t read(uint8_t address, uint8_t reg);
        void read(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length);
        void led_set(uint8_t number, uint8_t red, uint8_t green, uint8_t blue);

        uint8_t selectChannel(uint8_t channel);
        void configure_port(uint8_t number);
    private:
        TwoWire *wire;

        uint8_t input_reg  = 0x00;
        uint8_t output_reg = 0x01;
        uint8_t invert_reg = 0x02;
        uint8_t config_reg = 0x03;

        uint8_t expander_addr = 0x20;
        uint8_t i2c_hub_addr = 0x70;

        uint8_t getAddress(const bool A0, const bool A1, const bool A2);
        // todo: terminator

};
