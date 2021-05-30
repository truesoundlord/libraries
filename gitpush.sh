#! /bin/bash
export SITE="https://github.com/truesoundlord/libraries"
export LOCALFLDR="/librairies/*"
export GITFLDR="/root/NetBeansProjects/UEP/"
echo "Faire remonter les modifications sur le site $SITE !!"
mkdir -p $GITFLDR
cd $GITFLDR
echo "Nous sommes dans le répertoire $(pwd)"
git clone $SITE libraries
cd libraries
/usr/bin/cp -Rfv $LOCALFLDR .
cp -f /utilitaires/shell/école/*.sh $GITFLDR/compile
purgeall -d uep_wide -e -o -s
purgeall -d linkedlist -e -o -s
purgeall -d uep_debug -e -o -s
purgeall -d ueparduino -e -o -s
purgeall -d SDL2 -e -o -s
purgeall -d gfxContainer -e -o -s
git add .
git status
git commit -a -m "Version $(date +%m).$(date +%Y)"
git push
echo "Libération de l'espace non nécessaire..."
/usr/bin/rm -Rf $GITFLDR
echo "Terminé !!"


