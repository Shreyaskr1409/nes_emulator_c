# NES Emulator in C
This is a Nintendo Entertainment System emulator written in C which uses raylib to create UI to run NES games.
NES commonly refers to the Nintendo Entertainment System, a classic 8-bit home video game console developed by Nintendo released in 1983.
The emulation works from NES electronic components built from scratch such as cpu, ppu, bus, mappers, so on.

## Development
To develop on this repository, you first need to create your own fork on GitHub.
Apart from this, you must have `make` installed. You can find it on your respective **package manager** or even pre-installed on **Linux** `scoop` on **Windows** and `homebrew` on **Mac**.

> Raylib is being used for creating UIs.

> stclib/STC is being used as a standard library in some cases where I need some useful data structuresa and functions apart from what vanilla C offers.

- Clone the repository inside the folder where you want cloned repo to be added.
```bash
# clone using https
git clone https://github.com/Shreyaskr1409/nes_emulator_c.git
cd nes_emulator_c
```
Or if you prefer SSH:
```bash
# clone using ssh
git clone git@github.com:Shreyaskr1409/nes_emulator_c.git
cd nes_emulator_c
```

- To test the emulator, run the following command with `test-?` replaced by a test number from the Makefile.
```bash
# DO NOT RUN AS IS
# replace '?' with a valid test number from the Makefile
make test-? -B
```
Remember that this will add a `main.bin` in your root directory as an executable which you can run again as:
```bash
./main.bin
```
In future, ability to just run the `make` command as is to make a build will be there when I finish the emulator.

## Status of this project
This project started out as a personal project for recreation and hobby.
It still is but I do take it more seriously than before.
I have implemented the CPU completely and it works just right, atleast enough that I have not found an issue so far.
I will be adding more tests to it fully confirm that the emulation works accurately.
The cartridge is fully capable being loaded in the form of a `.nes` file.

### Shout-outs
- The amazing community at https://www.nesdev.org/ which provided me all of the resuources I could have asked for to learn and revise everything about NES which I could want.
- https://github.com/OneLoneCoder for a wonderful tutorial from where I have learnt a lot about how this works, where I got help from whenever I got stuck, and how `C++` works which is a nice "bonus".
- @shottferg's https://github.com/scottferg/Fergulator where I took initial referenced from when I started.

### To any beginner reading this
If this interests you, I recommend you to go to the previous links or just watch OneLoneCoder's YouTube tutorial on how these softwares work, it does an excellent job of explaining `nearly` everything you need to know as someone who wants to just develop. But, only `nearly` since there is a lot more which you can learn side-by-side if you want to learn. This is an excellent beginner project which will clear your concepts on Computer Architecture as well as C Programming.

## Status of my motivation
While I will contonue working on it, I can not confirm support for each and every game which is available for NES,
but I will atleast add support for some classic games released for NES from series such as
`The Legend of Zelda`, `Castlevania`, `Final Fantasy`, `Super Mario Bros.` and `Donkey Kong`.