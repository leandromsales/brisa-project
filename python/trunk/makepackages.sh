#!/bin/bash

./cleanpackages.sh

mkdir build 2> /dev/null

ADDITIONAL_PARAM="-rfakeroot -sa -S"
if [ -z "${SBOX_DPKG_INST_ARCH}" ];
then
    ADDITIONAL_PARAM="-tc"
fi

if [ "${1}" == "2.6" ];
then
	echo "WARNING: also generating for python-2.6"
fi

rm build.log &> /dev/null
for package in python-brisa-dependencies/circuits-1.1.1 python-brisa misc/python-brisa-qtreactor app/ app/media-server app/media-tools app/media-server-plugins app/media-renderer
do

    cd $package
    if [ "${1}" == "2.6" ];
    then
	echo "Building package $package (Python 2.6)..."
	mv debian/control debian/control.bk
	mv debian/rules debian/rules.bk
	mv debian/postinst debian/postinst.bk &> /dev/null
	cp debian/control-python-2.6 debian/control
	cp debian/rules-python-2.6 debian/rules
	cp debian/postinst-python-2.6 debian/postinst &> /dev/null
	dpkg-buildpackage ${ADDITIONAL_PARAM} &> ../build.log
	sudo rm -dr build &> /dev/null
	sudo rm -dr build-stamp &> /dev/null
	dh_clean
	mv debian/control.bk debian/control
	mv debian/rules.bk debian/rules
	mv debian/postinst.bk debian/postinst &> /dev/null
    fi

    # python 2.5
    echo "Building package $package (Python 2.5)..."
    dpkg-buildpackage ${ADDITIONAL_PARAM} &> ../build.log
    sudo rm -dr build &> /dev/null
    sudo rm -dr build-stamp &> /dev/null
    dh_clean

    cd - &> /dev/null
    echo "Finished building $package"
done

mv *.dsc build &> /dev/null
mv *.changes build &> /dev/null
mv *.tar.gz build &>/dev/null
mv *.deb build &> /dev/null

mv misc/*.dsc build &> /dev/null
mv misc/*.changes build &> /dev/null
mv misc/*.tar.gz build &>/dev/null
mv misc/*.deb build &> /dev/null

mv app/*.dsc build &> /dev/null
mv app/*.changes build &> /dev/null
mv app/*.tar.gz build &>/dev/null
mv app/*.deb build &> /dev/null

mv python-brisa-dependencies/*.dsc build &> /dev/null
mv python-brisa-dependencies/*.changes build &> /dev/null
mv python-brisa-dependencies/*.tar.gz build &> /dev/null
mv python-brisa-dependencies/*.deb build &> /dev/null
