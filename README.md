# Arduino-MEGA
Arduino Mega with Dataloger Old version.
To manage SD library you need to download a specific SD old version Library and substitute it the origina.
Solution for Arduino Mega working with Old-Datalogger
the solution is working with RTC integrated into Datalogger
Present data on LCD Display using I2C
The Temperature, Hunidity and HeatIndex data are stored in SD Card
The DHT sensor can be even DHT11 or DHT HTTYPE DHT21   // AM2301 
Pin connections ar ein Excel updated:
1 Connect SDA and SCL between themselfe into the Shield SDA and SCL
Use Pin 9 Digita to read sensor DHT
Final to connect LCD Display use I2C connecting  SDA to Pin 20SDA on Mega and SCL pin21
