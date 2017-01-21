#!/bin/bash

VERSION="3.3.1"

# check if its installed already
installed=$(pkg-config --modversion assimp)
if [[ "$VERSION" == "$installed" ]]; then
  echo "Assimp at version ${VERSION} already installed."
  exit 0
fi

mkdir -p build && cd build

wget -nc https://github.com/assimp/assimp/archive/v3.3.1.tar.gz
tar -xzf v${VERSION}.tar.gz
pushd assimp-${VERSION}
mkdir -p build && pushd build
cmake ..
make -j4
sudo make install
popd
popd

# we're going to let the files there, to enable caching
#rm -rf v${VERSION}.tar.gz assimp-${VERSION}
