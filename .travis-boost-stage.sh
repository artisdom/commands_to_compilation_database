#!/bin/sh

set -e

BOOST_VERSION=1.59.0
BOOST_VERSION_=1_59_0

BOOST_LIBRARIES="--with-filesystem --with-system --with-program_options"

if ! which wget > /dev/null ; then
    echo "error: wget is not available"
    exit 1
fi

cd /tmp
wget --output-document "boost_${BOOST_VERSION_}.tar.gz" "http://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}/boost_${BOOST_VERSION_}.tar.gz/download"
tar xfz boost_1_59_0.tar.gz
cd /tmp/boost_1_59_0
./bootstrap.sh
./b2 --prefix=/tmp/boost --minimal --layout=system ${BOOST_LIBRARIES}
