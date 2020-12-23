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

VAR equ 0x20
	cblock VAR
	;N64 related variables
	RECEIVED_COMMAND_N64
	DATA_TO_SEND_N64

	;Information variables
	;A,B,Z,Start,D-Up,D-Down,D-Left,D-Right
	N64_BYTE_1
	;0, 0, L, R, C-Up, C-Right, C-Down, C-Left
	;OR
	;0, 0, L, R, C-Up, C-Down, C-Left, C-Right
	N64_BYTE_2
	;X coordinate
	N64_BYTE_3
	;Y coordinate
	N64_BYTE_4

	;Playstation variables
	;Left, Down, Right, Up, Start, R3, L3, Select
	PSX_DIGITAL_1
	;Square, X, O, Triangle, R2, L2, R1, L1
	PSX_DIGITAL_2
	PSX_ANALOG_L_X
	PSX_ANALOG_L_Y
	PSX_ANALOG_R_X
	PSX_ANALOG_R_Y

	;Temp controller variables
	;Under certain circunstances controller update may be invalid. That's
	;That's why we use these variables instead of the main ones
	;directly.
	TEMP_PSX_DIGITAL_1
	TEMP_PSX_DIGITAL_2
	TEMP_PSX_ANALOG_L_X
	TEMP_PSX_ANALOG_L_Y
	TEMP_PSX_ANALOG_R_X
	TEMP_PSX_ANALOG_R_Y

	COMMAND_TO_SEND_PSX
	DATA_RECEIVED_PSX

	;Temporary variables
	TEMP
	TEMP2

	;Variables used for debugging functions.
	EEPROM_POINTER
	OLD_EEADR
	OLD_EEADRH
	endc

	;Interruption variables, common to all banks
	cblock 0x71
	INTERRUPT_W
	INTERRUPT_STATUS
	INTERRUPT_PCLATH
	TEMP_COMMON
	TEMP_COMMON_2
	CONFIGURATION_MODE_FLAG
	CURRENT_BUTTON_PRESET
	endc