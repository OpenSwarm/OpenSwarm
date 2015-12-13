;START_HEADER
;
; dsPIC30F6014 Demo Source File
; (c) Copyright 2003 Microchip Technology, All rights reserved
;
; --------------------------------------------------------------------------
; File Revision History:
; --------------------------------------------------------------------------
;
; $Log: common.inc,v $
; Revision 1.1.1.1  2003/08/23 00:38:33  VasukiH
; First import of demo source into CVS Repository
;
;
;
; --------------------------------------------------------------------------
;
; Software and Development Tools Info:
; --------------------------------------------------------------------------
; Tool                          Version
; --------------------------------------------------------------------------
; MPLAB IDE                     6.30
; MPLAB C30 Toolsuite           1.10.02
; dsPICDEM QFP Processor Board  1.10
; --------------------------------------------------------------------------
;
; File Notes:
;
;
;END_HEADER


; Data Size Unit constants
.equ    WORD,           2
.equ    CPLXWORD,       4
.equ    BYTE,           1

;Constants used to initialize the DCI module
.equ  Fs, 8000
.equ  FSCKD, Fs * 256                           ; frame clock rate
.equ  Fcy, 7372800*2                            ; device instruction rate
.equ  BCG,  ( Fcy / ( 2 * FSCKD ) ) - 1         ; equation for DCI clock rate

;Constants used while playing DTMF tones via the DCI module
;.equ NUMSAMPSTORED, 15704
.equ NUMSAMPSTORED, 13204

;Constants useful to initialize Si3000 Codec
.equ  minus_one, 0x8000
.equ  minus_one_with_secondary,  0x8001

.equ  plus_one, 0x7FFE
.equ  plus_one_with_secondary,   0x7FFF

; Si3000 Register Address Summary (Table 13, Si30000-DS11)
; Read Control Address has bit 13 set
; Write Control Address has bit 13 clear

.equ  Read_Control_1,           0x2100
.equ  Write_Control_1,          0x0100

.equ  Read_Control_2,           0x2200
.equ  Write_Control_2,          0x0200

.equ  Read_PLL1_Divide_N1,      0x2300
.equ  Write_PLL1_Divide_N1,     0x0300

.equ  Read_PLL1_Multiply_M1,    0x2400
.equ  Write_PLL1_Multiply_M1,   0x0400

.equ  Read_RX_Gain_Control_1,   0x2500
.equ  Write_RX_Gain_Control_1,  0x0500

.equ  Read_ADC_Volume_Control,  0x2600
.equ  Write_ADC_Volume_Control, 0x0600

.equ  Read_DAC_Volume_Control,  0x2700
.equ  Write_DAC_Volume_Control, 0x0700

.equ  Read_Status_Report,       0x2800
.equ  Write_Status_Report,      0x0800

.equ  Read_Analog_Attenuation,  0x2900
.equ  Write_Analog_Attenuation, 0x0900
