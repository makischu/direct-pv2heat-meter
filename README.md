# direct-pv2heat-meter

This project is about an experimental power2heat application. Photovoltaic shall be used to heat water with a resistive heating rod. Directly off-grid, without any inverter, MPPT tracker, or the like. Note that this is known to be a dumb solution in terms of efficiency (e.g. see [1] or [2]). My goal with this project is to collect data to see how bad my solution really is. And another goal is to keep it safe.

**DANGER!** This project deals with dangerous electric power. Do not reproduce except you really know what you are doing!

As there is no regulator involved to match the characteristic of the pv panels and the heater, special care was taken to dimensioning of the parts. At the start of the project I was about to buy a hot-water heat pump, which comes with a heating rod of 1.5kW@230V (equivalents 6,52A or 35,3 Ohm). This one is not required for the intended usage, so I will use it for the project to avoid any extra plumber work. My local photovoltaic dealer offered 375W modules (datasheet names Vmpp=34,6/32,3V and Impp=10,8/8,7A which equivalents Rmpp=3,2/3,7Ohm per module).

[3] proposes a "PV magic water heater formula: Element ohms equal (Vmp/Imp) x 1.40", which translates to a recommondation of 6.8 pv modules in my case. I chose 6. 6 es even, so I can build 2 strings of 3 modules for an alternate usage. Kirchhoff allows us to double the current by simply putting 2 strings in parallel, and power goes with current squared, so I did not want to waste that option too early.

Dividing into 2 strings resulted in another advantage. The maximum input voltage reduced to 3 *41,1V *1,1=135V. This lead me to the idea of using a TL783 as a first stage of my logic power supply, as it allows 125V in-out voltage difference. Unforfunately it was not availabe at the time of ordering and I did not find a 1:1 replacement. I still wanted to power the logic by the pv itself and ensure galvanic isolation, so i designed in an extra intermediate tap in one string. So the maximum input voltage the logic has to deal with is 41,1V *1,1=45V. This is low enough to find an (available) linear regulator for 24V (for relais supply) and a DC DC converter with 3.3V output (for the logic). Furthermore this allows for direct connection to the current metering IC's breakout board SparkX Power Meter - ACS37800 (Qwiic) [4] (<60V).

See image for the resulting overall wiring. 

![Wiring](./img/wiring.png)

I said I want to keep the usage safe. Power to the heater shall be interruptable. A certain temperature shall not be exceeded. The interruption is realized with FETs. Big ones with screw terminals, not PCB mounted. As a temperature sensor I want to use the built-on from the hot-water heat pump. The control is not realized yet, but for the pv hardware I realized it as following: A on-command/trigger-signal has to be received every few seconds. If not, the power output will shut down. That should be safe enough for me.

The logic is realized on an ESP32, as it is well-affordable and offers BLE (read: established way of low-power galvanically isolated communication). As counterpart I built a BLE-Wifi-Bridge, also based on an ESP32 (I chose a Firebeetle, so no soldering on this part). That is enough that all the rest (logging, controlling) can be realized in software.

System overview:

![Overview](./img/overview.png)


[1] https://www.solarpaneltalk.com/forum/solar/solar-energy-facts/325254-solar-panel-directly-connected-to-immersion-heater
[2] https://diysolarforum.com/threads/direct-solar-pv-water-heater.27937/
[3] http://waterheatertimer.org/Solar-power-curves-for-water-heater.html
[4] https://www.sparkfun.com/products/17873
