#!/bin/bash
# author: Mengsen
# description: Remove Special File Recursively.

find . -type f -name '*.out' -print
find . -type f -name '*.s' -print

find . -type f -name '*.out' -print | xargs rm
find . -type f -name '*.s' -print | xargs rm