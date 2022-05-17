# Chaosbox
This is the source code of a mod for classic Serious Sam: The Second Encounter v1.07 that consists out of many smaller mods and ideas, which when combined together, give you quite a chaotic gameplay experience. 
Based on [Serious Sam SDK v1.07 for Visual Studio 2013](https://github.com/DreamyCecil/SeriousSam_SDK107)

**Chaosbox modules:**
- `Randomized` - taking roots from semi-public **Tiny Mod Pack** mods, this mod introduces options for randomizing specific elements of the world, ranging from randomly pitched sounds to random enemy spawns and more.
- `Single Specimen` - originally planned as a part of **Tiny Mod Pack**, this mod adds an option to replace every single enemy on a map with only one specific type, be it a Beheaded Kamikaze or a Reptiloid Highlander.
- `Shrunk` - taking roots from aforementioned **Tiny Mod Pack**, this mod adds options for resizing the player and every enemy in the game.
- `Shuffled` - taking roots from **Tiny Mod Pack**, this mod adds ability to shuffle game resources such as textures and sounds around in a chaotic manner, making everything look and sound totally ridiculous.
- `Slowdown Bug: Ultra Deluxe` - the latest addition to Chaosbox, which was a standalone mod that adds an option to add a specific amount of time to the in-game timer as a 32-bit floating point number, making logic and even animations break down more and more with time.

Building
--------

To compile the source code, you'll need to use a compiler from Microsoft Visual C++ 6.0.

Full guide: https://github.com/DreamyCecil/SeriousSam_SDK107#building

Running
-------

Once the project is compiled, there should be three libraries in the Bin folder: `EntitiesMP.dll`, `GameGUIMP.dll` and `GameMP.dll`.

There are two ways to start the mod:
1. Create a `.des` file in your Mods directory under the same name as this repository, open it in any text editor and type your mod name in it. Then you'll be able to launch your mod from the game's `Mods` list.
2. Run `ModStart.bat` or `EditorStart.bat` from the Bin folder to open the editor or the mod.

When running a selected project, make sure the mod in project properties **Debugging** -> **Command Arguments** is set to your mod name instead of `Chaosbox` (example: `+game Chaosbox_Mod`).

License
-------

Just like Croteam's [Serious Engine 1.10](https://github.com/Croteam-official/Serious-Engine) source code, Serious Sam SDK is licensed under the GNU GPL v2 (see LICENSE file).

This SDK includes Croteam's Entity Class Compiler (`Sources/Extras/Ecc.exe`) that is used to compile `.es` files and officially distributed with classic Serious Sam games. Its source code is included in Serious Engine 1.10.

Some of the code included with the SDK may not be licensed under the GNU GPL v2:

* DirectX8 SDK (Headers & Libraries) (`d3d8.h`, `d3d8caps.h` and `d3d8types.h` located in `Sources/Extras`) by Microsoft
