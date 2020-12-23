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

;Please notice all the timing is made counting instruction
;cycles.
Interrupt_handling macro
	;When it reaches here, 0.6 or 0.8us will have passed.
	;Let's assume 0.8us (we're OK with this little deviance).
	;We must read the values near 2us (extremelly limits are 1us and 3us).

	;Save values before interruption.
	movwf		INTERRUPT_W
	movfw		STATUS	
	movwf		INTERRUPT_STATUS
	movfw		PCLATH
	movwf		INTERRUPT_PCLATH

	;--------------------------
	;READ COMMAND FROM CONSOLE
	;--------------------------

	banksel VAR
	;VAR and PORTA are in the same bank.
	;2.2us has passed
	;Each instructions cycle = 2.2us. I ended not indicating the times
	;in the code, but you can find out counting the instruction cycles.
	;------ BYTE 7 ------
	Read_bit_from_console
	;3.2 us has passed
	call ACTIONS_BETWEEN_BITS_SHORTER
	;------ BYTE 6 ------
	Read_bit_from_console
	call ACTIONS_BETWEEN_BITS
	;------ BYTE 5 ------
	Read_bit_from_console
	call ACTIONS_BETWEEN_BITS
	;------ BYTE 4 ------
	Read_bit_from_console
	call ACTIONS_BETWEEN_BITS
	;------ BYTE 3 ------
	Read_bit_from_console
	call ACTIONS_BETWEEN_BITS
	;------ BYTE 2 ------
	Read_bit_from_console
	call ACTIONS_BETWEEN_BITS
	;------ BYTE 1 ------
	Read_bit_from_console
	call ACTIONS_BETWEEN_BITS
	;------ BYTE 0 ------
	Read_bit_from_console
	
	;----CONSOLE IS NOW SENDING US THE STOP BIT----
	;We need to execute a minimum pause so we
	;answer the command AFTER the stop bit (at least 3us
	;after it starts, like official controller does).
	;
	nop ;3.2us
	call DELAY_LABEL ;4us
	;Stop bit starting now...
	call DELAY_LABEL ;0.8us
	goto $+1         ;1.2us
	;We'll make some processing, so this pause is enough.

	;--------------------------
	;SEND_RESPONSE
	;--------------------------

	;3.4us
	;Prepare port to send data
	;Sets value 1
	bsf PORTA,N64_DATA
	;Configure port as output
	banksel TRISA
	bcf TRISA,N64_DATA

	;Select the correct subroutine depending on the command sent
	banksel VAR
	;If 0x01 = Send buttons
	movlw 0x01
	subwf RECEIVED_COMMAND_N64,w
	btfss STATUS,Z
	goto AFTER_01_CHECK
	Send_Controller_Status
	goto SEND_RESPONSE_END
AFTER_01_CHECK
	;If 0x00 = Send controller type
	movlw 0x00
	subwf RECEIVED_COMMAND_N64,w
	btfss STATUS,Z
	goto AFTER_00_CHECK
	Send_Controller_Type
AFTER_00_CHECK

SEND_RESPONSE_END

	;Put port again to receive data
	Prepare_port_to_receive_data

	;Configure interruption flags back
	banksel INTCON
	bcf INTCON,RAIF

	;Restore saved registers
	movfw INTERRUPT_PCLATH
	movwf PCLATH
	movfw INTERRUPT_STATUS
	movwf STATUS
	movfw INTERRUPT_W

	;End
	retfie
	endm

;This one reads a bit sent by the console
;This take 5 cycles = 1us
Read_bit_from_console macro
	local BYTE_IS_ZERO, BYTE_IS_ONE, BYTE_END
	btfsc PORTA,N64_DATA
	goto BYTE_IS_ONE
BYTE_IS_ZERO
	bcf RECEIVED_COMMAND_N64,0
	goto BYTE_END
BYTE_IS_ONE
	bsf RECEIVED_COMMAND_N64,0
	nop ;So this will take the same time regardless the received byte was 1 or 0.
BYTE_END
	endm

;This also takes 1us
;If STOP_BIT_RECEIVED_N64 bit 0 = 0, then it wasn't received
;Else it was received
Read_stop_bit macro
	local BYTE_IS_ZERO, BYTE_IS_ONE, BYTE_END
	btfsc PORTA,N64_DATA
	goto BYTE_IS_ONE
BYTE_IS_ZERO
	clrf STOP_BIT_RECEIVED_N64
	goto BYTE_END
BYTE_IS_ONE
	bsf STOP_BIT_RECEIVED_N64,0
	nop ;So this will take the same time regardless the received byte was 1 or 0.
BYTE_END
	endm

;This routine will take exactly 3.4us
Send_bit_to_console macro bit
	local PUT_LINE_HIGH, DO_NOTHING, SEND_BIT_END
	;Maybe add a nop here?
	bcf PORTA,N64_DATA
	;0us
	nop
	;0.2us
	btfsc DATA_TO_SEND_N64,bit
	goto PUT_LINE_HIGH
	goto DO_NOTHING
PUT_LINE_HIGH
	bsf PORTA,N64_DATA
	;If we needed to send 1, then seeing the instructions we see that it will happen in 1us as we wanted.
	goto SEND_BIT_END
DO_NOTHING
	goto $+1 ;Delay so it takes the same time as if it was 1
SEND_BIT_END
;We're now at 1.4us, we need to wait until 3us
	goto $+1
	nop
	call DELAY_LABEL
	              
	bsf PORTA,N64_DATA ;It will send 1 at 3us as we wanted
	goto $+1 ;3.4us
	endm

Send_stop_bit macro
	bcf PORTA,N64_DATA
;	;0us, we need to have bit up at 1us (2us?)
	call DELAY_LABEL ;0.8us
	call DELAY_LABEL ;1.6us
	nop
	bsf PORTA,N64_DATA 
	;2us
	call DELAY_LABEL ;2.8us
	goto $+1 ;3.2us
	nop ;3.4us

	endm

Send_N64_Byte macro
	;banksel VAR
	Send_bit_to_console 7
	goto $+1
	Send_bit_to_console 6
	goto $+1
	Send_bit_to_console 5
	goto $+1
	Send_bit_to_console 4
	goto $+1
	Send_bit_to_console 3
	goto $+1
	Send_bit_to_console 2
	goto $+1
	Send_bit_to_console 1
	goto $+1
	Send_bit_to_console 0

	endm

;Sends controller type - always 050000 (normal controller + no controller pack)
Send_Controller_Type macro
	movlw 0x05
	movwf DATA_TO_SEND_N64
	Send_N64_Byte

	movlw 0x00
	movwf DATA_TO_SEND_N64
	Send_N64_Byte

	movlw 0x00
	movwf DATA_TO_SEND_N64
	Send_N64_Byte
	goto $+1

	Send_stop_bit

	endm

;Sends controller status, assumes port is already configured
Send_Controller_Status macro
	movfw N64_BYTE_1
	movwf DATA_TO_SEND_N64
	Send_N64_Byte
	
	movfw N64_BYTE_2
	movwf DATA_TO_SEND_N64
	Send_N64_Byte

	movfw N64_BYTE_3
	movwf DATA_TO_SEND_N64
	Send_N64_Byte

	movfw N64_BYTE_4
	movwf DATA_TO_SEND_N64
	Send_N64_Byte
	goto $+1

	Send_stop_bit

	endm

Operations_between_bit_read macro
	rlf RECEIVED_COMMAND_N64
	;3.4us
	goto $+1 ;Each of these calls uses 2 cycles = 0.4us
	nop
	;4us ----- BYTE 1 -----
	call DELAY_LABEL ;Each of these calls uses 4 cycles = 0.8us
	call DELAY_LABEL
	goto $+1 ;Each of these calls uses 2 cycles = 0.4us

	endm

Prepare_port_to_receive_data macro
	;Configure port as input
	banksel TRISA
	bsf TRISA,N64_DATA

	;Turn on pull-up
	banksel OPTION_REG
	bcf OPTION_REG,NOT_RAPU
	banksel WPUA
	bsf WPUA,WPUA1
	
	endm