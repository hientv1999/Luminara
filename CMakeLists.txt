add_executable(Luminara
        main.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(Luminara pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(Luminara)

