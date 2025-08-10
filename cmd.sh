#!/bin/bash

set -e

if [ "$#" -eq 0 ]; then
  echo "Error: No arguments provided."
  echo "Usage: $0 <argument1> [argument2...]"
  exit 1 # Exit with a non-zero status code to indicate an error
fi

case "$1" in
    "test")
        echo "Do test."
        cmake . -DDO_TEST=1
        cmake --build . --config Debug
        ctest -V
        ;;
    "build")
        echo "Do build main."
        cmake .
        cmake --build .
        ;;
    "update")
        git status --porcelain
        if [ -n $? ]; then
          echo "There changes to deal before update."
          exit 1
        fi
        git pull
        ;;
    "upload")
        echo "Upload changes to github repository."
        # check position is in valid git repository
        if [ ! -d ".git" ]; then
          echo "This is currently not git repository. Return now"
          exit 1
        fi
        # default commit message
        commit_message="Update repository at $(date '+%x %R')"
        # if there is a second arguments
        if [ -n $2 ]; then
          commit_message=$2
        fi
        git add .
        git commit -m "$commit_message"
        if [ -n $? ]; then
          git push
        else
          echo "Nothing is changed."
          exit 1
        fi
        echo "Changes uploaded"
        ;;
        
    *)
        echo "Invalid command."
        ;;
esac
