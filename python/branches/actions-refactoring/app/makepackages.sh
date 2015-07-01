rm build.log &> /dev/null
for package in media-server media-tools media-server-plugins media-renderer
do
    echo "Building package $package..."

    cd $package
    dpkg-buildpackage &> ../build.log
    sudo rm -dr build &> /dev/null
    sudo rm -dr build-stamp &> /dev/null
    sudo rm -dr debian/tmp &> /dev/null
    sudo rm -dr debian/python* &> /dev/null
    dh_clean
    cd ..

    echo "Finished building $package"
done

echo "Finished building packages"
