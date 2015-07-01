cd BrisaBinaryLight_Maemo4/
make distclean
qmake
make
cp brisabinarylight ../BrisaDebExamples_Maemo4/usr/bin/
cd ..

cd BrisaConfiguration_Maemo4/
make distclean
qmake
make
cp brisaconfiguration ../BrisaDebExamples_Maemo4/usr/bin/
cd ..

cd BrisaControlPoint_Maemo4/
make distclean
qmake
make
cp brisacontrolpoint ../BrisaDebExamples_Maemo4/usr/bin/
cd ..

cd BrisaDeviceGenerator_Maemo4/
make distclean
qmake
make
cp brisadevicegen ../BrisaDebExamples_Maemo4/usr/bin
cd ..

cd BrisaLightSwitcher_Maemo4/
make distclean
qmake
make
cp brisalightswitcher ../BrisaDebExamples_Maemo4/usr/bin/
cd ..

dpkg-deb -b BrisaDebExamples_Maemo4
mv BrisaDebExamples_Maemo4.deb  ../../packagesDeb/maemo4_diablo/

