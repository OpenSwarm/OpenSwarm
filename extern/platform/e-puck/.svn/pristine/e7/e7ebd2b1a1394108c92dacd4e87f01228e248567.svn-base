; Author : Florian Vaussard
; Last modified : 22.08.2006

.text
		.global _e_fast_copy
_e_fast_copy:
		; W0 : input array
		; W1 : output array
		; W2 : arrays size
			
		DEC W2, W2
		MOV #0, W5
		DO W2,end
		MOV [W0++],[W1++]	; copy the element in the real part
end:	MOV W5, [W1++]		; put a 0 in the imag part

	RETURN
	
.end
