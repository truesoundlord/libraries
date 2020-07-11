export thisdir=$(pwd)
export libdir=/exemples/librairies
export shelldir=/utilitaires/shell/école
export systemdir=/usr/lib64/pkgconfig

# Il est possible d'améliorer ce shell pour "compiler" tout ce qui se trouve dans /exemples/librairies

cd $libdir/linkedlist
tar -cf linkedlist.tar *.h *.c makefile
mv -f linkedlist.tar $thisdir/
cd $libdir/uep_wide
tar -cf uepwide.tar *.h *.c makefile
mv -f uepwide.tar $thisdir/
cd $libdir/uep_debug
tar -cf uepdebugtool.tar *.h *.c makefile
mv -f uepdebugtool.tar $thisdir/
cd $libdir/documentation
tar -cf documentation.tar *.pdf
mv -f documentation.tar $thisdir/
cd $libdir/ueparduino
tar -cf ueparduino.tar *.h *.c makefile
mv -f ueparduino.tar $thisdir/
cd $shelldir
cp -vf executemefirst.sh $thisdir/
tar -cf utilitaires.tar *.sh
mv -f utilitaires.tar $thisdir/
cd $systemdir
tar -cf pkgconfig.tar uep*.pc linkedlist.pc
mv -f pkgconfig.tar $thisdir/
cd $thisdir
tar -cf utilz.tar uepwide.tar linkedlist.tar utilitaires.tar uepdebugtool.tar ueparduino.tar executemefirst.sh documentation.tar pkgconfig.tar
gzip -9 utilz.tar
rm -vf *.tar
echo "done..."

