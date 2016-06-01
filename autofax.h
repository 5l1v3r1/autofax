#include <regex.h> // regex_t, regex, regcomp, regexec

enum FaxTyp:uchar {capi=1,hyla};
enum FxStat:uchar {init,wartend,gesandt,gescheitert,fehlend,woasined};

class zielmustercl; // fuer die Verteilung der erfolgreich gefaxten Dateien auf verschiedene Dateien
class fxfcl; // Faxfile
class fsfcl; // Faxsendfile
class paramcl; // Programmparameter
string zielname(const string& qdatei, const string& zielverz,uchar wieweiterzaehl, string* zieldatei, int obverb, int oblog);
string zielname(const string& qdatei, zielmustercl *zmp,uchar wieweiterzaehl, string* zieldatei, int obverb, int oblog);
void dorename(const string& quelle, const string& ziel, uint *vfehler, int obverb, int oblog);
string verschiebe(const string& qdatei, const string& zielvz, uint *vfehler, uchar wieweiterzaehl, int obverb,int oblog);
void verschiebe(const string& qdatei, zielmustercl *zmp, uint *vfehler, uchar wieweiterzaehl, int obverb, int oblog);
string kopiere(const string& qdatei, const string& zieldp, uint *kfehler, uchar wieweiterzaehl, int obverb=0,int oblog=0);
void kopiere(const string& qdatei, zielmustercl *zmp, uint *kfehler, uchar wieweiterzaehl, int obverb=0, int oblog=0);
void pruefprocgettel3(DB *Myp, const string& usr, const string& pwd, const string& host, int obverb, int oblog);
void faxemitC(DB *My, const string& spooltab, fsfcl *fsfp, paramcl *pmp, int obverb, int oblog);
void faxemitH(DB *My, const string& spooltab, fsfcl *fsfp, paramcl *pmp, int obverb, int oblog);
int pruefcapi(paramcl *pmp, int obverb, int oblog);
void kuerzevtz(string *vzp);
pid_t PIDausName(const char* PName, uchar klgr, uchar exakt, int obverb, int oblog);
void getSender(paramcl *pmp,const string& faxnr, string *getnamep, string *bsnamep,int obverb=0,int oblog=0);
int tuloeschen(const string& zuloe,int obverb, int oblog);

// Service aus SystemD
class servc {
  public:
    uchar servicelaeuft=0, serviceda=0;
    string sname,ename; // ausgefuehrte Datei
    servc(string vsname,string vename): sname((vsname.empty()?vename:vsname)),ename(vename) {}
    uchar spruef(const string& sbez,uchar obfork,const string& sexec, const string& CondPath, const string& After, const string& wennnicht0,
        int obverb=0,int oblog=0);
    int obslaeuft(int obverb, int oblog);
    int restart(int obverb, int oblog);
    int enableggf(int obverb,int oblog);
};

// Steuerung der Abspeicherung gesendeter Faxe je nach Muster
class zielmustercl 
{
  // beim letzten Element muss ziel leer sein!
  private:
    string muster;
  public:
    string ziel;
    regex_t regex;
    // wird nur in Vorgaben gebraucht:
    zielmustercl(const char * const muster,const char * const ziel);
    zielmustercl();
    int kompilier();
    int setzemuster(const string& vmuster);
    const string& holmuster();
    int obmusterleer();
}; // class zielmustercl

class fxfcl // Faxfile
{
  public:
    string npdf; // nicht-PDF
    string spdf; // schon-PDF
    unsigned prio; // Prioritaet der Fax-Programme: 0=keine,1=Capi, 2=Hyla
    fxfcl(string npdf,string spdf,unsigned prio): npdf(npdf),spdf(spdf),prio(prio) {}
    fxfcl(string spdf,unsigned prio): npdf(""),spdf(spdf),prio(prio) {}
    fxfcl() {}
};

class fsfcl : public fxfcl // Faxsendfile
{
  public:
    string id;
    string telnr;
    string capisd; // capispooldatei
    int capids;  //capidials
    string hylanr; // hylanr
    int hylads; // hyladials
    uchar fobcapi; // ob es jetzt mit Capi weggefaxt werden muss
    uchar fobhyla; // ob es jetzt mit Hyla weggefaxt werden muss
    string original; // base_name(spdf)
    string origvu;   // base_name(npdf)
    string cspf;     // capispoolpfad
    string cdd;      // cdateidatum
    string cdials;   // capidials
    string hdials;   // hyladials
    string hdd;      // hdateidatum
    string sendqgespfad;
    string hgerg;  // hyla_gescheitert_erg
    int hversuzahl;
    FxStat capistat=init;// 1=wartend, 2=gesandt, 3=gescheitert, 4=fehlend (in spool keine Capi-Datei eingetragen oder die eingetragene gibts nicht)
    FxStat hylastat=init;// 1=wartend, 2=gesandt, 3=gescheitert, 4=fehlend (in spool keine Capi-Datei eingetragen oder die eingetragene gibts nicht)
    void archiviere(DB *My, paramcl *pmp, struct stat *entryp,uchar obgescheitert, FaxTyp ftyp, uchar *gel, int obverb, int oblog);
    int loeschecapi(int obverb, int oblog);
    int loeschehyla(paramcl *pmp,int obverb, int oblog);
    fsfcl(string id, string npdf, string spdf, string telnr, unsigned prio, string capisd, int capids, string hylanr, int hylads, 
        uchar obcapi, uchar obhyla):
      fxfcl(npdf,spdf,prio), 
            id(id), telnr(telnr), capisd(capisd), capids(capids), hylanr(hylanr), hylads(hylads), fobcapi(obcapi), fobhyla(obhyla) {}
    fsfcl(string id,string original): id(id), original(original) {}
    fsfcl(string id, string capisd, string hylanr, string cspf): id(id), capisd(capisd), hylanr(hylanr), cspf(cspf) {}
    fsfcl(string hylanr): hylanr(hylanr) {}
    fsfcl(string sendqgespfad, FxStat capistat): sendqgespfad(sendqgespfad), capistat(capistat) {}
    void setzcapistat(paramcl *pmp, struct stat *entrysendp);
    void capiwausgeb(stringstream *ausgp, string *maxtries, int obverb, string *ctriesp, int oblog,unsigned long faxord=0);
    void hylaausgeb(stringstream *ausgp, paramcl *pmp, int obsfehlt, int obverb=0, uchar obzaehl=0, int oblog=0);
}; // class fsfcl


class paramcl // Programmparameter 
{
  private:
    double tstart, tende;
    svec modems;       // gefundene Modems
    size_t optslsz=0; // last opts.size()
    char *ich; // argv[0]
  public:
    cppSchluess *hconfp=0;
    static constexpr const char *moeglhvz[2]={"/var/spool/fax","var/spool/hylafax"};
    uchar obfcard=1;    // ob Fritzcard eingesteckt
    uchar obmodem=1;    // ob Modem angeschlossen
    string spooltab = "spool";
    int obverb=0; // verbose
    int oblog=0;  // mehr Protokollieren
    uchar loef=0;   // loesche eine Fax-Datei
    uchar loew=0;  // loeschewaise in der Datenbank, aber nicht mehr real nachweisbare Dateien in der Datenbank loeschen
    uchar loea=0; // loesche alle wartenden Faxe und zugehoerige Dateieintraege
    uchar loee=0; // empfangene Dateien loeschen, die nicht verarbeitet werden koennen
    uchar kez=0;    // korrigiere Erfolgskennzeichen
    uchar bwv=0;    // bereinige Warteverzeichnis
    uchar lista=0;   // liste Archiv auf
    uchar listf=0;   // liste gescheiterte auf
    uchar listi=0;   // liste Eingegangene auf
    string dszahl="30"; // Datensatzzahl fuer Tabellenausgaben
    uchar logdateineu=0; // logdt vorher loeschen
    uchar hilfe=0;      // Hilfe anzeigen
    unsigned long faxord; // Ordinalzahl des Faxes unter allen anstehenden Faxen
    unsigned long geszahl=0;
    unsigned long ankzahl=0; // Zahl der angekommenen Faxe
    unsigned long dbzahl=0; // Zahl der ueberprueften Datenbankeintraege
    unsigned long wzahl=0;
    unsigned long ezahl=0;
    unsigned long gzahl=0;
    unsigned long fzahl=0;
    unsigned long weizahl=0; // Zahl der weiteren wartenden Faxe, die nicht in der Spooltabelle dieses Programms eingetragen sind
    uchar gleichziel; // faxe auch ohne Fax-Erfolg auf Zielverzeichnis abspeichern
    uchar obcapi=1; // ob ueberhaupt die Capisuite verwendet werden soll
    uchar obhyla=1; // ob ueberhaupt hylafax verwendet werden soll
    uchar konfobcapi; // ob obcapi in der Konfigurationsdatei eingestellt ist
    uchar konfobhyla; // ob obhyla in der Konfigurationsdatei eingestellt ist
    //    string hmodemstr; // Erkennung des Faxgeraetes nach /dev/tty, Standard ACM
    string hmodem;    // erkanntes und laufendes Modem ttyACM0
    //    string hmodname;  // ttyACM0
    uchar hylazuerst;  // ob ein Fax zuerst ueber Hylafax versucht werden soll zu faxen
    uchar rzf=0; // rueckzufragen
    string dbq; // Datenbank
    string cuser; // Linux-Benutzer fuer Capisuite
    string muser; // Benutzer fuer Mysql/MariaDB
    string mpwd;  // Passwort fuer Mysql/MariaDB
    DB* My=0;
    const string touta="outa"; // MariaDB-Tabelle fuer gesandte oder gescheiterte Faxe
    const string tinca="inca"; // MariaDB-Tabelle fuer empfangene Faxe
    // Parameter aus /etc/capisuite/fax.conf:
    string spoolcapivz; // Verzeichnis der Capi-Spool-Dateien /var/spool/capisuite/
    string cfaxuservz;    // /var/spool/capisuite/users/
    string cfaxusersqvz;    // /var/spool/capisuite/users/<user>/sendq
    string cfaxuserrcvz;    // /var/spool/capisuite/users/<user>/received
    string cdonevz; // Capisuite-Archiv: /var/spool/capisuite/done
    string cfailedvz; // Capisuite-Archiv der gescheiterten /var/spool/capisuite/failed
    string countrycode; // Landesvorwahl
    string citycode;    // Ortsvorwahl
    string msn;         // MSN fuer Fax
    string LongDistancePrefix; // Vorsatz fuer ausserorts
    string InternationalPrefix; // Vorsatz fuer ausserlandes
    string LocalIdentifier; // eigener Namen fuer Hylafax bis 10 Buchstaben
    string cFaxUeberschrift; // eigener Namen fuer Capisuite bis 20 Buchstaben
    string cklingelzahl; // Zahl der Klingeltoene, bis Capisuite einen Anruf annnimmt
    string hklingelzahl; // Zahl der Klingeltoene, bis Hylafax einen Anruf annnimmt
    uchar capizukonf=0; // capi zu konfigurieren
    uchar hylazukonf=0; // hyla zu konfigurieren
    uchar zmzukonf=0; // Zielmuster zu konfigurieren
    string varsphylavz; // Verzeichnis der Hyla-Spool-Dateien /var/spool/fax
    string faxgtpfad;   // /usr/lib/fax/faxgetty
    string hsendqvz; // /var/spool/fax/sendq
#ifdef _WIN32
    char cpt[255];
    DWORD dcpt;
#elif linux
    char cpt[MAXHOSTNAMELEN]; 
    size_t cptlen;
#endif
    string host;  // fuer MySQL/MariaDB
    string logdname; // Logdatei-Name ohne Pfad
    string logvz; // nur das Verzeichnis
    string loggespfad; // Gesamtpfad, auf den dann die in konsole.h verwiesene und oben definierte Variable logdt zeigt
    string zufaxenvz;
    string wvz; // Warteverzeichnis
    string gvz; // Gescheitertenverzeichnis
    string empfvz; // Empfangsverzeichnis
    string cronminut; // Minuten fuer crontab; 0 = kein Crontab-Eintrag
    string maxcapinr; // maximale Versuchnr in Capi, bis Hyla versucht wird
    string maxhylanr; // maixmale Versuchsnr in Hylafax, bis Capi versucht wird
    string langu; // Sprache (Anfangsbuchstabe)

    cppSchluess *cgconfp; // Gesamtkonfiguration
    size_t gcs; // dessen Groesse
    string sqlvz;  // Zahl der SQL-Befehle aus Vorgaben
    size_t sqlvzn; // Zahl der SQL-Befehle aus Vorgaben numerisch
    cppSchluess *sqlconfvp; // SQL-Pointer aus Vorgaben
    string sqlz;  // Zahl der SQL-Befehle
    size_t sqlzn; // Zahl der SQL-Befehle numerisch
    cppSchluess *sqlconfp; // SQL-Pointer

    zielmustercl *zmp; // Zielmusterzeiger
    cppSchluess *zmconfp; // dessen Serialisierung
    string zmz; // Zielmusterzahl
    size_t zmzn; // Zielmusterzahl numerisch
    zielmustercl *zmvp; // Zielmusterzeiger aus Vorgaben
    string zmvz; // Zielmusterzahl aus Vorgaben
    size_t zmvzn; // Zielmusterzahl numerisch aus Vorgaben

    cppSchluess *capiconfp; // Capi-Konfiguration (fax.conf)
    size_t ccs; // capiconf-confsize
    string konfdatname; // name der Konfigurationsdatei
    string anfaxstr, ancfaxstr, anhfaxstr; // 'an Fax', "an cFax", "an hFax"
    string anstr; // ' an '
    string undstr;  //  'und'
    string cmd; // string fuer command fuer Betriebssystembefehle
    vector<optioncl> opts;
    vector<argcl> argcmv; // class member vector
    servc *sfaxq=0, *shfaxd=0, *sfaxgetty=0, *scapisuite=0;

  private:
    void lgnzuw(); // in vorgaben, lieskonfein, getcommandl0, getcommandline, rueckfragen
    string neuerdateiname(const string& qpfad); // in DateienHerricht
    void WVZinDatenbank(vector<fxfcl> *fxvp); // in DateienHerricht
    string getzielvz(const string& datei); // in bereinigewv
    int setzegcp(const string& name, string *wert);
    void pruefcvz();
    void setzhylastat(fsfcl *fsf, string *protdaktp, uchar *hyla_uverz_nrp, int *obsfehltp, uchar startvznr,
                      int obverb=0, int oblog=0, string *hylastatep=0, string *hyladialsp=0, string *hylastatusp=0, string *hylastatuscodep=0);
    void konfcapi();
    int  xferlog(const string& jobid, string *erg,int obverb=0, int oblog=0);
    void richtcapiher();
    void setzhconfp(string *protdaktp,int obverb);
  public:
    paramcl(int argc,char** argv);
    ~paramcl();
    void logvorgaben();
    void getcommandl0();
    void pruefhardware();
    void VorgbAllg(); // allgemeine Vorgaben
    #ifdef autofaxcpp
    void VorgbSpeziell() __attribute__((weak)); // implementationsspezifische Vorgaben (aber nur Quellcodeaenderung aenderbar, Modul vorgaben.cpp)
    #else
    void VorgbSpeziell(); // implementationsspezifische Vorgaben (aber nur Quellcodeaenderung aenderbar, Modul vorgaben.cpp)
    #endif
    void lieskonfein();
    int  getcommandline();
    void rueckfragen();
    int setzhylavz(); // sucht das Hylaverzeichnis und setzt varsphylavz darauf, return 0, wenn nicht gefunden dann varsphylavz="", return 1
    void verzeichnisse();
    void pruefcron();
    void pruefsamba();
    int  initDB();

    void korrerfolgszeichen();
    void bereinigewv();
    void tu_lista(const string& oberfolg);
    void tu_listi();
    void DateienHerricht();
    int pruefcapi();
    int pruefhyla();
    int  loeschefax(int obverb, int oblog);
    int  loeschewaise(int obverb, int oblog);
    int  loescheallewartende(int obverb, int oblog);
    void faxealle();
    void untersuchespool();
    void zeigweitere();
    void empfarch();
    void schlussanzeige(const char* const progname);
    void autofkonfschreib();
}; // class paramcl
