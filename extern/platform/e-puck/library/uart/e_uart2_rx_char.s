/***************************************************************************************************************

Title:		UART2_RX_CHAR.s

Author:		Inspired Microchip library
			Davis Daidi�
			Bonani Michael
			Stephane Magnenat

History:
	11/07/05		Start day
	18/07/05		To be continued
	21/12/05		Optimisation and new function name 
	12/02/07		Added possibility of clearing an external on interrupt
	12/03/07		Major rewrite, added rx buffer
	03/10/13		Fixed reading of characters when internal buffer has more than one, clear overflow

****************************************************************************************************************/

; to be used with uart_txrx_char.h

.include "p30F6014A.inc"

.section  .data, near

.global _U2RXBuf
.global _U2RXRcvCnt
.global _U2RXReadCnt
_U2RXBuf: .space 64							; reception buffer
_U2RXRcvCnt: .space 2						; amount of received bytes
_U2RXReadCnt: .space 2						; amount of read bytes

.ifdef UART2_CLR_BIT_ON_INT
	.align 2
	.global _e_uart2_int_clr_mask
	.global _e_uart2_int_clr_addr
	_e_uart2_int_clr_mask: .space 2			; mask to apply upon interrupt
	_e_uart2_int_clr_addr: .space 2			; pointer where to apply mask upon interrupt
.endif

.section  .text


.global __U2RXInterrupt
__U2RXInterrupt:
		push.d	w0							; Save context - w0, w1
		
		bclr    IFS1, #U2RXIF				; Clear the interrupt flag.
		
.ifdef UART2_CLR_BIT_ON_INT
		mov		_e_uart2_int_clr_addr, w1	; Load pointer in w0
		mov		[w1], w0					; Deference pointer read
		and		_e_uart2_int_clr_mask		; Mask w0 with user-specified mask
		mov		w0, [w1]					; Deference pointer write
.endif

		; Clear any overflow
		bclr	U2STA,#1					; Clear overflow bit
		; Get all incoming characters (while U2STA[0] == 1)
get_char_loop:
		btst	U2STA,#0					; Test if char to receive
		bra		Z,no_more_char				; If no char, exit loop
		mov		_U2RXRcvCnt, w0				; Received counter in w0
		and		#0x3f, w0					; Mask at buffer length
		mov		#_U2RXBuf, w1				; Buffer pointer in w1
		add		w0, w1, w1					; Element pointer in w1
		mov.b   U2RXREG, WREG				; Transfer received byte to w0
		mov.b	w0, [w1]					; Store received byte
		inc		_U2RXRcvCnt					; Increment amount of received bytes
		bra		get_char_loop               ; Next loop iteration
no_more_char:
		pop.d   w0							; Restore context - w0, w1

		retfie								; Return from Interrupt


.global _e_ischar_uart2
_e_ischar_uart2:
		mov		_U2RXRcvCnt, w0				; Received counter in w0
		mov		_U2RXReadCnt, w1			; Read counter in w1
		sub		w0, w1, w0					; Diff (amount of unread) in w0
		return


.global _e_getchar_uart2
_e_getchar_uart2:
		mov		_U2RXRcvCnt, w1				; Received counter in w1
		mov		_U2RXReadCnt, w2			; Read counter in w2
		cp		w1, w2						; Compare w1 - w2
		
		bra		Z, no_char_to_ret			; If equal, no char to return
		
		and		#0x3f, w2					; Mask at buffer length
		mov		#_U2RXBuf, w1				; Buffer pointer in w1
		add		w1, w2, w2					; Element pointer in w2
		
		mov.b	[w2], w1					; Read byte from reception buffer
		mov.b	w1, [w0]					; Store byte to user buffer
		
		inc		_U2RXReadCnt				; Increment amount of read bytes
		
		mov		#1, w0						; Return 1
		return

no_char_to_ret:
		clr		w0							; Return 0
		return

.end										; EOF


