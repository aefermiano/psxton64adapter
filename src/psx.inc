; Copyright 2012 Antonio Fermiano
;
; This file is part of PsxToN64Adapter.
;
; PsxToN64Adapter is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; PsxToN64Adapter is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <https://www.gnu.org/licenses/>.

START_TRANSMISSION
	banksel PORTA
	bsf PORTA,PSX_CLK
	banksel PORTC
	bcf PORTC,PSX_ATT
	return

STOP_TRANSMISSION
	banksel PORTA
	bsf PORTA,PSX_CLK
	banksel PORTC
	bsf PORTC,PSX_ATT
	return

;SUBROUTINE
;Arguments = COMMAND_TO_SEND_PSX
;Results = DATA_RECEIVED_PSX
;This routine will send a command to PSX controller
;and receive it's data.
;-----
;The highest frequency in which the controller is read is in
;500khz (interval = 2us interval = 10 instructions)
;We some times put some useless instructions to match this time.
;-----
;Routine assumes ATT is already low (there is another
;routine for that).
;-----
;COMMAND_TO_SEND_PSX is erased.
SEND_COMMAND_TO_CONTROLLER
	banksel VAR
	;8 bits are transmitted/received
	movlw 0x08
	movwf TEMP
FOR_START
	;Test if we should transmit 1 or 0
	btfsc COMMAND_TO_SEND_PSX,0
	goto CURRENT_BYTE_IS_ONE
CURRENT_BYTE_IS_ZERO
	banksel PORTC
	bcf PORTC,PSX_COMMAND
	banksel PORTA
	goto CURRENT_BYTE_END
CURRENT_BYTE_IS_ONE
	banksel PORTC
	bsf PORTC,PSX_COMMAND
	banksel PORTA
CURRENT_BYTE_END
	;Turns clock down
	banksel PORTA
	bcf PORTA,PSX_CLK
	;Gives controller time to react
	movlw DELAY_OF_CLOCK_DOWN
	call W_COUNTER_DELAY
	;Turns clock up
	bsf PORTA,PSX_CLK
	banksel PORTA
	btfsc PORTA,PSX_DATA
	goto CURRENT_DATA_BYTE_IS_ONE
CURRENT_DATA_BYTE_IS_ZERO
	banksel VAR
	bcf DATA_RECEIVED_PSX,7
	goto CURRENT_DATA_BYTE_END
CURRENT_DATA_BYTE_IS_ONE
	banksel VAR
	bsf DATA_RECEIVED_PSX,7
CURRENT_DATA_BYTE_END
	;Pause between bits
	movlw DELAY_OF_CLOCK_UP
	call W_COUNTER_DELAY
	;Evaluates if we continue in the loop or not
	decfsz TEMP
	goto FOR_CONTINUE
	goto FOR_END
FOR_CONTINUE
	;Shift result left to keep transmiting
	rrf DATA_RECEIVED_PSX,f
	;Shift command right to keep transmiting
	rrf COMMAND_TO_SEND_PSX,f
	;If needed we can add a pause here
	goto FOR_START
FOR_END
	;Pause before returning
	movlw DELAY_BETWEEN_WORDS
	call W_COUNTER_DELAY
	return
	;--END--

;Subroutine - Busy loop decrementing value of W
;It delays:
;3+3*(W-1)+4 = cycles
;Time = 0.2us * cycles
W_COUNTER_DELAY
	movwf TEMP_COMMON
W_COUNTER_DELAY_LOOP
	decfsz TEMP_COMMON
	goto W_COUNTER_DELAY_LOOP
DELAY_LABEL
	return