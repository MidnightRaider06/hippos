#!/bin/sh
set -e
. ./iso.sh

if [ "$#" -eq 1 ] && [ "$1" = "--debug" ]; then
    echo "Debugging mode"
    qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom hippos.iso -s -S
else
    echo "Normal mode"
    qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom hippos.iso
fi
