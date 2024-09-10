#!/usr/bin/env bash

set -e

cd ./src

indent *.c *.h
find . -name "*.*~" -exec rm {} \;
