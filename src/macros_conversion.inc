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

Convert_buttons macro
	;Process digital buttons
	;Pointer to the first element
	movlw PSX_BUTTON_LEFT_CODE
	movwf FSR
	;We now start processing each playstation button
	;Left
	Process_digital_button b'10000000',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;Down
	Process_digital_button b'01000000',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;Right
	Process_digital_button b'00100000',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;Up
	Process_digital_button b'00010000',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;Start
	Process_digital_button b'00001000',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;R3
	Process_digital_button b'00000100',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;L3
	Process_digital_button b'00000010',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;Select
	Process_digital_button b'00000001',PSX_DIGITAL_1
	Increment_FSR_to_next_group
	;---SET 2--
	;Square
	Process_digital_button b'10000000',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;X
	Process_digital_button b'01000000',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;O
	Process_digital_button b'00100000',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;Triangle
	Process_digital_button b'00010000',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;R2
	Process_digital_button b'00001000',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;L2
	Process_digital_button b'00000100',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;R1
	Process_digital_button b'00000010',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;L1
	Process_digital_button b'00000001',PSX_DIGITAL_2
	Increment_FSR_to_next_group
	;----SET 3 - Analog button -----
	;Left Analog Left
	Process_analog_button PSX_ANALOG_L_X,PROCESS_ANALOG_LEFT_OR_DOWN
	Increment_FSR_to_next_group
	;Left Analog Right
	;Removed because we'll only use left analog for analog stick
	;X analog is already processed above.
	;Process_analog_button PSX_ANALOG_L_X,PROCESS_ANALOG_RIGHT_OR_UP
	Increment_FSR_to_next_group
	;Left Analog Up
	Process_analog_button PSX_ANALOG_L_Y,PROCESS_ANALOG_RIGHT_OR_UP
	Increment_FSR_to_next_group
	;Left Analog Down
	;Removed because we'll only use left analog for analog stick.
	;Y analog is already processed above.
	;Process_analog_button PSX_ANALOG_L_Y,PROCESS_ANALOG_LEFT_OR_DOWN
	Increment_FSR_to_next_group
	;Right Analog Left
	Process_analog_button PSX_ANALOG_R_X,PROCESS_ANALOG_LEFT_OR_DOWN
	Increment_FSR_to_next_group
	;Right Analog Right
	Process_analog_button PSX_ANALOG_R_X,PROCESS_ANALOG_RIGHT_OR_UP
	Increment_FSR_to_next_group
	;Right Analog Up
	Process_analog_button PSX_ANALOG_R_Y,PROCESS_ANALOG_RIGHT_OR_UP
	Increment_FSR_to_next_group
	;Right Analog Down
	Process_analog_button PSX_ANALOG_R_Y,PROCESS_ANALOG_LEFT_OR_DOWN

	endm

Increment_FSR_to_next_group macro
	movlw 0x03
	addwf FSR,f

	endm

Process_digital_button macro Mask,Register
	banksel VAR
	;Checks if button is pressed before calling function
	movlw Mask
	andwf Register,w
	call PROCESS_DIGITAL_BUTTON

	endm

Process_analog_button macro Register,Function_name
	banksel VAR
	movfw Register
	call Function_name

	endm