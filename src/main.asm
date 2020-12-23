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


	;To avoid showing bank related messages
	errorlevel 1
	processor 16f688
	__CONFIG _FOSC_HS & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _IESO_OFF & _FCMEN_OFF

	#include P16F688.inc

	#include "ram.inc"
	#include "constants.inc"
	#include "macros.inc"


	org 0x000
	goto MAIN

	org 0x004
	Interrupt_handling

MAIN
	Configure_MCU
	Initialize_variables
	Configure_timer
	clrw ;Load button preset 0 at the beggining
	call LOAD_DATA_FROM_EEPROM
	Configure_IO

MAIN_LOOP
	Poll_controller
	btfsc CONFIGURATION_MODE_FLAG,0
	goto CONFIGURATION_MODE_ENABLED
	Convert_buttons
	goto MAIN_LOOP
CONFIGURATION_MODE_ENABLED
	call CONFIGURATION_MODE
	goto MAIN_LOOP

	;External subroutines
	#include "external_code.inc"

	#include "eeprom_data.inc"
	#include "analog_table.inc"

	end