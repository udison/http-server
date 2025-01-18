#!/bin/bash

echo "Building http client"

gcc src/client.c -o build/client
clear
./build/client
