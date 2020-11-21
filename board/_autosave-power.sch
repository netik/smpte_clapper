EESchema Schematic File Version 4
LIBS:clapper-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L Regulator_SwitchedCapacitor:ICL7660 U?
U 1 1 5FA90885
P 1700 1550
F 0 "U?" H 1700 2117 50  0000 C CNN
F 1 "ICL7660" H 1700 2026 50  0000 C CNN
F 2 "" H 1800 1450 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/ICL7660-MAX1044.pdf" H 1800 1450 50  0001 C CNN
	1    1700 1550
	1    0    0    -1  
$EndComp
NoConn ~ 1300 1550
Wire Wire Line
	2100 1450 2550 1450
Wire Wire Line
	2550 1450 2550 1500
Wire Wire Line
	2550 1800 2550 1850
Wire Wire Line
	2550 1850 2100 1850
NoConn ~ 1300 1750
Text Notes 1000 700  0    50   ~ 0
-3.3v / +3.3v Power\n
Text GLabel 2400 1250 2    50   Output ~ 0
-3.3V
Wire Wire Line
	2100 1250 2400 1250
$Comp
L power:GND #PWR?
U 1 1 5FA90896
P 1700 2250
F 0 "#PWR?" H 1700 2000 50  0001 C CNN
F 1 "GND" H 1705 2077 50  0000 C CNN
F 2 "" H 1700 2250 50  0001 C CNN
F 3 "" H 1700 2250 50  0001 C CNN
	1    1700 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 2050 1700 2250
$Comp
L Device:CP1 C?
U 1 1 5FA9089D
P 2550 1650
F 0 "C?" H 2665 1696 50  0000 L CNN
F 1 "10mF" H 2665 1605 50  0000 L CNN
F 2 "" H 2550 1650 50  0001 C CNN
F 3 "~" H 2550 1650 50  0001 C CNN
	1    2550 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5FA90ABE
P 1100 1050
F 0 "#PWR?" H 1100 900 50  0001 C CNN
F 1 "+3.3V" H 1115 1223 50  0000 C CNN
F 2 "" H 1100 1050 50  0001 C CNN
F 3 "" H 1100 1050 50  0001 C CNN
	1    1100 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 1050 1100 1250
Wire Wire Line
	1100 1250 1300 1250
$EndSCHEMATC
