# crispy-invention

##Arduino
* Serial.read
* Switch cases for multiple commands
	* go(motornumber, pwm/dutycycle, optional stoptimer)
	* stop(motornumber, pwm/dutycycle)
* Setters & getters for
	* delay in void loop
	* max duty
	* gps functions (set lat, long, hour-min-sec, etc)
	* any other settings
* getters for encoder values
	* getRotation(encnumber) return rotationvalue

##Tegra
* Serial.write
* switch cases for which command to send
* decision making from separate thread
* ros vs threading
