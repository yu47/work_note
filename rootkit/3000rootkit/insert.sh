#! /usr/bin/env bash

# Find the UID of student
STUDENT_UID=$(id -u student)
# Files with this prefix should be hidden from ls
MAGIC_PREFIX="hideme"

echo -e "\e[31mInserting 3000rootkit with student_uid=$STUDENT_UID magic_prefix=$MAGIC_PREFIX\e[39m"

# Insert the rootkit module, providing the correct parameters above
insmod 3000rootkit.ko student_uid=$STUDENT_UID magic_prefix=$MAGIC_PREFIX
