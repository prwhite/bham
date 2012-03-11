#**bham** - an Arduino library for Bresenham-based PWM control.

This library provides two layers.  The lower layer using the *bham* class for determining the PWM control for one output, and the *bhams* class for managing a set of PWM outputs.

The real advantage of this library over other forms of PWM is that it performs time division differently than bit-angle methods.

It's quite common with other techniques to end up with flickering when using intensities at .5 of the maximum value.  With this library, running an interrupt routine at 240hz or better, flickering should be imperceptible, even when only updating the PWM values at 30hz.

This is useful for a range of applications, but in my case I'm using it for PWM over SPI to control 8 solid state lasers!

## Example Usage

Runs 8 inputs to a form ready for sending to a shift register.

At global scope.

```
const uint8_t NumTracks = 8;
bhams< NumTracks > gBhams;
```

In 'setup'.

```
gBhams.init ();
```

In 'loop'.<br>
This only needs to be run at 30-60hz for smooth animation.

```
uint8_t values[ NumTracks ];

// Fill in values appropriately for each track.

gBhams.set ( values );
```

In interrupt routine.<br>
Interrupt rate is determined by application, but should probably be a minimum of 240hz.
I run this in the range of 4Khz with plenty of room left over for other processing.
Output is up to 8 bits, each bit indicating the current PWM HIGH/LOW value for a corresponding output line.

```
uint8_t out = gBhams.loop ();
```

## Unit Test

You can verify the general behavior the library on a Unix-like host (not on Arduino) by doing the following:

```
make test
```

Output should be:

> Files - and bham-test-out.txt are identical

## For fun

There is also a JavaScript file, *bham.js*, that contains a generally identical Bresenham algorithm.  This file will work with **node.js** out-of-the box.

This is provided to be informative if nothing else.

