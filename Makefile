# kompiliert alle *.cpp-Dateien im aktuellen Verzeichnis
# statt erstmaliger Verwendung bitte "./install.sh" aufrufen, dort wird das hier eingeschlossene "vars" erstellt und  ggf. 'make' installiert
# nach "touch entwickeln" wird eine Datei 'version' erstellt, deren Inhalt, eine Versionsnummer, bei jedem Kompiliervorgang um 0.0001 erhoeht wird
# und die in die man_de und man_en-Datei eingebaut wird sowie ueber
#  const double& version=
#  #include version
#  ;
# auch in den c(++)-Quellcode eingebaut werden kann

# Aufrufvarianten:
# "mitpg=1 make <..>" => es wird mit/fuer postgres kompiliert und gelinkt, die Praeprozessordirektive "mitpostgres" wird dem Compiler uebergeben
# "make glei" => der Compiler wird vorher nicht ueberprueft
# "make opt" => optimiere mit -O; kompiliere alles neu
# "make opt2" => optimiere mit -O2; kompiliere alles neu
# "make opt3" => optimiere mit -O3; kompiliere alles neu
# "make opts" => optimiere mit -Os, nach Groesse; kompiliere alles neu
# "make optfast" => optimiere mit -Ofast, nach Ausfuerungsgeschwindigkeit; kompiliere alles neu
# "make optg" => optimiere mit -Og, zum Debuggen; kompiliere alles neu
# "make new" oder "make neu" => kompiliere alles neu
# "make hierclean" => loesche Objekt- und ausfuehrbare Dateien im Kompilierverzeichnis
# "make clean" => loesche Objekt- und ausfuehrbare Dateien einschliesslich installierter Version
# "make confclean" => loescht die Konfigurationsdatei $(INSTEXEC).conf nach Rueckfrage, falls vorhanden
# "make altc" => kompiliere mit -std=gnu++11
# "make anzeig" => zeige Informationen zu Programm, Quelldateien und Compiler an
# "make install => installiere die erstellte Datei in den kuerzesten Pfad aus $PATH, der '/usr/' enthaelt,
#                  installiert man-Seiten aus man_de und man_en, ferner *.html-Dateien und eine README.md-Datei fuer den git-Server
# "make stumm und make installstum: zum Aufruf aus vim"
# "make git => aktualisiert die Datei auf dem zugeordneten git-Server

ICH := $(firstword $(MAKEFILE_LIST))
SRCS = $(wildcard *.cpp)
OBJ = $(SRCS:.cpp=.o)
CFLAGSr=-c -Wall `mysql_config --cflags`
LDFLAGSr=`mysql_config --libs` -ltiff
ifdef mitpg
 CFLAGS=$(CFLAGSr) -I/usr/include/pgsql -Dmitpostgres
 LDFLAGS=$(LDFLAGSr) -lpq
else
 CFLAGS=$(CFLAGSr)
 LDFLAGS=$(LDFLAGSr)
endif
# wenn aus vi aufgerufen, kein unnoetigen Ausgaben, BA=bedingte Ausgabe, BFA=bedingte Fehlerausgabe
BA:=&1
BFA:=2>>fehler.txt
ifdef ausvi
 BA:=/dev/null
endif
PROGRAM=$(shell basename $(CURDIR))
PROGGROSS=`echo $(PROGRAM) | tr a-z A-Z`
#EXPFAD=/usr/local/sbin
EXPFAD := $(shell echo ${PATH} | tr -s ':' '\n' | grep /usr/ | head -n 1)
EXPFAD := $(shell echo ${PATH} | tr -s ':' '\n' | grep /usr/ | awk '{ print length, $$0 }' | sort -n -s | cut -d" " -f2- | head -n1)
EXEC=$(PROGRAM)
INSTEXEC=$(EXPFAD)/$(EXEC)

CCInst=gcc6-c++
-include vars # wird durch install.sh generiert
ifneq ($(shell g++-6 --version >$(KR)),0)
 CCName=g++
# CC=sudo $(CCName)
 CC:=$(CCName)
 CFLAGS:=$(CFLAGS) -std=gnu++11
else
 CCName=g++-6
# CC=sudo $(CCName)
 CC:=$(CCName)
endif
libmcd:=$(libmc)-$(dev)
libmc1d:=$(libmc1)-$(dev)
pgd:=postgresql-$(dev)
slc:=sudo /sbin/ldconfig
# deinstallieren und Ueberschrift vormerken
# uninst=printf '$(UPR)$(1)\nprintf "$$blau%%s$$reset\\n" "$(UPR)$(1)"\n'>>$(UNF);
# uninstd=printf '$(UDPR)$(1)\nprintf "$$blau%%s$$reset\\n" "$(UDPR)$(1)"\n'>>$(UNF); # direkt
# in Protokoll suchen und ...
# sunins=test -f $(UNF)&&grep -q '$(1)' $(UNF)||{ $(call uninst,$(2))};
# suninsd=test -f $(UNF)&&grep -q '$(1)' $(UNF)||{ $(call uninstd,$(2))}; # direkt
# installieren und ...
# Selbes Wort in Protokoll suchen wie deinstallieren
# iunins=$(IPR)$(1) &&{ $(call sunins,$(1),$(2))};
# iuninsd=$(IPR)$(1) &&{ $(call suninsd,$(1),$(2))}; # direkt
# i1unin=$(call iunins,$(1),$(1))
# i1unind=$(call iuninsd,$(1),$(1)) # direkt
# i_unins=$(IP_R)$(1) &&{ $(call sunins,$(1),$(2))};
# i_uninsd=$(IP_R)$(1) &&{ $(call suninsd,$(1),$(2))}; # direkt
# Programm suchen, ggf. installieren und ...
# siunins= $(SPR)$(1)>$(KR)||{ $(call iunins,$(1),$(2))};
# siuninsd= $(SPR)$(1)>$(KR)||{ $(call iuninsd,$(1),$(2))}; # direkt
# i1siun=$(call siunins,$(1),$(1))
# i1siund=$(call siuninsd,$(1),$(1)) # direkt
# si_unins=$(SPR)$(1)>$(KR)||{ $(call i_unins,$(1),$(2))};
# GROFFCHECK:=$(call i1siund,$(pgroff))true

DEPDIR := .d
$(shell mkdir -p $(DEPDIR)>$(KR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d 2>/dev/null

MANP=/usr/share/man
MANPD=${MANP}/de/man1/${PROGRAM}.1.gz
MANPE=${MANP}/man1/${PROGRAM}.1.gz
MANPDH=$(CURDIR)/man_de.html
MANPEH=$(CURDIR)/man_en.html
rot:="\033[1;31m"
gruen:="\033[0;32m"
#blau="\033[0;34;1;47m"
blau:="\033[1;34m"
reset:="\033[0m"

.PHONY: alles
alles: anzeig weiter

.PHONY: glei
glei: anzeig $(EXEC) README.md fertig

.PHONY: opt
opt: CFLAGS += -O
opt: neu

.PHONY: opt2
opt2: CFLAGS+= -O2
opt2: neu

.PHONY: opt3
opt3: CFLAGS+= -O3
opt3: neu

.PHONY: optfast
optfast: CFLAGS+= -Ofast
optfast: neu

.PHONY: optc
opts: CFLAGS+= -Os
opts: neu

.PHONY: optg
optg: CFLAGS+= -Og
optg: neu

.PHONY: altc
altc: CFLAGS+= -std=gnu++11
altc: CCInst=gcc-c++
altc: CCName=g++
altc: opts

.PHONY: neu
new: neu
neu: anzeig hierclean weiter

.PHONY: weiter
weiter: compiler $(EXEC) README.md fertig

# davor:
# git clone ssh://git@github.com/<user>/<reponame>.git
# git clone ssh://git@github.com/libelle17/autofax.git # https://www.github.com/libelle17/autofax
# ssh-keygen -t rsa -b 4096 -f ~/.ssh/id_rsa_git -C "gerald.schade@gmx.de@github.com"
# eval "$(ssh-agent -s)"
# ssh-add ~/.ssh/id_rsa_git
# xclip -sel clip < ~/.ssh/id_rsa_git.pub
# auf http://github.com -> view profile and more -> settings -> SSH and GPG keys -> New SSH key <Titel> <key> einfuegen
git:
#	@git config --global user.name "Gerald Schade"
#	@git config --global user.email "gerald.schade@gmx.de"
	@git config --global push.default simple
	@git add -u
	@git commit -m "Version $$(cat version)"
	@git push

anzeig:
# 'echo -e' geht nicht z.B. in ubuntu
	@printf " %bGNU Make%b, Zieldatei: %b%s%b, vorher:                                      \n" $(gruen) $(reset) $(rot) "$(EXEC)" $(reset) >$(BA)
	@printf " '%b%s%b'\n" $(blau) "$(shell ls -l --time-style=+' %d.%m.%Y %H:%M:%S' --color=always $(EXEC) $(KF))" $(reset) >$(BA)
	@printf " Quelldateien: %b%s%b\n" $(blau) "$(SRCS)" $(reset) >$(BA)
	@printf " Compiler: %b%s%b, installiert als: %b%s%b; Zielpfad fuer '%bmake install%b': %b%s%b\n"\
	  $(blau) "$(CCName)" $(reset) $(blau) "$(CCInst)" $(reset) $(blau) $(reset) $(blau) "'$(EXPFAD)'" $(reset) >$(BA)
	-@$(shell rm -f fehler.txt $(KF))

$(EXEC): $(OBJ)
	-@test -f version || echo 0.1>version; if test -f entwickeln; then awk "BEGIN {print `cat version`+0.00001}">version;\
	else eval "printf \" Datei 'entwickeln' fehlt, lasse die Version gleich\n\" >$(BA)"; fi;
	-@printf " verlinke %s zu %b%s%b ..." "$(OBJ)" $(blau) "$@" $(reset) >$(BA)
	-@df --output=ipcent / |tail -n1|grep - && sudo pkill postdrop; true
ifneq ("$(wildcard $(CURDIR)/man_en)","")
	-@$$(sed -i "s/\(Version \)[^\"]*/\1$$(cat version)/;s/\(\.TH[^\"]*\)\"[^\"]*/\1\"$$(date +'%d.%m.%y')/" man_en)
endif
ifneq ("$(wildcard $(CURDIR)/man_de)","")
	-@$$(sed -i "s/\(Version \)[^\"]*/\1$$(cat version)/;s/\(\.TH[^\"]*\)\"[^\"]*/\1\"$$(date +'%d.%m.%y')/" man_de)
endif
	-@printf " (Version: %b%s%s%b\n " $(blau) "$$(cat version)" ")" $(reset) >$(BA)
	$(CC) $^ -o $@ $(LDFLAGS)
	-@ls .d/*.Td >$(KR) &&{ for datei in .d/*.Td; do mv -f $${datei} $${datei%.Td}.d; done;};true
	$(shell touch *.o $${EXEC})

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	@printf " kompiliere %b%s%b: " $(blau) "$<" $(reset) >$(BA)
#	-@if ! test -f instvz; then printf \"$$(getent passwd $$(logname)|cut -d: -f6)\">instvz; fi;
	-@if ! test -f instvz; then printf \"$$(pwd)\">instvz; fi;
	-$(CC) $(DEPFLAGS) $(CFLAGS) -c $< $(BFA)
	-@sed -i 's/version //g' $(DEPDIR)/*.Td
	-@if test -s fehler.txt; then vi +0/error fehler.txt; else rm -f fehler.txt; fi;
#	-@$(shell $(POSTCOMPILE))
	@if test -s fehler.txt; then false; fi;

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

.PHONY: compiler
compiler:
	@printf " Untersuche Compiler ...\r" >$(BA)
#	@printf " CCName: %b%s%b                  \n" $(blau) "${CCName}" $(reset)
#	@printf " CCInst: %b%s%b\n" $(blau) "$(CCInst)" $(reset)
ifeq ('$(SPR)','')
$(warning Variable 'SPR' not assigned, please call './install.sh' before!)
$(error Variable 'SPR' nicht belegt, bitte vorher './install.sh' aufrufen!)
endif
#	@which $(CCName)>$(KR)||{ $(REPOS)for P in $(COMP);do $(SPR)$$P||{ $(IP_R)$$P;grep -q "$$P" $(UNF)||\
#	 printf '$(UPR)$$P;$(urepo)\nprintf "$$blau%%s$$reset\\n" "$(UPR)$$P;$(urepo)"\n'>>$(UNF);};done;};
#	@which $(CCName)>$(KR)||{ $(REPOS)$(foreach PG,$(COMP),$(call si_unins,$(PG),$(PG);$(urepo)))}
	@./configure inst "$(CCName)" "$(COMP)"
#	@if { $(slc);! $(slc) -p|grep -q "libmysqlclient.so ";}||! test -f /usr/include/mysql/mysql.h;then $(call iunins,$(libmcd))fi
	-@test -f /usr/include/mysql/mysql.h>$(KR)&&\
	find $$(find /usr -maxdepth 1 -name "lib*" $(KF)|sort -r) -name "libmysqlclient.so" -print -quit $(KF)|grep ''>$(KR)|| \
	./configure inst _ "$(libmcd)" verbose
#	$(call i1unin,$(libmcd))
	-@test -f /usr/include/mysql/mysql.h>$(KR)&&\
	find $$(find /usr -maxdepth 1 -name "lib*" $(KF)|sort -r) -name "libmysqlclient.so" -print -quit $(KF)|grep ''>$(KR)|| \
	./configure inst _ "$(libmc1d)" verbose
# $(call i1unin,$(libmc1d))
#	@[ -z $$mitpg ]||$(SPR) $(pgd)>$(KR)||{ $(IPR)$(pgd);grep -q '$(pgc)' $(UNF)||printf '$(UPR)$(pgd)\necho $(UPR)$(pgd)\n'>>$(UNF);$(slc);};
#	-@[ -z $$mitpg ]||$(SPR) $(pgd)>$(KR)||{ $(call i1unin,$(pgd))$(slc);};
	-@[ -z $$mitpg ]||$(SPR) $(pgd)>$(KR)||{ ./configure inst _ "$(pgd)" verbose;./configure inst _ "$(slc)" verbose;};
#	@test -f /usr/include/tiff.h&&test -f /usr/lib64/libtiff.so||{ $(UPR)$(LT) $(KF);$(IPR)$(LT);grep -q '$(LT)' $(UNF)||printf '$(UPR)$(LT)\n echo $(UPR)$(LT)\n'>>$(UNF);}
	-@find /usr/include -name tiff.h -print -quit $(KF)|grep ''>$(KR)&&\
	find $$(find /usr -maxdepth 1 -name "lib*" $(KF)|sort -r) -name "libtiff.so" -print -quit $(KF)|grep ''>$(KR)||\
	./configure inst _ "$(LT)" verbose
#	{ $(UDPR)$(LT) $(KF);$(call i1unind,$(LT))}
	-@if test -n "$(LT5)"; then { \
	find /usr/include -name tiff.h -print -quit $(KF)|grep ''>$(KR)&&\
	find $$(find /usr -maxdepth 1 -name "lib*" $(KF)|sort -r) -name "libtiff.so" -print -quit $(KF)|grep ''>$(KR)||\
	{ ./configure inst _ "$(LT5)" verbose;};}; else true; fi
#	{ $(call i1unind,$(LT5))};}; else true; fi
# ggf. Korrektur eines Fehlers in libtiff 4.0.7, notwendig fuer hylafax+, 17.1.17 in Programm verlagert
	@printf "                         \r" >$(BA)

ifneq ("$(wildcard $(CURDIR)/man_de)","")
ifneq ("$(wildcard $(CURDIR)/man_en)","")
README.md: ${MANPEH} ${MANPDH} 
	-@rm -f README.md
	-@echo "<h3>Manual: 1) <a href=\"#english_E\">english</a>, 2) <a href=\"#deutsch_D\">deutsch (unten anschließend)</a></h3>" > README.md
	-@sed -n '20,$$p' man_en.html >> README.md 
	-@sed -n '20,$$p' man_de.html >> README.md 
	@printf " %b%s%b neu aus %b%s%b und %b%s%b erstellt\n" $(blau) "README.md" $(reset) $(blau) "man_en" $(reset) $(blau) "man_de" $(reset)

else
README.md: ${MANPDH}
	-@rm -f README.md
	-@sed -n '20,$$p' man_de.html >> README.md 
	@printf " %b%s%b neu aus %b%s%b erstellt\n" $(blau) "README.md" $(reset) $(blau) "man_de" $(reset)
install: $(INSTEXEC) ${MANPD}
endif
else
ifneq ("$(wildcard $(CURDIR)/man_en)","")
README.md: ${MANPEH}
	-@rm -f README.md
	-@sed -n '20,$$p' man_en.html >> README.md 
	@printf " %b%s%b neu aus %b%s%b erstellt\n" $(blau) "README.md" $(reset) $(blau) "man_en" $(reset)
install: $(INSTEXEC) ${MANPE} 
else
README.md: 
install: $(INSTEXEC)
endif
endif

.PHONY: stumm
stumm: BA:=/dev/null
stumm: BFA:=
# die folgenden Farbenzuweisungen wirken nicht
stumm: rot:=""
stumm: blau:=""
stumm: gruen:=""
stumm: reset:=""
stumm: alles

.PHONY: installstumm
installstumm: stumm install

.PHONY: install
install: $(INSTEXEC) ${MANPE} ${MANPD} 

$(INSTEXEC): $(EXEC)
	@printf " Kopiere Programmdatei: %b%s%b -> %b%s%b\n" $(blau) "$(EXEC)" $(reset) $(blau) "$(INSTEXEC)" $(reset) >$(BA)
	-@sudo pkill $(EXEC) $(KF); sudo pkill -9 $(EXEC) $(KF); sudo cp -p "$(EXEC)" "$(INSTEXEC)"

$(CURDIR)/man_en: $(EXEC)
	-@TMP=tmp_opt;./$(EXEC) -lg e -h|sed -e 's/^/.br\n/;s/\[[01];3.m/\\fB/g;s/\[0m/\\fR/g;'|sed ':a;N;$!ba'>$$TMP; \
	nlinit=`echo 'nl="'; echo '"'`; eval "$$nlinit"; \
	von=".SH OPTIONS"; bis=".SH FUNCTIONALITY"; sed -i.bak "/$$von/,/$$bis/{/$$von/{n;p;r $$TMP$${nl}};/$$bis/p;d}" man_en
$(CURDIR)/man_de: $(EXEC)
	-@TMP=tmp_opt;./$(EXEC) -lg d -h|sed -e 's/^/.br\n/;s/\[[01];3.m/\\fB/g;s/\[0m/\\fR/g;'|sed ':a;N;$!ba'>$$TMP; \
	nlinit=`echo 'nl="'; echo '"'`; eval "$$nlinit"; \
	von=".SH OPTIONEN"; bis=".SH FUNKTIONSWEISE"; sed -i.bak "/$$von/,/$$bis/{/$$von/{n;p;r $$TMP$${nl}};/$$bis/p;d}" man_de

ifneq ("$(wildcard $(CURDIR)/man_en)","")
${MANPE}: $(CURDIR)/man_en
	@printf " "
	-sudo mkdir -p $(MANP)/man1
	@printf " "
	-sudo gzip -c $(CURDIR)/man_en >${PROGRAM}.1.gz
	@printf " "
	-sudo mv ${PROGRAM}.1.gz ${MANPE}
${MANPEH}: $(CURDIR)/man_en 
	-@./configure inst groff "$(pgroff)"
	-@rm -f man_en.html
	-@sed -e 's/Ä/\&Auml;/g;s/Ö/\&Ouml;/g;s/Ü/\&Uuml;/g;s/ä/\&auml;/g;s/ö/\&ouml;/g;s/ü/\&uuml;/g;s/ß/\&szlig;/g;/\.SH FUNCTIONALITY/,/^\.SH/ {s/\.br/.LP\n\.HP 3/g};/\.SH IMPLICATIONS/,/^\.SH/ {s/\.br/\.LP\n\.HP 3/g}' man_en | groff -mandoc -Thtml | sed "s/&amp;/\&/g;s@<h1 align=\"center\">man@<h1 align=\"center\">$(PROGGROSS) (Version $$(cat version)) - english<a name=\"english\"></a>@g;s/\(<a \(href\|name\)=\"[^\"]*\)/\1_E/g" > man_en.html
	@printf " %b%s%b neu aus %b%s%b erstellt\n" $(blau) "man_en.html" $(reset) $(blau) "man_en" $(reset)
endif

ifneq ("$(wildcard $(CURDIR)/man_de)","")
${MANPD}: $(CURDIR)/man_de 
	-@TMP=tmp_opt;./$(EXEC) -h|sed -e 's/^/.br\n/;s/\[[01];3.m/\\fB/g;s/\[0m/\\fR/g;'|sed ':a;N;$!ba'>$$TMP; \
	nlinit=`echo 'nl="'; echo '"'`; eval "$$nlinit"; \
	von=".SH OPTIONEN"; bis=".SH FUNKTIONSWEISE"; sed -i.bak "/$$von/,/$$bis/{/$$von/{n;p;r $$TMP$${nl}};/$$bis/p;d}" man_de
	@printf " "
	-sudo mkdir -p $(MANP)/de/man1
	@printf " "
	-sudo gzip -c $(CURDIR)/man_de >${PROGRAM}.1.gz
	@printf " "
	-sudo mv ${PROGRAM}.1.gz ${MANPD}
${MANPDH}: $(CURDIR)/man_de 
	-@./configure inst groff "$(pgroff)"
	-@rm -f man_de.html
	-@sed -e 's/Ä/\&Auml;/g;s/Ö/\&Ouml;/g;s/Ü/\&Uuml;/g;s/ä/\&auml;/g;s/ö/\&ouml;/g;s/ü/\&uuml;/g;s/ß/\&szlig;/g;/\.SH FUNKTIONSWEISE/,/^\.SH/ {s/\.br/.LP\n\.HP 3/g};/\.SH AUSWIRKUNGEN/,/^\.SH/ {s/\.br/\.LP\n\.HP 3/g}' man_de | groff -mandoc -Thtml | sed "s/&amp;/\&/g;s@<h1 align=\"center\">man@<h1 align=\"center\">$(PROGGROSS) (Version $$(cat version)) - deutsch<a name=\"deutsch\"></a>@g;s/\(<a \(href\|name\)=\"[^\"]*\)/\1_D/g" > man_de.html
#	-@rm -f README.md
	@printf " %b%s%b neu aus %b%s%b erstellt\n" $(blau) "man_de.html" $(reset) $(blau) "man_de" $(reset)
endif

.PHONY: fertig
fertig:
	@printf " Fertig mit %s, nachher:\n" "$(ICH)"
	@printf " '%b%s%b'\n" $(blau) "$(shell ls -l --time-style=+' %d.%m.%Y %H:%M:%S' --color=always $(EXEC))" $(reset)

.PHONY: clean
clean: hierclean distclean

.PHONY: hierclean
hierclean: 
	@printf " Bereinige ...\r" >$(BA)
	@$(shell rm -f $(EXEC) $(OBJ) .d/* $(KF))
	@$(shell sudo rm -f ${MANPEH} ${MANPDH} $(KF))
	@printf " %b%s,%s,%s,%s%b geloescht!\n" $(blau) "$(EXEC)" "$(OBJ)" "$(MANPEH)" "$(MANPDH)" $(reset)

.PHONY: distclean
distclean:
	-@$(shell sudo rm -f $(INSTEXEC) ${MANPD} ${MANPE} $(KF))
	-@printf "%b%s%b geloescht!\n" $(blau) "$(INSTEXEC)" $(reset)

.PHONY: confclean
confclean:
	-@sh -c 'D=$(INSTEXEC).conf;test -f $$D &&{ printf "Wollen Sie \"%b$$D%b\" wirklich loeschen (j/n)?" $(blau) $(reset); read answer; if echo "$$answer"|grep -iq "^j\|^y";then rm -r "$$D">$(KR)&&printf \"%b%s%b\" geloescht!\n" $(blau) "$(INSTEXEC).conf" $(reset); fi;true;} ||{ printf "Konfigurationsdatei \"%b$$D%b\" fehlt.\n" $(blau) $(reset);}'

.PHONY: uninstall
uninstall: distclean
	-@tac $(UNF)>tmp_$(UNROH)&& sudo blau=$(blau) reset=$(reset) bash tmp_$(UNROH) # uninstallinv von hinten nach vorne abarbeiten
	-@printf "Fertig mit uninstall!\n"

#rufe:
#	printf '$(UPR)$(pgroff)\nprintf "$$blau%%s$$reset\\n" "$(UPR)$(pgroff)"\n'>>gtest
#	-@sudo blau=$(blau) reset=$(reset) sh gerufen

#teste:
#	sh -c 'VAR="lange\
#	 Varible"; echo $$VAR'
#	 echo $(GROFFCHECK)
#	 @echo $(COMP)
#	 @echo $(words $(COMP))
#	 @printf "$$blau$(urepo)$$reset\n"
#	 for P in $(COMP); do echo $$P; VAR="$$P"; echo $$VAR; $(call uninst,$P) done
#	 $(foreach VAR,$(COMP),echo $(VAR)\n)
#	 @$(foreach VAR,$(COMP),$(call sunins,$(VAR),$(VAR)))
#	$(foreach PG,$(COMP),$(call si_unins,$(PG),$(PG);$(urepo)))
#	 echo $(call uninst,$(COMP))
#	sudo blau=$(blau) reset=$(reset) sh gtest
version: dovers
dovers:
	@mkdir -p res
	@cp -a $(DATEIEN) res
	@git rm --cached --ignore-unmatch $(DATEIEN)
	@git commit -m "nix"
	@git add $(DATEIEN)
	@git rm --cached vgb.cpp uninstallinv
	make opts && sudo make install && make git

zussetz:
	-@TMP=tmp_opt;./$(EXEC) -h|sed -e 's/^/.br\n/;s/\[[01];3.m/\\fB/g;s/\[0m/\\fR/g;'|sed ':a;N;$!ba'>$$TMP; \
	nlinit=`echo 'nl="'; echo '"'`; eval "$$nlinit"; \
	von=".SH OPTIONEN"; bis=".SH FUNKTIONSWEISE"; sed -i.bak "/$$von/,/$$bis/{/$$von/{n;p;r $$TMP$${nl}};/$$bis/p;d}" man_de
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
