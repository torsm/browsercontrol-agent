# browsercontrol agent
Native Java agent to block the problematic "browsercontrol" library to load in the application "JagexAppletViewer"

## Problem
Players of the game "Old School RuneScape" who wish to run multiple game clients on a machine at the same time may run into an error when attempting to launch a 3rd game client. The error message "Unable to create advertising" is displayed and the client will terminate before fully loading.

## Findings
By reverse engineering and debugging the (obfuscated) application `jagexappletviewer.jar`, which is used to load and execute the game itself, I came across the following points to understand and explain the problem:


- The error typically occurs when a certain native library (`browsercontrol.dll`), containing JNI bindings, could not be loaded.
- The application searches for a directory called `.jagex_cache_32` in several predefined locations, which are `C:/rscache`, `C:/Windows`, `C:/winnt`, `C:/`, the user home, and a few other places. The `.jagex_cache_32` directory is supposed to contain the native browsercontrol library.
- The search skips over non-existing file locations and files that cannot be opened in read-write mode (the dll is overwritten by a fresh file from the servers every time, presumably to avoid modification or replacements).
- If a library is loaded by a process, the file cannot be opened in read-write mode, which means that a running game client prevents an individual `browsercontrol.dll` file from being used again.
- If no remaining `browsercontrol.dll` file that exists AND is not being used yet could be found, the previously mentioned error message is shown.
- During installation, the installer creates `C:/.jagex_cache_32/browsercontrol.dll` and `userhome/.jagex_cache_32/browsercontrol.dll`, which explains why opening a 3rd game client fails, because the only 2 existing files are "occupied" by the already running clients.

## Solution

Even though easily possible, I wanted to find a solution to this problem that doesn't involve modifying/patching any launcher files.

The dll file produced by this project acts as a native Java agent. Using breakpoints in the `System.load` method responsible for loading native libraries, I was able to prevent the `browsercontrol.dll` file from being loaded in a lightweight manner. That allows further clients to rewrite the file with an up to date version.

The dll also provides the now missing JNI bindings. However, the native methods are effectively never executed (advertisements in the client are discontinued), so each JNI function can just be left empty without compromising functionality.

## Usage
To use the produced native Java agent, the JVM options of the launcher have to be extended by `-agentlib:browsercontrol-agent` by modifying the configuration launcher's file (something like `userhome/jagexlauncher/oldschool/oldschool.prm`).

For Java to be able to resolve `browsercontrol-agent`, the equally named dll file has to be placed somewhere so it can be found. I like to place it in the game's install directory alongside the other binary files (`userhome/jagexlauncher/bin`).

## Anecdote
A widely accepted work-around for this problem was to rapidly open game clients consecutively which allowed for more than just 2 clients being ran at once. This can be explained by a moderate delay caused by loading times. The time between the application checking if the dll file can be rewritten and actually calling `System.load` is enough time for several other clients to get past the writeability check.
