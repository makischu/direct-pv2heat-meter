EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Analog_ADC:ADS1115IDGS U?
U 1 1 644EFFA7
P 6650 5000
AR Path="/644EFFA7" Ref="U?"  Part="1" 
AR Path="/644BD67F/644EFFA7" Ref="U15"  Part="1" 
F 0 "U15" H 6650 5681 50  0000 C CNN
F 1 "ADS1115IDGS" H 6650 5590 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 6650 4500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 6600 4100 50  0001 C CNN
	1    6650 5000
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Current:ACS710xLATR-12BB U?
U 1 1 644EFFAD
P 3450 5000
AR Path="/644EFFAD" Ref="U?"  Part="1" 
AR Path="/644BD67F/644EFFAD" Ref="U14"  Part="1" 
F 0 "U14" H 3450 5681 50  0000 C CNN
F 1 "ACS710KLATR-12CB-T" H 3450 5590 50  0000 C CNN
F 2 "Package_SO:SOIC-16W_7.5x10.3mm_P1.27mm" H 3800 4950 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS710-Datasheet.ashx?la=en" H 3450 5000 50  0001 C CNN
	1    3450 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 5700 3450 5500
Wire Wire Line
	3450 5700 4050 5700
Wire Wire Line
	6650 5700 6650 5400
Connection ~ 3450 5700
Wire Wire Line
	2050 4150 2200 4150
Wire Wire Line
	3450 4150 3450 4500
Wire Wire Line
	3450 4150 5450 4150
Wire Wire Line
	6650 4150 6650 4500
Connection ~ 3450 4150
$Comp
L Device:C C?
U 1 1 644EFFBD
P 4050 5500
AR Path="/644EFFBD" Ref="C?"  Part="1" 
AR Path="/644BD67F/644EFFBD" Ref="C15"  Part="1" 
F 0 "C15" H 4165 5546 50  0000 L CNN
F 1 "1n CL21B102KBCNNNC" H 4165 5455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4088 5350 50  0001 C CNN
F 3 "~" H 4050 5500 50  0001 C CNN
	1    4050 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 5350 4050 5300
Wire Wire Line
	4050 5300 3850 5300
Wire Wire Line
	4050 5650 4050 5700
Connection ~ 4050 5700
Wire Wire Line
	4050 5700 4450 5700
$Comp
L Device:C C?
U 1 1 644EFFC8
P 4450 5400
AR Path="/644EFFC8" Ref="C?"  Part="1" 
AR Path="/644BD67F/644EFFC8" Ref="C14"  Part="1" 
F 0 "C14" H 4565 5446 50  0000 L CNN
F 1 "4.7u CL21A475KAQNNNE" H 4565 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4488 5250 50  0001 C CNN
F 3 "~" H 4450 5400 50  0001 C CNN
	1    4450 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 5250 4450 5200
Wire Wire Line
	4450 5200 3850 5200
Wire Wire Line
	4450 5550 4450 5700
Connection ~ 4450 5700
Wire Wire Line
	4050 5300 4300 5300
Connection ~ 4050 5300
Wire Wire Line
	3850 4800 4450 4800
Wire Wire Line
	2750 4150 2750 4800
Wire Wire Line
	2750 4800 3050 4800
Connection ~ 2750 4150
Wire Wire Line
	2750 4150 3450 4150
Wire Wire Line
	3050 4900 2200 4900
Wire Wire Line
	2200 5700 3450 5700
NoConn ~ 3850 5000
Wire Wire Line
	2200 4900 2200 5700
Wire Wire Line
	3050 5100 2850 5100
Wire Wire Line
	2850 5300 3050 5300
Wire Wire Line
	6250 5100 6100 5100
Wire Wire Line
	6100 5100 6100 5200
Connection ~ 6100 5700
Wire Wire Line
	6100 5700 6650 5700
NoConn ~ 7050 4800
Wire Wire Line
	7400 5000 7050 5000
Wire Wire Line
	7050 5100 7400 5100
Wire Wire Line
	4450 4900 4450 4800
Wire Wire Line
	4300 5300 4300 5000
Wire Wire Line
	4300 5000 4800 5000
Wire Wire Line
	6250 4900 4800 4900
$Comp
L Device:C C?
U 1 1 644EFFEE
P 2200 4500
AR Path="/644EFFEE" Ref="C?"  Part="1" 
AR Path="/644BD67F/644EFFEE" Ref="C12"  Part="1" 
F 0 "C12" H 2315 4546 50  0000 L CNN
F 1 "100n CC0805KRX7R9BB104 " H 2315 4455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2238 4350 50  0001 C CNN
F 3 "~" H 2200 4500 50  0001 C CNN
	1    2200 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 4350 2200 4150
Connection ~ 2200 4150
Wire Wire Line
	2200 4150 2750 4150
Wire Wire Line
	2200 4650 2200 4900
Connection ~ 2200 4900
$Comp
L Analog_ADC:ADS1115IDGS U?
U 1 1 644EFFF9
P 6650 2650
AR Path="/644EFFF9" Ref="U?"  Part="1" 
AR Path="/644BD67F/644EFFF9" Ref="U13"  Part="1" 
F 0 "U13" H 6650 3331 50  0000 C CNN
F 1 "ADS1115IDGS" H 6650 3240 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 6650 2150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 6600 1750 50  0001 C CNN
	1    6650 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 3100 3150 3100
Wire Wire Line
	6650 3100 6650 3050
Wire Wire Line
	6650 1800 6650 2150
$Comp
L Device:R R?
U 1 1 644F0004
P 2450 2400
AR Path="/644F0004" Ref="R?"  Part="1" 
AR Path="/644BD67F/644F0004" Ref="R24"  Part="1" 
F 0 "R24" V 2243 2400 50  0000 C CNN
F 1 "1M" V 2334 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2380 2400 50  0001 C CNN
F 3 "~" H 2450 2400 50  0001 C CNN
	1    2450 2400
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 644F000A
P 2900 2400
AR Path="/644F000A" Ref="R?"  Part="1" 
AR Path="/644BD67F/644F000A" Ref="R25"  Part="1" 
F 0 "R25" V 2693 2400 50  0000 C CNN
F 1 "1M" V 2784 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2830 2400 50  0001 C CNN
F 3 "~" H 2900 2400 50  0001 C CNN
	1    2900 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 2400 2050 2400
Wire Wire Line
	2600 2400 2750 2400
$Comp
L Device:R R?
U 1 1 644F0012
P 3150 2650
AR Path="/644F0012" Ref="R?"  Part="1" 
AR Path="/644BD67F/644F0012" Ref="R26"  Part="1" 
F 0 "R26" H 3080 2604 50  0000 R CNN
F 1 "6.8k RT0603BRE076K8L" H 3080 2695 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3080 2650 50  0001 C CNN
F 3 "~" H 3150 2650 50  0001 C CNN
	1    3150 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3150 2500 3150 2400
Wire Wire Line
	3150 2400 3050 2400
Wire Wire Line
	3150 2800 3150 3100
Connection ~ 3150 3100
Wire Wire Line
	3150 3100 4400 3100
Wire Wire Line
	5850 2400 5850 2650
Connection ~ 3150 2400
Wire Wire Line
	6250 2750 6100 2750
Wire Wire Line
	6100 2750 6100 2850
Connection ~ 6100 3100
Wire Wire Line
	6100 3100 6650 3100
$Comp
L Device:C C?
U 1 1 644F0023
P 4400 2650
AR Path="/644F0023" Ref="C?"  Part="1" 
AR Path="/644BD67F/644F0023" Ref="C11"  Part="1" 
F 0 "C11" H 4515 2696 50  0000 L CNN
F 1 "1u CL21B105KBFNNNE" H 4515 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4438 2500 50  0001 C CNN
F 3 "~" H 4400 2650 50  0001 C CNN
	1    4400 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2500 4400 2400
Connection ~ 4400 2400
Wire Wire Line
	4400 2400 4600 2400
Wire Wire Line
	4400 2800 4400 3100
Connection ~ 4400 3100
NoConn ~ 7050 2450
Wire Wire Line
	7400 2650 7050 2650
Wire Wire Line
	7050 2750 7400 2750
Wire Wire Line
	3150 2400 4400 2400
Wire Wire Line
	6650 3100 7200 3100
Wire Wire Line
	7200 3100 7200 2850
Wire Wire Line
	7200 2850 7050 2850
Connection ~ 6650 3100
Wire Wire Line
	6100 2750 6100 2550
Wire Wire Line
	6100 2550 6250 2550
Connection ~ 6100 2750
Wire Wire Line
	6250 2650 5850 2650
Wire Wire Line
	6250 2850 6100 2850
Connection ~ 6100 2850
Wire Wire Line
	6100 2850 6100 3100
Wire Wire Line
	5250 1800 5400 1800
$Comp
L Device:C C?
U 1 1 644F0041
P 5400 2150
AR Path="/644F0041" Ref="C?"  Part="1" 
AR Path="/644BD67F/644F0041" Ref="C10"  Part="1" 
F 0 "C10" H 5515 2196 50  0000 L CNN
F 1 "100n CC0805KRX7R9BB104 " H 5515 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5438 2000 50  0001 C CNN
F 3 "~" H 5400 2150 50  0001 C CNN
	1    5400 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2000 5400 1800
Connection ~ 5400 1800
Wire Wire Line
	5400 1800 6650 1800
Wire Wire Line
	5400 2300 5400 3100
Connection ~ 5400 3100
Wire Wire Line
	5400 3100 6100 3100
Wire Wire Line
	4400 3100 5400 3100
Wire Wire Line
	2050 5700 2200 5700
Connection ~ 2200 5700
$Comp
L Device:C C?
U 1 1 644F0051
P 5450 4500
AR Path="/644F0051" Ref="C?"  Part="1" 
AR Path="/644BD67F/644F0051" Ref="C13"  Part="1" 
F 0 "C13" H 5565 4546 50  0000 L CNN
F 1 "100n CC0805KRX7R9BB104 " H 5565 4455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5488 4350 50  0001 C CNN
F 3 "~" H 5450 4500 50  0001 C CNN
	1    5450 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4350 5450 4150
Connection ~ 5450 4150
Wire Wire Line
	5450 4150 6650 4150
Wire Wire Line
	5450 4650 5450 5700
Wire Wire Line
	4450 5700 5450 5700
Connection ~ 5450 5700
Wire Wire Line
	5450 5700 6100 5700
Wire Wire Line
	6650 4150 7700 4150
Wire Wire Line
	7700 4150 7700 5200
Wire Wire Line
	7050 5200 7700 5200
Connection ~ 6650 4150
Wire Wire Line
	6250 5200 6100 5200
Connection ~ 6100 5200
Wire Wire Line
	6100 5200 6100 5700
Wire Wire Line
	9450 2650 9100 2650
Wire Wire Line
	9100 2950 9450 2950
$Comp
L Device:R R?
U 1 1 644F006A
P 8950 2650
AR Path="/644F006A" Ref="R?"  Part="1" 
AR Path="/644BD67F/644F006A" Ref="R27"  Part="1" 
F 0 "R27" V 8743 2650 50  0000 C CNN
F 1 "DNP 4.7K" V 8834 2650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8880 2650 50  0001 C CNN
F 3 "~" H 8950 2650 50  0001 C CNN
	1    8950 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 2650 8350 2650
$Comp
L Device:R R?
U 1 1 644F0072
P 8950 2950
AR Path="/644F0072" Ref="R?"  Part="1" 
AR Path="/644BD67F/644F0072" Ref="R28"  Part="1" 
F 0 "R28" V 8743 2950 50  0000 C CNN
F 1 "DNP 4.7K" V 8834 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8880 2950 50  0001 C CNN
F 3 "~" H 8950 2950 50  0001 C CNN
	1    8950 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 2950 8350 2950
Text Notes 8500 2350 0    50   ~ 0
usage as testpoints too
$Comp
L Connector:TestPoint TP?
U 1 1 644F007A
P 4600 2400
AR Path="/644F007A" Ref="TP?"  Part="1" 
AR Path="/644BD67F/644F007A" Ref="TP5"  Part="1" 
F 0 "TP5" H 4658 2518 50  0000 L CNN
F 1 "TestPoint" H 4658 2427 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 4800 2400 50  0001 C CNN
F 3 "~" H 4800 2400 50  0001 C CNN
	1    4600 2400
	1    0    0    -1  
$EndComp
Connection ~ 4600 2400
Wire Wire Line
	4600 2400 5850 2400
$Comp
L Connector:TestPoint TP?
U 1 1 644F0082
P 4800 4900
AR Path="/644F0082" Ref="TP?"  Part="1" 
AR Path="/644BD67F/644F0082" Ref="TP6"  Part="1" 
F 0 "TP6" H 4858 5018 50  0000 L CNN
F 1 "TestPoint" H 4858 4927 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 5000 4900 50  0001 C CNN
F 3 "~" H 5000 4900 50  0001 C CNN
	1    4800 4900
	1    0    0    -1  
$EndComp
Text HLabel 2050 2400 0    50   Input ~ 0
S2+Fused
Text HLabel 2050 3100 0    50   Input ~ 0
GND
Text HLabel 2050 4150 0    50   Input ~ 0
V33P
Text HLabel 2050 5700 0    50   Input ~ 0
GND
Text HLabel 2850 5100 0    50   Input ~ 0
IP+
Text HLabel 2850 5300 0    50   Output ~ 0
IP-
Text HLabel 5250 1800 0    50   Input ~ 0
V33P
Text HLabel 8350 2650 0    50   Input ~ 0
V33P
Text HLabel 8350 2950 0    50   Input ~ 0
V33P
Text HLabel 7400 2650 2    50   Output ~ 0
SCL
Text HLabel 7400 2750 2    50   Output ~ 0
SDA
Text HLabel 9450 2650 2    50   Output ~ 0
SCL
Text HLabel 9450 2950 2    50   Output ~ 0
SDA
Text HLabel 7400 5000 2    50   Output ~ 0
SCL
Text HLabel 7400 5100 2    50   Output ~ 0
SDA
Text Notes 10700 4400 2    50   ~ 0
current measurement\n\nACS710 works best at 5V. \n3.3V is allowed at cost of accuracy. \n\nalternative: 5V generation via BD450M2W or eq.
Text Notes 10600 1500 2    50   ~ 0
voltage measurement
Text Notes 10650 6400 2    50   ~ 0
separated for simultaneous measurement
$Comp
L Connector:TestPoint TP?
U 1 1 646503E4
P 4800 5000
AR Path="/646503E4" Ref="TP?"  Part="1" 
AR Path="/644BD67F/646503E4" Ref="TP7"  Part="1" 
F 0 "TP7" H 4858 5118 50  0000 L CNN
F 1 "TestPoint" H 4858 5027 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 5000 5000 50  0001 C CNN
F 3 "~" H 5000 5000 50  0001 C CNN
	1    4800 5000
	-1   0    0    1   
$EndComp
Connection ~ 4800 5000
Wire Wire Line
	4800 5000 6250 5000
Connection ~ 4800 4900
Wire Wire Line
	4450 4900 4800 4900
$EndSCHEMATC
