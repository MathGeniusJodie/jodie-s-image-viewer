#!/bin/sh
gcc jiv.c -Os -o jiv_binary -nostartfiles -ffreestanding -nostdlib -fno-stack-protector -static -Wl,-n -fdata-sections -ffunction-sections -Wl,-gc-sections -s
