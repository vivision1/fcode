#!/bin/bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

cat /proc/version


pkg-config --modversion gtk+-3.0


apt install libgtk-3-0 at-spi2-core

cat <<EOF > /usr/share/applications/fastcoder.desktop
[Desktop Entry]
Version=1.0
Type=Application
Name=FastCoder IDE
GenericName=Integrated development environment
Comment=Configurable and extensible IDE
Exec=/home/super/desk/code/code %F
Icon=codeblocks
Terminal=false
X-MultipleArgs=false
Categories=Development;IDE;GTK;
StartupNotify=true
EOF

exit 1