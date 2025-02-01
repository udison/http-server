#!/bin/bash

echo "Building http server"

gcc src/main.c src/server.c src/logger.c -o out/rock

