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
    "update")
        git status --porcelain
        if [ ! $? -eq 0]; then
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
        # if there is a second arguments
        if [ -n $2 ]; then
          commit_message=$2
        else
          # ask for comment
          read -p "Commit message: " commit_message
        fi
        if [ -z $commit_message ]; then
          commit_message="Update repository at $(date '+%x %R')"
        fi
        git add .
        git commit -m "$commit_message"
        if [ $? -eq 0 ]; then
          git push --set-upstream origin main
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
