; Author : Florian Vaussard
; Last modified : 22.08.2006

.text

		.global _e_subtract_mean
_e_subtract_mean:
		; W0 : input array
		; W1 : array size
		; W2 : log2(array size)

		; Save the address of the array
		PUSH W0
		
		CLR A
		; Set up REPEAT loop with count 'ARRAYSIZE' (passed in W1)
		DEC W1, W1
		REPEAT W1
		; Add all the elements of the array in the A acc.
		ADD [W0++], A
		
		; Faut encore décaler de 16 pour retrouver le bon résultat
		; Décalage par log2(size)
		SFTAC A, W2
		
		; Result of the mean
		MOV ACCAH, W3	

		; Retrieve the address of the array
		POP W0
		
		; Save the address of the array
		PUSH W0
		DO W1, loop1
		; Subtract the mean from the array
		MOV [W0], W4
loop1:	SUB W4, W3, [W0++]

		; Retrieve the address of the array
		POP W0
	RETURN

.end
