# linker-test

This repository contains example of solving naming collision in C dynamic
libraries using version script with GNU linker. Libraries `a` and `b` have
corresponding wrapper shared objects and force main program to refer to `say_hi`
functions indirectly via their non-colliding counterparts. Keep in mind that
this approach is viable only with shared objects. With static objects you need
to manually rename colliding symbols with `objcopy --redefine-syms`.
