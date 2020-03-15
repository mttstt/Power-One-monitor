# Plugin EspEasy

Tested with Espeasy mega-20190202 e mega-20200310 version

Tested on Wemos d1 mini

### Output

	Aurora Inverter - PVI Address: 2

	Use URL: http://192.168.1.120/control?cmd=aurora,askData time: 27/4/2019 15:5:3

	Daily Energy: 17856
	Weekly Energy: 110516
	Month Energy: 410792
	Year Energy: 1438385
	LastFourAlarms: 0,0,0,0
	SystemPN: -3G79-
	SystemSerialNumber: 912139
	ReadFirmwareRelease: C.0.3.4
	ManufacturingWeekYear: 12,15
	Version: Aurora 4.2kW new,Sconosciuto,Transformerless Version,Wind version
	TransmissionState: Everything is OK.
	GlobalState: Run
	InverterState: Run
	Channel1State: 2
	Channel2State: 2
	AlarmState: 0
	Inverter Temperature (C): 61.99
	Booster Temperature (C): 55.96
	Input 1 Voltage (Volt): 241.99
	Input 1 Current (Ampere): 6.71
	Input 2 Voltage (Volt): 229.67
	Input 2 Current (Ampere): 6.76
	Riso : 11.93
	Power Peak (Watt): 4623.24
	Power Peak Today (Watt): 3882.54


#### Gateway wifi-serial

Command example from linux to connect to ser2net implemented on Espeasy:
(Attention: not obtaind reply from the inverter)

      sudo socat -d -d -d -x pty,link=/home/pi/ttyS0 tcp:192.168.1.120:9999

      aurora -a 2 -c -Y 3 -T -b  /home/pi/ttyS0
