# Embeded Weather Station

An AVR Basesd Sollution for a weather Station to aquire the fallowing data and upload them to Wunderground.com using PWS protocol
  - Temperature
  - Pressure
  - Rainfall
  - Wind Direction
  - Wind Speed
   
The sollution use an ATMAGA 328 microconteroller for the management of data aquision, display and transmission and uses the fallowing modules
  - BMP180 (Temperature and Pressure)
  - Water Level Sonsor (Rainfall)
  - HMC5883L (Wind Direction)
  - Hall Efect Sonsor (Wind Speed)
  - 16*2 LCD (Data display)
  - SIM 808 (Data transmission)

Custom librarys were written using the datasheet and using existiong codes to handle the modules. Multiple communication protocols were used when dealing with these divices such as paralal communication for LCD display, I2C for HMC5883L and BMP180 and UART for SIM 808. The sollution utilizes the A/D conversion of ATMEGA 328 to read the water level of the water level sonsors also the sollution uses external interupts and timer interupts to measure the wind speed using the hall effect sensor.
