#!/bin/bash

if [[ "$DISPLAY" == ":1.0" ]]; then
  Xephyr -br -ac -noreset -screen 800x600 :100 &
  echo Display on :100
else
  Xephyr -br -ac -noreset -screen 800x600 :1 &
  echo Display on :1
fi