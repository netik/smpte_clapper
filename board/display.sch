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
P 5450 2000
F 0 "U203" H 5450 2667 50  0000 C CNN
F 1 "MAN74A" H 5450 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 4950 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 5460 2340 50  0001 L CNN
	1    5450 2000
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U204
U 1 1 5F5B9FB3
P 6450 2000
F 0 "U204" H 6450 2667 50  0000 C CNN
F 1 "MAN74A" H 6450 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 5950 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 6460 2340 50  0001 L CNN
	1    6450 2000
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U205
U 1 1 5F5BE185
P 7500 2000
F 0 "U205" H 7500 2667 50  0000 C CNN
F 1 "MAN74A" H 7500 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 7000 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 7510 2340 50  0001 L CNN
	1    7500 2000
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U206
U 1 1 5F5BE18F
P 8500 2000
F 0 "U206" H 8500 2667 50  0000 C CNN
F 1 "MAN74A" H 8500 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 8000 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 8510 2340 50  0001 L CNN
	1    8500 2000
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U207
U 1 1 5F5BE199
P 9550 2000
F 0 "U207" H 9550 2667 50  0000 C CNN
F 1 "MAN74A" H 9550 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 9050 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 9560 2340 50  0001 L CNN
	1    9550 2000
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U208
U 1 1 5F5BE1A3
P 10550 2000
F 0 "U208" H 10550 2667 50  0000 C CNN
F 1 "MAN74A" H 10550 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 10050 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 10560 2340 50  0001 L CNN
	1    10550 2000
	1    0    0    -1  
$EndComp
Entry Wire Line
	2500 3650 2600 3550
Entry Wire Line
	2500 3750 2600 3650
Entry Wire Line
	2500 3950 2600 3850
Entry Wire Line
	2500 3850 2600 3750
Entry Wire Line
	2500 4050 2600 3950
Entry Wire Line
	2500 4150 2600 4050
Entry Wire Line
	2500 4250 2600 4150
Wire Wire Line
	2500 3650 2200 3650
Wire Wire Line
	2200 3750 2500 3750
Wire Wire Line
	2500 3850 2200 3850
Wire Wire Line
	2200 3950 2500 3950
Wire Wire Line
	2500 4050 2200 4050
Wire Wire Line
	2200 4150 2500 4150
Wire Wire Line
	2500 4250 2200 4250
Entry Wire Line
	2500 4350 2600 4250
Wire Wire Line
	2500 4350 2200 4350
$Comp
L clapper_symbols:MAX7219 U209
U 1 1 5F5F7862
P 1650 4100
F 0 "U209" H 1300 4950 50  0000 C CNN
F 1 "MAX7219" H 1450 4150 50  0001 L BNN
F 2 "" H 1650 4100 50  0001 C CNN
F 3 "" H 1650 4100 50  0001 C CNN
	1    1650 4100
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0205
U 1 1 5F5FBBD7
P 1800 5250
F 0 "#PWR0205" H 1800 5000 50  0001 C CNN
F 1 "GND" H 1805 5077 50  0000 C CNN
F 2 "" H 1800 5250 50  0001 C CNN
F 3 "" H 1800 5250 50  0001 C CNN
	1    1800 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 4950 1550 5100
Wire Wire Line
	1550 5100 1800 5100
Wire Wire Line
	1800 5100 1800 4950
Wire Wire Line
	1800 5100 1800 5250
Connection ~ 1800 5100
$Comp
L power:+5V #PWR0201
U 1 1 5F5FCF52
P 1650 2300
F 0 "#PWR0201" H 1650 2150 50  0001 C CNN
F 1 "+5V" H 1665 2473 50  0000 C CNN
F 2 "" H 1650 2300 50  0001 C CNN
F 3 "" H 1650 2300 50  0001 C CNN
	1    1650 2300
	1    0    0    -1  
$EndComp
Entry Bus Bus
	3750 1200 3850 1300
Entry Bus Bus
	4800 1200 4900 1300
Entry Bus Bus
	5800 1200 5900 1300
Entry Bus Bus
	6850 1200 6950 1300
Entry Bus Bus
	7850 1200 7950 1300
Entry Bus Bus
	8900 1200 9000 1300
Entry Bus Bus
	9900 1200 10000 1300
Entry Wire Line
	3850 2300 3950 2400
Entry Wire Line
	3850 2100 3950 2200
Entry Wire Line
	3850 2000 3950 2100
Entry Wire Line
	3850 1900 3950 2000
Entry Wire Line
	3850 1800 3950 1900
Entry Wire Line
	3850 1700 3950 1800
Entry Wire Line
	3850 1600 3950 1700
Entry Wire Line
	3850 1500 3950 1600
$Comp
L Display_Character:MAN74A U202
U 1 1 5F5AE0F6
P 4400 2000
F 0 "U202" H 4400 2667 50  0000 C CNN
F 1 "MAN74A" H 4400 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 3900 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 4410 2340 50  0001 L CNN
	1    4400 2000
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:MAN74A U201
U 1 1 5F5ACDA3
P 3400 2000
F 0 "U201" H 3400 2667 50  0000 C CNN
F 1 "MAN74A" H 3400 2576 50  0000 C CNN
F 2 "Display_7Segment:MAN71A" H 2900 1300 50  0001 L CNN
F 3 "https://www.digchip.com/datasheets/parts/datasheet/161/MAN3640A-pdf.php" H 3410 2340 50  0001 L CNN
	1    3400 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 1600 4100 1600
Wire Wire Line
	4100 1700 3950 1700
Wire Wire Line
	3950 1800 4100 1800
Wire Wire Line
	4100 1900 3950 1900
Wire Wire Line
	3950 2000 4100 2000
Wire Wire Line
	4100 2100 3950 2100
Wire Wire Line
	3950 2200 4100 2200
Wire Wire Line
	4100 2400 3950 2400
Text Label 4000 1600 0    50   ~ 0
A
Text Label 4000 1700 0    50   ~ 0
B
Text Label 4000 1800 0    50   ~ 0
C
Text Label 4000 1900 0    50   ~ 0
D
Text Label 4000 2000 0    50   ~ 0
E
Text Label 4000 2100 0    50   ~ 0
F
Text Label 4000 2200 0    50   ~ 0
G
Text Label 4000 2400 0    50   ~ 0
DP
Entry Wire Line
	2850 2300 2950 2400
Entry Wire Line
	2850 2100 2950 2200
Entry Wire Line
	2850 2000 2950 2100
Entry Wire Line
	2850 1900 2950 2000
Entry Wire Line
	2850 1800 2950 1900
Entry Wire Line
	2850 1700 2950 1800
Entry Wire Line
	2850 1600 2950 1700
Entry Wire Line
	2850 1500 2950 1600
Wire Wire Line
	2950 1600 3100 1600
Wire Wire Line
	3100 1700 2950 1700
Wire Wire Line
	2950 1800 3100 1800
Wire Wire Line
	3100 1900 2950 1900
Wire Wire Line
	2950 2000 3100 2000
Wire Wire Line
	3100 2100 2950 2100
Wire Wire Line
	2950 2200 3100 2200
Wire Wire Line
	3100 2400 2950 2400
Text Label 3000 1600 0    50   ~ 0
A
Text Label 3000 1700 0    50   ~ 0
B
Text Label 3000 1800 0    50   ~ 0
C
Text Label 3000 1900 0    50   ~ 0
D
Text Label 3000 2000 0    50   ~ 0
E
Text Label 3000 2100 0    50   ~ 0
F
Text Label 3000 2200 0    50   ~ 0
G
Text Label 3000 2400 0    50   ~ 0
DP
Entry Wire Line
	4900 2300 5000 2400
Entry Wire Line
	4900 2100 5000 2200
Entry Wire Line
	4900 2000 5000 2100
Entry Wire Line
	4900 1900 5000 2000
Entry Wire Line
	4900 1800 5000 1900
Entry Wire Line
	4900 1700 5000 1800
Entry Wire Line
	4900 1600 5000 1700
Entry Wire Line
	4900 1500 5000 1600
Wire Wire Line
	5000 1600 5150 1600
Wire Wire Line
	5150 1700 5000 1700
Wire Wire Line
	5000 1800 5150 1800
Wire Wire Line
	5150 1900 5000 1900
Wire Wire Line
	5000 2000 5150 2000
Wire Wire Line
	5150 2100 5000 2100
Wire Wire Line
	5000 2200 5150 2200
Wire Wire Line
	5150 2400 5000 2400
Text Label 5050 1600 0    50   ~ 0
A
Text Label 5050 1700 0    50   ~ 0
B
Text Label 5050 1800 0    50   ~ 0
C
Text Label 5050 1900 0    50   ~ 0
D
Text Label 5050 2000 0    50   ~ 0
E
Text Label 5050 2100 0    50   ~ 0
F
Text Label 5050 2200 0    50   ~ 0
G
Text Label 5050 2400 0    50   ~ 0
DP
Entry Wire Line
	5900 2300 6000 2400
Entry Wire Line
	5900 2100 6000 2200
Entry Wire Line
	5900 2000 6000 2100
Entry Wire Line
	5900 1900 6000 2000
Entry Wire Line
	5900 1800 6000 1900
Entry Wire Line
	5900 1700 6000 1800
Entry Wire Line
	5900 1600 6000 1700
Entry Wire Line
	5900 1500 6000 1600
Wire Wire Line
	6000 1600 6150 1600
Wire Wire Line
	6150 1700 6000 1700
Wire Wire Line
	6000 1800 6150 1800
Wire Wire Line
	6150 1900 6000 1900
Wire Wire Line
	6000 2000 6150 2000
Wire Wire Line
	6150 2100 6000 2100
Wire Wire Line
	6000 2200 6150 2200
Wire Wire Line
	6150 2400 6000 2400
Text Label 6050 1600 0    50   ~ 0
A
Text Label 6050 1700 0    50   ~ 0
B
Text Label 6050 1800 0    50   ~ 0
C
Text Label 6050 1900 0    50   ~ 0
D
Text Label 6050 2000 0    50   ~ 0
E
Text Label 6050 2100 0    50   ~ 0
F
Text Label 6050 2200 0    50   ~ 0
G
Text Label 6050 2400 0    50   ~ 0
DP
Entry Wire Line
	6950 2300 7050 2400
Entry Wire Line
	6950 2100 7050 2200
Entry Wire Line
	6950 2000 7050 2100
Entry Wire Line
	6950 1900 7050 2000
Entry Wire Line
	6950 1800 7050 1900
Entry Wire Line
	6950 1700 7050 1800
Entry Wire Line
	6950 1600 7050 1700
Entry Wire Line
	6950 1500 7050 1600
Wire Wire Line
	7050 1600 7200 1600
Wire Wire Line
	7200 1700 7050 1700
Wire Wire Line
	7050 1800 7200 1800
Wire Wire Line
	7200 1900 7050 1900
Wire Wire Line
	7050 2000 7200 2000
Wire Wire Line
	7200 2100 7050 2100
Wire Wire Line
	7050 2200 7200 2200
Wire Wire Line
	7200 2400 7050 2400
Text Label 7100 1600 0    50   ~ 0
A
Text Label 7100 1700 0    50   ~ 0
B
Text Label 7100 1800 0    50   ~ 0
C
Text Label 7100 1900 0    50   ~ 0
D
Text Label 7100 2000 0    50   ~ 0
E
Text Label 7100 2100 0    50   ~ 0
F
Text Label 7100 2200 0    50   ~ 0
G
Text Label 7100 2400 0    50   ~ 0
DP
Entry Wire Line
	7950 2300 8050 2400
Entry Wire Line
	7950 2100 8050 2200
Entry Wire Line
	7950 2000 8050 2100
Entry Wire Line
	7950 1900 8050 2000
Entry Wire Line
	7950 1800 8050 1900
Entry Wire Line
	7950 1700 8050 1800
Entry Wire Line
	7950 1600 8050 1700
Entry Wire Line
	7950 1500 8050 1600
Wire Wire Line
	8050 1600 8200 1600
Wire Wire Line
	8200 1700 8050 1700
Wire Wire Line
	8050 1800 8200 1800
Wire Wire Line
	8200 1900 8050 1900
Wire Wire Line
	8050 2000 8200 2000
Wire Wire Line
	8200 2100 8050 2100
Wire Wire Line
	8050 2200 8200 2200
Wire Wire Line
	8200 2400 8050 2400
Text Label 8100 1600 0    50   ~ 0
A
Text Label 8100 1700 0    50   ~ 0
B
Text Label 8100 1800 0    50   ~ 0
C
Text Label 8100 1900 0    50   ~ 0
D
Text Label 8100 2000 0    50   ~ 0
E
Text Label 8100 2100 0    50   ~ 0
F
Text Label 8100 2200 0    50   ~ 0
G
Text Label 8100 2400 0    50   ~ 0
DP
Entry Wire Line
	9000 2300 9100 2400
Entry Wire Line
	9000 2100 9100 2200
Entry Wire Line
	9000 2000 9100 2100
Entry Wire Line
	9000 1900 9100 2000
Entry Wire Line
	9000 1800 9100 1900
Entry Wire Line
	9000 1700 9100 1800
Entry Wire Line
	9000 1600 9100 1700
Entry Wire Line
	9000 1500 9100 1600
Wire Wire Line
	9100 1600 9250 1600
Wire Wire Line
	9250 1700 9100 1700
Wire Wire Line
	9100 1800 9250 1800
Wire Wire Line
	9250 1900 9100 1900
Wire Wire Line
	9100 2000 9250 2000
Wire Wire Line
	9250 2100 9100 2100
Wire Wire Line
	9100 2200 9250 2200
Wire Wire Line
	9250 2400 9100 2400
Text Label 9150 1600 0    50   ~ 0
A
Text Label 9150 1700 0    50   ~ 0
B
Text Label 9150 1800 0    50   ~ 0
C
Text Label 9150 1900 0    50   ~ 0
D
Text Label 9150 2000 0    50   ~ 0
E
Text Label 9150 2100 0    50   ~ 0
F
Text Label 9150 2200 0    50   ~ 0
G
Text Label 9150 2400 0    50   ~ 0
DP
Entry Wire Line
	10000 2300 10100 2400
Entry Wire Line
	10000 2100 10100 2200
Entry Wire Line
	10000 2000 10100 2100
Entry Wire Line
	10000 1900 10100 2000
Entry Wire Line
	10000 1800 10100 1900
Entry Wire Line
	10000 1700 10100 1800
Entry Wire Line
	10000 1600 10100 1700
Entry Wire Line
	10000 1500 10100 1600
Wire Wire Line
	10100 1600 10250 1600
Wire Wire Line
	10250 1700 10100 1700
Wire Wire Line
	10100 1800 10250 1800
Wire Wire Line
	10250 1900 10100 1900
Wire Wire Line
	10100 2000 10250 2000
Wire Wire Line
	10250 2100 10100 2100
Wire Wire Line
	10100 2200 10250 2200
Wire Wire Line
	10250 2400 10100 2400
Text Label 10150 1600 0    50   ~ 0
A
Text Label 10150 1700 0    50   ~ 0
B
Text Label 10150 1800 0    50   ~ 0
C
Text Label 10150 1900 0    50   ~ 0
D
Text Label 10150 2000 0    50   ~ 0
E
Text Label 10150 2100 0    50   ~ 0
F
Text Label 10150 2200 0    50   ~ 0
G
Text Label 10150 2400 0    50   ~ 0
DP
Entry Bus Bus
	2750 1200 2850 1300
Text Notes 3250 2650 0    50   ~ 10
Digit 7\n
Text Notes 10400 2700 0    50   ~ 10
Digit 0\n
Entry Wire Line
	900  3550 1000 3650
Wire Wire Line
	1100 3650 1000 3650
Text Label 1000 3650 0    50   ~ 0
DP
Entry Wire Line
	900  3650 1000 3750
Entry Wire Line
	900  3750 1000 3850
Entry Wire Line
	900  3850 1000 3950
Entry Wire Line
	900  3950 1000 4050
Entry Wire Line
	900  4050 1000 4150
Entry Wire Line
	900  4150 1000 4250
Entry Wire Line
	900  4250 1000 4350
Wire Wire Line
	1100 3750 1000 3750
Wire Wire Line
	1000 3850 1100 3850
Wire Wire Line
	1100 3950 1000 3950
Wire Wire Line
	1000 4050 1100 4050
Wire Wire Line
	1100 4150 1000 4150
Wire Wire Line
	1000 4250 1100 4250
Wire Wire Line
	1100 4350 1000 4350
Text Label 1000 3750 0    50   ~ 0
A
Text Label 1000 3850 0    50   ~ 0
B
Text Label 1000 3950 0    50   ~ 0
C
Text Label 1000 4050 0    50   ~ 0
D
Text Label 1000 4150 0    50   ~ 0
E
Text Label 1000 4250 0    50   ~ 0
F
Text Label 1000 4350 0    50   ~ 0
G
Text Label 2300 3650 0    50   ~ 0
CC0
Text Label 2300 3850 0    50   ~ 0
CC2
Text Label 2300 3750 0    50   ~ 0
CC1
Text Label 2300 3950 0    50   ~ 0
CC3
Text Label 2300 4050 0    50   ~ 0
CC4
Text Label 2300 4150 0    50   ~ 0
CC5
Text Label 2300 4250 0    50   ~ 0
CC6
Text Label 2300 4350 0    50   ~ 0
CC7
NoConn ~ 1100 3450
Entry Wire Line
	3750 2700 3850 2800
Wire Wire Line
	3700 2300 3750 2300
Wire Wire Line
	3750 2300 3750 2400
Wire Wire Line
	3700 2400 3750 2400
Connection ~ 3750 2400
Wire Wire Line
	3750 2400 3750 2700
Text Label 3750 2650 0    50   ~ 0
CC7
Entry Wire Line
	4750 2700 4850 2800
Wire Wire Line
	4700 2300 4750 2300
Wire Wire Line
	4750 2300 4750 2400
Wire Wire Line
	4700 2400 4750 2400
Connection ~ 4750 2400
Wire Wire Line
	4750 2400 4750 2700
Text Label 4750 2650 0    50   ~ 0
CC6
Entry Wire Line
	5800 2700 5900 2800
Wire Wire Line
	5750 2300 5800 2300
Wire Wire Line
	5800 2300 5800 2400
Wire Wire Line
	5750 2400 5800 2400
Connection ~ 5800 2400
Wire Wire Line
	5800 2400 5800 2700
Text Label 5800 2650 0    50   ~ 0
CC5
Entry Wire Line
	6800 2700 6900 2800
Wire Wire Line
	6750 2300 6800 2300
Wire Wire Line
	6800 2300 6800 2400
Wire Wire Line
	6750 2400 6800 2400
Connection ~ 6800 2400
Wire Wire Line
	6800 2400 6800 2700
Text Label 6800 2650 0    50   ~ 0
CC4
Entry Wire Line
	7850 2700 7950 2800
Wire Wire Line
	7800 2300 7850 2300
Wire Wire Line
	7850 2300 7850 2400
Wire Wire Line
	7800 2400 7850 2400
Connection ~ 7850 2400
Wire Wire Line
	7850 2400 7850 2700
Text Label 7850 2650 0    50   ~ 0
CC3
Entry Wire Line
	8850 2700 8950 2800
Wire Wire Line
	8800 2300 8850 2300
Wire Wire Line
	8850 2300 8850 2400
Wire Wire Line
	8800 2400 8850 2400
Connection ~ 8850 2400
Wire Wire Line
	8850 2400 8850 2700
Text Label 8850 2650 0    50   ~ 0
CC2
Entry Wire Line
	9900 2700 10000 2800
Wire Wire Line
	9850 2300 9900 2300
Wire Wire Line
	9900 2300 9900 2400
Wire Wire Line
	9850 2400 9900 2400
Connection ~ 9900 2400
Wire Wire Line
	9900 2400 9900 2700
Text Label 9900 2650 0    50   ~ 0
CC1
Entry Wire Line
	10900 2700 11000 2800
Wire Wire Line
	10850 2300 10900 2300
Wire Wire Line
	10900 2300 10900 2400
Wire Wire Line
	10850 2400 10900 2400
Connection ~ 10900 2400
Wire Wire Line
	10900 2400 10900 2700
Text Label 10900 2650 0    50   ~ 0
CC0
$Comp
L Device:R R201
U 1 1 5F68F8C2
P 1150 2700
F 0 "R201" H 1220 2746 50  0000 L CNN
F 1 "12K" H 1220 2655 50  0000 L CNN
F 2 "" V 1080 2700 50  0001 C CNN
F 3 "~" H 1150 2700 50  0001 C CNN
	1    1150 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C201
U 1 1 5F697F90
P 1950 2600
F 0 "C201" H 2065 2646 50  0000 L CNN
F 1 "10uF" H 2065 2555 50  0000 L CNN
F 2 "" H 1988 2450 50  0001 C CNN
F 3 "~" H 1950 2600 50  0001 C CNN
	1    1950 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C202
U 1 1 5F6994A8
P 2350 2600
F 0 "C202" H 2465 2646 50  0000 L CNN
F 1 "100nF" H 2465 2555 50  0000 L CNN
F 2 "" H 2388 2450 50  0001 C CNN
F 3 "~" H 2350 2600 50  0001 C CNN
	1    2350 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2300 1650 2350
Wire Wire Line
	2350 2450 2350 2350
Wire Wire Line
	2350 2350 1950 2350
Connection ~ 1650 2350
Wire Wire Line
	1650 2350 1650 3250
Wire Wire Line
	1950 2450 1950 2350
Connection ~ 1950 2350
Wire Wire Line
	1950 2350 1650 2350
$Comp
L power:GND #PWR0202
U 1 1 5F6AB6EA
P 2150 2950
F 0 "#PWR0202" H 2150 2700 50  0001 C CNN
F 1 "GND" H 2155 2777 50  0000 C CNN
F 2 "" H 2150 2950 50  0001 C CNN
F 3 "" H 2150 2950 50  0001 C CNN
	1    2150 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2750 1950 2850
Wire Wire Line
	1950 2850 2150 2850
Wire Wire Line
	2350 2850 2350 2750
Wire Wire Line
	2150 2950 2150 2850
Connection ~ 2150 2850
Wire Wire Line
	2150 2850 2350 2850
Wire Wire Line
	1650 2350 1150 2350
Wire Wire Line
	1150 2350 1150 2550
Text GLabel 1150 3000 3    50   Output ~ 0
ISET
Wire Wire Line
	1150 2850 1150 3000
Text GLabel 850  4550 0    50   Input ~ 0
ISET
Wire Wire Line
	850  4550 1100 4550
Text Notes 1900 2300 0    50   ~ 0
check datasheet\nfor cap values\n\n
$Comp
L Logic_LevelTranslator:TXS0108EPW U210
U 1 1 5F919864
P 4200 4400
F 0 "U210" H 4450 3700 50  0000 C CNN
F 1 "TXS0108EPW" H 4650 3600 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 4200 3650 50  0001 C CNN
F 3 "www.ti.com/lit/ds/symlink/txs0108e.pdf" H 4200 4300 50  0001 C CNN
	1    4200 4400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0203
U 1 1 5F91A952
P 4100 3250
F 0 "#PWR0203" H 4100 3100 50  0001 C CNN
F 1 "+3.3V" H 4115 3423 50  0000 C CNN
F 2 "" H 4100 3250 50  0001 C CNN
F 3 "" H 4100 3250 50  0001 C CNN
	1    4100 3250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0204
U 1 1 5F922290
P 4300 3400
F 0 "#PWR0204" H 4300 3250 50  0001 C CNN
F 1 "+5V" H 4315 3573 50  0000 C CNN
F 2 "" H 4300 3400 50  0001 C CNN
F 3 "" H 4300 3400 50  0001 C CNN
	1    4300 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3400 4300 3700
Wire Wire Line
	4100 3500 3600 3500
Wire Wire Line
	3600 3500 3600 4000
Wire Wire Line
	3600 4000 3800 4000
Wire Wire Line
	4100 3250 4100 3500
Connection ~ 4100 3500
Wire Wire Line
	4100 3500 4100 3700
$Comp
L power:GND #PWR0206
U 1 1 5F931B96
P 4200 5350
F 0 "#PWR0206" H 4200 5100 50  0001 C CNN
F 1 "GND" H 4205 5177 50  0000 C CNN
F 2 "" H 4200 5350 50  0001 C CNN
F 3 "" H 4200 5350 50  0001 C CNN
	1    4200 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5100 4200 5350
NoConn ~ 4600 4800
NoConn ~ 4600 4700
NoConn ~ 4600 4600
NoConn ~ 4600 4500
NoConn ~ 4600 4400
NoConn ~ 3800 4400
NoConn ~ 3800 4500
NoConn ~ 3800 4600
NoConn ~ 3800 4700
NoConn ~ 3800 4800
Text GLabel 2450 3350 1    50   Input ~ 10
DISP_DIN
Wire Wire Line
	2200 3450 2450 3450
Wire Wire Line
	2450 3450 2450 3350
Text GLabel 1000 4750 0    50   Input ~ 0
DISP_CLK
Wire Wire Line
	1000 4650 1100 4650
Wire Wire Line
	1000 4750 1100 4750
Text GLabel 5050 4100 2    50   Output ~ 0
DISP_DIN
Wire Wire Line
	5050 4100 4600 4100
Text GLabel 5050 4200 2    50   Output ~ 0
DISP_CLK
Text GLabel 5050 4300 2    50   Output ~ 0
DISP_LOAD
Wire Wire Line
	4600 4300 5050 4300
Wire Wire Line
	5050 4200 4600 4200
Text GLabel 3650 4100 0    50   Input ~ 0
DISP_DIN_3V
Wire Wire Line
	3650 4100 3800 4100
Text GLabel 3650 4200 0    50   Input ~ 0
DISP_CLK_3V
Text GLabel 3650 4300 0    50   Input ~ 0
DISP_LOAD_3V
Wire Wire Line
	3800 4300 3650 4300
Wire Wire Line
	3650 4200 3800 4200
Text GLabel 1000 4650 0    50   Input ~ 0
DISP_LOAD
Text Notes 4500 3650 0    50   ~ 10
3.3v to 5v level shift
$Comp
L Device:C C?
U 1 1 5FA69937
P 3950 6500
AR Path="/5FA69937" Ref="C?"  Part="1" 
AR Path="/5F5A5FDC/5FA69937" Ref="C203"  Part="1" 
F 0 "C203" H 4065 6546 50  0000 L CNN
F 1 "10uF" H 4065 6455 50  0000 L CNN
F 2 "" H 3988 6350 50  0001 C CNN
F 3 "~" H 3950 6500 50  0001 C CNN
	1    3950 6500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FA6993D
P 4400 6500
AR Path="/5FA6993D" Ref="C?"  Part="1" 
AR Path="/5F5A5FDC/5FA6993D" Ref="C204"  Part="1" 
F 0 "C204" H 4515 6546 50  0000 L CNN
F 1 "100uF" H 4515 6455 50  0000 L CNN
F 2 "" H 4438 6350 50  0001 C CNN
F 3 "~" H 4400 6500 50  0001 C CNN
	1    4400 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 6650 3950 6800
Wire Wire Line
	3950 6800 4150 6800
Wire Wire Line
	4400 6800 4400 6650
$Comp
L power:GND #PWR?
U 1 1 5FA69946
P 4150 6900
AR Path="/5FA69946" Ref="#PWR?"  Part="1" 
AR Path="/5F5A5FDC/5FA69946" Ref="#PWR0208"  Part="1" 
F 0 "#PWR0208" H 4150 6650 50  0001 C CNN
F 1 "GND" H 4155 6727 50  0000 C CNN
F 2 "" H 4150 6900 50  0001 C CNN
F 3 "" H 4150 6900 50  0001 C CNN
	1    4150 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6900 4150 6800
Connection ~ 4150 6800
Wire Wire Line
	4150 6800 4400 6800
Text Notes 5100 3250 0    50   ~ 10
Decoupling Caps\n
$Comp
L power:+3.3V #PWR?
U 1 1 5FA69950
P 3950 6100
AR Path="/5FA69950" Ref="#PWR?"  Part="1" 
AR Path="/5F5A5FDC/5FA69950" Ref="#PWR0207"  Part="1" 
F 0 "#PWR0207" H 3950 5950 50  0001 C CNN
F 1 "+3.3V" H 3965 6273 50  0000 C CNN
F 2 "" H 3950 6100 50  0001 C CNN
F 3 "" H 3950 6100 50  0001 C CNN
	1    3950 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 6100 3950 6200
Wire Wire Line
	3950 6200 4400 6200
Wire Wire Line
	4400 6200 4400 6350
Connection ~ 3950 6200
Wire Wire Line
	3950 6200 3950 6350
Text Notes 3800 6300 0    50   ~ 10
U1\n
Text Notes 4300 6100 0    50   ~ 0
Place near U210
Wire Bus Line
	900  1200 900  4350
Wire Bus Line
	900  1200 9900 1200
Wire Bus Line
	2600 2800 2600 4550
Wire Bus Line
	2600 2800 11150 2800
Wire Bus Line
	3850 1300 3850 2400
Wire Bus Line
	2850 1300 2850 2400
Wire Bus Line
	4900 1300 4900 2400
Wire Bus Line
	5900 1300 5900 2400
Wire Bus Line
	6950 1300 6950 2400
Wire Bus Line
	7950 1300 7950 2400
Wire Bus Line
	9000 1300 9000 2400
Wire Bus Line
	10000 1300 10000 2400
$EndSCHEMATC
