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

When you run `tdraw`, your terminal will become blank and a blinking cursor will appear, which you can move around with the arrow keys.

You can color the space by pressing a key associated with a color. Those keys are:

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

To erase, press `x`.

To quit, press `q`.

### Save your drawings

If you provide `tdraw` with a file, it can save your drawing to that file. Next time you open the file with `tdraw` your drawing will be displayed!

To open a file with `tdraw`, simply do `tdraw filename`. To save, press `s`.

## Summary

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

    capitalizing to make the colors bright, and pressing `x` to erase.
3. Press `s` to save and `q` to quit.
