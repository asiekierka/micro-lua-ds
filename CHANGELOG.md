# Changelog

## MicroLua 4.7.3

### Improvements

* Updated to BlocksDS 1.10.1.
* Migrated from the EFS library to the more standard NitroFS.

### Bug fixes

* As BlocksDS worked around the 3DS RTC emulation issue upstream, `os.time3DS` and `os.date3DS` are no longer necessary. They have been preserved as aliases to `os.time` and `os.date` for compatibility purposes.

## MicroLua 4.7.2

### Bug fixes

* Fix the Sprite module which was using a deprecated function that was removed from Lua.

### Comments

This is a (very) small bug fix version resulting from updating the version of Lua that MicroLua uses. Some functions were removed and the oldest code of 'libs.lua' was still using them. The part of the code that needed to be fixed passed through our tests ;)
It looks like I will make some comments for every new version from now on ahah! I love talking. To unkonwn people that will never answer me. Or maybe will I never be read. :'(

## MicroLua 4.7

### New features

* Support for the EFS lib: you can embed some files into the binary of MicroLua, which makes a nice way to distribute your scripts (available through the sources of MicroLua)
* Our INI lib can now crypt the tables (Motus algorithm) before saving them
* Upon creating a Timer, you can pass as an argument an initial time, so the timer will be set to this time (in milliseconds) instead of 0; this is also available for the method `:reset([time])`

### Improvements

* The INI library can seamlessly handle table with a simple structure, that is to say raw `key = value` pairs without any `[example]` sections; thus an argument has been added to `INI.load()` so the function knows what to do (`INI.save()` automatically detects the structure)
* Already loaded Image can be used while creating a new Sprite instance; just give it to the function in place of the path (giving a path is still supported)
* All the user-readable files are now presented with MarkDown (<http://en.wikipedia.org/wiki/Markdown>)
* The Timers' method `:time()` is now `:getTime()` as part of the goal of giving better names to the getters and setters
* Instead of the whole DateTime system, you can now use the Lua's functions `os.time()` and `os.date()`; thus DateTime does no longer exists
* Some variables in 'boot.lua' are now destroyed before running the shell
* `io.open()` accept the character "t" in is mode argument (this was the default behaviour anyway)
* The default shell now displays the version of Lua
* The way the EFS is handled upon compilation has been improved: the folder 'efsroot' is no longer necessary if the EFS is not needed
* The BAT files for Windows now make a better use of the make command (the general behaviour is unchanged)

### Bug fixes

* Fix the display of memory usage in Debug mode (it was in kilobytes, not in bytes)
* Fix `System.listDirectory()`: the function now works properly in an folder different than the current one
* Fix the time functions now working on 3DS: MicroLua implements a workaround for this problem (which comes from the fundamental libnds that Microlua has no power on it), you can find it there : <https://sourceforge.net/p/microlua/wiki/3DSTimeWorkaround/>
* Fix `dsUser.name` and `dsUser.message`: there was a problem while converting the UTF-16 provided by the libnds to simple chars

### Changes for Microlua developers

* The Makefile has been cleaned and improved:
 * the part for the EFS has been improved
 * there are a few more targets: all, cleanall, export
* With the migration to SourceForge, the whole repository structure has changed (and the 'docs' folder has been removed)
* Out Lua sources have been updated to Lua 5.2.2; its folder is now simply called 'lua' to ease future updates
* The warnings from our code have been fixed (the ones from the EFSlib are too weird)

### Comments

Hi, I'm Reylak and for this release at least, I felt like I needed to say many things about what has been done to provide you with MicroLua 4.7.

Looking behind me, I can see that the last "stable" version, the 4.6, was released in November 2012 (and the 4.6.1 in February 2013). Now we are in September 2013, seven months in the future and I can assure you that it was a little war to get to this.

The 4.6.1 came accross the finding of the time bug on the 3DS, which produced a 4.6.1_a because I could only figure a poor workaround for this bug. Moreover, we managed to include the utilisation of the EFS on about the same period, and all of this resulted in some weird, ugly version "Microlua 4.6.1\_a \[EFS\]". And I was thinking: come on, tidy up this mess. This lasted about five months I think, eh.

So two months ago, I felt like it was the right time to change how things were going. We were too attached to the old period around 2009 when Risike was working alone on this, we need to emancipate from this history. And Google helped us for this, as they decided to remove the upload space they allowed to the projects on GoogleCode. From this point, I migrated MicroLua to SourceForge, I rearranged things up by updating the Wiki, making new issues, designing the lead of MicroLua by building milestones; we have some ideas up to the version 6.0 you know!

When I finally got over this side-work, I pulled my sleeves up and did the things that needed to be done: update the sources of Lua (we were so outdated!), fix some stupid warnings on compilation process, finally fix the function `System.listDirectory()`, and above all, officially add the EFS and the workaround for the 3DS. Add to this a rearrangement of the Timers and the removal of DateTime, which were needed because of the update of the sources of Lua, and some nice small improvements, and you get the version in this folder: **Microlua 4.7**. Clean, pure version.

As I write these lines I feel like it was some kind of marathon, althought it could have started much sooner than two months ago. And I think this is one of the biggest changelog I have ever written for MicroLua. I'm proud of it, of MicroLua, of what we've done with this and of what we can achieve simply by keeping things up.

Now I'm gonna cry a river like a pussy, but hey this is a great adventure.


Hobie-ho my friends!

> Reylak

## MicroLua 4.6

### New features

* You can now control the LED and make it blink as you wish.
* You can shutdown the console.

### Improvements

* The whole Debug engine has been reworked by Papymouge ;)

### Changes for MicroLua devs

* Update for latest devkitPro release (as on 08/08/12)
* Fix warnings in some files
* Use ULUA_LIBS instead of plain path for libs.lua
* Change compilation outputs to something lighter (the big ugly text is put in 'out' in <lua-5.1.3>)
