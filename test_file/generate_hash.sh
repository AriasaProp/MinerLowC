#!/bin/bash

set -e

test_loc=$(dirname "$(readlink -f "$0")")
out_file=${test_loc}/sha256msg.txt
if test -f "$FILE"; then
  rm $out_file
fi
exec > $out_file 2>&1

file_msg=${test_loc}/message.txt

while IFS= read -r line; do
  hash=$(echo -n $line | sha256sum | cut -d' ' -f1)
  echo $line
  echo $hash
done < $file_msg
