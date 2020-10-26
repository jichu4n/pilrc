#!/bin/bash

# Script to set up a Debian-based Linux environment and build a binary
# distribution package.

dist="$1"
arch="$2"

cd "$(dirname "$0")/.."
base_dir="$PWD"
package_root_dir="$base_dir/dist"

version="$(cat "$base_dir/VERSION")"
deb_file_name="pilrc_${version}~${dist}_${arch}.deb"

if [ "$EUID" -eq 0 ]; then
  sudo=
else
  sudo='sudo'
fi

function install_build_deps() {
  export DEBIAN_FRONTEND=noninteractive
  $sudo apt-get -qq update
  $sudo apt-get -qq install -y \
    build-essential autoconf automake unzip \
    > /dev/null  # -qq doesn't actually silence apt-get install.
}

function build() {
  # Build and install into temp directory.
  rm -rf "$package_root_dir"

  # See contrib/package
  (cd "$base_dir/unix" && \
    aclocal && \
    automake --add-missing && \
    autoconf)
  # See unix/pilrc.build and unix/pilrc.spec
  env \
    prefix=/usr \
    DESTDIR="$package_root_dir" \
    "$base_dir/unix/pilrc.build"
  install -m 0755 "$base_dir/fonts/pilfont" "$package_root_dir/usr/bin/pilfont"
}

function package() {
  cd "$package_root_dir"
  mkdir -p DEBIAN
  cat <<EOF > DEBIAN/control
Package: pilrc
Version: $version
Section: devel
Priority: optional
Architecture: $arch
Depends:
Maintainer: Chuan Ji <chuan@jichu4n.com>
Description: Palm OS resource compiler
 PilRC is an application that takes a resource script file and generates
 one or more binary resource files that are to be used when developing
 for the Palm Computing Platform.
EOF

  cd "$base_dir"
  dpkg-deb --build "$package_root_dir" "$deb_file_name"
}

set -ex

install_build_deps
build
package

