# BaseInitializer

Adds common player/pawn classes with proper initialization routes which confirms character is entirely initialized

## Usage: 
1. Clone this to your Project\Plugins folder (do not use engine folder)
1. Modify this plugin to inherit from your C++ plugin classes
1. Derive your project C++ classes and/or BP classes
1. Modify the plugin and exclude the use of VoxelPlugin which is included by default, both for usage as well as an example of how to add custom behaviour
1. Use `OnInit()` in BP or C++ instead of `BeginPlay()` where full initialization is required
1. [Optional] Once ensured to be functional, add any custom behaviour you require (then test this)
