#!/usr/bin/env ruby

# Copyright 2012 Antonio Fermiano
#
# This file is part of PsxToN64Adapter.
#
# PsxToN64Adapter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PsxToN64Adapter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY# without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

#Constants
PERCENTAGE_RANGE = 0.63
SUPERIOR_LIMIT = 128
INFERIOR_LIMIT = -128
FILENAME="analog_table.inc"


def show_usage
	puts "========================================================"
	puts "Analog table generator for PSX to N64 Controller Adaptor"
	puts "========================================================"
	puts ""
	puts "ruby analog_deadzone.rb <deadzone>"
	puts "Deadzone must be a float. E.g.: 0.05 means 5%"
end

def float? value
		/[0-9]+\.[0-9]+/.match(value.to_s) ? true : false
end

if ARGV.length != 1
	show_usage
	exit!
end

def calculate_position psx_position
	psx_position -= 0x80

	return 0 if psx_position.between?($threshold_inferior, $threshold_superior)
	
	if psx_position > 0
		n64_position=((psx_position-$threshold_superior)*SUPERIOR_LIMIT)/(SUPERIOR_LIMIT-$threshold_superior)
	else
		n64_position=((-psx_position+$threshold_inferior)*INFERIOR_LIMIT)/($threshold_inferior-INFERIOR_LIMIT)
	end
	
	n64_position = n64_position * PERCENTAGE_RANGE
	n64_position = n64_position.round
	n64_position & 0xFF
end

deadzone = ARGV[0].to_f

if !float?(ARGV[0]) || deadzone >= 1.0 || deadzone < 0
	puts "Invalid deadzone."
	exit!
end

$threshold_superior, $threshold_inferior = SUPERIOR_LIMIT * deadzone, INFERIOR_LIMIT * deadzone

analog_table=""
for psx_position in 0..255 do
	n64_position = calculate_position psx_position
	analog_table << "\tdata 0x%.2X\n" % n64_position
end
#Removes last \n
analog_table.chomp!

template = DATA.read
processed_template = eval('"' + template + '"')

File.open(FILENAME,"w") do |destination_file|
	destination_file.write processed_template
end

puts "\n#{FILENAME} written."

__END__
;Table with values for N64 analog conversion.
;
;Please notice it's in the memory program.
;I don't know if assembler will alert if program overlaps
;this table (I don't think it will), so be careful with
;long programs.
;
;It needs to start at XX00, because we only change the offset
;of the least significant byte, and we need 256 different
;addresses.
	org N64_TABLE_LOCATION
	;For values of 0x00 to 0xFF of Playstation analog, here are 
	;the correspondent - in order - of the
	;value to use for N64
#{analog_table}
	end