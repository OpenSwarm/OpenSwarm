
;
; File Notes:
;
; 1. The DCI interrupt service routine transfers 4 new tone samples to TXBUF0-4.
;    It also keeps a count of the number of samples transferred thus far.
;
; 2.  A tone sample is 100 milliseconds long and is followed by
;     15 milliseconds of silence.
;
; 3.  A tone sample is obtained by adding amplitudes of the sinusoid components.
;



.include "p30F6014A.inc"
.include "e_common.inc"


.global __DCIInterrupt
.section .text
__DCIInterrupt:
        bclr    IFS2, #DCIIF                    ;Clear the interrupt flag
        push.d  w0                              ;Save w0-w3 off to stack
        push.d  w2
        mov     RXBUF0, w0                      ;Dummy read of RXBUF0-RXBUF3
        mov     RXBUF1, w0
        mov     RXBUF2, w0
        mov     RXBUF3, w0
		cp0		_e_stop_flag
		bra     nz, spcl_exit_DCI_ISR
        cp0     e_samples_count                ;Is the number of DTMF samples
        bra     z, spcl_exit_DCI_ISR            ;samples have been SENT? If so, then exit

sendsamples:                                    ;If not then send next DTMF sample
        push    PSVPAG                          ;DTMF samples are constructed
        push    CORCON                          ;from sine samples stored in
        bset    CORCON, #PSV                    ;Program Memory, so use PSV to
        mov     e_actual_page, w0
        mov     w0, PSVPAG                      ;Set up PSVPAG register
        mov     e_tone_ptr, w1                  ;Retrieve pointer to lower tone
                                                ;component into w1
        call    test_page
        mov     [w1], w0                        ;Read low-tone sample
        bclr    w0, #0                          ;Clear the LS bit of the resultant
                                                ;This ensures that the dsPIC does
                                                ;not accidentally request a secondary
                                                ;frame transmission from the codec.
                                                ;Note that the Si3000 codec only provides
                                                ;an 84 dB dynamic range
        mov     w0, TXBUF0                      ;Write the result to TXBUF0
        call    test_page
        mov     [w1], w0                        ;Perform the same steps as above to load
        bclr    w0, #0
        mov     w0, TXBUF1
        call    test_page
        mov     [w1], w0
        bclr    w0, #0
        mov     w0, TXBUF2
        call    test_page
        mov     [w1], w0
        bclr    w0, #0
        mov     w0, TXBUF3

        dec2    e_samples_count                ;Decrement the number of DTMF samples left
        dec2    e_samples_count                ;to transmit by 4

        mov     w1, e_tone_ptr                  ;Update the Low and High tone pointers
        mov     PSVPAG,w1
		mov		w1,e_actual_page		
        pop     CORCON                          ;Restore CORCON
        pop     PSVPAG                          ;Restore PSVPAG
exit_DCI_ISR:
        pop.d   w2                              ;Restore w0, w1, w2 and w3
        pop.d   w0
        retfie                                  ;Return from Interrupt


spcl_exit_DCI_ISR:
        clr     _e_dci_unavailable                  ;If silence has also been transmitted
		setm	_e_stop_flag
        pop.d   w2                              ;then clear the DCIUnavailable flag.
        pop.d   w0                              ;Restore w0-w3
        retfie                                  ;Return from Interrupt


test_page:
        add     w1, #2, w1
        bra     nc, return_testpage	

        mov     PSVPAG, w1
        add     w1, #1, w1			      ; change page
        mov     w1, PSVPAG	
        mov     #0x8000, w1	
return_testpage:
        return

.end                                            ;EOF






