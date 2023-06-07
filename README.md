# terminal_snake

Simple snake game for the UNIX terminal, made using ANSI escape codes (made easier thanks to my own terminal_gui header).

Can also serve as a further demo for the terminal_gui code.

Requires UNIX, as terminal_gui uses UNIX functionality.

## Building + running

To build the package, make is required

Compile and link executable normally using `make all`

Use `make help` to see all available make targets

Use `make DEBUG=address <...>` for any target to execute with the address sanitizer C flags

Use `make run` or execute the ELF file `output.elf` to run the game.
