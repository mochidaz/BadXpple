#! /usr/bin/sh

if [ -x /usr/bin/ffmpeg ]; then
  echo "Extracting frames from $1"
  mkdir badapple/
  ffmpeg -i $1 -vf fps=30 badapple/output_%04d.png
else
    echo "Please install ffmpeg first"
fi

