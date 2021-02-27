# Sonar-and-RFID
Prototype V1 2/27/2021
Combining Sonar Sensor with RFID reader/tag

From the testing of each individual sensor, RFID has great detection capabilities of distance less than 4cm. Sonar is great at detecting distances greater than 4cm up to about 40cm with +/-1cm of precision. The error accumulated from the sonar measurement ranges from +/-1cm to +/-5cm as the detected object goes beyond 40cm up to 140cm (this is for an object the size of a credit card or drivers license).

The RFID and RFID tag will be used to determine and check whether or not the sash is in the closed position or not. If the tag is read by the reader then the sash is said to be closed, however when the tag is not read or present the sash will be considered to be opened. Once the orientation of the sash is opened the device will trigger the sonar sensor to begin measuring distances and the time in which to sash is opened for will be obtained until the RFID tag is read by the reader again.

The orientation of the sash must be in the closed position to initialize and start the program. 
The program will check if the sash is closed or not from the RFID, if the sash is opened then the sonar will begin measuring distance and timer will start
When the sash is closed again sonar will stop measuring and the timer will be reset
The program only displays the values onto the serial monitor and does not save the distance or the time the sash has been opened.
Author: Willie Chow w3chow@ucsd.edu
