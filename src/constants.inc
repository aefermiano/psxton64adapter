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

;Are we debugging?
;DEBUG equ 1

;Pin roles
PSX_DATA equ RA0
N64_DATA equ RA1
PSX_CLK equ RA2
PSX_COMMAND equ RC1
PSX_ATT equ RC0
LED equ RC3

;Location of PSX to N64 analog mapping table.
;There is a bug in the assembler... __maxrom
;won't accept hexadecimal values. So I declare
;0x0E00 in decimal here.
N64_TABLE_LOCATION equ 0xE00
N64_TABLE_LOCATION_HIGH equ 0x0E
N64_TABLE_LOCATION_LOW equ 0x00

;Mapping codes
NOTHING_MAPPED equ 0x00
DIGITAL_TO_DIGITAL equ 0x02
ANALOG_TO_DIGITAL equ 0x03
ANALOG_TO_DIRECTIONAL equ 0x04

;Threshold to determine if an analog triggered a
;digital button or not
;No complement de 2
LOWER_THRESHOLD equ d'64'
HIGHER_THRESHOLD equ d'192'
;In complement of 2
;LOWER_THRESHOLD equ 0xEC
;HIGHER_THRESHOLD equ 0x14

;Playstation delays
DELAY_OF_CLOCK_DOWN equ d'60'
DELAY_OF_CLOCK_UP equ d'50'
DELAY_BETWEEN_WORDS equ d'110'
DELAY_BEFORE_START equ d'10'

;EEPROM location for button mapping preset
PRESET_TABLE_SIZE equ d'72'
EEPROM_PRESET_0_LOCATION equ 0x00
EEPROM_PRESET_1_LOCATION equ 0x48
EEPROM_PRESET_2_LOCATION equ 0x90

;Each cycle is 100ms
CYCLES_DURING_LED_BLINK equ d'3'