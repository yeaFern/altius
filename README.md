# Altius
Altius is a [roguelike](https://en.wikipedia.org/wiki/Roguelike) which is being developed as part of [RoguelikeDev Does The Complete Roguelike Tutorial](https://www.reddit.com/r/roguelikedev/wiki/python_tutorial_series#wiki_version_2020).

The goal of this event is to follow alongside [this tutorial](http://rogueliketutorials.com/tutorials/tcod/part-0/) over the span of many weeks and eventually have a complete game at the end.

## Building
Altius uses [CMake](https://cmake.org/).
```
$ git clone https://github.com/yeaFern/altius.git
$ cd altius
$ build
```
## Development
For this project I decided to use the [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) which is a realtime game engine primarily designed for experimentation with game development/algorithms, but can be used for larger projects too. Below is a miniature development log of Altius throughout the weeks that I have worked on it.
### Weeks 1/2/3

The fact that the Pixel Game Engine isn't primarily a roguelike engine meant that I had to do a few things to get it up and running in a manner which I could use for the rest of the tutorial (see ConsoleRenderer.cpp and ConsoleInputManager.cpp), I pleased to find out that writing these utilities only took a few hours and I could move on to working on the game.

On top of this choice of engine I joined the event late, in week three, below is a picture of the game as it stands after my first week of development, incorporating all elements from parts zero through five of the roguelike tutorial.

![Weeks 1 2 and 3 Part 1](/docs/week123_1.png?raw=true)

![Weeks 1 2 and 3 Part 2](/docs/week123_2.png?raw=true)
