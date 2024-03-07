add_executable(SOL-DN
        ssd1306_i2c.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(SOL-DN pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(SOL-DN)

