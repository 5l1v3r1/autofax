#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef KONSOLE_H_DRIN
#define KONSOLE_H_DRIN
#ifdef _WIN32
#include <winsock2.h> // entfaellt bei mysql
#include <io.h> // fuer isatty und _isatty
#endif

#include <iostream>
//#define obfstream
#ifdef obfstream
#include <fstream> // fstream
#endif  

#ifdef _MSC_VER
#include <string>
#endif

#ifdef linux
#define vtz '/' // Verzeichnistrennzeichen
#define vtzs "/" // Verzeichnistrennzeichenstring
#elif __MINGW32__ || _MSC_VER
#define vtz '\\' // Verzeichnistrennzeichen
#define vtzs "\\" // Verzeichnistrennzeichen
#endif

using namespace std;
int Log(const string& text,short screen=1,short file=1,bool oberr=0,short klobverb=0);

#ifdef _MSC_VER
#define fileno _fileno // sonst Warnung posix deprecated
#define isatty _isatty // sonst Warnung posix deprecated
#endif

#ifdef __MINGW32__
#undef PATH_MAX
#endif

#ifdef _WIN32
#define PATH_MAX MAX_PATH
// #include <limits.h>
#include <time.h> // fuer mktime und time in kuerzelogdatei
#define lstat stat

#elif linux
#include <string.h> // strcpy, strtok_r, strerror, memcmp, strcmp, strstr, 
#include <errno.h> // errno, ENOENT
#include <pwd.h>   // getuid, getpwuid
#include <sys/param.h>  // MAXHOSTNAMELEN
#include <sys/ioctl.h> // winsize, TIOCGWINST, w, ioctl
#include <stdlib.h>
#include <dirent.h> // DIR, dirHandle, opendir, readdir, PATH_MAX
#include <unistd.h>
#endif

#ifndef _MSC_VER
#include <stdio.h> // fuer fopen, perror, fgets, sscanf, fputs, fclose, stdout, fileno, stderr
#else
#define snprintf _snprintf
#endif

#include <sys/stat.h> // stat, lstat, S_IFMT, S_IFDIR ...
// #include <boost/algorithm/string.hpp> // clock, numeric_limits
#include <time.h>
#include <limits>

typedef unsigned char uchar;
enum binaer:uchar {falsch,wahr};
enum Sprache {deutsch,englisch,Smax};
extern const char *dir;
//extern const string datei;
// extern const char *rot, *weinrot, *schwarz, *blau, *gelb; // muss spaeter kompilerunabhaengig
extern const char *schwarz, *dgrau, *drot, *rot, *gruen, *hgruen, *braun, *gelb, *blau, *hblau, *violett, *hviolett,
       *tuerkis, *htuerkis, *hgrau, *weiss;
extern const string drots, rots, schwarzs, blaus, gelbs, tuerkiss, hgraus,violetts;
#ifdef linux
extern const char *_rot, *_hrot, *_schwarz, *_blau, *_gelb, *_tuerkis, *_hgrau;
#endif
#include <fstream> // kopiere
#include <algorithm>    // std::transform

// typedef const char *TCtp[][Smax];
typedef const char * const * const TCtp[Smax];
class TxB // Text-Basisklasse
{
 public:
  Sprache lgn; // Sprache numerisch
//  TCtp* TCp;
  const char * const * const * const *TCp;
  inline const char* operator[](long const& nr) const {
    TCtp *hilf = reinterpret_cast<TCtp*>(TCp);
    return (const char*)hilf[nr][lgn];
  }
};

string meinpfad();

enum Tkonsole_ {
  T_pfad,
  T_kuerze_logdatei,
  T_Logdatei,
  T_gibt_es_noch_nicht_Kuerze_sie_daher_nicht,
  T_Kann_Datei,
  T_nicht_als_fstream_zum_Schreiben_oeffnen,
  T_nicht_als_fstream_zum_Lesen_oeffnen,
  T_nicht_mit_fopen_zum_Schreiben_oeffnen,
  T_nicht_mit_fopen_zum_Lesen_oeffnen,
  T_Variable_logdatei_leer,
  T_nicht_als_fstream_zum_Anhaengen_oeffnen,
  T_nicht_mit_fopen_zum_Anhaengen_oeffnen,
  T_Bitte_mit,
  T_beenden,
  T_stern_zeile,
  T_Rueckmeldung,
  T_Suchbefehl,
  T_Fuehre_aus,
  T_fehlgeschlagen_bei,
  T_Fuehrte_aus,
  T_komma_Ergebnis,
  T_kein_Verzeichnis_nochmal,
  T_nicht_gefunden_soll_ich_es_erstellen,
  T_Fehlender_Parameter_string_zu,
  T_oder,
  T_Fehler_Parameter,
  T_ohne_gueltigen,
  T_mit_Datei_als,
  T_mit_falschem,
  T_Pfad_angegeben,
  T_Nicht_numerischer,
  T_Fehlender,
  T_Parameter_nr_zu,
  T_Fehler,
  T_Erfolg,
  T_Weder_zypper_noch_apt_get_als_Installationspgrogramm_gefunden,
  T_Logdateidpp,
  T_Lese_Konfiguration_aus,
  T_konsoleMAX,
};

class Txkonsolecl : public TxB
{
  public:
    static const char *TextC[T_konsoleMAX+1][Smax];
    Txkonsolecl();
//    inline const char* operator[](Tkonsole_ const& nr) const { return TextC[nr][lgn]; }
};

// arg-Class
class argcl
{
 public:
 char* argcs;  // Zeiger auf einen Commandline-Parameter
 uchar agef=0; // gefunden
 argcl(int i,char** argv);
};


inline string zustr(int _Val) {
  char _Buf[2 * sizeof(long long)];
  snprintf(_Buf,2*sizeof(long long), "%d", _Val);
  return string(_Buf);
}
inline std::string right(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
} // tail}
inline std::string mitvtz(std::string const& vz) {
  if (right(vz,1)==string(1,vtz)) {
    return vz;
  } else {
    return vz+vtz;
  }
}
inline int istelnr(std::string str) {
  if	(str.find_first_not_of("+ 0123456789.,")==string::npos && str.find_first_not_of(" ")!=string::npos) 
    return 1;
  return 0;
}

inline int isnumeric(std::string str)
{
  if	(str.find_first_not_of(" 0123456789.,")==string::npos && str.find_first_not_of(" ")!=string::npos) 
    return 1;
  return 0;
}

inline int isnumeric(char* str)
{
  while (*str) {
    if(!strchr(" 0123456789.,",*str)) return 0;
    str++;
  }
  return 1;
}

inline std::string *gtrim(std::string *str) {
  str->erase(0, str->find_first_not_of("\t "));       //prefixing spaces
  str->erase(str->find_last_not_of("\t ")+1);         //surfixing spaces
  return str;
}

inline std::string *ltrim(std::string *str) {
  str->erase(0, str->find_first_not_of("\t "));       //prefixing spaces
  return str;
}

inline void rtrim(char* str,size_t size) {
  for (int nd=0,i=size-1;i;i--) {
    if (!str[i]) nd=1; 
    else if (nd && str[i]!=32 && str[i]!=9) break;
    str[i]=0;
  }
}

string fersetze(const string& u, const char* alt, const char* neu);
string caseersetze(const string& u, const char* alt, const char* neu); 
string *loeschealleaus(string *u, const char* alt);

string ersetze(const char *const u, const char* const alt, const char* neu);
string *sersetze(string *src, string const& target, string const& repl);
// wstring ersetze(const wstring& u, const wchar_t* alt, const wchar_t* neu); 

string ersetzeAllezu(string& quelle, const string& alt, const string& neu);
void ersetzeAlle(string& quelle, const string& alt, const string& neu);
string ersetzeAllezu(string *quelle, const char* alt, const char* neu);
string ersetzeAllezu(const char *quelle, const char* alt, const char* neu);
void ersetzeAlle(string *quelle, const char* alt, const char* neu);
void ersetzeAlle(string *quelle, const string& alt, const string& neu);

char* charersetze(char *u, char alt, char neu);
void chersetze(const string& u, string *z, const string& alt, const char neu);
string ersetzefuerdatei(const string& u);
size_t zahlin(string *str, const char* was);
long cmpmem( char* feld, const char* search, int len_feld); // , int len_search
// fuer lies (Konfigurationsdatei lesen)
char* ltoan(long value, int base=10, uchar obtz=0, uchar minstel=0); 
char* ltoa_(long value, char* result, int base); 

class Schluessel {
  public:
    char key[90];
    char val[100];
    template <typename T> void zuweis(T *var) { *var=atol(val); }
    template <typename T> void beleg(T *var) { strncpy(val,ltoan(*var),sizeof val-1);val[sizeof val-1]=0; }
};
template <> inline void Schluessel::zuweis < char* > (char** var) { *var = val; }
template <> inline void Schluessel::zuweis < const char* > (const char** var) { *var = val; }
template <> inline void Schluessel::zuweis < string > (string *var) { *var = val; }
template <> inline void Schluessel::zuweis < binaer > (binaer *var) { *var = (binaer)atoi(val); }
template <> inline void Schluessel::beleg < char* > (char** var) { strncpy(val,*var,sizeof val-1);val[sizeof val-1]=0; }
template <> inline void Schluessel::beleg < const char* > (const char** var) { strncpy(val,*var,sizeof val-1);val[sizeof val-1]=0; }
template <> inline void Schluessel::beleg < string > (string *var) { strncpy(val,var->c_str(),sizeof val-1);val[sizeof val-1]=0;}

class cppSchluess {
  public:
    string name;
    string wert;
    template <typename T> void zuweis(T *var) { *var=atol(wert.c_str()); }
    template <typename T> void beleg(T *var) { wert=ltoan(*var); }
};
template <> inline void cppSchluess::zuweis < char* > (char** var) {*var = (char*)wert.c_str(); }
template <> inline void cppSchluess::zuweis < const char* > (const char** var) {*var = wert.c_str(); }
template <> inline void cppSchluess::zuweis < string > (string *var) {*var = wert; }
template <> inline void cppSchluess::zuweis < binaer > (binaer *var) { *var = (binaer)atoi(wert.c_str()); }
template <> inline void cppSchluess::beleg < char* > (char** var) {wert=*var; }
template <> inline void cppSchluess::beleg < const char* > (const char** var) {wert=*var;}
template <> inline void cppSchluess::beleg < string > (string *var) {wert=*var;}

class abSchl {
 public:
   string name;
   string wert;
   abSchl(string& vname, string& vwert):name(vname),wert(vwert) {}
};

#ifdef _MSC_VER
extern inline void wait();
#else
// #define wait();
#endif

#ifdef _WIN32
extern const char *drot, *rot, *schwarz, *blau, *gelb, *tuerkis, *hgrau;
extern const string drots, rots, schwarzs, blaus, gelbs, tuerkis, hgrau;
inline std::ostream& _drot(std::ostream &s){
  //    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
  return s;
}
inline std::ostream& _blau(std::ostream &s){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_INTENSITY);
  return s;
}
inline std::ostream& _rot(std::ostream &s){
  //    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  return s;
}
inline std::ostream& _schwarz(std::ostream &s){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
  return s;
}
inline std::ostream& _gelb(std::ostream &s){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN);
  return s;
}
struct color {
  color(WORD attribute):m_color(attribute){};
  WORD m_color;
};

#endif
#include <vector>
class svec: public vector<std::string> {
  public:
    inline svec& operator<<(const std::string& str) {
      this->push_back(str);
      return *this;
    }
};
//svec& operator<<(svec& v, const std::string& str);
template<class T>
class tsvec: public vector<T> {
  public:
    inline tsvec<T>& operator<<(const T& str) {
      this->push_back(str);
      ((T&)str).init();
      return *this;
    }
};

// Abschnitt einer Konfigurationsdatei
class absch {
 public:
 string aname;
 vector<abSchl> av;
 const string& suche(const char* const sname);
 const string& suche(const string& sname);
};

class confdat {
  public:
    ifstream *f;
    uchar obgelesen=0;
    svec zn;
    vector<absch> abschv;
    size_t richtige;
    confdat(const string& fname, int obverb);
    confdat(const string& fname,cppSchluess *conf, size_t csize, int obverb=0, char tz='=');
    int lies(const string& fname,int obverb);
    void auswert(cppSchluess *conf, size_t csize, int obverb=0, char tz='=');
    void Abschn_auswert(int obverb=0, char tz='=');
};

// fuer Commandline-Optionen
enum par_t:uchar {psons,pverz,pfile,pzahl}; // Parameterart: Sonstiges, Verzeichnis, Zahl

class optioncl
{
  public:
    string kurz;
    string lang;
    TxB *TxBp=0;
    long Txi;
    string *rottxt=0; // ggf rot zu markierender Text zwischen Txi und Txi2
    long Txi2=-1;
//    string oerkl;
    uchar *pptr=0; // Zeiger auf Parameter, der hier eingestellt werden kann
    int wert; // Wert, der pptr zugewiesen wird, falls dieser Parameter gewaehlt wird
    string *zptr=0; // zusatzzeiger auf Parameter, der hier eingegeben werden kann
    par_t art;
//    uchar ogefunden=0; // braucht man nicht, ist in argcl
    optioncl(string kurz, string lang, TxB *TxBp, long Txi) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, par_t art, string *zptr) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), zptr(zptr), art(art) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, string *rottxt, long Txi2, string *zptr, par_t art) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), rottxt(rottxt), Txi2(Txi2), zptr(zptr), art(art) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, uchar *pptr, int wert) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), pptr(pptr), wert(wert) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, string *rottxt, long Txi2, uchar *pptr, int wert) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), rottxt(rottxt), Txi2(Txi2), pptr(pptr), wert(wert) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, const string *rottxt, long Txi2, uchar *pptr, int wert) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), rottxt((string*)rottxt), Txi2(Txi2), pptr(pptr), wert(wert) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, binaer *pptr, int wert) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), pptr((uchar*)pptr), wert(wert) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, int *pptr, int wert) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), pptr((uchar*)pptr), wert(wert) {}
    optioncl(string kurz, string lang, TxB *TxBp, long Txi, string *rottxt, long Txi2, int *pptr, int wert) : 
               kurz(kurz), lang(lang), TxBp(TxBp), Txi(Txi), rottxt(rottxt), Txi2(Txi2), pptr((uchar*)pptr), wert(wert) {}
    int pruefp(vector<argcl> *argcvm , size_t *akt, uchar *hilfe); // 1 = das war der Parameter, 0 = nicht
    void hilfezeile(Sprache lg);
};

#endif // KONSOLE_H_DRIN

extern const char *logdt;

int kuerzelogdatei(const char* logdatei,int obverb);
void kopiere(string *quelle, string *ziel);
void aufSplit(vector<string> *tokens, const string *text, char sep, bool nichtdoppelt=1);
void aufSplit(vector<string> *tokens, const char *text, char sep, bool nichtdoppelt=1);
void aufiSplit(vector<string> *tokens, const string *text, const char* sep,bool nichtdoppelt=1);
void aufSplit(vector<string> *tokens, const string *text, char* sep,bool nichtdoppelt=1);
void getstammext(string *ganz, string *stamm, string *exten);
// int cpplies(string fname,cppSchluess *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
// int liesalt(const char *fname,Schluessel *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
// int liesalt(string fname,Schluessel *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
string XOR(const string& value, const string& key);
int schreib(const char *fname, Schluessel *conf, size_t csize);
int cppschreib(const string& fname, cppSchluess *conf, size_t csize);
int multicppschreib(const string& fname, cppSchluess **conf, size_t *csizes, size_t cszahl);
std::string base_name(std::string const & path);
std::string dir_name(std::string const & path);
int systemrueck(const string& cmd, char obverb=0, int oblog=0, vector<string> *rueck=0, 
                binaer ob0heissterfolg=wahr, binaer obergebnisanzeig=wahr, const string& ueberschr="");
string aktprogverz();
// char holbuchst(const string& frage, const char* moegl,const char *berkl[], const char* erlaubt=0, const char *vorgabe=0);
char holbuchst(const string& frage, const string& moegl,const char *berkl[], const char* erlaubt=0, const char *vorgabe=0);
// char holbuchst(const char *frage, const char* moegl,const char *berkl[], const char* erlaubt=0, const char *vorgabe=0);
// char holbuchst(const char *frage, string moegl,const char *berkl[], const char* erlaubt=0, const char *vorgabe=0);
string holstrings(const char *frage, char* moegl[], char *vorgabe=0); // das letzte Element von moegl muss 0 sein
string holstrings(const char *frage, vector<string> *moegl, string *vorgabe=0); 
string holzahl(const char *frage, const char *vorgabe=0);
string holzahl(const char *frage, string *vorgabe);
string holstring(const char *frage, string *vorgabe=0);
char* holcstring(const char *frage, char* buf, unsigned long buflen, const char* vorgabe="");
string holstring(const string& frage, string *vorgabe=0);
string holverz(const char *frage,string *vorgabe=0);
uchar VerzeichnisGibts(const char* vname);

enum linsten:uchar {uinst,zyp,apt,unent};
class linstcl
{
  public:
    linsten inst;
    uchar obnmr=1;
    linsten checkinst(int obverb=0, int oblog=0);
    string ersetzeprog(const string& prog);
    uchar doinst(const string& prog,int obverb=0,int oblog=0);
    uchar douninst(const string& prog,int obverb=0,int oblog=0);
    uchar obfehlt(const string& prog,int obverb=0,int oblog=0);
};
