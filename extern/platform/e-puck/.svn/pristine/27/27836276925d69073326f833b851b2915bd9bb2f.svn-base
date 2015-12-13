;
; File Notes:
;
; At this point the DCI module has been initialized without enabling DCI
; interrupts.
; We will now initialize the codec by using the DCI module in a polling-based
; fashion (as opposed to an interrupt-based fashion).
;
; During Codec Initialization, the communication will work as follows:
; 1. We load TXBUF0 with a control word that has an LSB set that informs the codec
;    that a "secondary" frame will be sent in the next frame.
; 2. We load TXBUF1 with the required setting for a control register in the Si3000.
;    For e.g. a value 0x0118 sent to the codec will initiate a Write operation to
;    Control Register 1 in the Si3000 codec. The value written to the register
;    will be 0x18.
; 3. On the first Frame Sync pulse, the DCI will send the Codec, the contents of
;    the TXBUF0 register.
; 4. On the next frame-sync pulse, the DCI will send the Codec, the contents of
;    the TXBUF1 register. Since the Codec is expecting a "secondary frame" on
;    this pulse, it will read/write a control register based on the command.
; 5. Steps 3 and 4 are repeated for various control registers.
;
; AFTER initializing the Si3000 codec, we will disable the DCI module,
; modify certain transmit properties, enable DCI interrupts and the eventually
; re-enable the DCI module itself.
;
; The following features of the DCI module will be modified after setting up the
; Codec:
; 1. The DCI module will continue to transmit/receive only on TimeSlot1.
; 2. Each time slot will be 16-bits long.
; 3. A total of 16 time slots will be transmitted before a Frame Sync pulse is
;    generated.
; 4. All four available buffer registers (TXBUF0-4) are set up for usage.
; 5. The DCI module will generate frame sync pulses at 7200 Hz.
;



.include "p30F6014A.inc"
.include "e_common.inc"


.global _e_init_codec_slave


.section .text

_e_init_codec_slave:

        push    w1
        push    w0



        mov     #PLUS_ONE_WITH_SECONDARY, w0    ;Load TXBUF0 with a value that
        mov     w0, TXBUF0                      ;requests a secondary frame


        mov     #WRITE_PLL1_DIVIDE_N1, w0       ;Load TXBUF1 with a value that
        ior     #0x0000, w0                     ;will initiate a WRITE to PLL
        mov     w0, TXBUF1                      ;Divide Control Register 3
                                                ;in the Si3000

                                                ;Synchronize DCI with Si3000 to
                                                ;ensure first frame synce is
                                                ;aligned with TXBUF0.
                                                ;This is not really required when
                                                ;the DCI is Master and Codec is
                                                ;the Slave but is required when
                                                ;the Codec is the Master and DCI
                                                ;is the Slave.

        bclr    TRISF, #RF0
        nop
        bclr    PORTF, #RF0                     ;Reset codec
        repeat  #400                            ;Transmit a minimum pulse
        nop                                     ;width of 5uS
        bset    PORTF, #RF0                     ;Release codec

        bset    DCICON1, #DCIEN                 ;Enable DCI module

                                                ;This ensures the first primary
                                                ;and secondary frame use data
                                                ;loaded already into TXBUF0
                                                ;and TXBUF1

not_empty1:
        btss    DCISTAT, #TMPTY                 ;Wait until TXBUF0 and TXBUF1
        bra     not_empty1                      ;have been moved to their shadow
                                                ;registers for transmission

        mov     #PLUS_ONE_WITH_SECONDARY, w0    ;Load TXBUF0 with a value that
        mov     w0, TXBUF0                      ;requests a secondary frame

        mov     #WRITE_PLL1_MULTIPLY_M1, w0     ;Load TXBUF1 with a value that
        ior     #0x0013, w0                     ;will initiate a WRITE to PLL
        mov     w0, TXBUF1                      ;Multiply Register 4

not_empty2:
        btss    DCISTAT, #TMPTY                 ;Wait until TXBUF0 and TXBUF1
        bra     not_empty2                      ;have been moved to their shadow
                                                ;registers for transmission


        mov     #PLUS_ONE_WITH_SECONDARY, w0    ;Load TXBUF0 with a value that
        mov     w0, TXBUF0                      ;requests a secondary frame

        mov     #WRITE_CONTROL_1, w0            ;Load TXBUF1 with a value that
        bset    w0, #3                          ;will power up line driver and
        bset    w0, #4                          ;the speaker driver in Si3000
        mov     w0, TXBUF1                      ;Control Register 1

not_empty3:
        btss    DCISTAT, #TMPTY                 ;Wait until TXBUF0 and TXBUF1
        bra     not_empty3                      ;have been moved to their shadow
                                                ;registers for transmission



        mov     #PLUS_ONE_WITH_SECONDARY, w0    ;Load TXBUF0 with a value that
        mov     w0, TXBUF0                      ;requests a secondary frame

        mov     #WRITE_CONTROL_2, w0            ;Load TXBUF1 with a value that
        mov     w0, TXBUF1                      ;will initialize Control Register 2
                                                ;to 0x00

not_empty4:
        btss    DCISTAT, #TMPTY                 ;Wait until TXBUF0 and TXBUF1
        bra     not_empty4                      ;have been moved to their shadow
                                                ;registers for transmission


        mov     #PLUS_ONE_WITH_SECONDARY, w0    ;Load TXBUF0 with a value that
        mov     w0, TXBUF0                      ;requests a secondary frame

        mov     #WRITE_ADC_VOLUME_CONTROL, w0   ;Load TXBUF1 with a value that
        bset    w0, #1                          ;sets the line out active in the
        mov     w0, TXBUF1                      ;Si3000's ADC Control Register 6

not_empty5:
        btss    DCISTAT, #TMPTY                 ;Wait until TXBUF0 and TXBUF1
        bra     not_empty5                      ;have been moved to their shadow
                                                ;registers for transmission


        mov     #PLUS_ONE_WITH_SECONDARY, w0    ;Load TXBUF0 with a value that
        mov     w0, TXBUF0                      ;requests a secondary frame

        mov     #WRITE_DAC_VOLUME_CONTROL, w0   ;Load TXBUF1 with a value that
        ior     #0x005F, w0                     ;sets TX PGA Gain to 0dB and
        mov     w0, TXBUF1                      ;activates L/R speaker in the
                                                ;DAC Volume Control Register 7

not_empty6:
        btss    DCISTAT, #TMPTY                 ;Wait until TXBUF0 and TXBUF1
        bra     not_empty6                      ;have been moved to their shadow
                                                ;registers for transmission

;At this point there is no need to load TXBUF registers again, since all initialization
;commands have been either loaded or transmitted.

;Note that the TMPTY bit only gives a status of whether or not TXBUF registers
;are ready to accept data. This does not tell you if the transmissions themselves were
;completed.
;So, we need to test the SLOT<3:0> bits in the DCISTAT register.
;While testing we will ensure that TXBUF0 and TXBUF1 shadow registers have been
;fully transmitted on TimeSlot0 two times each. This will guarantee that initialization
;of the codec is complete

        mov     #0x0F00, w1                     ;Set up a mask for the SLOT bits
txbuf0_complete1:
        mov     DCISTAT, w0                     ;Read the DCISTAT register
        and     w0, w1, w0                      ;Logical-AND to extract SLOT bits
        cp0     w0                              ;Wait until SLOT bits reach 0b0000
        bra     nz, txbuf0_complete1            ;Did TXBUF0 get transmitted?

txbuf1_complete1:
        mov     DCISTAT, w0                     ;Read the DCISTAT register
        and     w0, w1, w0                      ;Logical-AND to extract SLOT bits
        cp0     w0                              ;Wait until SLOT bits reach non-zero
        bra     z, txbuf1_complete1             ;Did TXBUF1 get transmitted?

txbuf0_complete2:
        mov     DCISTAT, w0                     ;Read the DCISTAT register
        and     w0, w1, w0                      ;Logical-AND to extract SLOT bits
        cp0     w0                              ;Wait until SLOT bits reach 0b0000
        bra     nz, txbuf0_complete2            ;Did TXBUF0 get transmitted?

txbuf1_complete2:
        mov     DCISTAT, w0                     ;Read the DCISTAT register
        and     w0, w1, w0                      ;Logical-AND to extract SLOT bits
        cp0     w0                              ;Wait until SLOT bits reach 0b0000
        bra     z, txbuf1_complete2             ;Did TXBUF0 get transmitted?


        btss    DCISTAT, #ROV                   ;Testing overflow bit
        bra     disable_module                  ;No need to perform the dummy reads


        mov     RXBUF0, w0                      ;Dummy reads to clear
        mov     RXBUF1, w0                      ;overflow conditions

disable_module:
        bclr    DCICON1, #DCIEN                 ;Disable the DCI module

;Now re-initialize some aspects of the DCI communication for normal interrupt-driven
;operation.

        mov     #0b0000000111101111, w0         ;Data word size is 16-bits (bits 3:0)
        mov     w0, DCICON2                     ;data frame is 16 words (bits 8:5)


        bset    DCICON2, #BLEN1                 ;set buffer length control (4 data
        bset    DCICON2, #BLEN0                 ;words will be buffered between interrupts)


        bclr    IFS2, #DCIIF                    ;Clear the interrupt flag

        bset    IPC10, #DCIIP2                  ;Set Interrupt Priority to 4
        bclr    IPC10, #DCIIP1
        bclr    IPC10, #DCIIP0

        bset    IEC2, #DCIIE                    ;Enable DCI interrupts
        bset    DCICON1, #DCIEN                 ;Re-enable the DCI module

        pop	    w0                              ;Restore w0, w1
        pop     w1

        return                                  ;Return to calling routine


.end                                            ;EOF

