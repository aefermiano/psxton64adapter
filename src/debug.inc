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

;Debug routine I used for "logging".
;Not used in the final version. I just let it here in
;for future development.
LOG_W_TO_EEPROM
	banksel VAR
	movwf TEMP
	banksel EEADR
	movfw EEADR
	banksel VAR
	movwf OLD_EEADR
	banksel EEADR
	movfw EEADRH
	banksel VAR
	movwf OLD_EEADRH
	banksel EEADRH
	clrf EEADRH
	banksel PIR1
	bcf PIR1,EEIF
	;Gives up if pointer is equal to maximum position
	movlw 0x80
	subwf EEPROM_POINTER,w
	btfsc STATUS,Z
	goto EEPROM_END
	movfw EEPROM_POINTER
	banksel EEADR
	movwf EEADR
	banksel VAR
	movfw TEMP
	banksel EEDAT
	movwf EEDAT
	banksel EECON1
	bcf EECON1,EEPGD
	bsf EECON1,WREN
	bcf INTCON,GIE
	btfsc INTCON,GIE
	goto $-2
	movlw 0x55
	movwf EECON2
	movlw 0xAA
	movwf EECON2
	bsf EECON1,WR
	bsf INTCON,GIE
	;Wait until write is complete
	banksel PIR1
EEPROM_LOOP
	btfss PIR1,EEIF
	goto EEPROM_LOOP
	banksel PIR1
	bcf PIR1,EEIF
	banksel EECON1
	bcf EECON1,WREN
	;Increment pointer
	banksel VAR
	incf EEPROM_POINTER
	goto EEPROM_END
EEPROM_END
	banksel VAR
	movfw OLD_EEADR
	banksel EEADR
	movwf EEADR
	banksel VAR
	movfw OLD_EEADRH
	banksel EEADR
	movwf EEADRH
	banksel VAR
	movfw TEMP
	return