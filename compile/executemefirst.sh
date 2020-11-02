#!/bin/bash

export libfolder=/exemples/librairies
export shelldir=/utilitaires/shell
export systemlib=/usr/lib64/pkgconfig

echo "Installation des librairies..."

mv /exemples/librairies /exemples/lib_old
rm -f /usr/bin/backuplibz
rm -f /usr/bin/installcssmlibs
rm -f /usr/bin/purge

echo "Création des répertoires..."

mkdir -pv $libfolder
mkdir -pv $libfolder/uep_wide
mkdir -pv $libfolder/linkedlist
mkdir -pv $libfolder/uep_debug
mkdir -pv $libfolder/documentation
mkdir -pv $libfolder/ueparduino

echo "...décompression des fichiers..."

if ! test -e utilz.tar.gz;
then
	echo "Assurez-vous que le fichier utilz.tar.gz soit dans le répertoire courant...";
	sleep 3;
	return;
else
	tar -xf utilz.tar.gz
fi

mv uepwide.tar $libfolder/uep_wide
mv linkedlist.tar $libfolder/linkedlist
mv uepdebugtool.tar $libfolder/uep_debug
mv ueparduino.tar $libfolder/ueparduino
mv documentation.tar $libfolder/documentation
mv pkgconfig.tar $systemlib

echo "...copie des scripts et installation..."
mkdir -p $shelldir
mv utilitaires.tar $shelldir

cd $libfolder/uep_wide
tar -xf uepwide.tar
echo "...compilation de uep_wide..."
make sharedlibrary
echo "...OK !"
rm -f uepwide.tar

cd $libfolder/linkedlist
tar -xf linkedlist.tar
echo "...compilation de linkedlist..."
make sharedlibrary
echo "...OK !"
rm -f linkedlist.tar

cd $libfolder/uep_debug
tar -xf uepdebugtool.tar
echo "...compilation de debug tools..."
make sharedlibrary
echo "...OK !"
rm -f uepdebugtool.tar

cd $libfolder/ueparduino
tar -xf ueparduino.tar
echo "...compilation des outils pour l'arduino..."
make sharedlibrary
echo "...OK !"
rm -f ueparduino.tar

cd $systemlib
tar -xf pkgconfig.tar
echo "...décompression des fichiers *.pc..."
rm -f pkgconfig.tar

cd $libfolder/documentation
tar -xf documentation.tar
echo "...OK !"
rm -f documentation.tar

cd $shelldir
tar -xf utilitaires.tar
echo "Est-ce que \"zenity\" est installé ??"
if ! test -e /usr/share/zenity;
then
	echo "Non ! Je l'installe ^^";
	zypper -qn install zenity;
else
	echo "Ca va aller...";
fi

ln -s $shelldir/backuplibz.sh /usr/bin/backuplibz
echo "INFO: pour faire un backup des librairies taper \"backuplibz\"..."
ln -s $shelldir/cssm_installlibz.sh /usr/bin/installueplibs
echo "INFO: pour installer les liens symboliques pour un nouveau projet, tapez \"installueplibs\"... [obsolète depuis 2017]"
ln -s $shelldir/purge.sh /usr/bin/purge
echo "INFO: pour purger un répertoire de tous les fichiers de sauvegarde, tapez \"purge\"..."
rm -f utilitaires.tar
echo "...installation terminée !!"
