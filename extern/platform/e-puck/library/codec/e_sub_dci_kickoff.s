; File Notes:
;
; 1. This subroutine maps a number received by the UART,
;    to a DTMF tone and kicks off DCI transmissions.
;
; 2. All tone samples are stored in Program Memory, so we use PSV to access
;    this data.
;
; 3. A tone sample is 100 milliseconds long and is followed by
;    15 milliseconds of silence.
;
; 4. A tone sample is obtained by adding amplitudes of the sinusoid components.
;    The low frequency omponent has a signal strength 8 dB higher than the
;    high frequency component

.include "p30F6014A.inc"
.include "e_common.inc"


.section .ndata,data,near
.align WORD

.global _e_dci_unavailable, _e_stop_flag, e_tone_ptr
.global e_samples_count, e_tones_record, e_actual_page

.section .nbss,bss,near
.align 2
e_tone_ptr:             .space WORD

.section                .ndata
.align  WORD

_e_stop_flag:				.hword 0x0000

_e_dci_unavailable:        .hword 0x0000                ;Flag that indicates the DCI
                                                        ;module is transmitting a tone
                                                        ;Flag is used to prevent one tone
                                                        ;"over-writing" another
e_samples_count:       	.hword 0x0000                   ;Keeps a count of number of DTMF
                                                        ;samples left to transmit
e_actual_page:				.hword 0x0000
 

.global _e_sub_dci_kickoff
.section .text

; sound offset in parameter 0, length in parameter 1
_e_sub_dci_kickoff:
        push    w1                              ; save W1 and W2 on the stack
        push    w2
        sl      w0,#1,w0
        bra     initiateDCI

exit_dci_kickoff:
        pop     w2
        pop     w1
        return


initiateDCI:
load_samp_cnt:
        mov     w1, e_samples_count			;set sumber of samples
        clr     w2
        mov     #psvoffset(e_sound_sample), w1   ;get datapointer to the sample    
        add     w0, w1, w1                      ;add our start offset
        bra     nc, continue1                   ;if our offset don't makes an overfflow of PSV address -> continue1
        mov     #0x8000, w2                     ;an overflow occured in PSV address.
        add     w1, w2, w1                      ;add 0x8000 to our offset bacause PSV address begin at 0x8000
        mov     #1, w2					;set W2 to 1 to mark that our sample begin in one page after the begining
                                                ;of "sound_samples"
		
continue1:
        push    CORCON
        push    PSVPAG                          ;Enable PSV to access tones
        bset    CORCON, #PSV
        mov     #psvpage(e_sound_sample), w0     ;get the page of our samples data
        add     w0, w2, w0                      ;if our sample begins at the nexte page, add 1
        mov     w0, PSVPAG
        nop
		
        call    test_page
        mov     [w1], w0
        bclr    w0, #0                          ;LS-bit cleared so that the codec does not
        mov     w0, TXBUF0                      ;assume a secondary frame request is being made
        call    test_page 
        mov     [w1], w0
        bclr    w0, #0                          ;LS-bit cleared so that the codec does not
        mov     w0, TXBUF1                      ;assume a secondary frame request is being made
        call    test_page 
        mov     [w1], w0
        bclr    w0, #0
        mov     w0, TXBUF2                      ;Load TXBUF0-TXBUF3
        call    test_page 
        mov     [w1], w0
        bclr    w0, #0
        mov     w0, TXBUF3

        dec2    e_samples_count                ;Decrement samples count by 4 since
        dec2    e_samples_count                ;we sent 4 samples out
        mov     w1, e_tone_ptr                  ;Update the High and Low tone pointers

        mov     PSVPAG,w1
		mov		w1,e_actual_page

        pop     PSVPAG                          ;Restore CORCON, PSVPAG
        nop
        pop     CORCON
        bclr    IFS2, #DCIIF                    ;Clear the DCI Interrupt flag

        bra     exit_dci_kickoff

test_page:
        add     w1, #2, w1
        bra     nc, return_testpage	

        mov     PSVPAG, w1
        add     w1, #1, w1                      ; change page
        mov     w1, PSVPAG	
        mov     #0x8000, w1		
return_testpage:
        return
.end

