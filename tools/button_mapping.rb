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

require 'yaml'

#Constants
OUTPUT_FILENAME="eeprom_data.inc"
HEADER = <<LONGTEXT
;Default EEPROM data
;Logic is the follow:
;First byte = mapping code
;Second byte = pointer related with N64_BYTE_1 location (0x00 =
;N64_BYTE_1, 0x01 = N64_BYTE_2, etc).
;Third byte = Mask that will be used to SET if button is pressed (if
;we need to unpress code will handle mask properly). Just used in case
;of DIGITAL_TO_DIGITAL and ANALOG_TO_DIGITAL.
	org	0x2100
LONGTEXT

$current_entry = 0

class N64_Button_Data
	attr_accessor :word_offset, :mask

	def initialize word_offset, mask
		@word_offset = word_offset
		@mask = mask
	end
end

class Table_Entry
	attr_accessor :mapping_code, :n64_button_data, :label
	
	def initialize(label="")
		@label=label
		@mapping_code="NOTHING_MAPPED"
		@n64_button_data=N64_Button_Data.new("0x00","0x00")
	end
	
	def to_s
		"#{@label}_#{$current_entry}\n" \
		"\tde #{@mapping_code}\n" \
		"\tde #{@n64_button_data.word_offset}\n" \
		"\tde #{@n64_button_data.mask}\n" \
		"\n"
	end
end

N64_BUTTON_DATA = { "A" => N64_Button_Data.new("0x00","b'10000000'"),
				    "B" => N64_Button_Data.new("0x00","b'01000000'"),
					"C-Up" => N64_Button_Data.new("0x01","b'00001000'"),
					"C-Down" => N64_Button_Data.new("0x01","b'00000100'"),
					"C-Left" => N64_Button_Data.new("0x01","b'00000010'"),
					"C-Right" => N64_Button_Data.new("0x01","b'00000001'"),
					"R" => N64_Button_Data.new("0x01","b'00010000'"),
					"Z" => N64_Button_Data.new("0x00","b'00100000'"),
					"Start" => N64_Button_Data.new("0x00","b'00010000'"),
					"L" => N64_Button_Data.new("0x01","b'00100000'"),
					"D-Up" => N64_Button_Data.new("0x00","b'00001000'"),
					"D-Down" => N64_Button_Data.new("0x00","b'00000100'"),
					"D-Left" => N64_Button_Data.new("0x00","b'00000010'"),
					"D-Right" => N64_Button_Data.new("0x00","b'00000001'"),
					"Analog Stick" => N64_Button_Data.new("0x02","0x00") }
					
def parse_button psx_button_name, label, config_file
	table_entry = Table_Entry.new(label)
	
	#Treats analog different
	if /Analog\sStick.*/.match(psx_button_name)
		table_entry.mapping_code="ANALOG_TO_DIRECTIONAL"
		if psx_button_name == "Analog Stick X"
			table_entry.n64_button_data.word_offset="0x02"
		else
			table_entry.n64_button_data.word_offset="0x03"
		end
		return table_entry
	end
	
	n64_button_name=config_file[psx_button_name]
	return table_entry unless N64_BUTTON_DATA.has_key?(n64_button_name)
		
	#We need to have a different code for analog stick
	if /R-.+/.match(psx_button_name) then
		table_entry.mapping_code="ANALOG_TO_DIGITAL"
	else
		table_entry.mapping_code="DIGITAL_TO_DIGITAL"
	end
	
	table_entry.n64_button_data = N64_BUTTON_DATA[n64_button_name]
	table_entry
end
					
def parse_preset filename
	config_file = YAML::load_file(filename)
	
	argument_array=[
		["D-Left","LEFT_EEPROM"],
		["D-Down","DOWN_EEPROM"],
		["D-Right","RIGHT_EEPROM"],
		["D-Up","UP_EEPROM"],
		["Start","START_EEPROM"],
		["R3","R3_EEPROM"],
		["L3","L3_EEPROM"],
		["Select","SELECT_EEPROM"],
		["Square","SQUARE_EEPROM"],
		["X","X_EEPROM"],
		["O","O_EEPROM"],
		["Triangle","TRIANGLE_EEPROM"],
		["R1","R1_EEPROM"],
		["L1","L1_EEPROM"],
		["R2","R2_EEPROM"],
		["L2","L2_EEPROM"],
		["Analog Stick X","ANALOG_L_X_LEFT_EEPROM"],
		["Analog Stick X","ANALOG_L_X_RIGHT_EEPROM"],
		["Analog Stick Y","ANALOG_L_Y_DOWN_EEPROM"],
		["Analog Stick Y","ANALOG_L_Y_UP_EEPROM"],
		["R-Left","ANALOG_R_X_LEFT_EEPROM"],
		["R-Right","ANALOG_R_X_RIGHT_EEPROM"],
		["R-Down","ANALOG_R_Y_DOWN_EEPROM"],
		["R-Up","ANALOG_R_Y_UP_EEPROM"],
	]
	
	processed_preset = ""
	
	argument_array.each do |arguments|
		processed_preset << (parse_button arguments[0],arguments[1],config_file).to_s
	end
	
	$current_entry += 1
	
	#Removes last line breaks
	processed_preset.chomp
	
end

def show_usage
	puts "============================================================"
	puts "Button preset configurator for PSX to N64 Controller Adaptor"
	puts "============================================================"
	puts ""
	puts "ruby button_mapping.rb <preset1.yaml> [<preset2.yaml> [<preset3.yaml]]"
	puts "Check the included example.yaml to know how to build your preset files!"
	puts 
	puts "You can have one to three number of presets"
end

unless (1..3).include? ARGV.length
	show_usage
	exit!
end

File.open(OUTPUT_FILENAME,"w") do |output_file|
	output_file.write("NUMBER_OF_CONTROLLER_PRESETS equ #{ARGV.length}\n\n")
	output_file.write(HEADER)
	
	ARGV.each do |preset_filename|
		output_file.write("\n;-- PRESET #{$current_entry} (#{preset_filename}) --\n\n")
		processed_preset = parse_preset preset_filename
		output_file.write processed_preset
	end
end

puts "\n#{OUTPUT_FILENAME} created."