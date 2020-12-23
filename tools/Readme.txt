These Ruby scripts will allow you to configure the adapter before
flashing the chip:

- analog_deadzone.rb: Configure deadzone (default in original source
code: 7%). Increasing it if you are having troubles with sensitivity
like having a directional being pressed when analog stick is free.
Note that increasing means you are losing precision.
- button_mapping.rb: Will read .yaml files with controller presets, check
default.yaml for an example. You can have one to three presets in the chip
- each preset has its own .yaml file. You can switch the preset in-game
by turning off the analog mode and pressing X. Original source code has only
one preset - default.yaml.

You will need a Ruby interpreter to execute the scripts - official one
can be downloaded here: http://www.ruby-lang.org.

Execute the scripts in command line since you will have to provide
parameters.

Very important: each script will generate .inc files that needs to be
copied over the original source code file. Then you will need to assemble
the code and generate a new .hex file.