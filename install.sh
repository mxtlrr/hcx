#!/bin/bash

prefix="/usr"
dir="$prefix/local/bin/hcx"
echo INSTALLING TO $dir...

flags=""
sudo install $flags "obj/hcx" $dir

# Reset permissions, give
# back to OG user
user=$(ls /home)
sudo chown $user $dir 

echo DONE
