cd BrisaBinaryLight_Maemo5/
make distclean
qmake
make
cp brisabinarylight ../BrisaDebExamples_Maemo5/usr/bin/
cd ..

cd BrisaConfiguration_Maemo5/
make distclean
qmake
make
cp brisaconfiguration ../BrisaDebExamples_Maemo5/usr/bin/
cd ..

cd BrisaControlPoint_Maemo5/
make distclean
qmake
make
cp brisacontrolpoint ../BrisaDebExamples_Maemo5/usr/bin/
cd ..

cd BrisaDeviceGenerator_Maemo5/
make distclean
qmake
make
cp brisadevicegen ../BrisaDebExamples_Maemo5/usr/bin
cd ..

cd BrisaLightSwitcher_Maemo5/
make distclean
qmake
make
cp brisalightswitcher ../BrisaDebExamples_Maemo5/usr/bin/
cd ..

dpkg-deb -b BrisaDebExamples_Maemo5
mv BrisaDebExamples_Maemo5.deb  ../../packagesDeb/maemo5_fremantle/

