EESchema Schematic File Version 4
LIBS:clapper-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L Display_Character:MAN74A U203
U 1 1 5F5B9FA9
P 5350 1950
F 0 "U203" H 5350 2617 50  0000 C CNN
F 1 "MAN74A" H 5350 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 4850 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 5360 2290 50  0001 L CNN
	1    5350 1950
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U204
U 1 1 5F5B9FB3
P 6350 1950
F 0 "U204" H 6350 2617 50  0000 C CNN
F 1 "MAN74A" H 6350 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 5850 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 6360 2290 50  0001 L CNN
	1    6350 1950
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U205
U 1 1 5F5BE185
P 7400 1950
F 0 "U205" H 7400 2617 50  0000 C CNN
F 1 "MAN74A" H 7400 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 6900 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 7410 2290 50  0001 L CNN
	1    7400 1950
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U206
U 1 1 5F5BE18F
P 8400 1950
F 0 "U206" H 8400 2617 50  0000 C CNN
F 1 "MAN74A" H 8400 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 7900 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 8410 2290 50  0001 L CNN
	1    8400 1950
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U207
U 1 1 5F5BE199
P 9450 1950
F 0 "U207" H 9450 2617 50  0000 C CNN
F 1 "MAN74A" H 9450 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 8950 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 9460 2290 50  0001 L CNN
	1    9450 1950
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U208
U 1 1 5F5BE1A3
P 10450 1950
F 0 "U208" H 10450 2617 50  0000 C CNN
F 1 "MAN74A" H 10450 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 9950 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 10460 2290 50  0001 L CNN
	1    10450 1950
	1    0    0    -1  
$EndComp
Entry Wire Line
	2400 3600 2500 3500
Entry Wire Line
	2400 3700 2500 3600
Entry Wire Line
	2400 3900 2500 3800
Entry Wire Line
	2400 3800 2500 3700
Entry Wire Line
	2400 4000 2500 3900
Entry Wire Line
	2400 4100 2500 4000
Entry Wire Line
	2400 4200 2500 4100
Wire Wire Line
	2400 3600 2100 3600
Wire Wire Line
	2100 3700 2400 3700
Wire Wire Line
	2400 3800 2100 3800
Wire Wire Line
	2100 3900 2400 3900
Wire Wire Line
	2400 4000 2100 4000
Wire Wire Line
	2100 4100 2400 4100
Wire Wire Line
	2400 4200 2100 4200
Entry Wire Line
	2400 4300 2500 4200
Wire Wire Line
	2400 4300 2100 4300
$Comp
L clapper_symbols:MAX7219 U209
U 1 1 5F5F7862
P 1550 4050
F 0 "U209" H 1200 4900 50  0000 C CNN
F 1 "MAX7219" H 1350 4100 50  0001 L BNN
F 2 "" H 1550 4050 50  0001 C CNN
F 3 "" H 1550 4050 50  0001 C CNN
	1    1550 4050
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 5F5FBBD7
P 1700 5200
F 0 "#PWR0122" H 1700 4950 50  0001 C CNN
F 1 "GND" H 1705 5027 50  0000 C CNN
F 2 "" H 1700 5200 50  0001 C CNN
F 3 "" H 1700 5200 50  0001 C CNN
	1    1700 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4900 1450 5050
Wire Wire Line
	1450 5050 1700 5050
Wire Wire Line
	1700 5050 1700 4900
Wire Wire Line
	1700 5050 1700 5200
Connection ~ 1700 5050
$Comp
L power:+5V #PWR0123
U 1 1 5F5FCF52
P 1550 2250
F 0 "#PWR0123" H 1550 2100 50  0001 C CNN
F 1 "+5V" H 1565 2423 50  0000 C CNN
F 2 "" H 1550 2250 50  0001 C CNN
F 3 "" H 1550 2250 50  0001 C CNN
	1    1550 2250
	1    0    0    -1  
$EndComp
Entry Bus Bus
	3650 1150 3750 1250
Entry Bus Bus
	4700 1150 4800 1250
Entry Bus Bus
	5700 1150 5800 1250
Entry Bus Bus
	6750 1150 6850 1250
Entry Bus Bus
	7750 1150 7850 1250
Entry Bus Bus
	8800 1150 8900 1250
Entry Bus Bus
	9800 1150 9900 1250
Entry Wire Line
	3750 2250 3850 2350
Entry Wire Line
	3750 2050 3850 2150
Entry Wire Line
	3750 1950 3850 2050
Entry Wire Line
	3750 1850 3850 1950
Entry Wire Line
	3750 1750 3850 1850
Entry Wire Line
	3750 1650 3850 1750
Entry Wire Line
	3750 1550 3850 1650
Entry Wire Line
	3750 1450 3850 1550
$Comp
L Display_Character:MAN74A U202
U 1 1 5F5AE0F6
P 4300 1950
F 0 "U202" H 4300 2617 50  0000 C CNN
F 1 "MAN74A" H 4300 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 3800 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 4310 2290 50  0001 L CNN
	1    4300 1950
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U201
U 1 1 5F5ACDA3
P 3300 1950
F 0 "U201" H 3300 2617 50  0000 C CNN
F 1 "MAN74A" H 3300 2526 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 2800 1250 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 3310 2290 50  0001 L CNN
	1    3300 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 1550 4000 1550
Wire Wire Line
	4000 1650 3850 1650
Wire Wire Line
	3850 1750 4000 1750
Wire Wire Line
	4000 1850 3850 1850
Wire Wire Line
	3850 1950 4000 1950
Wire Wire Line
	4000 2050 3850 2050
Wire Wire Line
	3850 2150 4000 2150
Wire Wire Line
	4000 2350 3850 2350
Text Label 3900 1550 0    50   ~ 0
A
Text Label 3900 1650 0    50   ~ 0
B
Text Label 3900 1750 0    50   ~ 0
C
Text Label 3900 1850 0    50   ~ 0
D
Text Label 3900 1950 0    50   ~ 0
E
Text Label 3900 2050 0    50   ~ 0
F
Text Label 3900 2150 0    50   ~ 0
G
Text Label 3900 2350 0    50   ~ 0
DP
Entry Wire Line
	2750 2250 2850 2350
Entry Wire Line
	2750 2050 2850 2150
Entry Wire Line
	2750 1950 2850 2050
Entry Wire Line
	2750 1850 2850 1950
Entry Wire Line
	2750 1750 2850 1850
Entry Wire Line
	2750 1650 2850 1750
Entry Wire Line
	2750 1550 2850 1650
Entry Wire Line
	2750 1450 2850 1550
Wire Wire Line
	2850 1550 3000 1550
Wire Wire Line
	3000 1650 2850 1650
Wire Wire Line
	2850 1750 3000 1750
Wire Wire Line
	3000 1850 2850 1850
Wire Wire Line
	2850 1950 3000 1950
Wire Wire Line
	3000 2050 2850 2050
Wire Wire Line
	2850 2150 3000 2150
Wire Wire Line
	3000 2350 2850 2350
Text Label 2900 1550 0    50   ~ 0
A
Text Label 2900 1650 0    50   ~ 0
B
Text Label 2900 1750 0    50   ~ 0
C
Text Label 2900 1850 0    50   ~ 0
D
Text Label 2900 1950 0    50   ~ 0
E
Text Label 2900 2050 0    50   ~ 0
F
Text Label 2900 2150 0    50   ~ 0
G
Text Label 2900 2350 0    50   ~ 0
DP
Entry Wire Line
	4800 2250 4900 2350
Entry Wire Line
	4800 2050 4900 2150
Entry Wire Line
	4800 1950 4900 2050
Entry Wire Line
	4800 1850 4900 1950
Entry Wire Line
	4800 1750 4900 1850
Entry Wire Line
	4800 1650 4900 1750
Entry Wire Line
	4800 1550 4900 1650
Entry Wire Line
	4800 1450 4900 1550
Wire Wire Line
	4900 1550 5050 1550
Wire Wire Line
	5050 1650 4900 1650
Wire Wire Line
	4900 1750 5050 1750
Wire Wire Line
	5050 1850 4900 1850
Wire Wire Line
	4900 1950 5050 1950
Wire Wire Line
	5050 2050 4900 2050
Wire Wire Line
	4900 2150 5050 2150
Wire Wire Line
	5050 2350 4900 2350
Text Label 4950 1550 0    50   ~ 0
A
Text Label 4950 1650 0    50   ~ 0
B
Text Label 4950 1750 0    50   ~ 0
C
Text Label 4950 1850 0    50   ~ 0
D
Text Label 4950 1950 0    50   ~ 0
E
Text Label 4950 2050 0    50   ~ 0
F
Text Label 4950 2150 0    50   ~ 0
G
Text Label 4950 2350 0    50   ~ 0
DP
Entry Wire Line
	5800 2250 5900 2350
Entry Wire Line
	5800 2050 5900 2150
Entry Wire Line
	5800 1950 5900 2050
Entry Wire Line
	5800 1850 5900 1950
Entry Wire Line
	5800 1750 5900 1850
Entry Wire Line
	5800 1650 5900 1750
Entry Wire Line
	5800 1550 5900 1650
Entry Wire Line
	5800 1450 5900 1550
Wire Wire Line
	5900 1550 6050 1550
Wire Wire Line
	6050 1650 5900 1650
Wire Wire Line
	5900 1750 6050 1750
Wire Wire Line
	6050 1850 5900 1850
Wire Wire Line
	5900 1950 6050 1950
Wire Wire Line
	6050 2050 5900 2050
Wire Wire Line
	5900 2150 6050 2150
Wire Wire Line
	6050 2350 5900 2350
Text Label 5950 1550 0    50   ~ 0
A
Text Label 5950 1650 0    50   ~ 0
B
Text Label 5950 1750 0    50   ~ 0
C
Text Label 5950 1850 0    50   ~ 0
D
Text Label 5950 1950 0    50   ~ 0
E
Text Label 5950 2050 0    50   ~ 0
F
Text Label 5950 2150 0    50   ~ 0
G
Text Label 5950 2350 0    50   ~ 0
DP
Entry Wire Line
	6850 2250 6950 2350
Entry Wire Line
	6850 2050 6950 2150
Entry Wire Line
	6850 1950 6950 2050
Entry Wire Line
	6850 1850 6950 1950
Entry Wire Line
	6850 1750 6950 1850
Entry Wire Line
	6850 1650 6950 1750
Entry Wire Line
	6850 1550 6950 1650
Entry Wire Line
	6850 1450 6950 1550
Wire Wire Line
	6950 1550 7100 1550
Wire Wire Line
	7100 1650 6950 1650
Wire Wire Line
	6950 1750 7100 1750
Wire Wire Line
	7100 1850 6950 1850
Wire Wire Line
	6950 1950 7100 1950
Wire Wire Line
	7100 2050 6950 2050
Wire Wire Line
	6950 2150 7100 2150
Wire Wire Line
	7100 2350 6950 2350
Text Label 7000 1550 0    50   ~ 0
A
Text Label 7000 1650 0    50   ~ 0
B
Text Label 7000 1750 0    50   ~ 0
C
Text Label 7000 1850 0    50   ~ 0
D
Text Label 7000 1950 0    50   ~ 0
E
Text Label 7000 2050 0    50   ~ 0
F
Text Label 7000 2150 0    50   ~ 0
G
Text Label 7000 2350 0    50   ~ 0
DP
Entry Wire Line
	7850 2250 7950 2350
Entry Wire Line
	7850 2050 7950 2150
Entry Wire Line
	7850 1950 7950 2050
Entry Wire Line
	7850 1850 7950 1950
Entry Wire Line
	7850 1750 7950 1850
Entry Wire Line
	7850 1650 7950 1750
Entry Wire Line
	7850 1550 7950 1650
Entry Wire Line
	7850 1450 7950 1550
Wire Wire Line
	7950 1550 8100 1550
Wire Wire Line
	8100 1650 7950 1650
Wire Wire Line
	7950 1750 8100 1750
Wire Wire Line
	8100 1850 7950 1850
Wire Wire Line
	7950 1950 8100 1950
Wire Wire Line
	8100 2050 7950 2050
Wire Wire Line
	7950 2150 8100 2150
Wire Wire Line
	8100 2350 7950 2350
Text Label 8000 1550 0    50   ~ 0
A
Text Label 8000 1650 0    50   ~ 0
B
Text Label 8000 1750 0    50   ~ 0
C
Text Label 8000 1850 0    50   ~ 0
D
Text Label 8000 1950 0    50   ~ 0
E
Text Label 8000 2050 0    50   ~ 0
F
Text Label 8000 2150 0    50   ~ 0
G
Text Label 8000 2350 0    50   ~ 0
DP
Entry Wire Line
	8900 2250 9000 2350
Entry Wire Line
	8900 2050 9000 2150
Entry Wire Line
	8900 1950 9000 2050
Entry Wire Line
	8900 1850 9000 1950
Entry Wire Line
	8900 1750 9000 1850
Entry Wire Line
	8900 1650 9000 1750
Entry Wire Line
	8900 1550 9000 1650
Entry Wire Line
	8900 1450 9000 1550
Wire Wire Line
	9000 1550 9150 1550
Wire Wire Line
	9150 1650 9000 1650
Wire Wire Line
	9000 1750 9150 1750
Wire Wire Line
	9150 1850 9000 1850
Wire Wire Line
	9000 1950 9150 1950
Wire Wire Line
	9150 2050 9000 2050
Wire Wire Line
	9000 2150 9150 2150
Wire Wire Line
	9150 2350 9000 2350
Text Label 9050 1550 0    50   ~ 0
A
Text Label 9050 1650 0    50   ~ 0
B
Text Label 9050 1750 0    50   ~ 0
C
Text Label 9050 1850 0    50   ~ 0
D
Text Label 9050 1950 0    50   ~ 0
E
Text Label 9050 2050 0    50   ~ 0
F
Text Label 9050 2150 0    50   ~ 0
G
Text Label 9050 2350 0    50   ~ 0
DP
Entry Wire Line
	9900 2250 10000 2350
Entry Wire Line
	9900 2050 10000 2150
Entry Wire Line
	9900 1950 10000 2050
Entry Wire Line
	9900 1850 10000 1950
Entry Wire Line
	9900 1750 10000 1850
Entry Wire Line
	9900 1650 10000 1750
Entry Wire Line
	9900 1550 10000 1650
Entry Wire Line
	9900 1450 10000 1550
Wire Wire Line
	10000 1550 10150 1550
Wire Wire Line
	10150 1650 10000 1650
Wire Wire Line
	10000 1750 10150 1750
Wire Wire Line
	10150 1850 10000 1850
Wire Wire Line
	10000 1950 10150 1950
Wire Wire Line
	10150 2050 10000 2050
Wire Wire Line
	10000 2150 10150 2150
Wire Wire Line
	10150 2350 10000 2350
Text Label 10050 1550 0    50   ~ 0
A
Text Label 10050 1650 0    50   ~ 0
B
Text Label 10050 1750 0    50   ~ 0
C
Text Label 10050 1850 0    50   ~ 0
D
Text Label 10050 1950 0    50   ~ 0
E
Text Label 10050 2050 0    50   ~ 0
F
Text Label 10050 2150 0    50   ~ 0
G
Text Label 10050 2350 0    50   ~ 0
DP
Entry Bus Bus
	2650 1150 2750 1250
Text Notes 3150 2600 0    50   ~ 10
Digit 7\n
Text Notes 10300 2650 0    50   ~ 10
Digit 0\n
Entry Wire Line
	800  3500 900  3600
Wire Wire Line
	1000 3600 900  3600
Text Label 900  3600 0    50   ~ 0
DP
Entry Wire Line
	800  3600 900  3700
Entry Wire Line
	800  3700 900  3800
Entry Wire Line
	800  3800 900  3900
Entry Wire Line
	800  3900 900  4000
Entry Wire Line
	800  4000 900  4100
Entry Wire Line
	800  4100 900  4200
Entry Wire Line
	800  4200 900  4300
Wire Wire Line
	1000 3700 900  3700
Wire Wire Line
	900  3800 1000 3800
Wire Wire Line
	1000 3900 900  3900
Wire Wire Line
	900  4000 1000 4000
Wire Wire Line
	1000 4100 900  4100
Wire Wire Line
	900  4200 1000 4200
Wire Wire Line
	1000 4300 900  4300
Text Label 900  3700 0    50   ~ 0
A
Text Label 900  3800 0    50   ~ 0
B
Text Label 900  3900 0    50   ~ 0
C
Text Label 900  4000 0    50   ~ 0
D
Text Label 900  4100 0    50   ~ 0
E
Text Label 900  4200 0    50   ~ 0
F
Text Label 900  4300 0    50   ~ 0
G
Text Label 2200 3600 0    50   ~ 0
CC0
Text Label 2200 3800 0    50   ~ 0
CC2
Text Label 2200 3700 0    50   ~ 0
CC1
Text Label 2200 3900 0    50   ~ 0
CC3
Text Label 2200 4000 0    50   ~ 0
CC4
Text Label 2200 4100 0    50   ~ 0
CC5
Text Label 2200 4200 0    50   ~ 0
CC6
Text Label 2200 4300 0    50   ~ 0
CC7
NoConn ~ 1000 3400
Entry Wire Line
	3650 2650 3750 2750
Wire Wire Line
	3600 2250 3650 2250
Wire Wire Line
	3650 2250 3650 2350
Wire Wire Line
	3600 2350 3650 2350
Connection ~ 3650 2350
Wire Wire Line
	3650 2350 3650 2650
Text Label 3650 2600 0    50   ~ 0
CC7
Entry Wire Line
	4650 2650 4750 2750
Wire Wire Line
	4600 2250 4650 2250
Wire Wire Line
	4650 2250 4650 2350
Wire Wire Line
	4600 2350 4650 2350
Connection ~ 4650 2350
Wire Wire Line
	4650 2350 4650 2650
Text Label 4650 2600 0    50   ~ 0
CC6
Entry Wire Line
	5700 2650 5800 2750
Wire Wire Line
	5650 2250 5700 2250
Wire Wire Line
	5700 2250 5700 2350
Wire Wire Line
	5650 2350 5700 2350
Connection ~ 5700 2350
Wire Wire Line
	5700 2350 5700 2650
Text Label 5700 2600 0    50   ~ 0
CC5
Entry Wire Line
	6700 2650 6800 2750
Wire Wire Line
	6650 2250 6700 2250
Wire Wire Line
	6700 2250 6700 2350
Wire Wire Line
	6650 2350 6700 2350
Connection ~ 6700 2350
Wire Wire Line
	6700 2350 6700 2650
Text Label 6700 2600 0    50   ~ 0
CC4
Entry Wire Line
	7750 2650 7850 2750
Wire Wire Line
	7700 2250 7750 2250
Wire Wire Line
	7750 2250 7750 2350
Wire Wire Line
	7700 2350 7750 2350
Connection ~ 7750 2350
Wire Wire Line
	7750 2350 7750 2650
Text Label 7750 2600 0    50   ~ 0
CC3
Entry Wire Line
	8750 2650 8850 2750
Wire Wire Line
	8700 2250 8750 2250
Wire Wire Line
	8750 2250 8750 2350
Wire Wire Line
	8700 2350 8750 2350
Connection ~ 8750 2350
Wire Wire Line
	8750 2350 8750 2650
Text Label 8750 2600 0    50   ~ 0
CC2
Entry Wire Line
	9800 2650 9900 2750
Wire Wire Line
	9750 2250 9800 2250
Wire Wire Line
	9800 2250 9800 2350
Wire Wire Line
	9750 2350 9800 2350
Connection ~ 9800 2350
Wire Wire Line
	9800 2350 9800 2650
Text Label 9800 2600 0    50   ~ 0
CC1
Entry Wire Line
	10800 2650 10900 2750
Wire Wire Line
	10750 2250 10800 2250
Wire Wire Line
	10800 2250 10800 2350
Wire Wire Line
	10750 2350 10800 2350
Connection ~ 10800 2350
Wire Wire Line
	10800 2350 10800 2650
Text Label 10800 2600 0    50   ~ 0
CC0
$Comp
L Device:R R?
U 1 1 5F68F8C2
P 1050 2650
F 0 "R?" H 1120 2696 50  0000 L CNN
F 1 "12K" H 1120 2605 50  0000 L CNN
F 2 "" V 980 2650 50  0001 C CNN
F 3 "~" H 1050 2650 50  0001 C CNN
	1    1050 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F697F90
P 1850 2550
F 0 "C?" H 1965 2596 50  0000 L CNN
F 1 "10uF" H 1965 2505 50  0000 L CNN
F 2 "" H 1888 2400 50  0001 C CNN
F 3 "~" H 1850 2550 50  0001 C CNN
	1    1850 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F6994A8
P 2250 2550
F 0 "C?" H 2365 2596 50  0000 L CNN
F 1 "100nF" H 2365 2505 50  0000 L CNN
F 2 "" H 2288 2400 50  0001 C CNN
F 3 "~" H 2250 2550 50  0001 C CNN
	1    2250 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 2250 1550 2300
Wire Wire Line
	2250 2400 2250 2300
Wire Wire Line
	2250 2300 1850 2300
Connection ~ 1550 2300
Wire Wire Line
	1550 2300 1550 3200
Wire Wire Line
	1850 2400 1850 2300
Connection ~ 1850 2300
Wire Wire Line
	1850 2300 1550 2300
$Comp
L power:GND #PWR?
U 1 1 5F6AB6EA
P 2050 2900
F 0 "#PWR?" H 2050 2650 50  0001 C CNN
F 1 "GND" H 2055 2727 50  0000 C CNN
F 2 "" H 2050 2900 50  0001 C CNN
F 3 "" H 2050 2900 50  0001 C CNN
	1    2050 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 2700 1850 2800
Wire Wire Line
	1850 2800 2050 2800
Wire Wire Line
	2250 2800 2250 2700
Wire Wire Line
	2050 2900 2050 2800
Connection ~ 2050 2800
Wire Wire Line
	2050 2800 2250 2800
Wire Wire Line
	1550 2300 1050 2300
Wire Wire Line
	1050 2300 1050 2500
Text GLabel 1050 2950 3    50   Output ~ 0
ISET
Wire Wire Line
	1050 2800 1050 2950
Text GLabel 750  4500 0    50   Input ~ 0
ISET
Wire Wire Line
	750  4500 1000 4500
Wire Bus Line
	800  1150 800  4300
Wire Bus Line
	800  1150 9800 1150
Wire Bus Line
	2500 2750 2500 4500
Wire Bus Line
	2500 2750 11050 2750
Wire Bus Line
	3750 1250 3750 2350
Wire Bus Line
	2750 1250 2750 2350
Wire Bus Line
	4800 1250 4800 2350
Wire Bus Line
	5800 1250 5800 2350
Wire Bus Line
	6850 1250 6850 2350
Wire Bus Line
	7850 1250 7850 2350
Wire Bus Line
	8900 1250 8900 2350
Wire Bus Line
	9900 1250 9900 2350
Text Notes 1800 2250 0    50   ~ 0
check datasheet\nfor cap values\n\n
$EndSCHEMATC
