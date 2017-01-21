#!/bin/bash

VERSION="2.0.0"

mkdir -p build && cd build

# check if its installed already
installed=$(pkg-config --modversion glew)
if [[ "$VERSION" == "$installed" ]]; then
  echo "GLEW at version ${VERSION} already installed."
  exit 0
fi

wget https://github.com/nigels-com/glew/releases/download/glew-${VERSION}/glew-${VERSION}.tgz
tar -xzvf glew-${VERSION}.tgz
pushd glew-${VERSION}
make
sudo make install
popd
#rm -rf glew-${VERSION}.tgz glew-${VERSION}
