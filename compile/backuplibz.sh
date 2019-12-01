export thisdir=$(pwd)
export libdir=/exemples/librairies
export shelldir=/utilitaires/shell/école
export systemdir=/usr/lib64/pkgconfig

# Il est possible d'améliorer ce shell pour "compiler" tout ce qui se trouve dans /exemples/librairies

cd $libdir/linkedlist
tar -cf linkedlist.tar *.h *.c makefile
mv -f linkedlist.tar $thisdir/
cd $libdir/cssm_wide
tar -cf cssmwide.tar *.h *.c makefile
mv -f cssmwide.tar $thisdir/
cd $libdir/cssm_debug
tar -cf cssmdebugtool.tar *.h *.c makefile
mv -f cssmdebugtool.tar $thisdir/
cd $libdir/documentation
tar -cf documentation.tar *.pdf
mv -f documentation.tar $thisdir/
cd $libdir/cssmarduino
tar -cf cssmarduino.tar *.h *.c makefile
mv -f cssmarduino.tar $thisdir/
cd $shelldir
cp -vf executemefirst.sh $thisdir/
tar -cf utilitaires.tar *.sh
mv -f utilitaires.tar $thisdir/
cd $systemdir
tar -cf pkgconfig.tar cssm*.pc linkedlist.pc
mv -f pkgconfig.tar $thisdir/
cd $thisdir
tar -cf utilz.tar cssmwide.tar linkedlist.tar utilitaires.tar cssmdebugtool.tar cssmarduino.tar executemefirst.sh documentation.tar pkgconfig.tar
gzip -9 utilz.tar
rm -vf *.tar
echo "done..."
