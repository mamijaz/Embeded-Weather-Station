# Embeded Weather Station

An AVR Based Solution for a weather Station to acquire the fallowing data and upload them to Wunderground.com using PWS protocol
- Temperature
- Pressure
- Rainfall
- Wind Direction
- Wind Speed

The solution use an ATMAGA 328 microconteroller for the management of data acquisition, display and transmission and uses the fallowing modules

- BMP180 (Temperature and Pressure)
- Water Level Sensor (Rainfall)
- HMC5883L (Wind Direction)
- Hall Efect Sensor (Wind Speed)
- 16*2 LCD (Data display)
- SIM 808 (Data transmission)

Custom library's were written using the datasheet and using existing codes to handle the modules. Multiple communication protocols were used when dealing with these divices such as parallel communication for LCD display, I2C for HMC5883L and BMP180 and UART for SIM 808. The solution utilizes the A/D conversion of ATMEGA 328 to read the water level of the water level sensors also the solution uses external interrupts and timer interrupts to measure the wind speed using the hall effect sensor.
