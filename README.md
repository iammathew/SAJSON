# Super Animation (.sam) To Json Converter

## Get Started

### Building

Requirements:
- gcc

Afterwards run `build.win.bat` or `build.osx.sh` (also works for Linux) respectively

### Usage

Run the just built binary like this:

**Windows:**

`SAJSON.exe [path to .sam file] > [path to new .json file]`

**Mac or Linux:**

`SAJSON [path to .sam file] > [path to new .json file]`

**Example:** `SAJSON animation.sam > animation.json`

If the following error message, pops up: `Assertion failed: aNumFrames > 0 && "We don't have valid frames.", file SuperAnimCore.cpp, line 642`

Try using the `--effect` flag like this: `SAJSON animation.sam --effect > animation.json`

**Tip:** I personally use `.sajson` as an extension for generated json files in order to distinguish them more easily.

## Super Animation Explained

Super Animation is a format created by [Raymondlu](https://github.com/raymondlu) in order to convert Adobe Flash animations to be used inside Cocos2D.
Part of the implementation can be found [here](https://github.com/raymondlu/super-animation-samples).

The format itself though is binary therefore hard to read and not easily human readable. This converter uses the original implementation to generate a more readable JSON format, that can be either used directly or as a reference for own parser implementations.

### Format

A sam file contains the following:
- AnimRate: Frames Per Second
- X: X Transform
- Y: Y Transform
- Width: Width of the Animation
- Height: Height of the Animation
- Images: List of images each containing
    - ImageName: Path to image file (relative to .sam file?)
    - Width: Width of Image
    - Height: Height of Image
    - Transform: Transformation Matrix (3x3) describing base transformation
- StartFrame: First Frame (always 0?)
- EndFrame: Last Frame
- Frames: List of images each containing
    - Objects: List of objects that are contained in this frame, each containing
        - ObjectNum: Object index (Used to track same object inside an animation)
        - ResNum: Image index
        - Transform: Transformation Matrix (3x3) describing further transformation
        - Color: RGBA color used to recolor part (or make transparent, some kind of blending)
- Labels: List of Labels (Sequences) each containing
    - Name: Name of Sequence
    - StartFrame: Start frame index of sequence (inside frames list)
    - EndFrame: End frame index of sequence (inside frames list)

#### Transformation Matrices

Since it is mainly 2D Animation i'm a bit confused why its a 3x3, but im new to 2D Game Development so pretty sure there is some explanation here:

The most important bits though are:
- Index [0, 2]: x coordinate
- Index [1, 2]: y coordinate

## Where is this format is used?

This format is used by some of Alim Gumi games like Brave Frontier.
This project was built in order to reproduce some of the animations in the game.

Battle Effects and UI Animations use a slightly different format and require the `--effect` flag to be passed in, in order to be parsed correctly.

## Credits
- [Raymondlu](https://github.com/raymondlu) for creating Super Animation and the parser implementation
- [Natural Style Co. Ltd.](https://na-s.jp/SuperAnimHTML5/) for base JSON converter
- [BluuArc](https://github.com/BluuArc) for figuring out how to parse battle effects and ui animations


