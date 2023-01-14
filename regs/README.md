# Preservation of registers

```regs.s``` should be understood, assembled and run under gdb. Step through various strategically chosen instructions and dump all the main AArch64 registers.

This will teach you what you should expect when making function calls.

# Building

```gcc -g regs.s```

The ```-g``` enables debugging support. The above command links with the standard C run time.

# Learning

Use this with ```gdb``` to learn ```gdb``` and some ```AArch64```.

## ```gdb``` commands you must learn

Look these up at a minumum:

* break
* step
* next
* print
* x (examine)
* run
* list
* info



