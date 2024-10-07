# Freecell

A free C++/Qt implementation of classic Freecell.

## Gameplay

![screenshot](resources/screenshots/freecell.png)

## Origins & Acknowledgements

Original game files courtesy of:
https://github.com/juchi/Freecell

Playing card assets courtesy of:
https://github.com/hayeah/playing-cards-assets

## Build from Source

You'll need a recent C++ compiler, a recent version of Qt6 (I use Qt 6.7.2), and CMake.

```bash
git clone https://github.com/nholthaus/freecell.git
cd freecell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target freecell -- -j
./bin/freecell
```