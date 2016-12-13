#!/bin/sh
P=autofax
HOSTER=github.com
ACC=libelle17
rot="\e[1;31m"
blau="\e[1;34m"
reset="\e[0m"
aPWD=`pwd`
nPWD=${PWD##*/}
IPR="nix"
SPR="nix"

# Installationsprogramm ermitteln
getIPR() {
	{ which zypper  >/dev/null 2>&1 && IPR="zypper -n --gpg-auto-import-keys in ";}||
	{ which apt-get >/dev/null 2>&1 && IPR="apt-get --assume-yes install ";}||
	{ which dnf     >/dev/null 2>&1 && IPR="dnf -y install ";}||
	{ which yum     >/dev/null 2>&1 && IPR="yum -y install ";}
	{ which rpm >/dev/null 2>&1 && SPR="rpm -q ";}||
	{ which dpkg >/dev/null 2>&1 && SPR="dpkg -s ";}
}

# Gruppen, deren Mitglieder sudo aufrufen koennen
SUG="root\|sudo\|wheel\|admin";

# hier geht's los
getIPR;
{ which sudo >/dev/null && id -Gzn $USER|grep -qw "$SUG";}||{ 
	printf "Must allow '$blau$USER$reset' to call '${blau}sudo$reset'. Please enter ${blau}root$reset's password if asked:\nMuss '$blau$USER$reset' den Aufruf von '${blau}sudo$reset' ermoeglichen. Bitte geben Sie bei der Frage das Passwort von '${blau}root$reset' ein:\n";
	su -c "$IPR sudo; usermod -aG $(cut -d: -f1 /etc/group|grep "$SUG"|head -n 1) "$USER";"||exit
	printf "Please log out and in again, change to the directory '$blau$PWD$reset' and then call this script again!\nBitte loggen Sie sich jetzt aus und nochmal ein, wechseln Sie nach '$blau$PWD$reset' und rufen Sie dieses Script dann nochmal auf!\n";
	exit;
}
$SPR make >/dev/null 2>&1 ||{
	echo Installing/ Installiere 'make' ....;
  id su >/dev/null 2>&1 && { su -c "$IPR make;";true;} || sudo $IPR make;
}
$SPR make >/dev/null || exit
# wenn schon im Verzeichnis $P und wenn es dort einige notwendige Dateien gibt, dann nur kompilieren und installieren
[ $nPWD = $P -a -f Makefile -a -f $P.cpp ]&&{
	printf "${rot}Installing/ Installiere ... $reset\n"
	true;
}||{
	printf "${rot}Downloading/ Lade runter ... $reset\n"
	T=$P.tar.gz;
	cd ~; 
	wget https://$HOSTER/$ACC/$P/archive/master.tar.gz -O "$T" && 
		tar xpvf $T && 
		rm -f $T && {
			VORIGE=$(ls -d ~/${P}_* 2>/dev/null | cut -d"_" -f2 | sort -nr);
			for i in $VORIGE; do 
			case $i in ""|*[!0-9]*);; *) 
				j=$((i+1)); 
				echo mv ${P}_$i ${P}_$j; 
				mv ${P}_$i ${P}_$j; 
				esac; 
				done; 
				test -e $P && mv $P ${P}_1; 
				mv $P-master $P && 
					cd $P;
		}|| exit;
}
make &&
sudo make install; erg=$?;
[ $erg = 0 ] && farbe=$blau || farbe=$rot;
printf "Result code/ Ergebniscode: ${farbe}$erg${reset}\n"
