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

Configure_MCU macro
	;Disable all interruptions
	banksel INTCON
	bcf INTCON,GIE
	;Enable use of external oscillator.
	banksel OSCCON
	bcf OSCCON,SCS
	endm

Configure_IO macro
	;Disable AD so shared PINs can be used for I/O
	banksel ANSEL
	clrf ANSEL
	;Disable comparator, so shared PINs can be used for I/O
	banksel CMCON0
	movlw 0x07
	movwf CMCON0
	;Configure direction of data, 1=Input, 0=Output
	banksel TRISA
	bsf TRISA,PSX_DATA
	bsf TRISA,N64_DATA
	bcf TRISA,PSX_CLK
	banksel TRISC
	bcf TRISC,PSX_ATT
	bcf TRISC,PSX_COMMAND
	bcf TRISC,LED
	;Enable pull-up resistors
	;Global
	banksel OPTION_REG
	bcf OPTION_REG,NOT_RAPU
	;Per pin
	banksel WPUA
	clrf WPUA
	;PSX_DATA
	bsf WPUA,WPUA0
	;N64_DATA
	bsf WPUA,WPUA1
	;Configure initial status PINs
	banksel PORTA
	bsf PORTA,PSX_CLK
	banksel PORTC
	bsf PORTC,PSX_ATT
	bcf PORTC,LED
	;Disable interruptions that we don't want
	banksel PIE1
	clrf PIE1
	;Enable interruptions on change for N64_DATA
	banksel IOCA
	movlw b'00000010'
	movwf IOCA
	;Enables interruptions for PORTA
	banksel INTCON
	clrf INTCON
	bsf INTCON,RAIE
	;Makes sure interruption flag is down before turning ON the interruptions
	;Reads PortA to "end mismatch condition" (manual)
	banksel PORTA
	movfw PORTA
	;Turns interruption flag OFF
	banksel INTCON
	bcf INTCON,RAIF
	;Enable global interruptions
	banksel INTCON
	bsf INTCON,GIE
	endm

;Force some variables into a default value
Initialize_variables macro
	banksel VAR
	;All buttons/sticks unpressed/free
	clrf N64_BYTE_1
	clrf N64_BYTE_2
	clrf N64_BYTE_3
	clrf N64_BYTE_4

	banksel VAR
	clrf PSX_DIGITAL_1
	clrf PSX_DIGITAL_2
	movlw 0x80
	movwf PSX_ANALOG_L_X
	movwf PSX_ANALOG_L_Y
	movwf PSX_ANALOG_R_X
	movwf PSX_ANALOG_R_Y

	banksel VAR
	movlw 0x50
	movwf EEPROM_POINTER

	clrf CONFIGURATION_MODE_FLAG
	clrf CURRENT_BUTTON_PRESET
	endm

Configure_timer macro
	banksel T1CON
	;1:8 prescale. At Fosc = 20mhz => Fosc/4 = 5mhz => 1:8 = 625khz
	bsf T1CON,T1CKPS1
	bsf T1CON,T1CKPS0
	;Uses internal oscillator as timer source
	bcf T1CON,TMR1CS
	endm