# Terminal Color Usage in ChimpOS

This document explains how to use terminal colors in ChimpOS.

## Color Constants

The following color constants are available:

```c
VGA_COLOR_BLACK
VGA_COLOR_BLUE
VGA_COLOR_GREEN
VGA_COLOR_CYAN
VGA_COLOR_RED
VGA_COLOR_MAGENTA
VGA_COLOR_BROWN
VGA_COLOR_LIGHT_GREY
VGA_COLOR_DARK_GREY
VGA_COLOR_LIGHT_BLUE
VGA_COLOR_LIGHT_GREEN
VGA_COLOR_LIGHT_CYAN
VGA_COLOR_LIGHT_RED
VGA_COLOR_LIGHT_MAGENTA
VGA_COLOR_LIGHT_BROWN
VGA_COLOR_WHITE
```

## Methods to Apply Colors

### 1. Using Direct Color Functions

These functions let you specify colors explicitly when printing:

```c
// Print a single character with specified colors
terminal_putchar_colored('A', VGA_COLOR_RED, VGA_COLOR_BLACK);

// Print a string with specified colors
terminal_writestring_colored("Error message", VGA_COLOR_RED, VGA_COLOR_BLACK);

// Print a portion of a string with specified colors
terminal_write_colored(buffer, length, VGA_COLOR_GREEN, VGA_COLOR_BLACK);
```

### 2. Setting Terminal Colors

You can set the current terminal color and then print normally:

```c
// Set the full color (foreground and background)
terminal_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK));

// Set just the foreground color
terminal_set_fg_color(VGA_COLOR_CYAN);

// Set just the background color
terminal_set_bg_color(VGA_COLOR_DARK_GREY);

// Print with current color
terminal_writestring("This text uses the current terminal color");
```

### 3. Using ANSI Escape Sequences

You can embed color codes directly in your strings:

```c
// Red text
terminal_writestring("\033[31mThis text is red\033[0m");

// Blue text on yellow background
terminal_writestring("\033[34;43mBlue on yellow\033[0m");

// Bold/bright green text
terminal_writestring("\033[92mBright green text\033[0m");
```

## ANSI Color Code Reference

### Foreground Colors
- `\033[30m` - Black
- `\033[31m` - Red
- `\033[32m` - Green
- `\033[33m` - Yellow/Brown
- `\033[34m` - Blue
- `\033[35m` - Magenta
- `\033[36m` - Cyan
- `\033[37m` - Light Grey/White

### Bright Foreground Colors
- `\033[90m` - Dark Grey
- `\033[91m` - Light Red
- `\033[92m` - Light Green
- `\033[93m` - Light Yellow
- `\033[94m` - Light Blue
- `\033[95m` - Light Magenta
- `\033[96m` - Light Cyan
- `\033[97m` - White

### Background Colors
- `\033[40m` - Black
- `\033[41m` - Red
- `\033[42m` - Green
- `\033[43m` - Yellow/Brown
- `\033[44m` - Blue
- `\033[45m` - Magenta
- `\033[46m` - Cyan
- `\033[47m` - Light Grey

### Bright Background Colors
- `\033[100m` - Dark Grey
- `\033[101m` - Light Red
- `\033[102m` - Light Green
- `\033[103m` - Light Yellow
- `\033[104m` - Light Blue
- `\033[105m` - Light Magenta
- `\033[106m` - Light Cyan
- `\033[107m` - White

### Other Control Codes
- `\033[0m` - Reset all colors and styles
- `\033[1m` - Bold/Bright (not directly supported in VGA, but we map to bright colors)

## Examples

```c
// Print a colorful error message
terminal_writestring("ERROR: ");
terminal_writestring_colored("Something went wrong!", VGA_COLOR_RED, VGA_COLOR_BLACK);

// Print a warning with mixed colors in one string
terminal_writestring("\033[33mWARNING:\033[0m \033[97mThis is important\033[0m");

// Print a system message
terminal_set_fg_color(VGA_COLOR_LIGHT_CYAN);
terminal_writestring("System initialized\n");
terminal_set_fg_color(VGA_COLOR_LIGHT_GREY);  // Reset to default

// Highlight a specific character
terminal_writestring("Your score: ");
terminal_putchar_colored('A', VGA_COLOR_GREEN, VGA_COLOR_BLACK);
terminal_writestring("+");
```
