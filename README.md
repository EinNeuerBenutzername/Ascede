# Ascede

## General

Ascede is a C99 2D framework, forked from the awesome game framework: [raylib](https://www.raylib.com). You can take a look if you don't know it yet.

This project is intended for my personal use and I'm not going to put a lot of effort into this project.

What I'm doing here is basically cutting out some parts of raylib to enhance my development. My goal is to reduce functionality and even scalability in exchange for shorter development cycles and probably smaller binary sizes. **I strongly suggest you try the original raylib library instead of using this library,** because it is more functional and is under constant development, whereas Ascede is less functional, more unstable and rarely updated.

- I won't fix any bugs in Ascede that don't affect my own use, because this is a personal project. I won't bother with Issues, either.
- Ascede is 2D-only. It has no 3D support and most probably never will.
- Ascede's target platforms are desktop platforms, mainly Windows. It would rarely be tested on Linux and MacOS.
- I won't accept collaborations and pull requests because that would complicate my work.

## Status

#### Major updates

- [x] remove 3D support
- [x] remove camera support
- [x] kill snapshot and screen recording
- [ ] add instancing
- [ ] remake timing & frame control
- [ ] rearrange API
- [ ] improve audio precision

#### Minor updates

- [ ] remove string manipulation support
- [ ] remove bad RNG
- [ ] change the default font