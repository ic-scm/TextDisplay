# TextDraw
Font creating utility for TextDisplay.

Dependencies:
- libpng and png++ for writing and reading PNG files.

## GenTemplate
Generates a PNG template image that you can edit in an image editor.

The first character is 32/0x20 (Space) in the ASCII table.

The green lines separate the characters and your characters should be drawn with red color (0xFF0000).

Remember that the last line of the character will always be empty so it's not included, and the last column should also be empty to keep a space between the characters.

## ToCHeader
Converts the font image to a C header file with the font.
