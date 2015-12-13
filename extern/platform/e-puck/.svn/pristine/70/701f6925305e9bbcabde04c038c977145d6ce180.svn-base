

; Data Size Unit constants
.equ    WORD,           2
.equ    CPLXWORD,       4
.equ    BYTE,           1

;Constants used to initialize the DCI module
.equ  FS, 7200					; due to BCB quantization, only 7200 etc... value are possible
.equ  FSCKD, FS * 256                           ; frame clock rate
.equ  FCY, 7372800*2                            ; device instruction rate
.equ  BCG,  ( FCY / ( 2 * FSCKD ) ) - 1         ; equation for DCI clock rate

.equ NUMSAMPSTORED, 13204			; number of sample stored in the soundsample.s file

;Constants useful to initialize Si3000 Codec
.equ  MINUS_ONE, 0x8000
.equ  MINUS_ONE_WITH_SECONDARY,  0x8001

.equ  PLUS_ONE, 0x7FFE
.equ  PLUS_ONE_WITH_SECONDARY, 0x7FFF

; Si3000 Register Address Summary (Table 13, Si30000-DS11)
; Read Control Address has bit 13 set
; Write Control Address has bit 13 clear

.equ  READ_CONTROL_1,           0x2100
.equ  WRITE_CONTROL_1,          0x0100

.equ  READ_CONTROL_2,           0x2200
.equ  WRITE_CONTROL_2,          0x0200

.equ  READ_PLL1_DIVIDE_N1,      0x2300
.equ  WRITE_PLL1_DIVIDE_N1,     0x0300

.equ  READ_PLL1_MULTIPLY_M1,    0x2400
.equ  WRITE_PLL1_MULTIPLY_M1,   0x0400

.equ  READ_RX_GAIN_CONTROL_1,   0x2500
.equ  WRITE_RX_GAIN_CONTROL_1,  0x0500

.equ  READ_ADC_VOLUME_CONTROL,  0x2600
.equ  WRITE_ADC_VOLUME_CONTROL, 0x0600

.equ  READ_DAC_VOLUME_CONTROL,  0x2700
.equ  WRITE_DAC_VOLUME_CONTROL, 0x0700

.equ  READ_Status_Report,       0x2800
.equ  WRITE_Status_Report,      0x0800

.equ  READ_ANALOG_ATTENUATION,  0x2900
.equ  WRITE_ANALOG_ATTENUATION, 0x0900
