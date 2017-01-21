#!/bin/bash

VERSION="3.2.1"

# check if its installed already
installed=$(pkg-config --modversion glfw3)
if [[ "$VERSION" == "$installed" ]]; then
  echo "GLFW at version ${VERSION} already installed."
  exit 0
fi

mkdir -p build && cd build

wget -nc https://github.com/glfw/glfw/archive/${VERSION}.tar.gz
tar -xzvf ${VERSION}.tar.gz
pushd glfw-${VERSION}
cmake -G "Unix Makefiles" .
make
sudo make install
popd

#rm -rf glfw-${VERSION} ${VERSION}.tar.gz
