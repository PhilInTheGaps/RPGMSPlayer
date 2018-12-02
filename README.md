# GM-Companion

A tool for the gamemaster of a tabletop RPG session.

[![Build Status](https://travis-ci.org/PhilInTheGaps/GM-Companion.svg?branch=ui-update)](https://travis-ci.org/PhilInTheGaps/GM-Companion)
[![Build status](https://ci.appveyor.com/api/projects/status/8q56pf3cnbtyp6f3?svg=true)](https://ci.appveyor.com/project/PhilInTheGaps/gm-companion)
[![GitHub (pre-)release](https://img.shields.io/github/release/PhilInTheGaps/GM-Companion/all.svg)](https://github.com/PhilInTheGaps/GM-Companion/releases)
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.svg?v=103)](https://opensource.org/licenses/GPL-3.0/)

![Screenshot](https://github.com/PhilInTheGaps/GM-Companion/blob/master/docs/audio-tool-01.png?raw=true)

[Official Website](https://gm-companion.github.io/)  
[Launchpad Repository](https://launchpad.net/~rophil/+archive/ubuntu/gm-companion)  
[GM-Companion at itch.io](https://philinthegaps.itch.io/gm-companion)  

[Documentation](https://github.com/PhilInTheGaps/GM-Companion/wiki)

## Credits

- Dice and effect icons by [Skoll](http://game-icons.net/), [Delapouite](http://delapouite.com/), [Lorc](http://lorcblog.blogspot.com/) and [Cathelineau](https://game-icons.net/) under the [CC BY 3.0](http://creativecommons.org/licenses/by/3.0/)  
- [FontAwesome](https://fontawesome.com/)  
- [FontAwesome QML Implementation](https://github.com/PhilInTheGaps/fontawesome.pri) fork of [FontAwesome.pri](https://github.com/benlau/fontawesome.pri) by [Ben Lau](https://github.com/benlau)  
- [o2 (OAuth2 library)](https://github.com/pipacs/o2) by [pipacs](https://github.com/pipacs)  
- Spotify playlists by [Brian Davis (Bezoing)](https://open.spotify.com/user/bezoing?si=acN6RQebQTS2iZEUWDKRKA) and [gentilpuck](https://open.spotify.com/user/gentilpuck?si=nZpk5I8wTBuWdcBrrix_qg)

# Installation

## Windows

For Windows there is only a 64 bits version available. If you want to use the GM-Companion on a 32 bits machine, you have to build the program from source.

[Release 1.0.2 (Win x64)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/1.0.2/gm-companion_1.0.2_win64.zip)  

The program works without an installer. Simply extract all the files from the .zip archive to a folder and run _gm-companion.exe_.

Older versions are available at the [GitHub Release Page](https://github.com/PhilInTheGaps/GM-Companion/releases)  


## Linux

### Arch

Install from AUR:

```
aurman -S gm-companion
```

### Ubuntu

For Ubuntu the easiest way is to add the gm-companion software repository and install the gm-companion from there.  
Note that the GM-Companion requires at least Qt 5.10, currently only Ubuntu 18.10 meets that requirement.

```
sudo add-apt-repository ppa:rophil/gm-companion  
sudo apt-get update  
sudo apt-get install gm-companion  
```

I also have a daily-build repsository that always has the newest development version.  
I do not recommend using it, but if for some reason you absolutely want to use it, here:  


```
sudo add-apt-repository ppa:rophil/gm-companion-daily  
sudo apt-get update  
sudo apt-get install gm-companion  
```

## Building From Source

You can also build the program from source. This should work for Windows, Linux and Mac.  
Even though the travis.ci build for MacOS compiles without errors, I don't know if that version works 100% as I don't own a Mac to test this on.

GM-Companion requires Qt5 to build.  

So the build steps would look something like this:  
1. Clone the [GitHub repository](https://github.com/PhilInTheGaps/GM-Companion). Make sure to initialize all submodules!  
`git clone --recursive https://github.com/PhilInTheGaps/GM-Companion`
2. Install [Qt5](https://www.qt.io/) (GM-Companion requires at least version 5.10)  
3. If you are on Linux install [TagLib](http://taglib.org/)  
4. Switch to the GM-Companion folder  
5. Run qmake  
6. Run make  

# Usage / Quickstart

For detailed instructions see the [Wiki](https://github.com/PhilInTheGaps/GM-Companion/wiki).

## Audio Tool

- Go to settings, set path of music and sound files
- Open audio editor, create new project
- Add a category and a scenario
- Create an element, add music/sound files, radio playlist file or URL
- Optionally create a thumbnail image, place it in your resources path, add image to element

### Spotify

Using Spotify with GM-Companion requires some additional steps first:

- Go to [Spotify developer page](https://developer.spotify.com/dashboard)
- "Create an App"
- In app settings add _http://127.0.0.1:1965/_ as a redirect URI
- Go to GM-Companion settings, add "Client ID" and "Client Secret" of your app
- Restart GM-Companion
- You can now add Spotify playlists and albums as elements, the "_Audio Addon_" has some great RPG related playlists. You can add by adding a Spotify element and pressing "_Add from Addons_"

## Map Tool

- Open your maps folder (see "Paths" settings)
- Create a new folder
- Place map images inside

## Character Tool

- Open your characters folder (see "Paths" settings)
- Create a new folder and name it after the character
- Place images of the character sheet inside

Have fun!
