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

;Subroutine
;It shifts the data register and execute some needed delays.
;Total = 15 or 14 cycles, depending on the label chosen.
ACTIONS_BETWEEN_BITS
	nop
ACTIONS_BETWEEN_BITS_SHORTER
	rlf RECEIVED_COMMAND_N64
	;3.4us
	nop
	;4us ----- BYTE 1 -----
	call DELAY_LABEL ;Each of these calls uses 4 cycles = 0.8us
	call DELAY_LABEL
	return