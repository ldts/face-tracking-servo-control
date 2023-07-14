# servo-udp

This application sends duty cycle to Xenomai's [gpiopwm demo](https://source.denx.de/Xenomai/xenomai/-/blob/master/demo/posix/cobalt/gpiopwm.c) code (when the demo code runs in udp mode). This code requires the [gpiopwm driver](https://source.denx.de/Xenomai/xenomai/-/blob/master/kernel/drivers/gpiopwm/gpiopwm.c) in order to execute.

Use the cursor keys to control up to two servo motors in a remote platform
```
up/down     : increments/decrements duty cycle of servo[0]
left/right  : increments/decrements duty cycle of servo[1]
```
 
The Xenomai gpiopwm demo code must have been started in **udp** mode on the target platform (as below)

```
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
```
From the host, use the current program as follows:

```
host_box$:  main 192.168.211.184 55556
```

and then use the up/down keys to increase/decrese duty cycles.
