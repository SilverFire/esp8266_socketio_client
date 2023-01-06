#include <I2C.h>
#include <Wire.h>
#include <HardwareSerial.h>

I2C::I2C()
{
}

void I2C::attach(TwoWire &wire)
{
    this->wire = &wire;
}

uint8_t I2C::getAddress(const bool A0, const bool A1, const bool A2)
{
    uint8_t addr = i2c_hub_addr;

    if (A2) addr |= 0x04;
    if (A1) addr |= 0x02;
    if (A0) addr |= 0x01;

    return addr;
}

uint8_t I2C::selectChannel(uint8_t channel)
{
    wire->beginTransmission(i2c_hub_addr);
    wire->write(1 << channel);
    uint8_t result = wire->endTransmission();

    Serial.print("Selecting channel ");
    Serial.print(channel);
    Serial.print(" on address ");
    Serial.print(i2c_hub_addr);
    Serial.print(" result: ");
    Serial.println(result);

    return result;
}

void I2C::configure_port(uint8_t number)
{
    wire->beginTransmission(expander_addr+number);
    wire->write(config_reg);
    wire->write(0b01110001);
    wire->endTransmission();
}

void I2C::write(uint8_t address, uint8_t reg, uint8_t data)
{
    wire->beginTransmission(address);
    wire->write(reg);
    wire->write(data);
    uint8_t result = wire->endTransmission();

    Serial.print("Writing to address ");
    Serial.print(address);
    Serial.print(" register ");
    Serial.print(reg);
    Serial.print(" data ");
    Serial.print(data);
    Serial.print(" result: ");
    Serial.println(result);
}

uint8_t I2C::read(uint8_t address, uint8_t reg)
{
    wire->beginTransmission(address);
    wire->write(reg);
    wire->endTransmission();
    wire->requestFrom(address, (uint8_t)1);

    return wire->read();
}

void I2C::led_set(uint8_t number, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t address = expander_addr+number;
    uint8_t term = this->read(address, output_reg)&0b10000000;

    uint8_t port = 0;
    if (red > 0) {
        port |= 0b01111101;
    }
    if (green > 0) {
        port |= 0b01111011;
    }
    if (blue > 0) {
        port |= 0b01110111;
    }
    port |= term;

    this->write(address, output_reg, port);
}

uint8_t I2C::read_button_state(uint8_t number)
{
    uint8_t address = expander_addr+number;
    uint16_t data = 0;
    wire->beginTransmission(address);
    wire->write(input_reg);
    wire->endTransmission();
    wire->requestFrom(address, (uint8_t)2);
    while (wire->available()) {
        data = wire->read();
        data = data << 8;
        data |= wire->read();
    }

    return data & 0b00000001;
}

void reportScanResult(uint8_t address, uint8_t error, const char* message)
{
    Serial.print(message);
    Serial.print(" 0x");
    if (address < 16) {
        Serial.print("0");
    }
    Serial.print(address, HEX);
    Serial.print(" (code ");
    Serial.print(error);
    Serial.println(")");
}

void I2C::scan()
{
    for (uint8_t channel = 0; channel < 3; channel++)
    {
        this->selectChannel(channel);
        this->configure_port(0);

        Serial.println("Scanning I2C bus on channel " + String(channel));
        for (uint8_t address = 1; address < 127; address++)
        {
            wire->beginTransmission(address);
            uint8_t error = wire->endTransmission();
            
            if (error == 0) {
                reportScanResult(address, error, "Found device at address");
            } else if (error == 4) {
                reportScanResult(address, error, "Unknown error at address");
            }
        }
        Serial.println("Done.");
        Serial.println();
    }
}
