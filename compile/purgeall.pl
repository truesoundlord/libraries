#! /usr/bin/perl

# De Zordi Dimitri (soundlord@gmail.com)

# Which objects we gonna use...

use Term::ProgressBar;	# cpan Term::ProgressBar.pm to install it... 
use File::Find;
use Time::HiRes qw(usleep nanosleep);
use Term::Screen;

my $parametres=@ARGV;

my $targetsourcedir;
my $flagexecutable='false';
my $flagrecursive='false';
my $flagobjects='false';
my $cpt;
my $fichiersaeffacer=0;
my $flagdeleteall='false';
my $flagparamok='false';
my $flagrecursive='false';

my $nbBackup=0;
my $nbExecutables=0;
my $nbObjects=0;

my $terminal=new Term::Screen;

# Reads the parameters given at command line...

if($parametres eq 0)
{
  PrintUsage();
}

$terminal->clrscr();
$terminal->at(0,0);
  

for($cpt=0;$cpt<$parametres;$cpt++)
{
  $_=$ARGV[$cpt];
  if(/-d/)	
  {
    $targetsourcedir=$ARGV[++$cpt];
    if($targetsourcedir eq "\/")  # interdit de traiter le répertoire '/' d'un système
    {
     exit; 
    }
    if(-e $targetsourcedir)
    {
     $terminal->at(0,0);
     print("Source folder: $targetsourcedir\n");
     $flagparamok='true';
    }
    else
    {
      print("Nom de répertoire invalide...($targetsourcedir)\n");
    }
  }
  if(/-e/)
  {
    $flagexecutable='true';
    print("\nPurge executables: $flagexecutable\n");
  }
  if(/-o/)
  {
   $flagobjects='true';
   print("\nPurge .o files: $flagobjets\n");
  }
  if(/-f/ || /-s/)
  {
   # DANGEREUX !!
   $flagdeleteall='true';
  }
}

if($flagparamok eq 'false')
{
 PrintUsage();
}

usleep(600000);
#$terminal->clrscr();
$terminal->at(2,0);

my @stackfichiers;
	
find({wanted => \&jeveux, follow => 1, follow_skip => 2, no_chdir=>0} , $targetsourcedir);
$fichiersaeffacer=@stackfichiers;
print("Nombre de fichiers à effacer: $fichiersaeffacer ");
print "[Backups: ";
$terminal->bold();
print $nbBackup;
$terminal->normal();
print "][Executables: ";
$terminal->bold();
print $nbExecutables;
$terminal->normal();
print "]";
print "[Objects: ";
$terminal->bold();
print $nbObjects;
$terminal->normal();
print "]";

return if $fichieraeffacer eq 0;					# we don't need to continue if there is NO files to delete...

# ATTENTION: unlink ne fait pas appel à la commande "rm" du shell, du coup il faut
# absolumment confirmer l'effacement des fichiers de la pile... 

# Vider la pile des fichiers à effacer...

for(0..$fichiersaeffacer-1)								# forme de for valide en Perl... 
{
	$target=pop @stackfichiers;							# on enlève un élément de la pile et on le met dans $target...
	if($flagdeleteall !~ /true/)						# si on a pas préalablement sélectionné (A)ll ou (T)out...
	{
		$terminal->at(4,0);
		print "\033[2K";
		print "Effacement de : ";
		$terminal->bold();
		print "$target";
		$terminal->normal();
		print " (Y/O,N/n,A/T) ?";
		$terminal->at(5,0);
		print("/!\\ BEWARE: selecting A or T will erase ALL the files in the stack /!\\");
		$terminal->at(6,0);
		$reponse=getc(STDIN);										# get a single character from STDIN (comme en langage C)
	}
	else
	{
		$reponse='Y';
	}

	# ATTENTION: pour l'instant le script n'est pas encore tout à fait au point donc le unlink est mis en commentaires...
	# ...jusqu'à ce que le script ait été testé en profondeur...

	if($reponse=~/Y|O|A|T/)									# vérifier si $reponse contient Y ou O ou A ou T
	{
		unlink $target;
		$terminal->at(4,0);
		print "\033[2K";
		$terminal->bold();
		print $target;
		$terminal->normal();
		print " deleted...";
	}
	if($reponse=~/A|T/)											# faire en sorte que le message ne s'affiche plus jamais...
	{
		$flagdeleteall='true';
	}
	if($reponse=~/N|n/)
	{
		$terminal->at(4,0);
		print "\033[2K";
		$terminal->bold();
		print "$target"; 
		$terminal->normal();
		print " skipped...";
	}

	$terminal->at(7,0);
	my $progressbar = Term::ProgressBar->new($fichiersaeffacer-1);
	$progressbar->update($_);								# mettre à jour la barre de progression...
	
	usleep(400000);													# afin de pouvoir lire les infos il faut une petite temporisation...
} 
$terminal->at(7,0);
print("\n");

# Fonction qui est appelée par le programme Find et du coup par le gestionnaire de barre de progression

sub jeveux
{
  if($File::Find::dir=~ /src/)					# never compute src
  {   
   return;
  }
  
  if($File::Find::dir=~ /bin/)					# never compute bin
  {   
   return;
  }
  
  if($File::Find::dir=~ /release/)			# never compute release
  {   
   return;
  }
  
  if($File::Find::dir=~ /librairies/)	# never compute librairies
  {   
   return;
  }
  
  if($File::Find::dir=~ /mp3/)	# never compute mp3
  {   
   return;
  }
    
  if(/~$/)																				# si le fichier se termine par '~' 
  {
   $nbBackup++;
   push @stackfichiers, $File::Find::name;				# on place le fichier dans la pile...
   return;
  } 
  
  # TODO: lire à partir d'un fichier les extensions à exclure, là c'est juste pour le test...
  
  # TODO: à terme proposer d'ajouter le nom ou l'extension d'un fichier si il ne doit pas être effacé
  # dans un fichier...
  
  # $trucdufichier=...
  # if(/$trucdufichier/)
  # {
  #		$expression=1;
  #	}
    
  my $expression = (!/sh$/ && !/png$/ && !/jpg$|JPG$/
					&& !/gif$/ && !/wav$/ && !/ogg$/ && !/odg$/ && !/bmp$/ && !/AVI$|avi$/ 
					&& !/txt$|TXT$/ && !/bin$/ && !/xcf$/ && !/gz$/ 
					&& !/zip$/ && !/jar$/ && !/class$/); 
  
  if($flagexecutable eq 'true')										
  {
   if(-x 	&& !-d && !-l && -B && $expression)
	 {
    $nbExecutables++;
    push @stackfichiers, $File::Find::name;				# on place le fichier dans la pile...
    return;
   }
  }
  if($flagobjects eq 'true')
  {
   if(/.o$/ && !-x)
   {
    $nbObjects++;
    push @stackfichiers, $File::Find::name;
    return;
   }
  }
	
}

sub PrintUsage()
{
	$terminal->clrscr();
	$terminal->at(0,0);
  print "USAGE: purgeall -d [folder] [-e] [-o]\n";
  $terminal->at(1,0);
  print "-e: option that let the script purge executables in the folder...\n";
  $terminal->at(2,0);
  print "-o: option that let the script purge objects '.o' files in the folder...\n";
  $terminal->at(3,0);
  exit;
}



