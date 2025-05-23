Description of the functions provided by MicroLua as of version 4.8.


[TOC]


The variables or functions that you will find here may be defined either in the Lua libraries (_libs_ tag) or in the C code (_nds_ tag).  
Some are also tagged as _deprecated_, or as _unused_ because you are not meant to access them directly.


Variables
=========

* `ULUA_VERSION` (_nds_): version of Micro Lua as a String; usually of the form _MicroLua X.X.X_
* `SCREEN_WIDTH`, `SCREEN_HEIGHT` (_nds_): width and height of the screens
* `NB_FPS` (_libs_): number of frame per second automatically updated every second
* _SCREEN_UP_, _SCREEN_DOWN_ (_nds_): screen number where to draw; see examples for more details
* `RAM` and `VRAM` (_nds_): destination where to load some object; see examples for more details
* `ATTR_X1`, `ATTR_Y1`, `ATTR_X2`, `ATTR_Y2`, `ATTR_X3`, `ATTR_Y3`, `ATTR_COLOR`, `ATTR_COLOR1`, `ATTR_COLOR2`, `ATTR_COLOR3`, `ATTR_COLOR4`, `ATTR_TEXT`, `ATTR_VISIBLE`, `ATTR_FONT`, `ATTR_IMAGE` (_nds_): attributes used in canvas
* `ALPHA_RESET` (_nds_): blending coefficient value to reset alpha transparency; is equal to 100
* `PLAY_LOOP`, `PLAY_ONCE` (_nds_): arguments to play mods respectively loop and once
* `LED_ON`, `LED_BLINK`, `LED_SLEEP` (_nds_): blink modes for the LED (always ON, blinking fast, blinking slow)
* Path constants (some of them are made up from precedent ones) (_nds_):
    * `ULUA_DIR`: _/lua/_ (main directory of µLua)
    * `ULUA_SCRIPTS`: _ULUA_DIR scripts/_ (default folder for scripts)
    * `ULUA_LIBS`: _ULUA_DIR libs/_ (default folder for libraries)
    * `ULUA_BOOT_FILE`: _boot.lua_ (first loaded file)
    * `ULUA_BOOT_FULLPATH`: _ULUA_DIR ULUA_BOOTFILE_ (full path of the first loaded file)
* `mustCountFPS` (_libs_, _unused_): regulate the FPS couting system; it is an internal variable, you should never have to deal with it
* `Debug.isDebugOn`, `Debug.debugText` and `Debug.debugColor` (_nds_, _unused_): variables used by the debug system; you should only modify them by using the _Debug_ functions

Functions and their tables
==========================

Screen
------

**`Void render()`** (_libs_)
Refresh the screen.

**`Void startDrawing()`**, **`Void stopDrawing()`** (_libs_, _deprecated_)
All your drawing instructions must be between these two functions.

**`Void screen.switch()`** (_nds_)
Switch the screens.

**`Number screen.getLayer()`** (_nds_)
Get the current layer.

**`Number screen.getAlphaLevel()`** (_nds_)
Get the current transparency level.

**`Void screen.setAlpha(level[, layer])`** (_nds_)
Set the transparency level for the given layer.

* `level` (Number): blending coefficient, between 0 and 99 (100, that is to say _ALPHA_RESET_, reset all the transparency system)
* `layer` (Number): layer to set; the function however automatically keeps the count so you don't have to give and layer

**`Void screen.print(screen, x, y, text [, color])`** (_nds_)
Print a text on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x` (Number): x-coordinate where to draw
* `y` (Number): y-coordinate where to draw
* `text` (String): text to print
* `color` (Color): color of the text

**`Void screen.printFont(screen, x, y, text , color, font)`** (_nds_)
Print a text on the screen using a special font.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x` (Number): x-coordinate where to draw
* `y` (Number): y-coordinate where to draw
* `text` (String): text to print
* `color` (Color): color of the text
* `font` (Font): special font

**`Void screen.blit(screen, x, y, image, [, sourcex, sourcey] [, width, height])`** (_nds_)
Blit an image on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x` (Number): x-coordinate where to draw
* `y` (Number): y-coordinate where to draw
* `image` (Image): image to blit
* `sourcex`, `sourcey` (Number): coordinates in the source image to draw
* `width`, `height` (Number): width and height of the rectangle to draw

**`Void screen.drawPoint(screen, x, y, color)`** (_nds_)
Draw a point on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x` (Number): x-coordinate where to draw
* `y` (Number): y-coordinate where to draw
* `color` (Color): color of the point

**`Void screen.drawLine(screen, x0, y0, x1, y1, color)`** (_nds_)
Draw a line on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1` (Number): coordinates of the line
* `color` (Color): color of the line

**`Void screen.drawRect(screen, x0, y0, x1, y1, color)`** (_nds_)
Draw a rectangle on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1` (Number): coordinates of the rectangle
* `color` (Color): color of the rectangle

**`Void screen.drawFillRect(screen, x0, y0, x1, y1, color)`** (_nds_)
Draw a fill rectangle on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1` (Number): coordinates of the rectangle
* `color` (Color): color of the rectangle

**`Void screen.drawGradientRect(screen, x0, y0, x1, y1, color1, color2, color3, color4)`** (_nds_)
Draw a gradient rectangle on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1` (Number): coordinates of the rectangle
* `color1`, `color2`, `color3`, `color4` (Color): colors of the rectangle

**`Void screen.drawTextBox(screen, x0, y0, x1, y1, text [, color])`** (_nds_)
Draw a text box on the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1` (Number): coordinates of the text box
* `text` (String): text to print
* `color` (Color): color of the text box

**`Void screen.drawTexturedQuad(screen, x0, y0, x1, y1, x2, y2, x3, y3, texture[, sourceX, sourceY][, width, height])`** (_nds_)
Display a textured quadrilateral.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1`, `x2`, `y2`, `x3`, `y3` (Number): coordinates of the quadrilateral angles
* `texture` (Image): texture image
* `sourcex`, `sourcey` (Number): coordinates in the texture image to draw
* `width`, `height` (Number): width and height of the texture image piece to draw

**`Void screen.drawTexturedTriangle(screen, x0, y0, x1, y1, x2, y2, texture[, sourceX, sourceY][, width, height])`** (_nds_)
Display a textured triangle.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x0`, `y0`, `x1`, `y1`, `x2`, `y2` (Number): coordinates of the triangle angles
* `texture` (Image): texture image
* `sourcex`, `sourcey` (Number): coordinates in the texture image to draw
* `width`, `height` (Number): width and height of the texture image piece to draw

**`Boolean screen.getMainLcd()`** (_nds_)
Give the screen the µLibrary is drawing on (_true_ for upper screen, _false_ for the lower screen).

**`Void screen.setSpaceBetweenScreens(space)`** (_nds_, _deprecated_ (you can no longer draw on the two screens at the same time))
Set the space between the screens (so some drawing will not be displayed if drawn in this space).

* `space` (Number): space (in pixels) between the screens

**`Void screen.init()`** (_nds_, _unused_)
Initialise some stuff about the screens. It is done automatically, so you should have nothing to care about this.

**`Void screen.startDrawing2D()`** (_nds_, _unused_)
Start a new drawing and configure 2D. You should have nothing to care about this.

**`Void screen.endDrawing()`** (_nds_, _unused_)
End drawing. You will never need to use it (use `render()` to command screens actualisation).

**`Void screen.waitForVBL()`** (_nds_, _unused_)
Wait for the VBlank period to reach 60FPS. You will never need to use it (using `render()` will synchronise the frames).

Colors
------

**`Color Color.new(r, g, b)`** (_nds_)
Create a new color.

* `r` (Number): red mask (from 0 to 31)
* `g` (Number): green mask (from 0 to 31)
* `b` (Number): blue mask (from 0 to 31)

**`Color Color.new256(r, g, b)`** (_nds_)
Create a new color from 8bit values.

* `r` (Number): red mask (from 0 to 255)
* `g` (Number): green mask (from 0 to 255)
* `b` (Number): blue mask (from 0 to 255)

Controls
--------

**`Void Controls.read()`** (_nds_)
Read the controls and update all control structures.

**`Void Controls.setStylusDblcFreq(dblcFreq)`** (_nds_)
Set the maximum delay between two Stylus clicks to make it a double click.

* `dblcFreq` (Number): maximum delay (from 0 to 30, 30 being half a second and the default value)

### Stylus ###

These are numbers or booleans. Everything here is defined in the _nds_.

* `Stylus.X`: x-coordinate of the stylus
* `Stylus.Y`: y-coordinate of the stylus
* `Stylus.held`: is the stylus held on the screen
* `Stylus.released`: is the stylus released
* `Stylus.doubleClick`: _true_ if stylus do a double click
* `Stylus.deltaX`: delta x of the stylus movement
* `Stylus.deltaY`: delta y of the stylus movement
* `Stylus.newPress`: _true_ if stylus do a new press

### Keys ###

These are booleans. Everything here is defined in the _nds_.

`[KEY]` can take one of the following values:

* `A`
* `B`
* `X`
* `Y`
* `L`
* `R`
* `Start`
* `Select`
* `Up`
* `Down`
* `Left`
* `Right`
* `Lid`

* `Keys.held.[KEY]`: is the key held?
* `Keys.released.[KEY]`: is the key released?
* `Keys.newPress.[KEY]`: _true_ if the key does a new press

Images
------

**`Image Image.load(path, destination)`** (_nds_)
Create a new image in memory from an image file (PNG, JPG or GIF).

* `path` (String): path of the image to load
* `destination` (Number): destination of the image in memory (can be RAM of VRAM)

**`Void Image.destroy(image)`** (_nds_)
Destroy the image.

* `image` (Image): image to destroy

**`Number Image.width(image)`** (_nds_)
Get the width of the image.

* `image` (Image): image to use

**`Number Image.height(image)`** (_nds_)
Get the height of the image.

* `image` (Image): image to use

**`Void Image.scale(image, width, height)`** (_nds_)
Scale the image.

* `image` (Image): image to scale
* `width` (Number): new width of the image
* `height` (Number): new height of the image

**`Void Image.rotate(image, angle [, centerx, centery])`** (_nds_)
Rotate the image around rotation center.

* `image` (Image): image to rotate
* `angle` (Number): angle of roattion (between 0 and 511)
* `centerx` (Number): x-coordinate of the new rotation center
* `centery` (Number): y-coordinate of the new rotation center

**`Void Image.rotateDegree(image, angle [, centerx, centery])`** (_nds_)
Rotate the image around rotation center, the angle is given in degree.

* `image` (Image): image to rotate
* `angle` (Number): angle of rotation in degree (between 0 and 360)
* `centerx` (Number): x-coordinate of the new rotation center
* `centery` (Number): y-coordinate of the new rotation center

**`Void Image.mirrorH(image, activate)`** (_nds_)
Mirror the image horizontally.

* `image` (Image): image to mirror
* `activate` (Boolean): _true_ to activate the mirroring, _false_ to cancel the transformation

**`Void Image.mirrorV(image, activate)`** (_nds_)
Mirror the image vertically.

* `image` (Image): image to mirror
* `activate` (Boolean): _true_ to activate the mirroring, _false_ to cancel the transformation

**`Void Image.setTint(image, color)`** (_nds_)
Set the tint of the image.

* `image` (Image): image to tint
* `color` (Color): color of the image

Timers
------

The unit is the millisecond.

**`Timer Timer.new()`** (_libs_)
Create a new timer, you can start it.

**`Number timer:time()`** (_libs_)
Return the time of the timer.

**`Void timer:start()`** (_libs_)
Start a timer.

**`Void timer:stop()`** (_libs_)
Stop a timer.

**`Void timer:reset()`** (_libs_)
Reset a timer.

Sprites
-------

**`Sprite Sprite.new(path, frameWidth, frameHeight, dest)`** (_libs_)
Create a sprite from an image file.

* `graph` (String): path to an Image or loaded Image that contain the sprite's frames
* `frameWidth` (Number): width of the frames
* `frameHeight` (Number): height of the frames
* `dest` (Number): destination (RAM or VRAM)

**`Void sprite:drawFrame(screen, x, y, nbFrame)`** (_libs_)
Draw a frame of the sprite.

* `screen` (Constant / Number): screen (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x` (Number): X-coordinate where to draw the frame
* `y` (Number): Y-coordinate where to draw the frame
* `nbFrame` (Number): number of the frame to draw

**`Void sprite:addAnimation(tabAnim, delay)`** (_libs_)
Create an animation.

* `tabAnim` (Table): the table of the animation frames
* `delay` (Number): delay between each frame

**`Void sprite:playAnimation(screen, x, y, nbAnim)`** (_libs_)
Play an animation on the screen.

* `screen` (Constant / Number): screen (_SCREEN_UP_ or _SCREEN_DOWN_)
* `x` (Number): X-coordinate where to draw the frame
* `y` (Number): Y-coordinate where to draw the frame
* `nbAnim` (Number): number of the animation to play

**`Void sprite:resetAnimation(nbAnim)`** (_libs_)
Reset an animation.

* `nbAnim` (Number): number of the animation

**`Void sprite:startAnimation(nbAnim)`** (_libs_)
Start an animation.

* `nbAnim` (Number): number of the animation

**`Void sprite:stopAnimation(nbAnim)`** (_libs_)
Stop an animation.

* `nbAnim` (Number): number of the animation

**`Boolean sprite:isAnimationAtEnd(nbAnim)`** (_libs_)
Return _true_ if the animation has drawn the last frame.

* `nbAnim` (Number): number of the animation

**`Table SpriteAnimation.new(tabAnim, delay)`** (_libs_, _unused_)
Create a new sprite animation. You don't have to use it, use `sprite:addAnimation()` instead.

* `tabAnim` (Table): table containing the animation frames
* `delay` (Number): delay between each frame

Debug
-----

**`Void Debug.ON()`** (_libs_)
Activate the debug mode.

**`Void Debug.OFF()`** (_libs_)
Deactivate the debug mode.

**`Void Debug.print(text)`** (_libs_)
Print a debug line.

* `text` (String): text to print

**`Void Debug.clear()`** (_libs_)
Clear the debug console.

**`Void Debug.setColor(color)`** (_libs_)
Set the debug text color.

* `color` (Color): color of the text

System
------

**`String System.currentDirectory()`** (_nds_)
Get the current working directory.

**`Void System.changeDirectory(path)`** (_nds_)
Change the current working directory.

* `path` (String): path of the directory

**`Void System.remove(name)`** (_nds_)
Remove a file or an empty folder.

* `name` (String): name of the file or directory to remove

**`Void System.rename(oldName, newName)`** (_nds_)
Rename file or an empty folder.

* `oldName` (String): name of the file or directory to rename
* `newName` (String): new name of the file or directory

**`Void System.makeDirectory(name)`** (_nds_)
Create a new directory.

* `name` (String): path and name of the directory

**`Table System.listDirectory(path)`** (_nds_)
List all files and folders of a directory.

* `path` (String): path of the directory to list

_NOTE_: See examples for more details.
  
**`Number System.CurrentVramUsed()`** (_nds_)
Give the current amount of used VRAM (seems to return some weird number).

**`Number System.CurrentVramFree()`** (_nds_)
Give the current amount of free VRAM.

**`Number System.CurrentPalUsed()`** (_nds_)
Give the current amount of used palette memory (seems to return some weird number).

**`Number System.CurrentPalFree()`** (_nds_)
Give the current amount of free palette memory.

**`Void System.setLedBlinkMode(mode)`** (_nds_)
Set the blink mode for the LED.

* `mode` (Number): the blink mode; choose between LED_ON, LED_BLINK and LED_SLEEP

**`Void System.shutDown()`** (_nds_)
Shutdown the console.

**`Void System.sleep()`** (_nds_)
Put the system in sleep state.

Fonts
-----

**`Font Font.load(path)`** (_nds_)
Create a new font from a font file (oslib and µLibrary format).

* `path` (String): path of the file to load

**`Void Font.destroy(font)`** (_nds_)
Destroy the font.

* `font` (Font): font to destroy

**`Number Font.getCharHeight(font)`** (_nds_)
Get the height of the characters of a font.

* `font` (Font): font to use

**`Number getStringWidth(font, text)`** (_nds_)
Get the width of a text with a specific font.

* `font` (Font): font to use
* `text` (String): text

Maps
----

**`Map Map.new(image, mapfile, width, height, tileWidth, tileHeight)`** (_nds_)
Create a new map by giving a map file.

* `image` (Image): image which contains tiles
* `mapfile` (String): path to the map file (`.map`)
* `width` (Number): width of the map in tiles
* `height` (Number): height of the map in tiles
* `tileWidth` (Number): width of the tiles in pixels
* `tileHeight` (Number): height of the tiles in pixels

**`Void Map.destroy(map)`** (_nds_)
Destroy a map.

* `map` (Map): map to destroy

**`Void Map.draw(screen, map, x, y, width, height)`** (_nds_)
Draw a map.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `map` (Map): map to destroy
* `x` (Number): x-coordinate where to draw the map
* `y` (Number): y-coordinate where to draw the map
* `width` (Number): x number of tiles to draw
* `height` (Number): y number of tiles to draw

**`Void Map.scroll(map, x, y)`** (_nds_)
Scroll a map.

* `map` (Map): map to scroll
* `x` (Number): x number of tiles to scroll
* `y` (Number): y number of tiles to scroll

**`Void Map.space(map, x, y)`** (_nds_)
Set the space between each tiles of a map.

* `map` (Map): map to modify
* `x` (Number): x space between tiles
* `y` (Number): y space between tiles

**`Void Map.setTile(map, x, y, tile)`** (_nds_)
Change a tile value.

* `map` (Map): map to modify
* `x` (Number): x-coordinate of the tile to change in the map table
* `y` (Number): y-coordinate of the tile to change in the map table
* `tile` (Number): new tile value

**`Number Map.getTile(map, x, y)`** (_nds_)
Get a tile value.

* `map` (Map): map to read
* `x` (Number): x-coordinate of the tile to get
* `y` (Number): y-coordinate of the tile to get

Scroll Maps
-----------

**`ScrollMap ScrollMap.new(image, mapfile, width, height, tileWidth, tileHeight)`** (_nds_)
Create a new ScrollMap by giving a map file.

* `image` (Image): image which contains tiles
* `mapfile` (String): path to the map file (_.map_)
* `width` (Number): width of the map in tiles
* `height` (Number): height of the map in tiles
* `tileWidth` (Number): width of the tiles in pixels
* `tileHeight` (Number): height of the tiles in pixels

**`Void ScrollMap.destroy(scrollmap)`** (_nds_)
Destroy a ScrollMap.

* `scrollmap` (ScrollMap): ScrollMap to destroy

**`Void ScrollMap.draw(screen, scrollmap)`** (_nds_)
Draw a ScrollMap.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `scrollmap` (ScrollMap): ScrollMap to draw

**`Void ScrollMap.scroll(scrollmap, x, y)`** (_nds_)
Scroll a ScrollMap.

* `scrollmap` (ScrollMap): ScrollMap to scroll
* `x` (Number): x-coordinate of the upper-left corner of the map (can be a negative value)
* `y` (Number): y-coordinate of the upper-left corner of the map (can be a negative value)

**`Void ScrollMap.setTile(scrollmap, x, y, tile)`** (_nds_)
Change a tile value.

  * `scrollmap` (ScrollMap): ScrollMap to modify
  * `x` (Number): x-coordinate of the tile to change in the scrollmap table
  * `y` (Number): y-coordinate of the tile to change in the scrollmap table
  * `tile` (Number): new tile value

**`Number ScrollMap.getTile(scrollmap, x, y)`** (_nds_)
Get a tile value.

* `scrollmap` (ScrollMap): ScrollMap to read
* `x` (Number): x-coordinate of the tile to get
* `y` (Number): y-coordinate of the tile to get

Canvas
------

**`Canvas Canvas.new()`** (_nds_)
Create a new canvas.

**`Void Canvas.destroy(canvas)`** (_nds_)
Destroy a canvas.

* `canvas` (Canvas): canvas to destroy

**`CanvasObject Canvas.newLine(x1, y1, x2, y2, color)`** (_nds_)
Create a new line.

* `x1`, `y1`, `x2`, `y2` (Number): coordinates of the line
* `color` (Color): color of the line

**`CanvasObject Canvas.newPoint(x1, y1, color)`** (_nds_)
Create a new point.

* `x1`, `y1` (Number): coordinates of the point
* `color` (Color): color of the point

**`CanvasObject Canvas.newRect(x1, y1, x2, y2, color)`** (_nds_)
Create a new rectangle.

* `x1`, `y1`, `x2`, `y2` (Number): coordinates of the rectangle
* `color` (Color): color of the rectangle

**`CanvasObject Canvas.newFillRect(x1, y1, x2, y2, color)`** (_nds_)
Create a new fill rectangle.

* `x1`, `y1`, `x2`, `y2` (Number): coordinates of the rectangle
* `color` (Color): color of the rectangle

**`CanvasObject Canvas.newGradientRect(x1, y1, x2, y2, color1, color2, color3, color4)`** (_nds_)
Create a new gradient rectangle.

* `x1`, `y1`, `x2`, `y2` (Number): coordinates of the rectangle
* `color1`, `color2`, `color3`, `color4` (Color): colors of the fill rectangle

**`CanvasObject Canvas.newText(x1, y1, text [, color])`** (_nds_)
Create a new text.

* `x1`, `y1` (Number): coordinates of the text
* `text` (String): text
* `color` (Color): color of the text

**`CanvasObject Canvas.newTextFont(x1, y1, text, color, font)`** (_nds_)
Create a new text with a special font.

* `x1`, `y1` (Number): coordinates of the text
* `text` (String): text
* `color` (Color): color of the text
* `font` (Font): special font of the text

**`CanvasObject Canvas.newTextBox(x1, y1, x2, y2, text [, color])`** (_nds_)
Create a new textbox.

* `x1`, `y1`, `x2`, `y2` (Number): coordinates of the textbox
* `text` (String): text
* `color` (Color): color of the textbox

**`CanvasObject Canvas.newImage(x1, y1, image [, x2, y2] [, x3, y3])`** (_nds_)
Create a new image.

* `x1`, `y1` (Number): coordinates of the image
* `x2`, `y2` (Number): coordinates in the source image to draw
* `x3`, `y3` (Number): width and height of the rectangle to draw

**`Void Canvas.add(canvas, object)`** (_nds_)
Add a CanvasObject in a canvas.

* `canvas` (Canvas): canvas to draw
* `object` (CanvasObject): object to add

**`Void Canvas.draw(screen, canvas, x, y)`** (_nds_)
Draw a canvas to the screen.

* `screen` (Constant / Number): screen where to draw (_SCREEN_UP_ or _SCREEN_DOWN_)
* `canvas` (Canvas): canvas to draw
* `x` (Number): x-coordinate where to draw
* `y` (Number): y-coordinate where to draw

**`Void Canvas.setAttr(object, attrName, attrValue)`** (_nds_)
Set an attribute value.

* `object` (CanvasObject): object to modify
* `attName` (Constant / Number): attribute to modify; must be `ATTR_XXX`. See _Variables_ for more details
* `attrValue` (?): new value for the attribute; must be the good type

**`? Canvas.getAttr(object, attrName)`** (_nds_)
Get an attribute value. Return type depends of the attribute (number, font or image).

* `object` (CanvasObject): object to use
* `attName` (Constant / Number): attribute to get value. Must be `ATTR_XXX`. See _Variables_ for more details
  
**`Void Canvas.setObjOnTop(canvas, object)`** (_nds_)
Make a CanvasObject on the first plan of a Canvas.

* `canvas` (Canvas): Canvas in which we work.
* `object` (CanvasObject): Object that we want to set on Top.
  
**`Void Canvas.removeObj(canvas, object)`** (_nds_)
Delete an CanvasObject in a Canvas.

* `canvas` (Canvas): Canvas in which we want to delete an Object.
* `object` (CanvasObject): Object that we want to delete.
  
Rumble
------

**`Boolean Rumble.isInserted()`** (_nds_)
Check if a rumble pack is inserted.

**`Void Rumble.set(status)`** (_nds_)
Set the rumble status.

* `status` (Boolean): status of the rumble (_true_: ON, _false_: OFF)

Motion
------

**`Boolean Motion.init`** (_nds_)
Init the motion system if a motion device is detected. Return _true_ if a motion device is detected.

**`Void Motion.calibrate`** (_nds_)
Calibrate the motion system.

**`Number Motion.readX`** (_nds_)
Read the X tilt of the motion.

**`Number Motion.readY`** (_nds_)
Read the Y tilt of the motion.

**`Number Motion.readZ`** (_nds_)
Read the Z tilt of the motion.

**`Number Motion.accelerationX`** (_nds_)
Read the X acceleration of the motion.

**`Number Motion.accelerationY`** (_nds_)
Read the Y acceleration of the motion.

**`Number Motion.accelerationZ`** (_nds_)
Read the Z acceleration of the motion.

**`Number Motion.readGyro`** (_nds_)
Read the gyro value of the motion.

**`Number Motion.rotation`** (_nds_)
Read the rotation value of the motion.

DateTime
--------

**`DateTime DateTime.new()`** (_libs_)
Create a new DateTime object.

**`DateTime DateTime.getCurrentTime()`** (_libs_)
Create a new DateTime object with current time values.

_Attributes_ (Numbers):

* `year`
* `month`
* `day`
* `hour`
* `minute`
* `second`

_NOTE_: see examples for more details.

Wifi
----

**`Void Wifi.connectWFC()`** (_nds_)
Connect the DS to the Wifi connection. Use the firmware configuration. So, you need to configure your connection with an official DS game.

**`Void Wifi.initDefault()`** (_nds_)
Init the DS Wifi system. Must be done before all Wifi functions except Wifi.connectWFC(). Don't use with Wifi.connectWFC().

**`Void Wifi.autoConnectWFC()`** (_nds_)
Same as Wifi.connectWFC() but used after Wifi.initDefault().

**`Void Wifi.scanAP()`** (_nds_)
Set the DS in scan mode for AP detection (Access Points such as Livebox, Freebox, Neufbox...). When starting, the DS searches all the time but doesn't warn for a found connection.

**`Number Wifi.getNumAP()`** (_nds_)
Return the number of AP found after a Wifi.scanAP(). The search may be long.

**`Tableau Wifi.newAP()`** (_nds_)
Return an empty table with AP data. There are:

* `ssid` (String): name of the network's AP
* `bssid` (String):
* `macaddr` (String): MAC address of the AP
* `channel` (Number): network channel number
* `rssi` (Number): signal strength
* `maxrate` (Number): communication speed (1M or 2M)
* `protection` (String): Wifi protection (_NONE_, _WEP_, _WPA_)
* `adhoc` (Boolean): is the connection adhoc (_true_) or not (_false_)
* `active` (Boolean): is the connection active (_true_) or not (_false_)

**`Tableau Wifi.getAP(numap)`** (_nds_) 
Return data about the AP with the number `numap` found by `Wifi.scanAP()`.

* `numap` (Number): number of the AP.

**`Number Wifi.connectAP(numap, numkey, key)`** (_nds_)
Make a association request to the AP of index `numap` with the key number `numkey` and the WEP key `key`.
If you intend to associate with an unprotected AP set `numkey` to 1 and `key` to an empty String (_""_).
`numkey` must be between 1 and 4, but the first key is more often used.
Return -1 if the association is refused.

* `numap` (Number): AP number
* `numkey` (Number): key number
* `key` (String): WEP key.

**`Void Wifi.disconnect()`** (_nds_)
Disconnect the DS from the Wifi connection.

**`Void Wifi.stop()`** (_nds_)
Disengage the Wifi.

**`String Wifi.getLocalConf(choice)`** (_nds_)
Return data of the DS connection.

* `choice` (Number): 
    * _1_: console's IP.
    * _2_: gateway.
    * _3_: subnet mask.
    * _4_: first DNS.
    * _5_: second DNS.
    * _6_: MAC address of the console

**`Void Wifi.resetIP()`** (_nds_)
Reset the IP, GateWay, SubnetMask and DNS data.
If called before association, the DS queries the DHCP server.

**`Void Wifi.setLocalIp(ip, gateway[, subnetmask[, dns1[, dns2]]])`** (_nds_)
Set the IP, gateWay, subnetMask and DNS. The strings must be formated like "N1.N2.N3.N4" where N is a number between 0 to 255.

* `ip` (String): IP address.
* `gateway` (String): gateway address.
* `subnetmask` (String): subnet mask.
* `dns1` (String): first DNS.
* `dns2` (String): second DNS.

**`Socket Wifi.createTCPSocket(host, port)`** (_nds_)
Create a TCP socket on a server.

* `host` (String): hostname or IP adress of the server
* `port` (Number): port to use

**`Socket Wifi.createUDPSocket(host, port)`** (_nds_)
Create an UDP socket on a server.

* `host` (String): hostname or IP adress of the server
* `port` (Number): port to use

**`Void Wifi.closeSocket(socket)`** (_nds_)
Close a socket (TCP or UDP).

* `socket` (Socket): socket to close

**`Void Wifi.send(socket, buffer)`** (_nds_)
Send data to a server using a socket.

* `socket` (Socket): Socket to use
* `buffer` (String): data to send

**`Number Wifi.checkData(socket)`** (_nds_)
Check if there are data in the Wifi buffer and Return the number of character to read.

* `socket` (Socket): socket to verify.

**`String Wifi.receive(socket, length)`** (_nds_)
Receive data from a server using a socket.

* `socket` (Socket): socket to use
* `length` (Number): size of the data to receive

Nifi
----

**`Boolean Nifi.init(channel)`** (_nds_)  
Init the Wifi in Nifi mode on a channel and return true if success, false otherwise.
  
* `channel` (Number): the wifi channel where to discuss (must be between 1 and 13).  
  
**`Void Nifi.changeChannel(channel)`** (_nds_)  
Change the channel where the Nifi discuss.

* `channel` (Number): The new channel (must be between 1 and 13).

**`Void Nifi.stop()`** (_nds_)  
Stop the nifi.  

**`Boolean Nifi.checkMessage()`** (_nds_)  
Check if a new message is arrived and return true if there is one and false otherwise.  

**`String Nifi.getTrame()`** (_nds_)  
Get the last trame receive by Nifi even if it's not address to the NDS. Its max size is 4096 bytes.  

**`Void Nifi.sendMessage(message)`** (_nds_)  
Send a message on the channel.

* `message` (String): Message to send. The max size of the message is 2048 bytes.  

**`String Nifi.readMessage()`** (_nds_)  
Read the last message received.  

Sound
-----

### Sound banks ###

**`Void Sound.loadBank(filename)`** (_nds_)
Load a soundbank from a file in memory.

* `filename` (String): path of the file to load

**`Void Sound.unloadBank()`** (_nds_)
Unload the sound bank from the memory.

### Mods ###

**`Void Sound.loadMod(id)`** (_nds_)
Load a module in memory.

* `id` (Number): id of the module to load

**`Void Sound.unloadMod(id)`** (_nds_)
Unload a module from the memory.

* `id` (Number): id of the module to unload

**`Void Sound.startMod(id, playmode)`** (_nds_)
Start playing a module already loaded in memory.

* `id` (Number): id of the module to play
* `playmode` (Number): playing mode (`PLAY_ONCE` or `PLAY_LOOP`)

**`Void Sound.pause()`** (_nds_)
Pause all modules.

**`Void Sound.resume()`** (_nds_)
Resume all modules.

**`Void Sound.stop()`** (_nds_)
Stop all modules.

**`Void Sound.setPosition(id, position)`** (_nds_)
Set the cursor position of a module.

* `id` (Number): id of the module

**`Boolean Sound.isActive()`** (_nds_)
Return _true_ if the player is active and _false_ otherwise.

**`Void Sound.startJingle(id)`** (_nds_)
Start playing a module as a jingle.

* `id` (Number): id of the module to play

**`Void Sound.setModVolume(volume)`** (_nds_)
Set the volume of the played module.

* `volume` (Number): new volume value between 0 and 1024

**`Void Sound.setJingleVolume(volume)`** (_nds_)
Set the volume of the played jingle.

* `volume` (Number): new volume value between 0 and 1024

**`Void Sound.setModTempo(tempo)`** (_nds_)
Set the tempo of the module player.

* `tempo` (Number): new tempo value between 512 and 2048

**`Void Sound.setModPitch(pitch)`** (_nds_)
Set the pitch of the module player.

* `pitch` (Number): new pitch value

### SFX ###

**`Void Sound.loadSFX(id)`** (_nds_)
Load a SFX in the meory.

* `id` (Number): id of the SFX to load

**`Void Sound.unloadSFX(id)`** (_nds_)
Unload a SFX in the meory.

* `id` (Number): id of the SFX to unload

**`Handle Sound.startSFX(id)`** (_nds_)
Start a sound effect already loaded in memory. Return and handle to this SFX.

* `id` (Number): id of the SFX to start

**`Void Sound.stopSFX(handle)`** (_nds_)
Stop a played SFX.

* `handle` (Handle): handle of a SFX, given by the startSFX function

**`Void Sound.releaseSFX(handle)`** (_nds_)
Mark an effect as low priority.

* `handle` (Handle): handle of a SFX, given by the startSFX function

**`Void Sound.stopAllSFX()`** (_nds_)
Stop all payed SFX.

**`Void Sound.setSFXVolume(handle, volume)`** (_nds_)
Set the volume of a playing SFX.

* `handle` (Handle): handle of a SFX, given by the startSFX function
* `volume` (Number): new volume value between 0 and 255 (different from Mods)

**`Void Sound.setSFXPanning(handle, panning)`** (_nds_)
Set the panning of a playing SFX.

* `handle` (Handle): handle of a SFX, given by the startSFX function
* `panning` (Number): new panning value between 0 (left) and 255 (right)

**`Void Sound.setSFXPitch(handle, pitch)`** (_nds_)
Set the pitch of a playing SFX.

* `handle` (Handle): handle of a SFX, given by the startSFX function
* `pitch` (Number): new pitch value

**`Void Sound.setSFXScalePitch(handle, scale)`** (_nds_)
Set the scaling pitch ratio of a playing SFX.

* `handle` (Handle): handle of a SFX, given by the startSFX function
* `scale` (Number): new scale pitch value

INI Files
---------

**`Table INI.load(filename)`** (_libs_)
Load an INI file and create a table with it.

* `filename` (String): file to load

**`Void INI.save(filename, tab)`** (_libs_)
Save a table in an INI file.

* `filename` (String): file to load
* `tab` (Table): table to save

**`Table INI.load2(filename)`** (_libs_)
Load an simple INI file and create a table with it.

* `filename` (String): file to load

**`Void INI.save2(filename, tab)`** (_libs_)
Save a simple table in a simple INI file.

* `filename` (String): file to load
* `tab` (Table): table to save

_Example_:

An INI file contains:
    
    :::ini
    [infos]
    name=toto
    age=25

And then in the Lua script:

    :::lua
    tab = INI.load(“myfile.ini”)
    tab[“infos”][“name”] = “tata”
    INI.save(“myfile.ini”, tab)

This example changes the line _name=toto_ into _name=tata_.

These tables can only contain Strings! If you want to store numbers convert them with the `tostring()` function.

User's informations
-------------------

### Variables ###

They are defined in _nds_.

* `(Number) dsUser.color`: gives the color theme, between 0 and 15
* `(Number) dsUser.birthDay`: gives the birth day
* `(Number) dsUser.birthMonth`: gives the birth month
* `(String) dsUser.name`: gives the user's name
* `(Number) dsUser.nameLength`: gives the user's name length
* `(String) dsUser.message`: gives the user's personal message
* `(Number) dsUser.messageLength`: gives the user's personal message length
* `(Number) dsUser.alarmHour`: gives the alarm hour setting
* `(Number) dsUser.alarmMinute`: gives the alarm minute setting
* `(Number) dsUser.language`: gives the user's language, between 0 and 5
* `(Boolean) dsUser.GBAScreen`: gives the GBA screen setting (_true_ for the upper screen, and _false_ for the lower screen)
* `(Number) dsUser.defaultBrightness`: gives the default brightness (between 0 and 3 with a NDS Lite)
* `(Boolean) dsUser.getAutoMode`: gives the auto launching mode setting (_true_ if enabled, and _false_ if disabled)

### Functions (_unused_, _nds_)###

They are not really useful since the variables herebefore are based on them.

**`Number dsUser.getColor()`**
Give the color theme, between 0 and 15.

**`Number dsUser.getBirthDay()`** 
Give the birth day.

**`Number dsUser.getBirthMonth()`** 
Give the birth month.

**`String dsUser.getName()`** 
Give the user's name.

**`Number dsUser.getNameLength()`** 
Give the user's name length.

**`String dsUser.getMessage()`** 
Give the user's personal message.

**`Number dsUser.getMessageLength()`** 
Give the user's personal message length.

**`Number dsUser.getAlarmHour()`** 
Give the alarm hour setting.

**`Number dsUser.getAlarmMinute()`** 
Give the alarm minute setting.

**`Number dsUser.getLanguage()`** 
Give the user's language, between 0 and 5.

**`Boolean dsUser.getGBAScreen()`** 
Give the GBA screen setting (_true_ for the upper screen, and _false_ for the lower screen).

**`Number dsUser.getDefaultBrightness()`** 
Give the default brightness (between 0 and 3 with a NDS Lite).

**`Boolean dsUser.getAutoMode()`** 
Give the auto launching mode setting (_true_ if enabled, and _false_ if disabled).
