# Jump or Branch Tables

A jump or branch table is a powerful instruction saving technique that
can be used to switch between multiple single instructions or even
choose one of a series of functions to call (or branches to take).

This concept can be found as the implementation of some `switch`
statements and is found at the very very lowest end of an Operating
System (interrupt vectors, for example).

The

## Single Instructions a la Duff's Device

[Duff's Device](https://en.wikipedia.org/wiki/Duff%27s_device) shoe
horned a jump table into the middle of a `while` loop. At the same
time, it also correctly demonstrates a simple case of *loop unrolling*.
It's very creative.

Let's expand on Duff's Device.

wedwedwed

### Performing Multiple Instructions

## Multiple Instructions (modification of Duff's Device)

## Small Gaps in Sequential Indexes

Suppose your range of indexes was 0 through 8 inclusive (notice there
are 9 integers in the range) but index 7 is skipped. That is, your
potential indexes are 0 through 6 inclusive and then 8 but never
7.

In a `switch` statement, this would look like:

```c++
switch (index) {
    case 0: blah blah;
            break;
    case 1: blah blah;
            break;
    case 2: blah blah;
            break;
    case 3: blah blah;
            break;
    case 4: blah blah;
            break;
    case 5: blah blah;
            break;
    case 6: blah blah;
            break;
    case 8: blah blah;
            break;
}
```

Gaps in the potential indexes presents a surmountable problem if the
gaps are few.

In the case where there are a small number of gaps simple fill them
with a branch to a common, otherwise "do nothing", label. For example,
you might have:

```asm
b_table:    b       label0
            b       label1
            b       label2
            b       label3
            b       label4
            b       label5
            b       label6
            b       do_nothing
            b       label8
```

in a Duff's Device where you are executing sequential single
instructions, it might loop like this:

```asm
x_fer:      str     w1, [x0], 1
            str     w1, [x0], 1
            str     w1, [x0], 1
            str     w1, [x0], 1
            str     w1, [x0], 1
            str     w1, [x0], 1
            str     w1, [x0], 1
            nop
            str     w1, [x0], 1
```

Here, the `nop` instruction means "no operation". It does nothing but
is a valid instruction meant to take up space (and decades ago, take
up time).

In a high level language this might look like this:

```c
for (int i = 0; i <= 8; i++) {
    if (i == 7)
        continue;
    blah blah
}
```
