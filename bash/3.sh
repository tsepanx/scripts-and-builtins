#!/bin/bash


echo "Kernel name: $(uname -s)"
echo "Kernel version: $(uname -r)"

echo "Arc: $(uname -m)"

echo
echo -e "Logged users info:\n$(who -T)\n"

if [ -d "/sys/firmware/efi" ]; then
    echo "EFI is enabled;"
else
    echo "EFI is NOT enabled;"
fi

echo
echo


lsblk --fs

efi_boot_order() {
    echo
    echo
    efibootmgr | grep "BootOrder" | awk '{ print $2 }'
}

efi_boot_order

