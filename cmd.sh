#!/bin/bash

set -e

if [ "$#" -eq 0 ]; then
  echo "Error: No arguments provided."
  echo "Usage: $0 <argument1> [argument2...]"
  exit 1 # Exit with a non-zero status code to indicate an error
fi

case "$1" in
    "test")
        echo "You do test."
        cmake . -DDO_TEST=1
        cmake --build . --config Debug
        ctest -V
        ;;
    "build")
        echo "You do build main."
        cmake .
        cmake --build .
        ;;
    "upload")
        echo "Upload changes to github repository."
        # check position is in valid git repository
        if [ ! -d ".git" ]; then
          echo "This is currently not git repository. Return now"
          exit 1
        fi
        # if there is no second arguments
        if [ -z $2 ]; then
          # ask for comment
          read -p "Commit message: " commit_message
        else
          commit_message=$2
        fi
        if [ -z $commit_message ]; then
          commit_message="Update repository at $(date '+%x %R')"
        fi
        git add .
        git commit -m "$commit_message"
        if [ $? -eq 0 ]; then
          echo "trigger"
        fi
        ;;
    *)
        echo "Invalid command."
        ;;
esac
