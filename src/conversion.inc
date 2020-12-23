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

;SUBROUTINE
;Requirements
;FSR is pointing to the button "structure" (3 consecutive bytes in RAM)
;w indicates if button is pressed or not.
;0 = Button not pressed
;Anything else = button is pressed
PROCESS_DIGITAL_BUTTON
	;Set variables
	banksel VAR2
	movwf IS_BUTTON_PRESSED
	movfw FSR
	movwf LAST_FSR
	bankisel VAR2
	movfw INDF
	movwf CURRENT_CODE
	incf FSR
	movfw INDF
	movwf CURRENT_POINTER
	incf FSR
	movfw INDF
	movwf CURRENT_MASK
	;We now check which code we need to process
	;NOTHING_MAPPED
	movlw NOTHING_MAPPED
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	goto EXIT_FUNCTION_DIGITAL_TO_DIGITAL
	;DIGITAL_TO_DIGITAL
	banksel VAR2
	movlw DIGITAL_TO_DIGITAL
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	call PROCESS_DIGITAL_TO_DIGITAL
EXIT_FUNCTION_DIGITAL_TO_DIGITAL
	banksel VAR2
	movfw LAST_FSR
	movwf FSR
	return

PROCESS_DIGITAL_TO_DIGITAL
	banksel VAR
	;Makes FSR point to first command data
	movlw N64_BYTE_1
	movwf FSR
	;Adds pointer to FSR
	banksel VAR2
	movfw CURRENT_POINTER
	addwf FSR,f
	;Check if button is pressed or not
	movfw IS_BUTTON_PRESSED
	btfsc STATUS,Z
	goto BUTTON_IS_NOT_PRESSED
BUTTON_IS_PRESSED
	movfw CURRENT_MASK
	banksel VAR
	bankisel VAR
	iorwf INDF,f
	return
BUTTON_IS_NOT_PRESSED
	comf CURRENT_MASK,w
	banksel VAR
	bankisel VAR
	andwf INDF,f
	return

;SUBROUTINE
;Requirements
;FSR is pointing to the "structure" (3 consecutive bytes in RAM)
;w = value
PROCESS_ANALOG_LEFT_OR_DOWN
;Set variables
	banksel VAR2
	bankisel VAR2
	;IS_BUTTON_PRESSED actually contains the value of the analog (ugly, I know!)
	movwf IS_BUTTON_PRESSED
	movfw FSR
	movwf LAST_FSR
	movfw INDF
	movwf CURRENT_CODE
	incf FSR
	movfw INDF
	movwf CURRENT_POINTER
	incf FSR
	movfw INDF
	movwf CURRENT_MASK
	;We now check which code we need to process
	;NOTHING_MAPPED
	movlw NOTHING_MAPPED
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	goto EXIT_FUNC_ANALOG_LEFT_OR_DOWN
	;ANALOG_TO_DIRECTIONAL
	banksel VAR2
	movlw ANALOG_TO_DIRECTIONAL
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	call PROCESS_ANALOG_TO_DIRECTIONAL
	;ANALOG_TO_DIGITAL
	banksel VAR2
	movlw ANALOG_TO_DIGITAL
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	call PROC_ANALOG_TO_DIGI_LEFT_OR_DOWN
EXIT_FUNC_ANALOG_LEFT_OR_DOWN
	banksel VAR2
	movfw LAST_FSR
	movwf FSR
	return

PROCESS_ANALOG_TO_DIRECTIONAL
	banksel VAR2
	;Setups pointer
	movlw N64_BYTE_1
	movwf FSR
	;Adds pointer to FSR
	movfw CURRENT_POINTER
	addwf FSR,f
	;Setups value
	;movfw IS_BUTTON_PRESSED
	call CONVERT_ANALOG_PSX_TO_N64
	banksel VAR2
	movfw IS_BUTTON_PRESSED
	banksel VAR
	bankisel VAR
	movwf INDF
	return

;Will convert IS_BUTTON_PRESSED to a N64 value
CONVERT_ANALOG_PSX_TO_N64
	;Remove fast!
	;banksel VAR2
	;movlw 0xCC
	;movwf IS_BUTTON_PRESSED
	;Read of flash memory is pretty much
	;copy of the manual.
	banksel EEADR
	movlw N64_TABLE_LOCATION_HIGH
	movwf EEADRH
	movlw N64_TABLE_LOCATION_LOW
	movwf EEADR
	;Maps the correct value
	banksel VAR2
	movfw IS_BUTTON_PRESSED
	banksel EEADR
	addwf EEADR,f
	banksel INTCON
	banksel EECON1
	;Require sequence
	;bcf INTCON, GIE
	bsf EECON1,EEPGD
	bsf EECON1,RD
	nop
	nop
	;bsf INTCON, GIE
	banksel EEDAT
	movfw EEDAT
	banksel VAR2
	movwf IS_BUTTON_PRESSED
	banksel INTCON
	return
	
PROC_ANALOG_TO_DIGI_LEFT_OR_DOWN
	;Setups pointer
	banksel VAR2
	movlw N64_BYTE_1
	movwf FSR
	;Adds pointer to FSR
	movfw CURRENT_POINTER
	addwf FSR,f
	;Checks if button is pressed or not
	;For that, value must be lower than the THRESHOLD
	;To compare using hardware, we'll have to convert everything
	;to 2 complement...
	;We'll convert the value - threshold is already in 2-complement!
	;goto BUTTON_NOT_PRESSED_LEFT_OR_DOWN
	;movlw 0x80
	;subwf IS_BUTTON_PRESSED,f
	movlw LOWER_THRESHOLD
	subwf IS_BUTTON_PRESSED,w
	btfsc STATUS,C
	goto BUTTON_NOT_PRESSED_LEFT_OR_DOWN
BUTTON_IS_PRESSED_LEFT_OR_DOWN
	movfw CURRENT_MASK
	banksel VAR
	bankisel VAR
	iorwf INDF,f
	return
BUTTON_NOT_PRESSED_LEFT_OR_DOWN
	comf CURRENT_MASK,w
	banksel VAR
	bankisel VAR
	andwf INDF,f
	return

;SUBROUTINE
;Requirements
;FSR is pointing to the "structure" (3 consecutive bytes in RAM)
;w = value
PROCESS_ANALOG_RIGHT_OR_UP
;Set variables
	banksel VAR2
	bankisel VAR2
	;IS_BUTTON_PRESSED actually contains the value of the analog
	movwf IS_BUTTON_PRESSED
	movfw FSR
	movwf LAST_FSR
	movfw INDF
	movwf CURRENT_CODE
	incf FSR
	movfw INDF
	movwf CURRENT_POINTER
	incf FSR
	movfw INDF
	movwf CURRENT_MASK
	;We now check which code we need to process
	;NOTHING_MAPPED
	movlw NOTHING_MAPPED
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	goto EXIT_FUNCTION_RIGHT_OR_UP
	;ANALOG_TO_DIRECTIONAL
	;We may process this function 2 times, but that seems OK...
	banksel VAR2
	movlw ANALOG_TO_DIRECTIONAL
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	call PROCESS_ANALOG_TO_DIRECTIONAL
	;ANALOG_TO_DIGITAL
	banksel VAR2
	movlw ANALOG_TO_DIGITAL
	subwf CURRENT_CODE,w
	btfsc STATUS,Z
	call PROC_ANALOG_TO_DIGI_RIGHT_OR_UP
EXIT_FUNCTION_RIGHT_OR_UP
	banksel VAR2
	movfw LAST_FSR
	movwf FSR
	return

PROC_ANALOG_TO_DIGI_RIGHT_OR_UP
	banksel VAR2
	;Setups pointer
	movlw N64_BYTE_1
	movwf FSR
	;Adds pointer to FSR
	movfw CURRENT_POINTER
	addwf FSR,f
	;Checks if button is pressed or not
	;For that, value must be higher than the threshold
	movlw HIGHER_THRESHOLD
	subwf IS_BUTTON_PRESSED,w
	btfss STATUS,C
	goto BUTTON_IS_NOT_PRESSED
BUTTON_IS_PRESSED_RIGHT_OR_UP
	movfw CURRENT_MASK
	banksel VAR
	bankisel VAR
	iorwf INDF,f
	return
BUTTON_NOT_PRESSED_RIGHT_OR_UP
	comf CURRENT_MASK,w
	banksel VAR
	bankisel VAR
	andwf INDF,f
	return

;W contains the preset that we're going to load:
;0, 1 or 2.
LOAD_DATA_FROM_EEPROM
	;TEMP_COMMON holds the preset number we need to hold
	movwf TEMP_COMMON
	banksel VAR2
	bankisel VAR2
	;First RAM address
	movlw PSX_BUTTON_LEFT_CODE
	movwf FSR
	;Setups address of EEPROM
	banksel EEADR
	;Assume it's preset 0
	movlw EEPROM_PRESET_0_LOCATION
	movwf EEADR
EEPROM_PRESET_SELECT_TEST_1
	movlw 0x01
	subwf TEMP_COMMON,w
	btfss STATUS,Z
	goto EEPROM_PRESET_SELECT_TEST_2
EEPROM_PRESET_1
	movlw EEPROM_PRESET_1_LOCATION
	movwf EEADR
	goto EEPROM_PRESET_SELECT_END
EEPROM_PRESET_SELECT_TEST_2
	movlw 0x02
	subwf TEMP_COMMON,w
	btfss STATUS,Z
	goto EEPROM_PRESET_SELECT_END
EEPROM_PRESET_2
	movlw EEPROM_PRESET_2_LOCATION
	movwf EEADR
EEPROM_PRESET_SELECT_END
	banksel EECON1
	bcf EECON1,EEPGD
	;Setups loop counter
	banksel VAR2
	movlw PRESET_TABLE_SIZE
	movwf TEMP_BANK_2
	;Reading loop
LOAD_DATA_FROM_EEPROM_LOOP
	;Main code is copy from the manual
	banksel EECON1
	bsf EECON1,RD
	movfw EEDAT
	banksel VAR2
	movwf INDF
	incf FSR
	banksel EEADR
	incf EEADR
	banksel VAR2
	decfsz TEMP_BANK_2
	goto LOAD_DATA_FROM_EEPROM_LOOP
	return

;0x0BDB->0xFFFF ~ 100ms
PAUSE_100MS
	banksel TMR1H
	movlw 0x0B
	movwf TMR1H
	banksel TMR1L
	movlw 0xDB
	movwf TMR1L
	;Clears interrupt flag
	banksel PIR1
	bcf PIR1,TMR1IF
	;Turns timer1 ON
	banksel T1CON
	bsf T1CON,TMR1ON
	banksel PIR1
PAUSE_100MS_LOOP
	btfss PIR1,TMR1IF
	goto PAUSE_100MS_LOOP
	;Turns timer OFF again
	banksel T1CON
	bcf T1CON,TMR1ON
	return

;W = times of 100ms to pause
;Pause 500ms => W = 5
PAUSE_W_TIMES_100MS
	movwf TEMP_COMMON
PAUSE_W_TIMES_100MS_LOOP
	call PAUSE_100MS
	decfsz TEMP_COMMON
	goto PAUSE_W_TIMES_100MS_LOOP
	return

TURN_ON_LED
	banksel PORTC
	bsf PORTC,LED
	return

TURN_OFF_LED
	banksel PORTC
	bcf PORTC,LED
	return

BLINK_LED_ONCE
	call TURN_ON_LED
	movlw CYCLES_DURING_LED_BLINK
	call PAUSE_W_TIMES_100MS
	call TURN_OFF_LED
	movlw CYCLES_DURING_LED_BLINK
	call PAUSE_W_TIMES_100MS
	return

BLINK_LED_W_TIMES
	movwf TEMP_COMMON_2
BLINK_LED_W_TIMES_LOOP
	call BLINK_LED_ONCE
	decfsz TEMP_COMMON_2
	goto BLINK_LED_W_TIMES_LOOP
	return

INCREMENT_CURRENT_BUTTON_PRESET
	movlw (NUMBER_OF_CONTROLLER_PRESETS-1)
	subwf CURRENT_BUTTON_PRESET,w
	btfsc STATUS,Z
	goto RESET_CURRENT_BUTTON_PRESET
ACTUALLY_INCREMENT_CURRENT_BUTTON_PRESET
	incf CURRENT_BUTTON_PRESET,f
	return
RESET_CURRENT_BUTTON_PRESET
	clrf CURRENT_BUTTON_PRESET
	return

CONFIGURATION_MODE
	;If user presses X, adaptor changes button profile
	banksel VAR
	btfss PSX_DIGITAL_2,6
	return
CHANGE_PRESET
	;Effectivelly change the preset
	call INCREMENT_CURRENT_BUTTON_PRESET
	movfw CURRENT_BUTTON_PRESET
	call LOAD_DATA_FROM_EEPROM
	;Blink the led to indicate which profile we are
	movfw CURRENT_BUTTON_PRESET
	;We increment W so if we are at profile 0, we blink 1 time
	addlw 0x01
	call BLINK_LED_W_TIMES
	;Now we pause 1 seconds o user have time to unpress
	;the button before profile changes again
	movlw d'10'
	call PAUSE_W_TIMES_100MS
	return