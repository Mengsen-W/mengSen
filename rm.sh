#!/bin/bash
# author: Mengsen
# description: Remove Special File Recursively.

find . -type f -name '*.out' -print | xargs rm