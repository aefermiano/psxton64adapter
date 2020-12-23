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

;Please notice we don't check ACK (it's not even connected)
;because it would require the use of another I/O pin with pull-up.
;I had no problem because of it so far - checking if controller is analog
;and aborting otherwise is a check good enough.
Poll_controller macro
	call START_TRANSMISSION
	;If we send a message too fast after putting ACK down,
	;controller won't answer properly.
	movlw DELAY_BEFORE_START
	call W_COUNTER_DELAY
	;Command start
	banksel VAR
	movlw 0x01
	movwf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	;Controller didn't send anything for us yet
	;Asking controller for its button
	movlw 0x42
	movwf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	;We'll only work with controller in analog mode, so if it's other type
	;we'll abort the polling.
	movfw DATA_RECEIVED_PSX
	Check_if_controller_is_analog
	;We turn configuration mode ON in case controller is
	;not in analog mode
	sublw 0x01
	btfsc STATUS,Z
	goto UNSET_CONFIGURATION_MODE
	bsf CONFIGURATION_MODE_FLAG,0
	goto CONTINUE_POLLING
UNSET_CONFIGURATION_MODE
	clrf CONFIGURATION_MODE_FLAG
CONTINUE_POLLING
	banksel VAR
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	;Controller will send a 5A, but we don't check.
	;Now we get the first data byte (digital 1)
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	comf DATA_RECEIVED_PSX,w
	movwf PSX_DIGITAL_1
	;Gets second data byte (digital 2)
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	comf DATA_RECEIVED_PSX,w
	movwf PSX_DIGITAL_2
	;Gets third data byte (RX)
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	movfw DATA_RECEIVED_PSX
	movwf PSX_ANALOG_R_X
	;Gets fourth data byte (RY)
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	movfw DATA_RECEIVED_PSX
	movwf PSX_ANALOG_R_Y
	;Gets fifth data byte (LX)
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	movfw DATA_RECEIVED_PSX
	movwf PSX_ANALOG_L_X
	;Gets last data byte (LY)
	clrf COMMAND_TO_SEND_PSX
	call SEND_COMMAND_TO_CONTROLLER
	movfw DATA_RECEIVED_PSX
	movwf PSX_ANALOG_L_Y
	;We invert UP with DOWN because it's inverted in N64!
	movlw 0x80
	subwf PSX_ANALOG_L_Y,f
	comf PSX_ANALOG_L_Y,w
	addlw 0x80
	movwf PSX_ANALOG_L_Y
ABORT_POLLING
	call STOP_TRANSMISSION
	endm

;Response = 0, not analog. Response = 1, it's analog.
;Analog means controller identifies itself as 0x73.
Check_if_controller_is_analog macro
	;Checks first nibble
	sublw 0x73
	btfsc STATUS,Z
	goto ITS_ANALOG
ITS_NOT_ANALOG
	clrw
	goto CHECK_ANALOG_END
ITS_ANALOG
	movlw 0x01
CHECK_ANALOG_END
	
	endm