# BadXpple

Run Bad Apple on an X window.

## Requirements

- OpenCV
- Xlib

## Usage

Extract the frames from the video. You need ffmpeg for this.

```sh
sh extract_frames.sh <bad_apple_video>
```

Compile the program.

```sh
cmake -S . -B build/
cmake --build build/
```

Run the program.

```sh
./build/badxpple
```