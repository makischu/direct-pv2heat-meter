EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L Regulator_Linear:LM317_3PinPackage U9
U 1 1 637381A3
P 2800 4850
F 0 "U9" H 2800 5092 50  0000 C CNN
F 1 "LM317K-TA3-T" H 2800 5001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 2800 5100 50  0001 C CIN
F 3 "https://www.electro-tech-online.com/threads/lm317-spice-model.18270/" H 2800 4850 50  0001 C CNN
F 4 "X" H 2800 4850 50  0001 C CNN "Spice_Primitive"
F 5 "LM317/TI" H 2800 4850 50  0001 C CNN "Spice_Model"
F 6 "Y" H 2800 4850 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "lm317.lib" H 2800 4850 50  0001 C CNN "Spice_Lib_File"
F 8 "3 1 2" H 2800 4850 50  0001 C CNN "Spice_Node_Sequence"
	1    2800 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 63738E07
P 3250 5200
F 0 "R16" H 3320 5246 50  0000 L CNN
F 1 "680" H 3320 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3180 5200 50  0001 C CNN
F 3 "~" H 3250 5200 50  0001 C CNN
	1    3250 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 63739032
P 3250 5900
F 0 "R20" H 3320 5946 50  0000 L CNN
F 1 "62000 CR1812F62K0E04Z" H 3320 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_1812_4532Metric" V 3180 5900 50  0001 C CNN
F 3 "CR1812F62K0E04Z not 1206W4F6202T5E" H 3250 5900 50  0001 C CNN
	1    3250 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 4850 3250 5050
Wire Wire Line
	3250 5350 3250 5550
Wire Wire Line
	3250 6050 3250 6150
Wire Wire Line
	2800 5150 2800 5550
Wire Wire Line
	2800 5550 3250 5550
Connection ~ 3250 5550
Wire Wire Line
	3250 5550 3250 5750
Wire Wire Line
	3100 4850 3250 4850
Connection ~ 3250 4850
$Comp
L Device:D_Zener D18
U 1 1 63749EEE
P 2800 4300
F 0 "D18" H 2800 4516 50  0000 C CNN
F 1 "1SMB5938BT3G" H 2800 4425 50  0000 C CNN
F 2 "Diode_SMD:D_SMB" H 2800 4300 50  0001 C CNN
F 3 "34.2-37.8V@10.4mA" H 2800 4200 50  0000 C CNN
F 4 "X" H 2800 4300 50  0001 C CNN "Spice_Primitive"
F 5 "DI_BZT52C36" H 2800 4300 50  0001 C CNN "Spice_Model"
F 6 "Y" H 2800 4300 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "/mnt/family/haus/photovoltaik/südwand/elektronik/sim/fetSaveArea/zener.lib" H 2800 4300 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 2800 4300 50  0001 C CNN "Spice_Node_Sequence"
	1    2800 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4300 3250 4300
Wire Wire Line
	3250 4300 3250 4850
Wire Wire Line
	2650 4300 2200 4300
Wire Wire Line
	2200 4300 2200 4850
Connection ~ 2200 4850
Wire Wire Line
	2200 4850 2500 4850
$Comp
L Device:CP C8
U 1 1 6375D186
P 2200 5350
F 0 "C8" H 2318 5396 50  0000 L CNN
F 1 "FS43X105K251EGG" H 2318 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1812_4532Metric" H 2238 5200 50  0001 C CNN
F 3 "1 μF / 250 V / 1812 MLCC statt 0.1u" H 2200 5350 50  0001 C CNN
	1    2200 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 5200 2200 4850
Wire Wire Line
	2200 5500 2200 6150
Connection ~ 2200 6150
Connection ~ 3250 6150
Wire Wire Line
	6900 6150 6400 6150
Wire Wire Line
	6900 5600 6900 6150
Wire Wire Line
	6900 4850 6400 4850
Connection ~ 6900 4850
Wire Wire Line
	6900 5300 6900 4850
Connection ~ 6400 4850
Connection ~ 5350 4850
Wire Wire Line
	5350 4300 5350 4850
Wire Wire Line
	5800 4300 5350 4300
Wire Wire Line
	6400 4300 6400 4850
Wire Wire Line
	6100 4300 6400 4300
Wire Wire Line
	4300 4850 4600 4850
Connection ~ 4300 4850
Wire Wire Line
	4300 4300 4300 4850
Wire Wire Line
	4750 4300 4300 4300
Wire Wire Line
	5050 4300 5350 4300
Wire Wire Line
	3250 4850 3550 4850
Wire Wire Line
	4000 4300 4300 4300
Connection ~ 6400 6150
Wire Wire Line
	3250 6150 4300 6150
Wire Wire Line
	4300 6150 5350 6150
Connection ~ 4300 6150
Wire Wire Line
	5350 6150 6400 6150
Connection ~ 5350 6150
Wire Wire Line
	5600 4850 5350 4850
Wire Wire Line
	4150 4850 4300 4850
Wire Wire Line
	3850 5550 3850 5150
Wire Wire Line
	4900 5150 4900 5550
Wire Wire Line
	5900 5550 5900 5150
Wire Wire Line
	6400 5550 6400 5750
Connection ~ 6400 5550
Wire Wire Line
	5900 5550 6400 5550
Wire Wire Line
	6400 6050 6400 6150
Wire Wire Line
	6400 5350 6400 5550
Wire Wire Line
	6400 4850 6400 5050
Wire Wire Line
	6200 4850 6400 4850
$Comp
L Device:R R23
U 1 1 6373E3F5
P 6400 5900
F 0 "R23" H 6470 5946 50  0000 L CNN
F 1 "4870 RS-06K4871FT" H 6470 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 6330 5900 50  0001 C CNN
F 3 "RS-06K4871FT" H 6400 5900 50  0001 C CNN
	1    6400 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 6373E3EB
P 6400 5200
F 0 "R19" H 6470 5246 50  0000 L CNN
F 1 "270" H 6470 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6330 5200 50  0001 C CNN
F 3 "~" H 6400 5200 50  0001 C CNN
	1    6400 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 5550 5350 5750
Connection ~ 5350 5550
Wire Wire Line
	4900 5550 5350 5550
Wire Wire Line
	5350 6050 5350 6150
Wire Wire Line
	5350 5350 5350 5550
Wire Wire Line
	5350 4850 5350 5050
Wire Wire Line
	5200 4850 5350 4850
$Comp
L Device:R R22
U 1 1 6373CBFF
P 5350 5900
F 0 "R22" H 5420 5946 50  0000 L CNN
F 1 "20k RS-10K2002FT" H 5420 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_2010_5025Metric" V 5280 5900 50  0001 C CNN
F 3 "RS-10K2002FT  not 1206W4F2002T5E" H 5350 5900 50  0001 C CNN
	1    5350 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 6373CBF5
P 5350 5200
F 0 "R18" H 5420 5246 50  0000 L CNN
F 1 "470" H 5420 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5280 5200 50  0001 C CNN
F 3 "~" H 5350 5200 50  0001 C CNN
	1    5350 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 5550 4300 5750
Connection ~ 4300 5550
Wire Wire Line
	3850 5550 4300 5550
Wire Wire Line
	4300 6050 4300 6150
Wire Wire Line
	4300 5350 4300 5550
Wire Wire Line
	4300 4850 4300 5050
$Comp
L Device:R R21
U 1 1 6373B24F
P 4300 5900
F 0 "R21" H 4370 5946 50  0000 L CNN
F 1 "45300 WR12X4532FTL" H 4370 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4230 5900 50  0001 C CNN
F 3 "WR12X4532FTL" H 4300 5900 50  0001 C CNN
	1    4300 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 6373B245
P 4300 5200
F 0 "R17" H 4370 5246 50  0000 L CNN
F 1 "680" H 4370 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4230 5200 50  0001 C CNN
F 3 "~" H 4300 5200 50  0001 C CNN
	1    4300 5200
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM317_3PinPackage U12
U 1 1 6373AB3D
P 5900 4850
F 0 "U12" H 5900 5092 50  0000 C CNN
F 1 "LM317K-TA3-T" H 5900 5001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5900 5100 50  0001 C CIN
F 3 "https://www.electro-tech-online.com/threads/lm317-spice-model.18270/" H 5900 4850 50  0001 C CNN
F 4 "X" H 5900 4850 50  0001 C CNN "Spice_Primitive"
F 5 "LM317/TI" H 5900 4850 50  0001 C CNN "Spice_Model"
F 6 "Y" H 5900 4850 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "lm317.lib" H 5900 4850 50  0001 C CNN "Spice_Lib_File"
F 8 "3 1 2" H 5900 4850 50  0001 C CNN "Spice_Node_Sequence"
	1    5900 4850
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM317_3PinPackage U11
U 1 1 6373A826
P 4900 4850
F 0 "U11" H 4900 5092 50  0000 C CNN
F 1 "LM317K-TA3-T" H 4900 5001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4900 5100 50  0001 C CIN
F 3 "https://www.electro-tech-online.com/threads/lm317-spice-model.18270/" H 4900 4850 50  0001 C CNN
F 4 "X" H 4900 4850 50  0001 C CNN "Spice_Primitive"
F 5 "LM317/TI" H 4900 4850 50  0001 C CNN "Spice_Model"
F 6 "Y" H 4900 4850 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "lm317.lib" H 4900 4850 50  0001 C CNN "Spice_Lib_File"
F 8 "3 1 2" H 4900 4850 50  0001 C CNN "Spice_Node_Sequence"
	1    4900 4850
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM317_3PinPackage U10
U 1 1 6373A192
P 3850 4850
F 0 "U10" H 3850 5092 50  0000 C CNN
F 1 "LM317K-TA3-T" H 3850 5001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3850 5100 50  0001 C CIN
F 3 "https://www.electro-tech-online.com/threads/lm317-spice-model.18270/" H 3850 4850 50  0001 C CNN
F 4 "X" H 3850 4850 50  0001 C CNN "Spice_Primitive"
F 5 "LM317/TI" H 3850 4850 50  0001 C CNN "Spice_Model"
F 6 "Y" H 3850 4850 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "lm317.lib" H 3850 4850 50  0001 C CNN "Spice_Lib_File"
F 8 "3 1 2" H 3850 4850 50  0001 C CNN "Spice_Node_Sequence"
	1    3850 4850
	1    0    0    -1  
$EndComp
Connection ~ 3250 4300
Wire Wire Line
	3250 4300 3700 4300
Connection ~ 4300 4300
Connection ~ 5350 4300
Wire Wire Line
	2200 6150 3250 6150
Text GLabel 1650 4850 0    50   Input ~ 0
S+
$Comp
L Device:D_Zener D19
U 1 1 65154884
P 3850 4300
F 0 "D19" H 3850 4516 50  0000 C CNN
F 1 "1SMB5938BT3G" H 3850 4425 50  0000 C CNN
F 2 "Diode_SMD:D_SMB" H 3850 4300 50  0001 C CNN
F 3 "34.2-37.8V@10.4mA" H 3850 4200 50  0000 C CNN
F 4 "X" H 3850 4300 50  0001 C CNN "Spice_Primitive"
F 5 "DI_BZT52C36" H 3850 4300 50  0001 C CNN "Spice_Model"
F 6 "Y" H 3850 4300 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "/mnt/family/haus/photovoltaik/südwand/elektronik/sim/fetSaveArea/zener.lib" H 3850 4300 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 3850 4300 50  0001 C CNN "Spice_Node_Sequence"
	1    3850 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener D20
U 1 1 65154FC1
P 4900 4300
F 0 "D20" H 4900 4516 50  0000 C CNN
F 1 "1SMB5938BT3G" H 4900 4425 50  0000 C CNN
F 2 "Diode_SMD:D_SMB" H 4900 4300 50  0001 C CNN
F 3 "34.2-37.8V@10.4mA" H 4900 4200 50  0000 C CNN
F 4 "X" H 4900 4300 50  0001 C CNN "Spice_Primitive"
F 5 "DI_BZT52C36" H 4900 4300 50  0001 C CNN "Spice_Model"
F 6 "Y" H 4900 4300 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "/mnt/family/haus/photovoltaik/südwand/elektronik/sim/fetSaveArea/zener.lib" H 4900 4300 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 4900 4300 50  0001 C CNN "Spice_Node_Sequence"
	1    4900 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener D21
U 1 1 65155787
P 5950 4300
F 0 "D21" H 5950 4516 50  0000 C CNN
F 1 "1SMB5938BT3G" H 5950 4425 50  0000 C CNN
F 2 "Diode_SMD:D_SMB" H 5950 4300 50  0001 C CNN
F 3 "34.2-37.8V@10.4mA" H 5950 4200 50  0000 C CNN
F 4 "X" H 5950 4300 50  0001 C CNN "Spice_Primitive"
F 5 "DI_BZT52C36" H 5950 4300 50  0001 C CNN "Spice_Model"
F 6 "Y" H 5950 4300 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "/mnt/family/haus/photovoltaik/südwand/elektronik/sim/fetSaveArea/zener.lib" H 5950 4300 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 5950 4300 50  0001 C CNN "Spice_Node_Sequence"
	1    5950 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C9
U 1 1 651584A4
P 6900 5450
F 0 "C9" H 7018 5496 50  0000 L CNN
F 1 "FS43X105K251EGG" H 7018 5405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1812_4532Metric" H 6938 5300 50  0001 C CNN
F 3 "1 μF / 250 V / 1812 MLCC statt 0.1u" H 6900 5450 50  0001 C CNN
	1    6900 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4850 2200 4850
$Comp
L pspice:R R12
U 1 1 6349DACD
P 1900 1200
F 0 "R12" H 1968 1246 50  0000 L CNN
F 1 "220K" H 1968 1155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 1900 1200 50  0001 C CNN
F 3 "~" H 1900 1200 50  0001 C CNN
	1    1900 1200
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R13
U 1 1 6349DEB2
P 1900 2000
F 0 "R13" H 1968 2046 50  0000 L CNN
F 1 "220K" H 1968 1955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 1900 2000 50  0001 C CNN
F 3 "~" H 1900 2000 50  0001 C CNN
	1    1900 2000
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R14
U 1 1 6349E220
P 3700 2100
F 0 "R14" H 3768 2146 50  0000 L CNN
F 1 "220K" H 3768 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3700 2100 50  0001 C CNN
F 3 "~" H 3700 2100 50  0001 C CNN
	1    3700 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 900  4300 1250
Wire Wire Line
	3700 1000 3700 900 
Connection ~ 3700 900 
Wire Wire Line
	3700 900  4300 900 
Wire Wire Line
	2350 1000 2350 900 
Connection ~ 2350 900 
Wire Wire Line
	2350 900  2950 900 
Wire Wire Line
	1900 950  1900 900 
Wire Wire Line
	1900 900  2350 900 
Wire Wire Line
	1900 2250 1900 2450
Wire Wire Line
	1900 2750 1900 3000
Wire Wire Line
	4000 1450 3700 1450
Wire Wire Line
	3700 1300 3700 1450
Connection ~ 3700 1450
Wire Wire Line
	3700 1450 3700 1800
Wire Wire Line
	2950 1800 3700 1800
Connection ~ 3700 1800
Wire Wire Line
	3700 1800 3700 1850
Wire Wire Line
	2950 1400 2950 900 
Connection ~ 2950 900 
Wire Wire Line
	4300 1650 4300 1750
Wire Wire Line
	3700 2350 3700 2400
Wire Wire Line
	2950 900  3700 900 
$Comp
L pspice:R R15
U 1 1 63E1BECD
P 3700 2650
F 0 "R15" H 3768 2696 50  0000 L CNN
F 1 "220K" H 3768 2605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3700 2650 50  0001 C CNN
F 3 "~" H 3700 2650 50  0001 C CNN
	1    3700 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2900 3700 3000
Wire Wire Line
	1900 3000 3700 3000
Connection ~ 1900 1600
Wire Wire Line
	1900 1600 1900 1750
Wire Wire Line
	1900 1450 1900 1600
Connection ~ 2350 1600
Wire Wire Line
	2350 1600 1900 1600
Wire Wire Line
	2350 1600 2350 1300
Wire Wire Line
	2650 1600 2350 1600
$Comp
L Device:D_Zener D12
U 1 1 634A1683
P 2350 1150
F 0 "D12" V 2304 1229 50  0000 L CNN
F 1 "ZMM5V6-M" V 2395 1229 50  0000 L CNN
F 2 "Diode_SMD:D_MiniMELF" H 2350 1150 50  0001 C CNN
F 3 "~" H 2350 1150 50  0001 C CNN
F 4 "X" H 2350 1150 50  0001 C CNN "Spice_Primitive"
F 5 "DI_BZX84C16" H 2350 1150 50  0001 C CNN "Spice_Model"
F 6 "Y" H 2350 1150 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "zener.lib" H 2350 1150 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 2350 1150 50  0001 C CNN "Spice_Node_Sequence"
	1    2350 1150
	0    1    1    0   
$EndComp
Text GLabel 4850 1750 2    50   Output ~ 0
S2+_IfLow
Wire Wire Line
	4300 1750 4850 1750
Text HLabel 1300 900  0    50   Input ~ 0
S2+Fused
Text HLabel 1300 3000 0    50   Input ~ 0
S1-GND
Wire Wire Line
	1300 3000 1900 3000
Connection ~ 1900 3000
Wire Wire Line
	1300 900  1900 900 
Connection ~ 1900 900 
Text HLabel 9600 4850 2    50   Output ~ 0
V24
Wire Wire Line
	6900 4850 9600 4850
Text HLabel 1300 6150 0    50   Input ~ 0
S1-GND
Wire Wire Line
	1300 6150 2200 6150
Text GLabel 7500 1750 0    50   Input ~ 0
S2+_IfLow
Text GLabel 9650 2200 2    50   Output ~ 0
S+
Text HLabel 7500 2200 0    50   Input ~ 0
S1+Fused
$Comp
L Device:D_Zener D?
U 1 1 6432EE74
P 9100 2500
AR Path="/6432EE74" Ref="D?"  Part="1" 
AR Path="/6423EAA2/6432EE74" Ref="D16"  Part="1" 
F 0 "D16" V 9054 2579 50  0000 L CNN
F 1 "SMAJ150A" V 9145 2579 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 9100 2500 50  0001 C CNN
F 3 "~" H 9100 2500 50  0001 C CNN
	1    9100 2500
	0    1    1    0   
$EndComp
Text HLabel 7500 3100 0    50   Input ~ 0
S1-GND
Wire Wire Line
	7500 3100 9100 3100
Wire Wire Line
	9100 2650 9100 3100
Wire Wire Line
	9100 2350 9100 2200
Wire Wire Line
	9100 2200 9650 2200
$Comp
L Device:D D14
U 1 1 64346243
P 8200 1750
F 0 "D14" H 8200 1534 50  0000 C CNN
F 1 "SM4007PL" H 8200 1625 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123F" H 8200 1750 50  0001 C CNN
F 3 "~" H 8200 1750 50  0001 C CNN
	1    8200 1750
	-1   0    0    1   
$EndComp
$Comp
L Device:D D15
U 1 1 6434682C
P 8200 2200
F 0 "D15" H 8200 1984 50  0000 C CNN
F 1 "SM4007PL" H 8200 2075 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123F" H 8200 2200 50  0001 C CNN
F 3 "~" H 8200 2200 50  0001 C CNN
	1    8200 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	8050 1750 7500 1750
Wire Wire Line
	7500 2200 8050 2200
Wire Wire Line
	8350 2200 9100 2200
Connection ~ 9100 2200
Wire Wire Line
	8350 1750 9100 1750
Wire Wire Line
	9100 1750 9100 2200
Text Notes 5650 850  2    50   ~ 0
overvoltage off
Text Notes 9850 850  2    50   ~ 0
input selection ('or')
Text Notes 9900 3950 2    50   ~ 0
linear regulator with wide input range\n(but not wide enough for both strings in series)
$Comp
L Device:D_Zener D17
U 1 1 634A1971
P 1900 2600
F 0 "D17" V 1854 2679 50  0000 L CNN
F 1 "3SMAJ5952B-TP" V 1945 2679 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 1900 2600 50  0001 C CNN
F 3 "~" H 1900 2600 50  0001 C CNN
F 4 "X" H 1900 2600 50  0001 C CNN "Spice_Primitive"
F 5 "DI_1N4764A" H 1900 2600 50  0001 C CNN "Spice_Model"
F 6 "Y" H 1900 2600 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "zener.lib" H 1900 2600 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 1900 2600 50  0001 C CNN "Spice_Node_Sequence"
	1    1900 2600
	0    1    1    0   
$EndComp
Text Notes 1750 2650 2    50   ~ 0
130v zener
$Comp
L Device:D_Zener D13
U 1 1 643775D5
P 3700 1150
F 0 "D13" V 3654 1229 50  0000 L CNN
F 1 "ZMM5V6-M" V 3745 1229 50  0000 L CNN
F 2 "Diode_SMD:D_MiniMELF" H 3700 1150 50  0001 C CNN
F 3 "~" H 3700 1150 50  0001 C CNN
F 4 "X" H 3700 1150 50  0001 C CNN "Spice_Primitive"
F 5 "DI_BZX84C16" H 3700 1150 50  0001 C CNN "Spice_Model"
F 6 "Y" H 3700 1150 50  0001 C CNN "Spice_Netlist_Enabled"
F 7 "zener.lib" H 3700 1150 50  0001 C CNN "Spice_Lib_File"
F 8 "2 1" H 3700 1150 50  0001 C CNN "Spice_Node_Sequence"
	1    3700 1150
	0    1    1    0   
$EndComp
$Comp
L Device:Q_PMOS_GSD Q5
U 1 1 64396CEE
P 2850 1600
F 0 "Q5" H 3055 1554 50  0000 L CNN
F 1 "FTZ30P35G" H 3055 1645 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3050 1700 50  0001 C CNN
F 3 "~" H 2850 1600 50  0001 C CNN
	1    2850 1600
	1    0    0    1   
$EndComp
$Comp
L Device:Q_PMOS_GSD Q4
U 1 1 64398291
P 4200 1450
F 0 "Q4" H 4405 1404 50  0000 L CNN
F 1 "FTZ30P35G" H 4405 1495 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4400 1550 50  0001 C CNN
F 3 "~" H 4200 1450 50  0001 C CNN
	1    4200 1450
	1    0    0    1   
$EndComp
$EndSCHEMATC
