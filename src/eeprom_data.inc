NUMBER_OF_CONTROLLER_PRESETS equ 1

;Default EEPROM data
;Logic is the follow:
;First byte = mapping code
;Second byte = pointer related with N64_BYTE_1 location (0x00 =
;N64_BYTE_1, 0x01 = N64_BYTE_2, etc).
;Third byte = Mask that will be used to SET if button is pressed (if
;we need to unpress code will handle mask properly). Just used in case
;of DIGITAL_TO_DIGITAL and ANALOG_TO_DIGITAL.
	org	0x2100

;-- PRESET 0 (default.yaml) --

LEFT_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

DOWN_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

RIGHT_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

UP_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

START_EEPROM_0
	de DIGITAL_TO_DIGITAL
	de 0x00
	de b'00010000'

R3_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

L3_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

SELECT_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

SQUARE_EEPROM_0
	de DIGITAL_TO_DIGITAL
	de 0x00
	de b'01000000'

X_EEPROM_0
	de DIGITAL_TO_DIGITAL
	de 0x00
	de b'10000000'

O_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

TRIANGLE_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

R1_EEPROM_0
	de DIGITAL_TO_DIGITAL
	de 0x01
	de b'00010000'

L1_EEPROM_0
	de DIGITAL_TO_DIGITAL
	de 0x00
	de b'00100000'

R2_EEPROM_0
	de NOTHING_MAPPED
	de 0x00
	de 0x00

L2_EEPROM_0
	de DIGITAL_TO_DIGITAL
	de 0x01
	de b'00100000'

ANALOG_L_X_LEFT_EEPROM_0
	de ANALOG_TO_DIRECTIONAL
	de 0x02
	de 0x00

ANALOG_L_X_RIGHT_EEPROM_0
	de ANALOG_TO_DIRECTIONAL
	de 0x02
	de 0x00

ANALOG_L_Y_DOWN_EEPROM_0
	de ANALOG_TO_DIRECTIONAL
	de 0x03
	de 0x00

ANALOG_L_Y_UP_EEPROM_0
	de ANALOG_TO_DIRECTIONAL
	de 0x03
	de 0x00

ANALOG_R_X_LEFT_EEPROM_0
	de ANALOG_TO_DIGITAL
	de 0x01
	de b'00000010'

ANALOG_R_X_RIGHT_EEPROM_0
	de ANALOG_TO_DIGITAL
	de 0x01
	de b'00000001'

ANALOG_R_Y_DOWN_EEPROM_0
	de ANALOG_TO_DIGITAL
	de 0x01
	de b'00000100'

ANALOG_R_Y_UP_EEPROM_0
	de ANALOG_TO_DIGITAL
	de 0x01
	de b'00001000'
