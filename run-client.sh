#!/bin/bash

echo "Building http client"

gcc src/client.c -o out/client
clear
./build/client
