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
        echo $commit_message
        ;;
    *)
        echo "Invalid command."
        ;;
esac

#!/bin/bash

# Nama skrip: git-push-if-changes.sh
# Deskripsi: Skrip untuk melakukan add, commit, dan push ke repositori Git
#            hanya jika ada perubahan yang ditemukan.

# Memastikan kita berada di dalam direktori repositori Git

# Meminta pesan commit dari user

# Memeriksa apakah pesan commit kosong

echo "---"
echo "Memulai proses Git..."
echo "---"

# 1. Menambahkan semua file yang diubah atau baru
echo "1. Menambahkan semua file dengan 'git add .'..."
git add .
echo "---"

# 2. Pengecekan apakah ada perubahan yang di-stage
if [[ -z $(git status --porcelain) ]]; then
  echo "✅ Tidak ada perubahan baru untuk di-commit. Proses dihentikan."
  exit 0
fi

# 3. Melakukan commit dengan pesan dari user
echo "2. Melakukan 'git commit'..."
git commit -m "$commit_message"
if [ $? -ne 0 ]; then
  echo "❌ Error: Gagal melakukan git commit."
  exit 1
fi
echo "---"

# 4. Melakukan push ke remote repository
echo "3. Melakukan 'git push'..."
git push
if [ $? -ne 0 ]; then
  echo "❌ Error: Gagal melakukan git push. Periksa kredensial atau status repositori."
  exit 1
fi
echo "---"

echo "✅ Proses Git selesai! Repositori Anda berhasil diperbarui."
