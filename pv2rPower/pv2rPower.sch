EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 4
Title "pv2rPower"
Date "2023-02-13"
Rev ""
Comp "makischu"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 2200 2100 2    50   Output ~ 0
V24
Text GLabel 2200 2500 2    50   Output ~ 0
V33P
Text GLabel 2200 2600 2    50   Output ~ 0
GND
Text GLabel 2200 2200 2    50   Output ~ 0
1W
Text GLabel 2200 2300 2    50   Output ~ 0
SDA
Text GLabel 2200 2400 2    50   Output ~ 0
SCL
Text GLabel 2100 1100 2    50   Output ~ 0
S2+
Text GLabel 2100 1200 2    50   Output ~ 0
S2-
Text GLabel 2100 1300 2    50   Output ~ 0
S1+
Text GLabel 2100 1400 2    50   Output ~ 0
S1-
Wire Wire Line
	2100 1400 1800 1400
Wire Wire Line
	1800 1300 2100 1300
Wire Wire Line
	2100 1200 1800 1200
Wire Wire Line
	1800 1100 2100 1100
$Comp
L Connector:Screw_Terminal_01x06 J1
U 1 1 63670C61
P 1600 1200
F 0 "J1" H 1518 1617 50  0000 C CNN
F 1 "Screw_Terminal_01x06" H 1518 1526 50  0000 C CNN
F 2 "TerminalBlock_RND:TerminalBlock_RND_205-00016_1x06_P5.00mm_Horizontal" H 1600 1200 50  0001 C CNN
F 3 "~" H 1600 1200 50  0001 C CNN
	1    1600 1200
	-1   0    0    -1  
$EndComp
Text GLabel 2100 1000 2    50   Output ~ 0
PWR_OUT+
Text GLabel 2100 1500 2    50   Output ~ 0
PWR_OUT-
Wire Wire Line
	2100 1500 1800 1500
Wire Wire Line
	1800 1000 2100 1000
$Comp
L Relay:RT42xFxx K1
U 1 1 63664E38
P 3250 6950
F 0 "K1" H 3880 6996 50  0000 L CNN
F 1 "RT424F24" H 3880 6905 50  0000 L CNN
F 2 "Relay_THT:Relay_DPDT_Schrack-RT2-FormC-Dual-Coil_RM5mm" H 3250 6950 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FRT2_bistable%7F1116%7Fpdf%7FEnglish%7FENG_DS_RT2_bistable_1116.pdf%7F1-1415537-8" H 3250 6950 50  0001 C CNN
	1    3250 6950
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:2N7002 Q3
U 1 1 63669790
P 3950 8750
F 0 "Q3" H 4154 8796 50  0000 L CNN
F 1 "2N7002" H 4154 8705 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4150 8675 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7002.pdf" H 3950 8750 50  0001 L CNN
	1    3950 8750
	1    0    0    -1  
$EndComp
$Comp
L Device:Fuse F1
U 1 1 636E2568
P 5850 4450
F 0 "F1" V 5653 4450 50  0000 C CNN
F 1 "Fuse" V 5744 4450 50  0000 C CNN
F 2 "Fuse:Fuseholder_Cylinder-5x20mm_Schurter_0031_8201_Horizontal_Open" V 5780 4450 50  0001 C CNN
F 3 "~" H 5850 4450 50  0001 C CNN
	1    5850 4450
	0    1    1    0   
$EndComp
$Comp
L Transistor_FET:C2M0045170D Q1
U 1 1 636F0D82
P 14150 8550
F 0 "Q1" H 14355 8596 50  0000 L CNN
F 1 "IRFB4332PbF C19941" H 14355 8505 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 14150 8550 50  0001 C CIN
F 3 "" H 14150 8550 50  0001 L CNN
	1    14150 8550
	1    0    0    -1  
$EndComp
Wire Wire Line
	14250 7950 14250 8350
Text GLabel 3250 7900 3    50   Input ~ 0
V24
Wire Wire Line
	4050 9150 4050 8950
Text GLabel 3600 8750 0    50   Input ~ 0
R_SET
Wire Wire Line
	3600 8750 3650 8750
$Comp
L Transistor_FET:2N7002 Q2
U 1 1 6372655A
P 2400 8750
F 0 "Q2" H 2604 8796 50  0000 L CNN
F 1 "2N7002" H 2604 8705 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2600 8675 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7002.pdf" H 2400 8750 50  0001 L CNN
	1    2400 8750
	1    0    0    -1  
$EndComp
Text GLabel 2050 9150 0    50   Input ~ 0
GND
Wire Wire Line
	2050 9150 2100 9150
Wire Wire Line
	2500 9150 2500 8950
Text GLabel 2050 8750 0    50   Input ~ 0
R_RESET
Wire Wire Line
	2050 8750 2100 8750
Wire Wire Line
	2500 9150 3650 9150
Connection ~ 2500 9150
Wire Wire Line
	4050 7350 3550 7350
Wire Wire Line
	2950 7350 2500 7350
$Comp
L Device:R R2
U 1 1 6373F950
P 2100 8950
F 0 "R2" H 2170 8996 50  0000 L CNN
F 1 "100K" H 2170 8905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2030 8950 50  0001 C CNN
F 3 "~" H 2100 8950 50  0001 C CNN
	1    2100 8950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 63740237
P 3650 8950
F 0 "R3" H 3720 8996 50  0000 L CNN
F 1 "100K" H 3720 8905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3580 8950 50  0001 C CNN
F 3 "~" H 3650 8950 50  0001 C CNN
	1    3650 8950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 8800 3650 8750
Connection ~ 3650 8750
Wire Wire Line
	3650 8750 3750 8750
Wire Wire Line
	3650 9100 3650 9150
Connection ~ 3650 9150
Wire Wire Line
	3650 9150 4050 9150
Wire Wire Line
	2100 8800 2100 8750
Connection ~ 2100 8750
Wire Wire Line
	2100 8750 2200 8750
Wire Wire Line
	2100 9100 2100 9150
Connection ~ 2100 9150
Wire Wire Line
	2100 9150 2500 9150
Text GLabel 5450 7250 0    50   Input ~ 0
S1+
Text GLabel 2600 6850 0    50   Input ~ 0
V33P
Text GLabel 1050 6050 0    50   Input ~ 0
S2-
Text GLabel 6150 4350 2    50   Output ~ 0
S2+Fused
$Comp
L Sensor_Temperature:DS18B20U U1
U 1 1 6384A12A
P 4150 2550
F 0 "U1" H 3806 2596 50  0000 R CNN
F 1 "DNP" H 3806 2505 50  0000 R CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 3200 2300 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 4000 2800 50  0001 C CNN
	1    4150 2550
	1    0    0    -1  
$EndComp
Text GLabel 3500 2050 0    50   Input ~ 0
V33P
Text GLabel 3500 3050 0    50   Input ~ 0
GND
Wire Wire Line
	4150 2250 4150 2050
Wire Wire Line
	4150 2050 3500 2050
Wire Wire Line
	3500 3050 4150 3050
Wire Wire Line
	4150 3050 4150 2850
Text GLabel 15150 7950 2    50   Output ~ 0
PWR_OUT-
$Comp
L Mechanical:MountingHole H1
U 1 1 638BB586
P 9600 10800
F 0 "H1" H 9700 10846 50  0000 L CNN
F 1 "MountingHole" H 9700 10755 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.5mm" H 9600 10800 50  0001 C CNN
F 3 "~" H 9600 10800 50  0001 C CNN
	1    9600 10800
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Bridged JP1
U 1 1 63689A48
P 5850 9500
F 0 "JP1" H 5850 9705 50  0000 C CNN
F 1 "SolderJumper_2_Bridged" H 5850 9614 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 5850 9500 50  0001 C CNN
F 3 "~" H 5850 9500 50  0001 C CNN
	1    5850 9500
	1    0    0    -1  
$EndComp
Text GLabel 3800 1100 2    50   Output ~ 0
LOGIC2
Text GLabel 3800 1200 2    50   Output ~ 0
LOGIC3
Text GLabel 3800 1300 2    50   Output ~ 0
LOGIC4
Text GLabel 3800 1400 2    50   Output ~ 0
LOGIC5
Wire Wire Line
	3800 1400 3500 1400
Wire Wire Line
	3500 1300 3800 1300
Wire Wire Line
	3800 1200 3500 1200
Wire Wire Line
	3500 1100 3800 1100
$Comp
L Connector:Screw_Terminal_01x06 J2
U 1 1 636CF2AC
P 3300 1200
F 0 "J2" H 3218 1617 50  0000 C CNN
F 1 "Screw_Terminal_01x06" H 3218 1526 50  0000 C CNN
F 2 "TerminalBlock_RND:TerminalBlock_RND_205-00016_1x06_P5.00mm_Horizontal" H 3300 1200 50  0001 C CNN
F 3 "~" H 3300 1200 50  0001 C CNN
	1    3300 1200
	-1   0    0    -1  
$EndComp
Text GLabel 3800 1000 2    50   Output ~ 0
GND
Text GLabel 3800 1500 2    50   Output ~ 0
LOGIC6
Wire Wire Line
	3800 1500 3500 1500
Wire Wire Line
	3500 1000 3800 1000
Text GLabel 2200 2800 2    50   Output ~ 0
S1+Fused
Text GLabel 2200 2900 2    50   Output ~ 0
LOGIC6
NoConn ~ 2950 6650
$Comp
L Mechanical:MountingHole H2
U 1 1 638BBB3E
P 10600 10800
F 0 "H2" H 10700 10846 50  0000 L CNN
F 1 "MountingHole" H 10700 10755 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.5mm" H 10600 10800 50  0001 C CNN
F 3 "~" H 10600 10800 50  0001 C CNN
	1    10600 10800
	1    0    0    -1  
$EndComp
Text GLabel 1200 2800 0    50   Output ~ 0
LOGIC2
Text GLabel 1200 2100 0    50   Output ~ 0
GND
Text GLabel 1200 2200 0    50   Output ~ 0
R_SET
Text GLabel 1200 2300 0    50   Output ~ 0
R_RESET
Text GLabel 1200 2400 0    50   Output ~ 0
R_STATUS
Wire Wire Line
	1200 2100 1450 2100
Wire Wire Line
	1450 2200 1200 2200
Wire Wire Line
	1200 2300 1450 2300
Wire Wire Line
	1200 2400 1450 2400
Wire Wire Line
	1450 2500 1200 2500
Wire Wire Line
	1200 2600 1450 2600
Wire Wire Line
	1450 2700 1200 2700
Wire Wire Line
	1200 2800 1450 2800
Wire Wire Line
	1450 2900 1200 2900
Wire Wire Line
	1200 3000 1450 3000
Wire Wire Line
	1950 3000 2200 3000
Wire Wire Line
	2200 2900 1950 2900
Wire Wire Line
	1950 2800 2200 2800
Wire Wire Line
	2200 2700 1950 2700
Wire Wire Line
	1950 2600 2200 2600
Wire Wire Line
	2200 2500 1950 2500
Wire Wire Line
	1950 2400 2200 2400
Wire Wire Line
	2200 2300 1950 2300
Wire Wire Line
	1950 2200 2200 2200
Wire Wire Line
	2200 2100 1950 2100
$Comp
L Connector_Generic:Conn_02x10_Odd_Even J3
U 1 1 6369A14E
P 1650 2500
F 0 "J3" H 1700 3117 50  0000 C CNN
F 1 "C3405" H 1700 3026 50  0000 C CNN
F 2 "gitidc:IDC-Header_2x10_P2.54mm_Vertical" H 1650 2500 50  0001 C CNN
F 3 "~" H 1650 2500 50  0001 C CNN
	1    1650 2500
	1    0    0    -1  
$EndComp
Text GLabel 1200 2900 0    50   Output ~ 0
LOGIC3
Text GLabel 1200 3000 0    50   Output ~ 0
LOGIC4
Text GLabel 2200 3000 2    50   Output ~ 0
LOGIC5
Text GLabel 2200 2700 2    50   Output ~ 0
GND
Text GLabel 5450 9200 0    50   Input ~ 0
S1-
Wire Wire Line
	5450 9800 5450 9500
Wire Wire Line
	5450 7250 5450 6700
Wire Wire Line
	5450 6550 3550 6550
Text GLabel 1050 4300 0    50   Input ~ 0
S2+
Wire Wire Line
	1050 4300 1050 3950
Wire Wire Line
	5450 3950 5450 4450
Wire Wire Line
	2950 6450 1050 6450
Wire Wire Line
	1050 6450 1050 6050
Connection ~ 1050 6450
Connection ~ 5450 9800
Wire Wire Line
	1050 3950 5450 3950
Text GLabel 15150 4850 2    50   Output ~ 0
PWR_OUT+
Wire Wire Line
	15150 4850 14850 4850
Wire Wire Line
	14250 4850 14250 3950
Wire Wire Line
	14250 7950 14850 7950
Text GLabel 2600 7050 0    50   Input ~ 0
GND
Text GLabel 4050 6950 2    50   Output ~ 0
R_STATUS
Wire Wire Line
	2600 6850 2950 6850
Wire Wire Line
	2950 7050 2600 7050
Wire Wire Line
	3550 6950 3650 6950
Wire Wire Line
	5450 4450 5700 4450
$Comp
L Device:Fuse F2
U 1 1 63F61796
P 5850 6700
F 0 "F2" V 5653 6700 50  0000 C CNN
F 1 "Fuse" V 5744 6700 50  0000 C CNN
F 2 "Fuse:Fuseholder_Cylinder-5x20mm_Schurter_0031_8201_Horizontal_Open" V 5780 6700 50  0001 C CNN
F 3 "~" H 5850 6700 50  0001 C CNN
	1    5850 6700
	0    1    1    0   
$EndComp
Text GLabel 6150 6600 2    50   Output ~ 0
S1+Fused
Wire Wire Line
	5450 6700 5700 6700
Connection ~ 5450 6700
Wire Wire Line
	5450 6700 5450 6550
Wire Wire Line
	5450 9500 5700 9500
Connection ~ 5450 9500
Wire Wire Line
	5450 9500 5450 9200
Wire Notes Line
	800  4650 1300 4650
Wire Notes Line
	1300 4650 1300 5650
Wire Notes Line
	1300 5650 800  5650
Wire Notes Line
	800  5650 800  4650
Wire Notes Line
	5200 7800 5700 7800
Wire Notes Line
	5700 7800 5700 8800
Wire Notes Line
	5700 8800 5200 8800
Wire Notes Line
	5200 8800 5200 7800
Wire Notes Line
	14350 5950 14150 5950
Wire Notes Line
	14150 5950 14150 6950
Wire Notes Line
	14150 6950 14350 6950
Wire Notes Line
	14350 6950 14350 5950
Wire Notes Line
	14250 5950 14250 5400
Wire Notes Line
	14250 6950 14250 7550
$Comp
L Device:D_Zener D8
U 1 1 64102D36
P 13650 9100
F 0 "D8" V 13604 9179 50  0000 L CNN
F 1 "SMAJ15A" V 13695 9179 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 13650 9100 50  0001 C CNN
F 3 "~" H 13650 9100 50  0001 C CNN
	1    13650 9100
	0    1    1    0   
$EndComp
$Comp
L Device:D_Zener D9
U 1 1 641035E9
P 14850 9100
F 0 "D9" V 14804 9179 50  0000 L CNN
F 1 "SMCJ250A" V 14895 9179 50  0000 L CNN
F 2 "Diode_SMD:D_SMC" H 14850 9100 50  0001 C CNN
F 3 "~" H 14850 9100 50  0001 C CNN
	1    14850 9100
	0    1    1    0   
$EndComp
Wire Wire Line
	14850 7950 14850 8950
Connection ~ 14850 7950
Wire Wire Line
	14850 7950 15150 7950
Wire Wire Line
	14850 6200 14850 4850
Connection ~ 14850 4850
Wire Wire Line
	14850 4850 14250 4850
Wire Wire Line
	1050 9800 5450 9800
Wire Wire Line
	13650 8950 13650 8550
Wire Wire Line
	13650 8550 13950 8550
Wire Wire Line
	14850 6500 14850 7950
Wire Wire Line
	14250 8750 14250 9800
Connection ~ 14250 9800
Wire Wire Line
	14250 9800 14850 9800
Wire Wire Line
	14850 9250 14850 9800
Wire Wire Line
	13650 9250 13650 9800
Connection ~ 13650 9800
Wire Wire Line
	13650 9800 14250 9800
$Comp
L Device:R R1
U 1 1 6436DAC7
P 3800 6950
F 0 "R1" H 3870 6996 50  0000 L CNN
F 1 "1K" H 3870 6905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3730 6950 50  0001 C CNN
F 3 "~" H 3800 6950 50  0001 C CNN
	1    3800 6950
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 6950 4050 6950
Wire Wire Line
	5450 9800 13650 9800
Text GLabel 6500 9300 2    50   Output ~ 0
GND
$Comp
L Connector:TestPoint_Probe TP2
U 1 1 64945C81
P 6550 9100
F 0 "TP2" H 6703 9201 50  0000 L CNN
F 1 "TestPoint_Probe" H 6703 9110 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_4.0x4.0mm_Drill2.0mm" H 6750 9100 50  0001 C CNN
F 3 "~" H 6750 9100 50  0001 C CNN
	1    6550 9100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 9100 6400 9100
Wire Wire Line
	6400 9100 6400 9300
Wire Wire Line
	6400 9300 6500 9300
$Comp
L Connector:TestPoint_Probe TP1
U 1 1 64A02F5F
P 6550 8900
F 0 "TP1" H 6703 9001 50  0000 L CNN
F 1 "TestPoint_Probe" H 6703 8910 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_4.0x4.0mm_Drill2.0mm" H 6750 8900 50  0001 C CNN
F 3 "~" H 6750 8900 50  0001 C CNN
	1    6550 8900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 8900 6400 8900
Wire Wire Line
	6400 8900 6400 9100
Connection ~ 6400 9100
Wire Wire Line
	6000 9500 6400 9500
Wire Wire Line
	6400 9300 6400 9500
Connection ~ 6400 9300
$Comp
L Device:D_Zener D3
U 1 1 64AB42C4
P 14850 6350
F 0 "D3" V 14804 6429 50  0000 L CNN
F 1 "SMCJ250A" V 14895 6429 50  0000 L CNN
F 2 "Diode_SMD:D_SMC" H 14850 6350 50  0001 C CNN
F 3 "~" H 14850 6350 50  0001 C CNN
	1    14850 6350
	0    1    1    0   
$EndComp
$Comp
L Device:D_Zener D4
U 1 1 64AC03ED
P 700 7700
F 0 "D4" V 654 7779 50  0000 L CNN
F 1 "SMAJ150A" V 745 7779 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 700 7700 50  0001 C CNN
F 3 "~" H 700 7700 50  0001 C CNN
	1    700  7700
	0    1    1    0   
$EndComp
$Comp
L Device:D_x2_KCom_AKA D7
U 1 1 64AC7EDD
P 1350 7850
F 0 "D7" H 1350 7973 50  0000 C CNN
F 1 "V30200C-E3/4W C500643 " H 1350 8064 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 1350 7850 50  0001 C CNN
F 3 "~" H 1350 7850 50  0001 C CNN
	1    1350 7850
	-1   0    0    1   
$EndComp
Wire Wire Line
	1050 7850 1050 8150
Wire Wire Line
	1050 8150 1650 8150
Wire Wire Line
	1650 8150 1650 7850
Wire Wire Line
	1350 7650 1350 7550
Wire Wire Line
	1350 7550 1050 7550
Wire Wire Line
	700  7850 700  8150
Wire Wire Line
	700  8150 1050 8150
Connection ~ 1050 8150
Wire Wire Line
	1050 8150 1050 9800
Wire Wire Line
	1050 6450 1050 7550
Connection ~ 1050 7550
Wire Wire Line
	1050 7550 700  7550
$Comp
L Device:D_Zener D1
U 1 1 64BD31ED
P 5100 5050
F 0 "D1" V 5054 5129 50  0000 L CNN
F 1 "SMAJ150A" V 5145 5129 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 5100 5050 50  0001 C CNN
F 3 "~" H 5100 5050 50  0001 C CNN
	1    5100 5050
	0    1    1    0   
$EndComp
$Comp
L Device:D_x2_KCom_AKA D2
U 1 1 64BD31F7
P 5750 5200
F 0 "D2" H 5750 5323 50  0000 C CNN
F 1 "V30200C-E3/4W C500643 " H 5750 5414 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5750 5200 50  0001 C CNN
F 3 "~" H 5750 5200 50  0001 C CNN
	1    5750 5200
	-1   0    0    1   
$EndComp
Wire Wire Line
	6050 5500 6050 5200
Wire Wire Line
	5750 5000 5750 4900
Wire Wire Line
	5750 4900 5450 4900
Wire Wire Line
	5100 5200 5100 5500
Wire Wire Line
	5450 4450 5450 4900
Connection ~ 5450 4900
Wire Wire Line
	5450 4900 5100 4900
Connection ~ 5450 4450
Wire Wire Line
	5450 5200 5450 5500
Connection ~ 5450 6550
Wire Wire Line
	5450 5500 5100 5500
Connection ~ 5450 5500
Wire Wire Line
	5450 5500 5450 6550
Wire Wire Line
	5450 5500 6050 5500
Text Notes 16700 8700 0    50   ~ 0
to247 alternativen wären:\nIXFH80N25X3 or CI72N65 or eq
$Comp
L Mechanical:Heatsink_Pad HS3
U 1 1 64CA7CCE
P 8250 10750
F 0 "HS3" H 8391 10789 50  0000 L CNN
F 1 "SK 525 30 ST" H 8391 10698 50  0000 L CNN
F 2 "myheatsink:Heatsink_Fischer_SK525_30_ST_TO220_2xDrill2.5mm" H 8262 10700 50  0001 C CNN
F 3 "~" H 8262 10700 50  0001 C CNN
	1    8250 10750
	1    0    0    -1  
$EndComp
NoConn ~ 8250 10850
$Comp
L Mechanical:Heatsink_Pad HS2
U 1 1 64CE0347
P 7400 10750
F 0 "HS2" H 7541 10789 50  0000 L CNN
F 1 "SK 525 30 ST" H 7541 10698 50  0000 L CNN
F 2 "myheatsink:Heatsink_Fischer_SK525_30_ST_TO220_2xDrill2.5mm" H 7412 10700 50  0001 C CNN
F 3 "~" H 7412 10700 50  0001 C CNN
	1    7400 10750
	1    0    0    -1  
$EndComp
NoConn ~ 7400 10850
$Comp
L Mechanical:Heatsink_Pad HS1
U 1 1 64CFAF64
P 6500 10750
F 0 "HS1" H 6641 10789 50  0000 L CNN
F 1 "SK 525 30 ST" H 6641 10698 50  0000 L CNN
F 2 "myheatsink:Heatsink_Fischer_SK525_30_ST_TO220_2xDrill2.5mm" H 6512 10700 50  0001 C CNN
F 3 "~" H 6512 10700 50  0001 C CNN
	1    6500 10750
	1    0    0    -1  
$EndComp
NoConn ~ 6500 10850
$Comp
L Device:D_x2_Serial_AKC D5
U 1 1 64D9C63F
P 2850 7750
F 0 "D5" H 2850 7966 50  0000 C CNN
F 1 "BAV99,215 C2500" H 2850 7875 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2850 7750 50  0001 C CNN
F 3 "~" H 2850 7750 50  0001 C CNN
	1    2850 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 7350 2500 7950
Wire Wire Line
	2850 7950 2500 7950
Connection ~ 2500 7950
Wire Wire Line
	2500 7950 2500 8550
Wire Wire Line
	4050 7350 4050 7550
Wire Wire Line
	3250 7650 3250 7750
Wire Wire Line
	3150 7750 3250 7750
Connection ~ 3250 7750
Wire Wire Line
	3250 7750 3250 7900
NoConn ~ 2550 7750
$Comp
L Device:D_x2_Serial_AKC D6
U 1 1 64DF29A7
P 3700 7750
F 0 "D6" H 3700 7873 50  0000 C CNN
F 1 "BAV99,215 C2500" H 3700 7964 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3700 7750 50  0001 C CNN
F 3 "~" H 3700 7750 50  0001 C CNN
	1    3700 7750
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 7550 4050 7550
Connection ~ 4050 7550
Wire Wire Line
	4050 7550 4050 8550
Wire Wire Line
	3400 7750 3250 7750
NoConn ~ 4000 7750
$Comp
L Sensor_Temperature:DS18B20U U2
U 1 1 63F05B3B
P 5350 2550
F 0 "U2" H 5006 2596 50  0000 R CNN
F 1 "DS18B20U" H 5006 2505 50  0000 R CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 4400 2300 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 5200 2800 50  0001 C CNN
	1    5350 2550
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Temperature:DS18B20U U3
U 1 1 63F0679E
P 6550 2550
F 0 "U3" H 6206 2596 50  0000 R CNN
F 1 "DNP" H 6206 2505 50  0000 R CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 5600 2300 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 6400 2800 50  0001 C CNN
	1    6550 2550
	1    0    0    -1  
$EndComp
Text GLabel 6950 2150 2    50   Output ~ 0
1W
Wire Wire Line
	6850 2550 6900 2550
Wire Wire Line
	6900 2550 6900 2150
Connection ~ 6900 2150
Wire Wire Line
	6900 2150 6950 2150
Wire Wire Line
	5650 2550 5700 2550
Wire Wire Line
	5700 2550 5700 2150
Connection ~ 5700 2150
Wire Wire Line
	5700 2150 6900 2150
Wire Wire Line
	4500 2150 5700 2150
Wire Wire Line
	4500 2150 4500 2550
Wire Wire Line
	4500 2550 4450 2550
Wire Wire Line
	4150 2050 5350 2050
Wire Wire Line
	5350 2050 5350 2250
Connection ~ 4150 2050
Wire Wire Line
	5350 2050 6550 2050
Wire Wire Line
	6550 2050 6550 2250
Connection ~ 5350 2050
Wire Wire Line
	6550 2850 6550 3050
Wire Wire Line
	6550 3050 5350 3050
Connection ~ 4150 3050
Wire Wire Line
	5350 2850 5350 3050
Connection ~ 5350 3050
Wire Wire Line
	5350 3050 4150 3050
$Comp
L Connector:Conn_Coaxial J4
U 1 1 63E2AF98
P 3650 10350
F 0 "J4" H 3750 10325 50  0000 L CNN
F 1 "Conn_Coaxial" H 3750 10234 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132203-12_Horizontal" H 3650 10350 50  0001 C CNN
F 3 " ~" H 3650 10350 50  0001 C CNN
	1    3650 10350
	1    0    0    -1  
$EndComp
Connection ~ 13650 8550
$Sheet
S 11750 7600 1100 1300
U 63FA16CC
F0 "pv2rGateDrv" 50
F1 "pv2rGateDrv.sch" 50
F2 "GND_GATE_DRV" I L 11750 8750 50 
F3 "FET_GATE" O R 12850 8550 50 
F4 "UC_DRV_START" I L 11750 7700 50 
F5 "UC_DRV_EN" I L 11750 7850 50 
F6 "V24_GATE_DRV" I L 11750 8350 50 
F7 "UC_DRV_OUT" O R 12850 8000 50 
$EndSheet
Wire Wire Line
	12850 8550 13650 8550
Wire Wire Line
	6400 9500 7700 9500
Wire Wire Line
	11650 9500 11650 8750
Wire Wire Line
	11650 8750 11750 8750
Connection ~ 6400 9500
$Sheet
S 8250 7600 1100 1300
U 6423EAA2
F0 "pv2rPwr" 50
F1 "pv2rPwr.sch" 50
F2 "S2+Fused" I L 8250 7700 50 
F3 "S1-GND" I L 8250 8750 50 
F4 "V24" O R 9350 8350 50 
F5 "S1+Fused" I L 8250 8200 50 
$EndSheet
Wire Wire Line
	6100 4350 6100 4450
Wire Wire Line
	6100 4450 7900 4450
Wire Wire Line
	7900 4450 7900 7700
Wire Wire Line
	7900 7700 8250 7700
Wire Wire Line
	6100 4350 6150 4350
Wire Wire Line
	8250 8200 7700 8200
Wire Wire Line
	7700 8200 7700 6700
Wire Wire Line
	7700 6700 6100 6700
Wire Wire Line
	6100 6700 6100 6600
Wire Wire Line
	6100 6600 6150 6600
Wire Wire Line
	8250 8750 7700 8750
Wire Wire Line
	7700 8750 7700 9500
Connection ~ 7700 9500
Wire Wire Line
	7700 9500 11650 9500
Wire Wire Line
	9350 8350 9650 8350
Text GLabel 9750 8200 2    50   Output ~ 0
V24
Wire Wire Line
	9750 8200 9650 8200
Wire Wire Line
	9650 8200 9650 8350
Connection ~ 9650 8350
Wire Wire Line
	9650 8350 11750 8350
Wire Wire Line
	6000 6700 6100 6700
Connection ~ 6100 6700
Wire Wire Line
	6000 4450 6100 4450
Connection ~ 6100 4450
$Sheet
S 9450 4050 1100 1300
U 644BD67F
F0 "pv2rMeas" 50
F1 "pv2rMeas.sch" 50
F2 "S2+Fused" I L 9450 4450 50 
F3 "GND" I L 9450 5250 50 
F4 "V33P" I L 9450 5100 50 
F5 "IP+" I L 9450 4150 50 
F6 "IP-" O R 10550 4150 50 
F7 "SCL" O R 10550 5100 50 
F8 "SDA" O R 10550 5250 50 
$EndSheet
Wire Wire Line
	5450 3950 10700 3950
Wire Wire Line
	10700 3950 10700 4150
Wire Wire Line
	10700 4150 10550 4150
Connection ~ 5450 3950
Wire Wire Line
	9450 4150 9300 4150
Wire Wire Line
	9300 4150 9300 3850
Wire Wire Line
	9300 3850 10800 3850
Wire Wire Line
	10800 3850 10800 3950
Wire Wire Line
	10800 3950 14250 3950
Text Notes 12350 4250 2    50   ~ 0
Ip+/IP- interchanged for easier layout.\nbidirectional current metering permits.\n(and this would permit 5V hall supply)
Wire Wire Line
	9450 4450 7900 4450
Connection ~ 7900 4450
Text GLabel 9200 5250 0    50   Input ~ 0
GND
Wire Wire Line
	9200 5250 9450 5250
Text GLabel 9200 5100 0    50   Input ~ 0
V33P
Wire Wire Line
	9200 5100 9450 5100
Text GLabel 10850 5250 2    50   Output ~ 0
SDA
Text GLabel 10850 5100 2    50   Output ~ 0
SCL
Wire Wire Line
	10850 5250 10550 5250
Wire Wire Line
	10550 5100 10850 5100
Text GLabel 11450 7700 0    50   Input ~ 0
FET_START
Text GLabel 11450 7850 0    50   Input ~ 0
FET_EN
Text GLabel 13100 8000 2    50   Output ~ 0
FET_STATUS
Wire Wire Line
	11450 7700 11750 7700
Wire Wire Line
	11450 7850 11750 7850
Wire Wire Line
	12850 8000 13100 8000
Text GLabel 1200 2500 0    50   Output ~ 0
FET_START
Text GLabel 1200 2600 0    50   Output ~ 0
FET_EN
Text GLabel 1200 2700 0    50   Output ~ 0
FET_STATUS
Text Notes 1100 5350 1    50   ~ 0
pv string 2
Text Notes 5500 8550 1    50   ~ 0
pv string 1
Text Notes 14300 6750 1    50   ~ 0
resistive heater
NoConn ~ 3650 10550
NoConn ~ 3450 10350
$EndSCHEMATC
