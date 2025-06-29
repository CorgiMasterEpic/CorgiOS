#!/usr/bin/env bash

should_run=true

if [ "$1" == "--no-run" ]; then
    echo "Nice try. This is run.sh. It runs... OK FINE:\n\n"
    should_run=false
fi

set -e

echo -e "Welcome to the awesome automatic building and running bash script for corgi os!\n"

echo -e "[BUILDING]: Bootloader\n"
cd gnu-efi && make && make bootloader

echo -e "\n[BUILDING]: Kernel\n"
cd ../kernel && make setup && make kernel && make buildimg

echo -e "\n[BUILD]: COMPLETE!\n\n[RUNNING or not?]: Using Qemu!\n"
if [ "$should_run" = true ]; then
    make run
else
    echo "Not running due to --no-run."
fi

echo "DONE."
