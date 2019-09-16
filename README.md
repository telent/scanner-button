# scanner-button

I want to activate my scanner when someone presses the "Scan" button
on its front panel.  I don't want to set up scanbd and the scanb
"proxy" and saned and all that, I just want something to tell me when
the button is pressed.  This is that thing.

## Building

On Nix, you should be able to `nix-build .` in this
directory. Otherwise, there's a fairly simple Makefile.  Note that the
SANE backend libraries/header files need to be available.

## Usage

1. Run `scanimage -L` to find out what names the SANE backend has given to the "sensors" on your scanner that correspond to buttons.
2. Decide which of them you are interested in
3. Run `scanner-button button1 button2 ... buttonn`.  If any of them are pressed it will print the button name to standard output
4. Or to poll until one or more requested buttons are pressed, run `scanner-button --wait button1 button2 ... buttonn`.  As soon a it detects any of them are pressed it will print the button name(s) to standard output and then exit.

