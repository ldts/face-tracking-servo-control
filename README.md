# servo-udp

Sends duty cycle to xenomai's gpiopwm demo code (when demo code run in udp mode)
Use the cursor keys to control up to two servo motors in a remote platform

up/down     : increments/decrements duty cycle of servo[0]
left/right  : increments/decrements duty cycle of servo[1]
 
The Xenomai gpiopwm demo code must have been started in udp mode on the target platform (as below)

xenomai_box$: gpiopwm --config=1:1000:2000:20000000:492:0 --udp=55556

 Config: UDP Server
 ip      : 127.0.0.1
 ip      : 192.168.211.184
 port    : 55556

 device     : /dev/rtdm/gpiopwm1
 range      : [1000, 2000]
 period     : 20000000 nsec
 gpio pin   : 492
 duty cycle : 0

From the host, use the current program as follows:

host_box$:  main 192.168.211.184 55556

and use the up/down keys to increase/decrese duty cycles.

note: main accept two remote servers: ie <ip1> <port1> <ip2> <port2>



