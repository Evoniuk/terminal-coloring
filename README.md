# Terminal Coloring

Terminal coloring is a little program you can use to draw pictures in UNIX-like terminal emulators.

## Getting started

Clone this repo and make it:

```zsh
% git clone https://github.com/Evoniuk/terminal-coloring.git
% cd terminal-coloring
% make tdraw
```

This will make an executable called `tdraw`.

## How it works

When you run `tdraw`, your terminal will become blank and a blinking cursor will appear, which you can move around with the arrow keys. There will also be a sidebar describing what keys do what.

You can color the space your cursor is on by pressing a key associated with a color. Those keys are:

| Key   | Color   |
| ----- | ------- |
| `r`   | Red     |
| `g`   | Green   |
| `b`   | Blue    |
| `c`   | Cyan    |
| `m`   | Magenta |
| `y`   | Yellow  |
| `l`   | Black   |
| `w`   | White   |

A capital letter will make the color bright. For example, `R` will produce a bright red, and `C` a bright cyan.

To erase, press delete (backspace).

To quit, press `CTRL+Q`.

### Save your drawings

If you provide `tdraw` with a file, it can save your drawing to that file. Next time you open the file with `tdraw` your drawing will be displayed!

To open a file with `tdraw`, simply run `tdraw filename`. To save, press `CTRL+S`.

### Display without editing

If you just want to admire your drawing, you can run `tdraw -p filename`.

## Quick start

1. Start with `tdraw filename`.
2. Move the cursor with the arrow keys, placing colors with the following keys:

    | Key   | Color   |
    | ----- | ------- |
    | `r`   | Red     |
    | `g`   | Green   |
    | `b`   | Blue    |
    | `c`   | Cyan    |
    | `m`   | Magenta |
    | `y`   | Yellow  |
    | `l`   | Black   |
    | `w`   | White   |

    capitalizing to make the colors bright, and pressing delete (backspace) to erase.
3. Press `CTRL+S` to save and `CTRL+Q` to quit.
4. Admire your creation by running `tdraw -p filename`.
