// Namenskonvention: erste Praeferenz wurde darauf gelegt, dass mit der Editor-Suchfunktion moeglichst alle gleichen Bezeichner gefunden werden koennen, zweite Praeferenz darauf, dass deutsche und englische Namenbestandteile moeglichst gut gemischt werden
#ifdef linux
// #include <pgsql/libpq-fe.h> // PGconn
#include <utime.h> // utime(
#include <tiffio.h> // fuer empfarch
#include <iomanip> // setprecision
#elif __MINGW32__ || _MSC_VER
#include <sys/utime.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
// // #include "c:\programme\postgresql\9.1\include\libpq-fe.h"
// #include "..\..\..\..\programme\postgresql\9.2\include\libpq-fe.h"
#endif

// #include <iostream>
// #include <fstream> // jetzt in konsole.h
// #include <sstream>
// #include <dirent.h> // DIR, dirent, opendir, readdir, PATH_MAX
// #include <boost/algorithm/string.hpp> // clock // jetzt in konsole.h

// #include "vmime/vmime.hpp"
// #include "vmime/platforms/posix/posixHandler.hpp"
#include "konsole.h"
#include "DB.h"
// #include "iconverter.h"
// #include "datetime_utils.hpp"
// G.Schade 22.4.14 fuer Zusatz-Debugging (SQL): 255, sonst: 0
// fuer die Prozessliste
#define PROC_VERZ "/proc/"
#ifdef linux
#include <sys/utsname.h> // utsname
#endif
#include <set>

#define obhp // ob hylafax+ (statt hylafax)

// Bestandteile der Ueberpruefung auf Funktionsfaehigkeit von hylafax: 
// faxmodem


// const char *logvz; // das reine Verzeichnis
// string logpfad; // zusammengesetzt aus Verzeichnis und name
// const char lgs logdatei_a[PATH_MAX+1] ="/var/log/log_vmparse.txt";
// const char *logdatei = logdatei_a;
const char *logdt="/var/log/autofaxvorgabe.log";// darauf wird in konsole.h verwiesen; muss dann auf lgp zeigen

// const char* logdatname;
uchar ZDB=0; // 255 = mit Debug

class Txautofaxcl: public TxB
{
  public:
    static const char *TextC[][Smax];
    Txautofaxcl() {TCp=(const char* const * const * const *)&TextC;}
} Tx; // class Txautofaxcl

enum T_ 
{/*{{{*/
  T_sprachstr,
  T_j_af,
  T_verarbeitete_Nicht_PDF_Dateien,
  T_verarbeitete_PDF_Dateien,
  T_Soll_die_Capisuite_verwendet_werden,
  T_Soll_Hylafax_verwendet_werden,
  T_Soll_vorrangig_hylafax_gewaehlt_werden,
  T_Zahl_der_Versuche_in_Capisuite_bis_hylafax_eingeschaltet_wird,
  T_Zahl_der_Versuche_in_hylafax_bis_Capisuite_eingeschaltet_wird,
  T_Linux_Benutzer_fuer_Capisuite,
  T_Hylafax_eigene_Landesvorwahl_ohne_plus_oder_00,
  T_Hylafax_eigene_Ortsvorwahl_ohne_0,
  T_Hylafax_und_Capisuite_eigene_MSN_Faxnummer_ohne_Vorwahl,
  T_Hylafax_Praefix_fuer_ausserorts_zB_0,
  T_Hylafax_Praefix_fuer_das_Ausland_zB_00,
  T_Hylafax_bis_10_Buchstabe_fuer_eigenen_Namen,
  T_Capisuite_bis_20_Buchstaben_fuer_eigenen_Namen,
  T_Zahl_der_Klingeltoene_bis_Capisuite_den_Anruf_annimmt,
  T_Zahl_der_Klingeltoene_bis_Hylafax_den_Anruf_annimmt,
  T_Sollen_die_Dateien_unabhaengig_vom_Faxerfolg_im_Zielverzeichnis_gespeichert_werden,
  T_wartend,
  T_gesandt,
  T_gescheitert,
  T_nicht_in_der_Warteschlange,
  T_woasined,
  T_Der_regulaere_Ausdruck,
  T_konnte_nicht_kompiliert_werden,
  Verbindung_zur_Datenbank_nicht_herstellbar,
  T_Breche_ab,
  T_Konnte_Verzeichnis,
  T_nicht_oeffnen,
  T_zielname_erstes_Ziel,
  T_zielname_Ausweichziel,
  T_Datei,
  T_entspricht,
  T_entsprichtnicht,
  T_entsprichtdoch,
  T_Muster_Doppelpunkt,
  T_Verschiebe,
  T_Fehler_beim_Verschieben,
  T_Kopiere_Doppelpunkt,
  T_Fehler_beim_Kopieren,
  T_Dateiname,
  T_schlechtgeformt,
  T_Fehler_af,
  T_bei,
  T_ja,
  T_nein,
  T_obcapimitDoppelpunkt,
  T_obhylamitDoppelpunkt,
  T_Endung,
  T_Stamm,
  T_trennenach,
  T_ErstelledurchKopieren,
  T_FehlerbeimUmbenennen,
  T_ErstelledurchBenennen,
  T_autofaxabgebrochen,
  T_FehlerbeimUmbenennenbei,
  T_aus,
  T_nichtbeschreibbar,
  T_KeinModemmitNamensmuster,
  T_gefundenverwendeHylafaxnicht,
  T_Baudratevon,
  T_mit_af,
  T_zugeringVerwendeHylafaxnicht,
  T_hylafax_faxmodem_nicht_gefunden_Versuche_es_zu_installieren_mit,
  T_StarteHylafax,
  T_hylafaxspringtnichtan,
  T_haengean,
  T_an_mdpp,
  T_KannFcpciNInstVerwCapiNicht,
  T_Kernelversion,
  T_nichtgefFcpciMfdKinstallierwerden,
  T_Konnte,
  T_nichtstarten,
  T_VersucheDatei,
  T_zuinstallieren,
  T_bittestellenSiedieInstallationsdatei,
  T_bereitevtlvonfkn,
  T_StarteCapisuite,
  T_Umwandlungvon,
  T_inPDFmit,
  T_beendetErgebnis,
  T_misserfolg,
  T_Erfolg_af,
  T_lstatfehlgeschlagen,
  T_beiDatei,
  T_konntecapisuiteservice,
  T_malnichtstartenverwN,
  T_SpoolDateierstellt,
  T_SpoolDatei,
  T_SpoolPfad,
  T_DieFaxnrausTabelle,
  T_istleerfaxeesdahernicht,
  T_DieFaxnrvon,
  T_ist,
  T_faxemitCFehler,
  T_Faxdatei,
  T_fehlt,
  T_hat0Bytes,
  T_Faxbefehl,
  T_nichtgefundenloeschesieausDB,
  T_KeinErgebnisbeimFaxen,
  T_HylafaxBefehl,
  T_Anwahlen,
  T_kommaDatei,
  T_Status,
  T_bzw,
  T_Falschals,
  T_eingeordnet,
  T_DateienzahlimVz,
  T_ZahldDSmwegzuschickendenFaxenin,
  T_PDFDatei,
  T_beiSQLAbfrage,
  T_obgescheitert,
  T_Fehler_beim_Verschieben_Ausrufezeichen,
  T_0Bytes,
  T_Loesche_Ausrufezeichen,
  T_nicht_geloescht_war_eh_nicht_mehr_da,
  T_Verwaiste_Datei,
  T_geloescht_Fax_schon_in,
  T_archiviert_Ausrufezeichen,
  T_Weitere_Spool_Eintraege,
  T_Zeit_Doppelpunkt,
  T_Zahl_der_ueberpruefen_Datenbankeintraege,
  T_davon_gescheiterte_Faxe,
  T_davon_erfolgreiche_Faxe,
  T_davon_noch_wartende_Faxe,
  T_davon_nicht_in_Warteschlange,
  T_Fertig_mit,
  T_Fehler_beim_Datumsetzen_von,
  T_eindeutige_Identifikation,
  T_Originalname_der_Datei,
  T_Originalname_der_Datei_vor_Umwandlung_in_PDF,
  T_zu_senden_an,
  T_Prioritaet_aus_Dateinamen,
  T_Zahl_der_bisherigen_Versuche_in_Capisuite,
  T_Zahl_der_bisherigen_Versuche_in_Hylafax,
  T_Spooldatei_in_Capisuite,
  T_Pfad_zur_Spooldatei_in_Capisuite_ohne_abschliessendes_Verzeichnistrennzeichen,
  T_Aenderungszeit_der_CapiSpoolDatei,
  T_Aenderungszeit_der_CapiSpooldatei_im_time_Format,
  T_Aenderungszeit_der_Hylaspooldatei,
  T_Aenderungszeit_der_Hylaspooldatei_im_Time_Format,
  T_jobid_in_letztem_gescheitertem_hylafax,
  T_state_Feld_in_hylafax,
  T_statuscode_in_letztem_gescheitertem_hylafax,
  T_status_in_letztem_gescheitertem_hylafax,
  T_capispooldateien_der_Capisuite,
  T_Fehler_beim_Pruefen_von,
  T_Name_des_Adressaten,
  T_Datum_des_Losschickens,
  T_Ende_der_Uebertragung,
  T_Kennung_im_Faxsystem_hylafax_Nr_Capisuite_Datei_MSFax_Fax_Dateiname,
  T_Dateigroesse,
  T_Seitenzahl,
  T_Zahl_der_Anwahlen,
  T_FAxnummer_des_Adressaten,
  T_Titel_des_Adressaten,
  T_PID_falls_aus_Dateinamen_ermittelt,
  T_Kurzbezeichnung_der_eigenen_Faxstelle,
  T_Geraetename,
  T_outgoing_schedule_time_oder_so_aehnlich_nur_MSFax,
  T_Hinweis_auf_Dateiinhalt_oder_Adressat_nur_MSFax,
  T_identisch_zu_submid_nur_MSFax,
  T_Prioritaet_nur_MSFax,
  T_FAxnummer_des_Adressaten_nur_MSFax,
  T_Faxnummer_des_Senders_nur_MSFax,
  T_Beginn_der_Uebertragung_nur_MSFax,
  T_Archiv_fuer_die_erfolgreichen_Faxe,
  T_Archiv_fuer_die_gescheiterten_Faxe,
  T_Welches_Fax_soll_geloescht_werden,
  T_Soll_das_Fax_geloescht_werden_0_ist_Abbruch,
  T_wirklich_geloescht_werden,
  T_Kein_Fax_zum_Loeschen_vorhanden,
  T_Keine_ISDN_Karte_gefunden,
  T_mitCapi,
  T_auf,
  T_verwendet_Kofigurationsdatei_string_anstatt,
  T_faxt_die_Dateien_aus_pfad_anstatt,
  T_Dateien_warten_in_pfad_anstatt,
  T_Empfangsverzeichnis_fuer_Faxempfang,
  T_Capisuite_nicht_verwenden,
  T_hylafax_nicht_verwenden,
  T_versuche_faxe_zuerst_ueber_Capisuite_wegzuschicken,
  T_versuche_faxe_zuerst_ueber_hylafax_wegzuschicken,
  //  T_sucht_nach_dev_tty_string_als_Modem_mit_string_anstatt,
  T_nach_zahl_Versuchen_Capisuite_wird_Hylafax_versucht,
  T_nach_zahl_Versuchen_Hylafax_wird_Capisuite_verwendet,
  T_Zahl_der_Klingeltoene_bis_Capisuite_den_Anruf_annimmt_anstatt,
  T_Zahl_der_Klingeltoene_bis_Hylafax_den_Anruf_annimmt_anstatt,
  T_verwendet_fuer_Capisuite_den_Linux_Benutzer_string_anstatt,
  T_FAxe_werden_auch_ohne_Faxerfolg_ins_Zielverzeichnis_kopiert,
  T_faxnr_wird_hinter_string_erwartet_statt_hinter,
  T_faxnr_fuer_primaer_Capisuite_wird_hinter_string_erwartet_statt_hinter,
  T_faxnr_fuer_primaer_hylafax_wird_hinter_string_erwartet_statt_hinter,
  T_Adressatenname_wird_hinter_string_erwartet_statt_hinter,
  T_Trennstring_string_fuer_mehrere_Adressaten_Telefonnummern_statt,
  T_verwendet_die_Datenbank_auf_Host_string_anstatt_auf,
  T_verwendet_fuer_MySQL_MariaDB_den_Benutzer_string_anstatt,
  T_verwendet_fuer_MySQL_MariaDB_das_Passwort_string_anstatt,
  T_verwendet_die_Datenbank_string_anstatt,
  T_protokolliert_ausfuehrlich_in_Datei,
  T_sonst_knapper,
  T_waehlt_als_Logverzeichnis_pfad_derzeit,
  T_logdatei_string_im_Pfad,
  T_wird_verwendet_anstatt,
  T_logdatei_vorher_loeschen,
  T_Bildschirmausgabe_gespraechiger,
  T_Bildschirmausgabe_mit_SQL_Befehlen,
  T_alle_Parameter_werden_abgefragt_darunter_einige_hier_nicht_gezeigte,
  T_ein_Fax_nach_Rueckfrage_loeschen,
  T_Eintraege_aus,
  T_loeschen_zu_denen_kein_Datei_im_Wartevz_und_kein_Capi_oder_Hylafax_nachweisbar_ist,
  T_alle_wartenden_Faxe_und_zugehoerige_Eintraege_aus,
  T_loeschen,
  T_Dateien_aus_Warteverzeichnis_gegen,
  T_pruefen_und_verschieben,
  T_listet_Datensaetze_aus,
  T_Zahl_der_aufzulistenden_Datensaetze_ist_zahl_statt,
  T_Zeigt_diesen_Bildschirm_an,
  T_Fertig_mit_Parsen_der_Befehlszeile,
  T_Gebrauch,
  T_Faxt_Dateien_aus_Verzeichnis_pfad_die,
  T_faxnr_enthalten_und_durch_soffice_in_pdf_konvertierbar_sind_und_traegt_sie,
  T_Tabellen,
  T_aein,
  T_Verzeichnis_mit_zu_faxenden_Dateien,
  T_Verzeichnis_mit_wartenden_Dateien,
  T_Verzeichnis_mit_gescheiterten_Dateien,
  T_Verzeichnis_fuer_empfangene_Faxe,
  T_Buchstabenfolge_vor_erster_Faxnummer,
  T_Buchstabenfolge_vor_erster_Fax_Nummer_primaer_fuer_Capisuite,
  T_Buchstabenfolge_vor_erster_Fax_Nummer_primaer_fuer_Hylafax,
  T_Buchstabenfolge_vor_erstem_Adressaten,
  T_Buchstabenfolge_vor_weiterem_Adressaten_sowie_weiterer_Faxnummer,
  T_Host_fuer_MySQL_MariaDB_Datenbank,
  T_Benutzer_fuer_MySQL_MariaDB,
  T_Passwort_fuer_MySQL_MariaDB,
  T_Datenbankname_fuer_MySQL_MariaDB_auf,
  T_Logverzeichnis,
  T_Logdateiname,
  T_Zahl_der_SQL_Befehle_fuer_Namenszuordnung,
  T_SQL_Befehl_Nr,
  T_Capisuite_ist_offenbar_noch_nicht_richtig_konfiguriert,
  T_ist_Apostroph,
  T_Die_Einstellungen_koennen_spaeter_in,
  T_geaendert_werden,
  T_Faxnr_die_zum_Adressaten_gesandt_wird_bis_20_Zeichen_nur_plus_und_Ziffern,
  T_ausgehende_Multiple_Subscriber_Number_Faxnummer_ohne_Vorwahl,
  T_Amtsholung_ueblicherweise_kk_oder_0,
  T_Faxueberschrift,
  T_Adressat_empfangener_Faxe_die_ueber_Email_verteilt_werden,
  T_Zahl_der_Sendeversuche,
  T_kommagetrennte_Liste_mit_Sekundenabstaenden_zwischen_Sendeversuchen,
  T_Geduld_bis_zum_Verbindungsaufbau_in_s,
  T_komma_wert,
  T_komma_Altwert,
  T_Programm,
  T_Muster,
  T_Ziel,
  T_Logpfad,
  T_oblog,
  T_Ende,
  T_Fax_von,
  T_an,
  T_vom,
  T_fuer_Benutzer,
  T_Alle_wieviel_Minuten_soll_Autofax_aufgerufen_werden_0_ist_gar_nicht,
  T_Kann_Capisuite_nicht_installieren_verwende_Capi_nicht,
  T_wird_verwendet,
  T_mit_Baudrate,
  T_Fuer_Hylafax_verwendetes_Modem,
  T_archiviere,
  T_loeschecapi,
  T_loeschehyla,
  T_WVZinDatenbank,
  T_pruefhardware,
  T_setzhylavz,
  T_lieskonfein,
  T_rueckfragen,
  T_autokonfschreib,
  T_konfcapi,
  T_verzeichnisse,
  T_pruefcron,
  T_korrerfolgszeichen,
  T_bereinigewv,
  T_loeschefax,
  T_loeschewaise,
  T_loescheallewartenden,
  T_tu_lista, 
  T_tu_listi, 
  T_DateienHerricht,
  T_faxealle,
  T_untersuchespool,
  T_zeigweitere,
  T_empfarch,
  T_schlussanzeige,
  T_zielname,
  T_PIDausName,
  T_pruefhyla,
  T_pruefrules,
  T_pruefblack,
  T_pruefmodcron,
  T_pruefcapi,
  T_inDbc,
  T_faxemitC,
  T_inDBh,
  T_faxemitH,
  T_prufespool,
  T_pruefouta,
  T_pruefinca,
  T_pruefprocgettel3,
  T_capiwausgeb,
  T_hylaausgeb,
  T_tuloeschen,
  T_Installation_von_Hylafax_nicht_feststellbar_versuche_es_zu_installieren,
  T_Hylafaxplus_entdeckt_muss_ich_loeschen,
  T_RueckmlgZeile,
  T_Hylafax_laeuft,
  T_Pruefe_ob_Hylafax_gestartet,
  T_1_ist_erfolgreiche_Uebertragung_0_ist_fehlgeschlagen,
  T_in_der_Datenbanktabelle,
  T_wird_das_Erfolgszeichen_korrigiert,
  T_mit_Erfolgskennzeichen_auf,
  T_ohne_Erfolgskennzeichen_auf,
  T__auf,
  T_Zahl_der_empfangenen_Faxe,
  T_von,
  T_nicht_erkannt,
  T_Fehlermeldung_beim_Loeschversuch_eines_Hyla_Faxes_mit_faxrm,
  T_Zahl_der_nicht_geloeschten_Dateien,
  T_hylanr,
  T_Capispooldatei,
  T_Gesamt,
  T_Verwende,
  T_Hylafax_ohne_plus_entdeckt_muss_ich_loeschen,
  T_Versuch,
  T_nicht_gefunden_kein_Datenbankeintrag,
  T_setzhylastat,
  T_empfangene_Dateien_loeschen_die_nicht_verarbeitet_werden_koennen,
  T_hylafehlt,
  T_capilaeuft,
  T_spruef_sname,
  T_nicht_gefunden_versuche_ihn_einzurichten,
  T_gefunden,
  T_Kein_Modem_gefunden,
  T_kein_Faxprogramm_verfuegbar,
  T_Capisuite_gestartet,
  T_schreibe_Konfigurkation,
  T_Dateien,
  T_nicht_verarbeitbar_Verschiebe_sie_nach,
  T_Fuehre_aus_Dp,
  T_falls_es_hier_haengt_bitte_erneut_aufrufen,
  T_pruefsamba,
  T_Zufaxen,
  T_Warteauffax,
  T_Nichtgefaxt,
  T_Faxempfang,
  T_Gefaxt,
  T_lief_schon,
  T_zu_schreiben,
  T_Zahl_der_Verzeichnisse_fuer_erfolgreich_verschickte_Faxe,
  T_Verzeichnis,
  T_nicht_als_Sambafreigabe_gefunden_wird_ergaenzt,
  T_zufaxenvz,
  VorgbSpeziell_Basis,
  T_Wolle_Sie_noch_einen_SQL_Befehl_eingeben,
  T_Strich_ist_SQL_Befehl_loeschen_faxnr_wird_ersetzt_mit_der_Faxnr_vollnr_faxnr_mit_deren_Zusammenziehlung,
  T_faxnr_wird_ersetzt_mit_der_Faxnr_vollnr_faxnr_mit_deren_Zusammenziehlung,
  T_MAX
};


const string sprachstr=string("language/Sprache/Lingue/Lingua [")+blau+'d'+schwarz+"eutsch,"+blau+'e'+schwarz+"nglisch]"+"";
const char* sprachcstr=&sprachstr.front();

char const *Txautofaxcl::TextC[T_MAX+1][Smax]={
  // T_sprachstr
  {sprachcstr,sprachcstr},
  // T_j_af,
  {"j","y"},
  // T_verarbeitete_Nicht_PDF_Dateien
  {"' verarbeitete Nicht-PDF-Dateien: ","' processed Non-PDF-Files: "},
  // T_verarbeitete_PDF_Dateien
  {"' verarbeitete       PDF-Dateien: ","' processed     PDF-Files: "},
  // T_Soll_die_Capisuite_verwendet_werden
  {"Soll die Capisuite verwendet werden","Shall Capisuite be used"},
  // T_Soll_Hylafax_verwendet_werden
  {"Soll hylafax verwendet werden","Shall hylafax be uses"},
  // T_Soll_vorrangig_hylafax_gewaehlt_werden
  {"Soll vorrangig hylafax gewaehlt werden","Shall hylafax bei chosen preferentially"},
  // T_Zahl_der_Versuche_in_Capisuite_bis_hylafax_eingeschaltet_wird
  {"Zahl der Versuche in Capisuite, bis hylafax eingeschaltet wird","Number of tries in Capisuite, until hylafax is started"},
  // T_Zahl_der_Versuche_in_hylafax_bis_Capisuite_eingeschaltet_wird
  {"Zahl der Versuche in hylafax, bis Capisuite eingeschaltet wird","Number of tries in hylafax, until Capisuite is started"},
  // T_Linux_Benutzer_fuer_Capisuite
  {"Linux-Benutzer fuer Capisuite","Linux user for Capisuite"},
  // T_Hylafax_eigene_Landesvorwahl_ohne_plus_oder_00
  {"Hylafax: eigene Landesvorwahl ohne '+' oder '00'","hylafax: own international prefix without '+' or '00'"},
  // T_Hylafax_eigene_Ortsvorwahl_ohne_0
  {"Hylafax: eigene Ortsvorwahl ohne '0'","hylafax: own long distance prefix without '0'"},
  // T_Hylafax_und_Capisuite_eigene_MSN_Faxnummer_ohne_Vorwahl
  {"Hylafax und Capisuite: eigene MSN (Faxnummer ohne Vorwahl)","hylafax and capisuite: own MSN (fax number without prefix)"},
  // T_Hylafax_Praefix_fuer_ausserorts_zB_0
  {"Hylafax: Praefix fuer ausserorts (z.B. '0')","hylafax: prefix for long distance (e.g. '0')"},
  // T_Hylafax_Praefix_fuer_das_Ausland_zB_00
  {"Hylafax: Praefix fuer das Ausland (z.B. '00')","hylafax: prefix for abroad (e.g. '00')"},
  // T_Hylafax_bis_10_Buchstabe_fuer_eigenen_Namen
  {"Hylafax: bis 10 Buchstaben fuer eigenen Namen","hylafax: up to 10 letters for own name"},
  // T_Capisuite_bis_20_Buchstaben_fuer_eigenen_Namen
  {"Capisuite: bis 20 Buchstaben fuer eigenen Namen","Capisuite: up to 20 letters for own name"},
  // T_Zahl_der_Klingeltoene_bis_Capisuite_den_Anruf_annimmt
  {"Zahl der Klingeltoene, bis Capisuite den Anruf annimmt","Number of ring bells, until capisuite accepts call"},
  // T_Zahl_der_Klingeltoene_bis_Hylafax_den_Anruf_annimmt
  {"Zahl der Klingeltoene, bis Hylafax den Anruf annimmt","Number of ring bells, until hylafax accepts call"},
  // T_Sollen_die_Dateien_unabhaengig_vom_Faxerfolg_im_Zielverzeichnis_gespeichert_werden
  {"Sollen die Dateien unabhaengig vom Faxerfolg im Zielverzeichnis gespeichert werden",
    "Shall files be stored in target directory irrespective of fax success"},
  // T_wartend
  {"wartend","waiting"},
  // T_gesandt
  {"gesandt","sent"},
  // T_gescheitert
  {"gescheitert","failed"},
  // T_nicht_in_der_Warteschlange
  {"nicht in der Warteschlage","not in queue"},
  // T_woasined
  {"unbekannt","unknown"},
  // T_Der_regulaere_Ausdruck
  {"Der regulaere Ausdruck '","The regular expression '"},
  // T_konnte_nicht_kompiliert_werden
  {"' konnte nicht kompiliert werden.","could not be compiled."},
  // Verbindung_zur_Datenbank_nicht_herstellbar
  {"Verbindung zur Datenbank nicht herstellbar, fehnr: ","Connection to the database could not be established, errnr: "},
  // T_Breche_ab
  {". Breche ab.","Stopping."},
  // T_Konnte_Verzeichnis
  {"Konnte Verzeichnis ","Could not open the directory"},
  // T_nicht_oeffnen
  {" nicht oeffnen",""},
  // T_zielname_erstes_Ziel
  {" zielname:  erstes Ziel: '"," target name:  first target: '"},
  // T_zielname_Ausweichziel
  {" zielname: Ausweichziel: '"," target name:  alternate target: '"},
  // T_Datei
  {"Datei: '","File: '"},
  // T_entspricht
  {"' entspricht "," "},
  // T_entsprichtnicht
  {"nicht ","does not match to "},
  // T_entsprichtdoch
  {"","matches to "},
  // T_Muster_Doppelpunkt
  {"Muster: '","pattern: '"},
  // T_Verschiebe
  {"Verschiebe: '","Moving: '"},
  // T_Fehler_beim_Verschieben
  {"Fehler beim Verschieben","Error while moving"},
  // T_Kopiere_Doppelpunkt
  {"Kopiere: '","Copying: "},
  // T_Fehler_beim_Kopieren
  {"Fehler beim Kopieren: ","Error while copying: "},
  // T_Dateiname,
  {"Dateiname ","File name "},
  // T_schlechtgeformt
  {" schlecht geformt!","malformed!"},
  // T_Fehler_af,
  {"Fehler ","Errror "},
  // T_bei
  {" bei: "," at: "},
  // T_ja
  {"ja","yes"},
  // T_nein
  {"nein","no"},
  // T_obcapimitDoppelpunkt,
  {"mitCapi: ","withCapi: "},
  // T_obhylamitDoppelpunkt
  {"mitHyla: ","with hyla: "},
  // T_Endung
  {"Endung: ","Ending: "},
  // T_Stamm
  {"Stamm:  ","Root: "},
  // T_trennenach
  {"trenne nach: '","separating by: '"},
  // T_ErstelledurchKopieren
  {"Erstelle durch Kopieren: ","Providing by copying: "},
  // T_FehlerbeimUmbenennen
  {"Fehler beim Umbenennen!","Error while renaming!"},
  // T_ErstelledurchBenennen
  {"Erstelle durch Benennen: ","Providing by naming: "},
  // T_autofaxabgebrochen
  {"autofax abgebrochen! ","autofax aborted! "},
  // T_FehlerbeimUmbenennenbei
  {" Fehler beim Umbenennen bei:"," Error while renaming:"},
  // T_aus
  {"aus '","in '"},
  // T_nichtbeschreibbar
  {" nicht beschreibbar!","not to open for writing!"},
  // T_KeinModemmitNamensmuster
  {"Kein Modem mit Namensmuster '","No modem found with name pattern '"},
  // T_gefundenverwendeHylafaxnicht
  {"' gefunden, verwende Hylafax nicht.",", not utilizing hylafax."},
  // T_Baudratevon
  {"Baudrate von '","Baud rate of '"},
  // T_mit_af
  {"' mit ","' with "},
  // T_zugeringVerwendeHylafaxnicht
  {" zu gering. Verwende Hylafax nicht."," too small. Not utilizing hylafax."},
  // T_hylafax_faxmodem_nicht_gefunden_Versuche_es_zu_installieren_mit
  {"hylafax (faxmodem) nicht gefunden. Versuche es zu installieren mit","hylafax (fax modem) not found. Trying to install it with"},
  // T_StarteHylafax
  {"Starte Hylafax ...","Starting hylafax ..."},
  // T_hylafaxspringtnichtan
  {"hylafax.service springt nicht an. Verwende Hylafax nicht.","hylafax.service does not start up. Not utilizing hylafax."},
  // T_haengean
  {"haenge an ","appending to "},
  // T_an_mdpp
  {" an: ",": "},
  // T_KannFcpciNInstVerwCapiNicht
  {"Kann Fritz-Modul fcpci nicht installieren, verwende capi nicht.","Cannot install module fcpci for the fritz card, not utilizing capi."},
  // T_Kernelversion
  {"Kernelversion: ","Kernel version: "},
  // T_nichtgefFcpciMfdKinstallierwerden
  {"' nicht gefunden, fcpci muss fuer diesen Kernel installiert werden, versuche es:",
    "' not found, fcpci must be installed for this kernel, trying to:"},
  // T_Konnte
  {"Konnte '","Could not start '"},
  // T_nichtstarten
  {"' nicht starten",""},
  // T_VersucheDatei
  {"Versuche Datei: '","Trying to install file: '"},
  // T_zuinstallieren
  {"' zu installieren",""},
  // T_bittestellenSiedieInstallationsdatei
  {"Bitte stellen Sie die Installationsdatei '","Please provide the installation file '"},
  // T_bereitevtlvonfkn
  {"' bereit, evtl. von www.fkn-systems.de!",", possibly from www.fkn-systems.de!"},
  // T_StarteCapisuite
  {"Starte Capisuite ...","Starting Capisuite ..."},
  // T_Umwandlungvon
  {"Umwandlung von ","Conversion of "},
  // T_inPDFmit
  {" in PDF mit "," to PDF using "},
  // T_beendetErgebnis
  {" beendet, Ergebnis: "," finished, result: "},
  // T_misserfolg
  {"Misserfolg","failure"},
  // T_Erfolg_af
  {"Erfolg","success"},
  // T_lstatfehlgeschlagen
  {"lstat fehlgeschlagen: ","lstat failed: "},
  // T_beiDatei
  {" bei Datei:"," at file: "},
  // T_konntecapisuiteservice
  {"Konnte capisuite.service ","Could not start capisuite.service "},
  // T_malnichtstartenverwN
  {" mal nicht starten, verwende es nicht."," times, not utilizing it."},
  // T_SpoolDateierstellt
  {"Spool-Datei erstellt: '","Generated spool file: '"},
  // T_SpoolDatei
  {" Spool-Datei: '","Spool file: '"},
  // T_SpoolPfad
  {" Spool-Pfad: '"," Spool directory: '"},
  // T_DieFaxnrausTabelle
  {"Die Faxnr aus Tabelle `","The fax number from table `"},
  // T_istleerfaxeesdahernicht
  {" ist leer! Faxe es dahaer nicht."," is empty! Not faxing it therefore."},
  // T_DieFaxnrvon
  {"Die Faxnr von ","The fax number of "},
  // T_ist
  {" ist: "," is: "},
  // T_faxemitCFehler
  {"faxemitC() Fehler: ","faxemitC() Error: "},
  // T_Faxdatei
  {"Faxdatei ","Fax file "},
  // T_fehlt
  {" fehlt."," is missing."},
  // T_hat0Bytes
  {" hat 0 Bytes."," has 0 bytes."},
  // T_Faxbefehl
  {"Faxbefehl","Fax command"},
  // T_nichtgefundenloeschesieausDB
  {") nicht gefunden, loesche sie aus der Datenbank",") not found, deleting it from database"},
  // T_KeinErgebnisbeimFaxen
  {"Kein Ergebnis beim Faxen!","Faxing with no result!"},
  // T_HylafaxBefehl
  {"Hylafax-Befehl","Hylafax command"},
  // T_Anwahlen
  {" Anwahlen, "," tries, "},
  // T_kommaDatei
  {", Datei: ",", file: "},
  // T_Status
  {" Status: "," State: "},
  // T_bzw
  {", bzw.: ",", and: "},
  // T_Falschals
  {"Falsch als ","Wrongly archived as "},
  // T_eingeordnet
  {" eingeordnet: ",": "},
  // T_DateienzahlimVz
  {"Dateienzahl im Vz: ","No. of files in directory: "},
  // T_ZahldDSmwegzuschickendenFaxenin
  {"Zahl der Datensaetze mit wegzuschickenden Faxen in `","No. of data records with enqueued faxes in `"},
  // T_PDFDatei
  {" PDF-Datei: "," PDF file: "},
  // T_beiSQLAbfrage
  {" bei SQL-Abfrage: "," at SQL request: "},
  // T_obgescheitert
  {" obgescheitert: "," failed?: "},
  // T_Fehler_beim_Verschieben_Ausrufezeichen
  {"Fehler beim Verschieben!","Error while moving!"},
  // T_0Bytes
  {"0 Bytes: '","0 bytes: '"},
  // T_Loesche_Ausrufezeichen
  {"Loesche: ","Deleting: "},
  // T_nicht_geloescht_war_eh_nicht_mehr_da
  {" nicht geloescht, war eh nicht mehr da."," not deleted, was no more there."},
  // T_Verwaiste_Datei
  {"Verwaiste Datei: ","Orphaned file: "},
  // T_geloescht_Fax_schon_in
  {" geloescht (Fax schon in "," deleted (fax already archived in "},
  // T_archiviert_Ausrufezeichen
  {" archiviert)!","!"},
  // T_Weitere_Spool_Eintraege
  {"Weitere Spool-Eintraege:","Further spool entries:"},
  // T_Zeit_Doppelpunkt
  {"Zeit: ","Time: "},
  // T_Zahl_der_ueberpruefen_Datenbankeintraege
  {" Ueberpruefte Datenbankeintraege: ","No.of reviewed database entries: "},
  // T_davon_gescheiterte_Faxe
  {"... davon      gescheiterte Faxe: ","... thereof        failed faxes: "},
  // T_davon_erfolgreiche_Faxe
  {"... davon      erfolgreiche Faxe: ","... thereof    successful faxes: "},
  // T_davon_noch_wartende_Faxe
  {"... davon     noch wartende Faxe: ","... thereof still waiting faxes: "},
  // T_davon_nicht_in_Warteschlange
  {"... davon nicht in Warteschlange: ","... thereof    not queued faxes: "},
  // T_Fertig_mit
  {"Fertig mit ","Ready with "},
  // T_Fehler_beim_Datumsetzen_von
  {"Fehler beim Datumsetzen von '","Error setting date of '"},
  // T_eindeutige_Identifikation
  {"eindeutige Identifikation","distinct identification"},
  // T_Originalname_der_Datei
  {"Originalname der Datei","original name of the file"},
  // T_Originalname_der_Datei_vor_Umwandlung_in_PDF
  {"Originalname der Datei vor Umwandlung in PDF","original name of the file before converting to pdf"},
  // T_zu_senden_an
  {"zu senden an","to be sent to"},
  // T_Prioritaet_aus_Dateinamen
  {"Prioritaet aus Dateinamen: 0=keine, 1=capi, 2=hyla","Priority from file name: 0=none, 1=capi, 2=hyla"},
  // T_Zahl_der_bisherigen_Versuche_in_Capisuite
  {"Zahl der bisherigen Versuche in Capisuite","No. of previous tries in Capisuite"},
  // T_Zahl_der_bisherigen_Versuche_in_Hylafax
  {"Zahl der bisherigen Versuche in Hylafax","No. of previous tries in Hylafax"},
  // T_Spooldatei_in_Capisuite
  {"Spooldatei in Capisuite","Spool file in Capisuite"},
  // T_Pfad_zur_Spooldatei_in_Capisuite_ohne_abschliessendes_Verzeichnistrennzeichen
  {"Pfad zur Spooldatei in Capisuite, ohne abschliessendes Verzeichnistrennzeichen",
    "Path to the spool file in Capisuite, without closing directory separator"},
  // T_Aenderungszeit_der_CapiSpoolDatei
  {"Aenderungszeit der CapiSpooldatei","Modification time of the capi spool file"},
  // T_Aenderungszeit_der_CapiSpooldatei_im_time_Format
  {"Aenderungszeit der CapiSpooldatei, im time()-Format","Modification time of the capi spool file, in time() format"},
  // T_Aenderungszeit_der_Hylaspooldatei
  {"Aenderungszeit der Hyla-Spooldatei","Modification time of the hyla spool file"},
  // T_Aenderungszeit_der_Hylaspooldatei_im_Time_Format
  {"Aenderungszeit der Hyla-Spooldatei, im time()-Format","Modification time of the hyla spool file, in time() format"},
  // T_jobid_in_letztem_gescheitertem_hylafax
  {"jobid in letztem gescheitertem hylafax","job id of the last failed hylafax"},
  // T_state_Feld_in_hylafax
  {"state-Feld in hylafax","state field in hylafax"},
  // T_statuscode_in_letztem_gescheitertem_hylafax
  {"statuscode in letztem gescheitertem hylafax","status code of the last failed hylafax"},
  // T_status_in_letztem_gescheitertem_hylafax
  {"status in letztem gescheitertem hylafax","status of the last failed hylafax"},
  // T_capispooldateien_der_Capisuite
  {"capiSpooldateien der Capisuite","capi spool files of capisuite"},
  // T_Fehler_beim_Pruefen_von
  {"Fehler beim Pruefen von: ","Error while examining: "},
  // T_Name_des_Adressaten
  {"Name des Adressaten","Name of the receiver"},
  // T_Datum_des_Losschickens
  {"Datum des Losschickens","submit time"},
  // T_Ende_der_Uebertragung
  {"Ende der Uebertragung","end of submission"},
  // T_Kennung_im_Faxsystem_hylafax_Nr_Capisuite_Datei_MSFax_Fax_Dateiname
  {"Kennung im Faxsystem (hylafax: Nr., Capisuite: Datei, MS Fax: Fax-Dateiname",
    "identifier in fax system (hylafax: no, capisuite: file, ms fax: fax file name"},
  // T_Dateigroesse
  {"Dateigroesse","file size"},
  // T_Seitenzahl
  {"Seitenzahl","no. of pages"},
  // T_Zahl_der_Anwahlen
  {"Zahl der Anwahlen","No. of tries"},
  // T_FAxnummer_des_Adressaten
  {"Faxnummer des Adressaten","receiver's fax no"},
  // T_Titel_des_Adressaten
  {"Titel des Adressaten","receiver's fax no"},
  // T_PID_falls_aus_Dateinamen_ermittelt
  {"PID, falls aus Dateinamen ermittelt","PID, if excerpted from file name"},
  // T_Kurzbezeichnung_der_eigenen_Faxstelle
  {"Kurzbezeichnung der eigenen Faxstelle","short name of the own fax location"},
  // T_Geraetename
  {"Geraetename","device name"},
  // T_outgoing_schedule_time_oder_so_aehnlich_nur_MSFax
  {"outgoing schedule time oder so aehnlich (nur MS Fax)","outgoing schedule time or similar (only ms fax)"},
  // T_Hinweis_auf_Dateiinhalt_oder_Adressat_nur_MSFax
  {"Hinweis auf Dateiinhalt oder Adressat (nur MS Fax)","indication of file content or adressat (only ms fax)"},
  // T_identisch_zu_submid_nur_MSFax
  {"identisch zu submid (nur MS Fax)","identical to submid (only ms fax)"},
  // T_Prioritaet_nur_MSFax
  {"Prioritaet (nur MS Fax)","priority (only ms fax)"},
  // T_FAxnummer_des_Adressaten_nur_MSFax
  {"Faxnummer des Adressaten (nur MS Fax)","receiver's fax no. (only ms fax)"},
  // T_Faxnummer_des_Senders_nur_MSFax
  {"Faxnummer des Senders (nur MS Fax)","sender's fax no. (only ms fax)"},
  // T_Beginn_der_Uebertragung_nur_MSFax
  {"Beginn der Uebertragung (nur MS Fax)","transmission begin (only ms fax)"},
  // T_Archiv_fuer_die_erfolgreichen_Faxe
  {"Archiv fuer die erfolgreichen Faxe","archive for successful faxes"},
  // T_Archiv_fuer_die_gescheiterten_Faxe
  {"Archiv fuer die gescheiterte Faxe","archive for failed faxes"},
  // T_Welches_Fax_soll_geloescht_werden
  {"Welches Fax soll geloescht werden (0=Abbruch)","Which fax shall be deleted (0=Cancel)"},
  // T_Soll_das_Fax_geloescht_werden_0_ist_Abbruch
  {"Soll das Fax ","Shall the fax "},
  // T_wirklich_geloescht_werden
  {" wirklich geloescht werden"," really be deleted"},
  // T_Kein_Fax_zum_Loeschen_vorhanden
  {"Kein Fax zum Loeschen vorhanden","No fax there to be deleted"},
  // T_Keine_ISDN_Karte_gefunden
  {"Keine ISDN-Karte gefunden. Setze ","No ISDN-Card found. Setting "},
  // T_mitCapi
  {"mitCapi","withCapi"},
  // T_auf
  {" auf "," to "},
  // T_verwendet_Kofigurationsdatei_string_anstatt
  {"verwendet Konfigurationsdatei <string> anstatt","uses configuration file <string> instead of"},
  // T_faxt_die_Dateien_aus_pfad_anstatt
  {"faxt die Dateien aus <pfad> anstatt","faxes the files from <path> instead of"},
  // T_Dateien_warten_in_pfad_anstatt
  {"Dateien warten in <pfad> anstatt","files are waiting in <path> instead of"},
  // T_Empfangsverzeichnis_fuer_Faxempfang
  {"Empfangsverzeichnis fuer Faxempfang","directory for recieved faxes"},
  // T_Capisuite_nicht_verwenden
  {"capisuite nicht verwenden","do not use capisuite"},
  // T_hylafax_nicht_verwenden
  {"hylafax nicht verwenden","do not use hylafax"},
  // T_versuche_faxe_zuerst_ueber_Capisuite_wegzuschicken
  {"versuche faxe zuerst ueber Capisuite wegzuschicken","try to send faxes primarily via capisuite"},
  // T_versuche_faxe_zuerst_ueber_hylafax_wegzuschicken
  {"versuche faxe zuerst ueber hylafax wegzuschicken","try to send faxes primarily via hylafax"},
  //      // T_sucht_nach_dev_tty_string_als_Modem_mit_string_anstatt
  //      {"suche nach '/dev/tty<string>*' als Modem, mit <string> anstatt","search for '/dev/tty<string>*' as modem, with <string> instead of"},
  // T_nach_zahl_Versuchen_Capisuite_wird_Hylafax_versucht
  {"nach <zahl> Versuchen Capisuite wird Hylafax versucht, anstatt nach","try Hylafax after <no> tries of Capisuite instead of"}, 
  // T_nach_zahl_Versuchen_Hylafax_wird_Capisuite_verwendet
  {"nach <zahl> Versuchen Hylafax wird Capisuite versucht, anstatt nach","try Capisuite after <no> tries of Hylafax instead of"},
  // T_Zahl_der_Klingeltoene_bis_Capisuite_den_Anruf_annimmt_anstatt
  {"Zahl der Klingeltoene, bis Capisuite den Anruf annimmt, anstatt","No. of bell rings until Capisuite accepts the call, instead of"},
  // T_Zahl_der_Klingeltoene_bis_Hylafax_den_Anruf_annimmt_anstatt
  {"Zahl der Klingeltoene, bis Hylafax den Anruf annimmt, anstatt","No. of bell rings until hylafaxs accepts the call, instead of"},
  // T_verwendet_fuer_Capisuite_den_Linux_Benutzer_string_anstatt
  {"verwendet fuer Capisuite den Linux-Benutzer <string> anstatt","takes the linux user <string> for Capisuite instead of"},
  // T_FAxe_werden_auch_ohne_Faxerfolg_ins_Zielverzeichnis_kopiert
  {"Faxe werden auch ohne Faxerfolg ins Zielverzeichnis kopiert","copy faxes into target directory irrespective of faxing success"},
  // T_faxnr_wird_hinter_string_erwartet_statt_hinter
  {"faxnr wird hinter <string> erwartet statt hinter","the fax number will be expected after <string> instead of"},
  // T_faxnr_fuer_primaer_Capisuite_wird_hinter_string_erwartet_statt_hinter
  {"faxnr fuer primaer Capisuite wird hinter <string> erwartet statt hinter",
    "fax no.for fax with preference to capisuite is expected after <string> instead of"},
  // T_faxnr_fuer_primaer_hylafax_wird_hinter_string_erwartet_statt_hinter
  {"faxnr fuer primaer hylafax wird hinter <string> erwartet statt hinter",
    "fax no.for fax with preference to hylafax is expected after <string> instead of"},
  // T_Adressatenname_wird_hinter_string_erwartet_statt_hinter
  {"Adressatenname wird hinter <string> erwartet statt hinter","name of addressee is expected after <string> instead of"},
  // T_Trennstring_string_fuer_mehrere_Adressaten_Telefonnummern_statt
  {"Trennstring <string> fuer mehrere Adressaten/Telefonnummern statt",
    "separating string <string> for multiple addressees/tel'numbers, instead of"},
  // T_verwendet_die_Datenbank_auf_Host_string_anstatt_auf
  {"verwendet die Datenbank auf Host <string> anstatt auf","takes the database on host <string> instead of"},
  // T_verwendet_fuer_MySQL_MariaDB_den_Benutzer_string_anstatt
  {"verwendet fuer MySQL/MariaDB den Benutzer <string> anstatt","takes the user <string> for MySQL/MariaDB instead of"},
  // T_verwendet_fuer_MySQL_MariaDB_das_Passwort_string_anstatt
  {"verwendet fuer MySQL/MariaDB das Passwort <string> anstatt","takes the password <string> for MySQL/MariaDB instead of"},
  // T_verwendet_die_Datenbank_string_anstatt
  {"verwendet die Datenbank <string> anstatt","uses the database <string> instead of"},
  // T_protokolliert_ausfuehrlich_in_Datei
  {"protokolliert ausfuehrlich in Datei '","put detailed logs in file '"},
  // T_sonst_knapper
  {"' (sonst knapper)","' (otherwise shorter)"},
  // T_waehlt_als_Logverzeichnis_pfad_derzeit
  {"waehlt als Logverzeichnis <pfad>, derzeit","choses <path> as log directory, currently"},
  // T_logdatei_string_im_Pfad
  {"logdatei <string> (im Pfad '","log file <string> (in path '"},
  // T_wird_verwendet_anstatt
  {"') wird verwendet anstatt","') will be used instead of"},
  // T_logdatei_vorher_loeschen
  {"logdatei vorher loeschen","delete log file afore"},
  // T_Bildschirmausgabe_gespraechiger
  {"Bildschirmausgabe gespraechiger","screen output more verbose"},
  // T_Bildschirmausgabe_mit_SQL_Befehlen
  {"Bildschirmausgabe mit SQL-Befehlen","screen output with SQL commands"},
  // T_alle_Parameter_werden_abgefragt_darunter_einige_hier_nicht_gezeigte
  {"alle Parameter werden abgefragt (darunter einige hier nicht gezeigte)","all parameters will be prompted (some of them not shown here)"},
  // T_ein_Fax_nach_Rueckfrage_loeschen
  {"ein Fax nach Rueckfrage loeschen","delete a fax with query"},
  // T_Eintraege_aus
  {"Eintraege aus `","delete entries from `"},
  // T_loeschen_zu_denen_kein_Datei_im_Wartevz_und_kein_Capi_oder_Hylafax_nachweisbar_ist
  {"` loeschen, zu denen keine Datei im Wartevz.und kein Capi- oder Hylafax nachweisbar ist",
    "` without detection of file in waiting directory or capisuite fax or hyla-fax"},
  // T_alle_wartenden_Faxe_und_zugehoerige_Eintraege_aus
  {"alle wartenden Faxe und zugehoerige Eintraege aus `","delete all waiting faxes and associated entries from `"},
  // T_loeschen
  {"` loeschen","`"},
  // T_Dateien_aus_Warteverzeichnis_gegen
  {"Dateien aus Warteverzeichnis gegen `","Examine files in waiting directory against the tables `"},
  // T_pruefen_und_verschieben
  {"` pruefen und ggf. verschieben","` and clean them up"},
  // T_listet_Datensaetze_aus
  {"listet Datensaetze aus `","lists entries from `"},
  // T_Zahl_der_aufzulistenden_Datensaetze_ist_zahl_statt
  {"Zahl der aufzulistenden Datensaetze = <zahl> statt","No. of listed entries = <zahl> instead of"},
  // T_Zeigt_diesen_Bildschirm_an
  {"Zeigt diesen Bildschirm an","shows this screen"},
  // T_Fertig_mit_Parsen_der_Befehlszeile
  {"Fertig mit Parsen der Befehlszeile","Parsing the command line finished"},
  // T_Gebrauch
  {"Gebrauch: ","Usage: "},
  // T_Faxt_Dateien_aus_Verzeichnis_pfad_die
  {"Faxt Dateien aus Verzeichns <pfad>, die '","Faxes files from directory <path>, which contain '"},
  // T_faxnr_enthalten_und_durch_soffice_in_pdf_konvertierbar_sind_und_traegt_sie
  {" <faxnr>' enthalten und durch soffice in pdf konvertierbar sind \nund traegt sie in MariaDB-Datenbank '",
    " <faxno>' and are convertible into pdf, \nand logs this in the the mariadb database '"}, 
  // T_Tabellen,
  {"' (Tabellen: `","' (tables: `"},
  // T_aein
  {"`) ein.","`)."},
  // T_Verzeichnis_mit_zu_faxenden_Dateien
  {"Verzeichnis mit zu faxenden Dateien","Directory with files to fax"},
  // T_Verzeichnis_mit_wartenden_Dateien
  {"Verzeichnis mit wartenden Dateien","Directory with waiting files"},
  // T_Verzeichnis_mit_gescheiterten_Dateien
  {"Verzeichnis mit gescheiterten Dateien","Directory with failed files"},
  // T_Verzeichnis_fuer_empfangene_Faxe
  {"Verzeichnis fuer empfangene Faxe","Directory for received faxes"},
  // T_Buchstabenfolge_vor_erster_Faxnummer
  {"Buchstabenfolge vor erster Fax-Nummer","Letter-sequence before the first fax number"},
  // T_Buchstabenfolge_vor_erster_Fax_Nummer_primaer_fuer_Capisuite
  {"Buchstabenfolge vor erster Fax-Nummer primaer fuer Capisuite","Letter-sequence before the first fax number primarily for Capisuite"},
  // T_Buchstabenfolge_vor_erster_Fax_Nummer_primaer_fuer_Hylafax
  {"Buchstabenfolge vor erster Fax-Nummer fuer Hylafax","Letter-sequence before the first fax number primarily for hylafax"},
  // T_Buchstabenfolge_vor_erstem_Adressaten
  {"Buchstabenfolge vor erstem Adressaten","Letter-sequence before the first addressee"},
  // T_Buchstabenfolge_vor_weiterem_Adressaten_sowie_weiterer_Faxnummer,
  {"Buchstabenfolge vor weiterem Adressaten sowie weiterer Faxnummer","Letter-sequence before further addressee or fax number"},
  // T_Host_fuer_MySQL_MariaDB_Datenbank
  {"Host fuer MySQL/MariaDB-Datenbank","host for mysql/mariadb-database"},
  // T_Benutzer_fuer_MySQL_MariaDB,
  {"Benutzer fuer MySQL/MariaDB:","user for mysql/mariadb:"},
  // T_Passwort_fuer_MySQL_MariaDB,
  {"Passwort fuer MySQL/MariaDB","password for mysql/mariadb"},
  // "Datenbankname fuer MySQL/MariaDB auf '"
  {"Datenbankname fuer MySQL/MariaDB auf '","database name for mysql/mariabd on '"},
  // T_Logverzeichnis
  {"Logverzeichnis","log directory"},
  // T_Logdateiname
  {"Logdateiname","log file name"},
  // T_Zahl_der_SQL_Befehle_fuer_Namenszuordnung
  {"Zahl der SQL-Befehle fuer Namenszuordnung:","No.of SQL-commands for assignment of fax names"},
  // T_SQL_Befehl_Nr,
  {"SQL-Befehl Nr. ","SQL command no. "},
  // T_Capisuite_ist_offenbar_noch_nicht_richtig_konfiguriert
  {"Capisuite ist offenbar noch nicht konfiguriert (","Capisuite seems not to be configured correcty ("},
  // T_ist_Apostroph,
  {" ist '"," is '"},
  // T_Die_Einstellungen_koennen_spaeter_in,
  {" Die Einstellungen koennen spaeter in '"," The settings can later be modified in '"},
  // T_geaendert_werden,
  {"' geaendert werden.","'"},
  // T_Faxnr_die_zum_Adressaten_gesandt_wird_bis_20_Zeichen_nur_plus_und_Ziffern,
  {"Faxnr., die zum Adressaten gesandt wird (bis 20 Zeichen, nur '+' und Ziffern)",
    "Fax number which ist sent to the addressee (up to 20 characters, only '+' and numerals)"},
  // T_ausgehende_Multiple_Subscriber_Number_Faxnummer_ohne_Vorwahl,
  {"hinausgehende 'Multiple Subscriber Number' (Faxnummer ohne Vorwahl)","outgoing 'multiple subscriber number' (fax number without prefix)"},
  // T_Amtsholung_ueblicherweise_kk_oder_0,
  {"Amtsholung, ueblicherweise '' oder '0'","outside line access, typically '' or '0'"},
  // T_Faxueberschrift,
  {"Faxueberschrift","fax headline"},
  // T_Adressat_empfangener_Faxe_die_ueber_Email_verteilt_werden,
  {"Adressat empfangener Faxe, die ueber Email verteilt werden","addressee of received faxes which are distributed via email"},
  // T_Zahl_der_Sendeversuche,
  {"Zahl der Sendeversuche","Number of transmission tries"},
  // T_kommagetrennte_Liste_mit_Sekundenabstaenden_zwischen_Sendeversuchen,
  {"kommagetrennte Liste mit Sekundenabstaenden zwischen Sendeversuchen","comma separated list with distances between transmission tries"},
  // T_Geduld_bis_zum_Verbindungsaufbau_in_s,
  {"Geduld bis zum Verbindungsaufbau in s","timeout for connection establishment"},
  // T_komma_wert,
  {", wert: ",", value: "},
  // T_komma_Altwert,
  {", Altwert: ",", old value: "},
  // T_Programm,
  {"Programm: ","Program: "},
  // T_Muster,
  {"Muster ","Pattern "},
  // T_Ziel,
  {"Ziel ","Target "},
  // T_Logpfad,
  {"Logpfad: '","Log path: '"},
  // T_oblog,
  {"' (oblog: ","' (with logging: "},
  // T_Ende,
  {"-Ende-","-End-"},
  // T_Fax_von
  {"Fax von ","Fax from "},
  // T_an,
  {" an "," to "},
  // T_vom
  {" vom "," at "},
  // T_fuer_Benutzer
  {" fuer Benutzer '"," for user '"},
  // T_Alle_wieviel_Minuten_soll_Autofax_aufgerufen_werden_0_ist_gar_nicht
  {"alle wieviel Minuten soll autofax ueber crontab aufgerufen werden (0=gar nicht)",
    "every how many minutes shall autofax be called in crontab (0=not at all)"},
  // T_Kann_Capisuite_nicht_installieren_verwende_Capi_nicht
  {"Kann Capisuite nicht installieren, verwende Capi nicht","Can't install capisuite, not using capi"},
  // T_wird_verwendet
  {" wird verwendet."," being used."},
  // T_mit_Baudrate,
  {"' mit Baudrate ","' with baud rate "},
  // T_Fuer_Hylafax_verwendetes_Modem
  {"Fuer Hylafax verwendetes Modem","Modem used for hylafax"},
  // T_archiviere
  {"archiviere()","archiving()"},
  // T_loeschecapi
  {"loeschecapi()","deletecapi()"},
  // T_loeschehyla
  {"loeschehyla()","deletehyla()"},
  // T_WVZinDatenbank
  {"WVZinDatenbank()","WDirinDatabase()"},
  // T_pruefhardware
  {"pruefhardware()","checkhardware()"},
  // T_setzhylavz
  {"setzhylavz()","sethyladir()"},
  // T_lieskonfein
  {"lieskonfein()","readconf()"},
  // T_rueckfragen
  {"rueckfragen()","callbacks()"},
  // T_autokonfschreib
  {"autokonfschreib()","autoconfwrite()"},
  // T_konfcapi
  {"konfcapi()","confcapi()"},
  // T_verzeichnisse
  {"verzeichnisse()","directories()"},
  // T_pruefcron
  {"pruefcron()","checkcron()"},
  // T_korrerfolgszeichen
  {"korrerfolgszeichen()","correctwrongsuccessflag()"},
  // T_bereinigewv
  {"bereinigewv()","purgewaitingdirectory()"},
  // T_loeschefax
  {"loeschefax()","deletefax()"},
  // T_loeschewaise,
  {"loeschewaise()","deleteorphans()"},
  // T_loescheallewartenden,
  {"loescheallewartenden()","deleteallwaiting()"},
  // T_tu_lista
  {"tu_lista()","do_listo()"},
  // T_tu_listi
  {"tu_listi()","do_listi()"},
  // T_DateienHerricht
  {"DateienHerricht()","PrepareFiles()"},
  // T_faxealle
  {"faxealle()","faxingall()"},
  // T_untersuchespool,
  {"untersuchespool()","examinespool()"},
  // T_zeigweitere
  {"zeigweitere()","showothers()"},
  // T_empfarch
  {"empfarch()","archiveReceived()"},
  // T_schlussanzeige
  {"schlussanzeige()","finaldisplay()"},
  // T_zielname
  {"zielname()","targetname()"},
  // T_PIDausName
  {"PIDAausName()","PIDfromName()"},
  // T_pruefhyla
  {"pruefhyla()","checkhyla()"},
  // T_pruefrules
  {"pruefrules()","checkrules()"},
  // T_pruefblack
  {"pruefblack()","checkblack()"},
  // T_pruefmodcron
  {"pruefmodcron()","checkmodcron()"},
  // T_pruefcapi,
  {"pruefcapi()","checkcapi()"},
  // T_inDbc
  {"inDbc()","intoDatabaseCapi()"},
  // T_faxemitC
  {"faxemitC()","faxingwithCapi()"},
  // T_inDBh
  {"inDBh()","intoDatabaseHyla()"},
  // T_faxemitH
  {"faxemitH()","faxingwithHyla()"},
  // T_prufespool
  {"pruefespool()","checkspool()"},
  // T_pruefouta
  {"pruefeouta()","checkouta()"},
  // T_pruefinca
  {"pruefinca()","checkinca()"},
  // T_pruefprocgettel3
  {"pruefprocgettel3()","checkprocgettel3()"},
  // T_capiwausgeb
  {"capiwausgeb()","displaycapitries()"},
  // T_hylaausgeb
  {"hylaausgeb()","displayhylatries()"},
  // T_tuloeschen
  {"tuloeschen()","dodelete()"},
  // T_Installation_von_Hylafax_nicht_feststellbar_versuche_es_zu_installieren
  {"Installation von Hylafax nicht feststellbar, versuche es zu installieren ...",
    "Installation of hylafax not statable, trying to install it ..."},
  // T_Hylafaxplus_entdeckt_muss_ich_loeschen
  {"Hylafax+ entdeckt, muss ich loeschen ...","Hylafax+ found, having to delete it ..."},
  // T_RueckmlgZeile
  {"Rueckmldg.Zeile: ","Response line: "},
  // T_Hylafax_laeuft
  {"Hylafax laeuft!","Hylfax is running!"},
  // T_Pruefe_ob_Hylafax_gestartet
  {"Pruefe, ob Hylafax gestartet ...","Checking if hylafax is running ..."},
  // T_1_ist_erfolgreiche_Uebertragung_0_ist_fehlgeschlagen
  {"1=erfolgreiche Uebertragung, 0=fehlgeschlagen","1=successful transmission, 0=failure"},
  // T_in_der_Datenbanktabelle
  {"in der Datenbanktabelle `","in the database table `"},
  // T_wird_das_Erfolgszeichen_korrigiert
  {"` wird das Erfolgszeichen korrigiert","` the success flag is being corrected"},
  // T_mit_Erfolgskennzeichen_auf
  {"` mit Erfolgskennzeichen auf","` with success flag"},
  // T_ohne_Erfolgskennzeichen_auf
  {"` ohne Erfolgskennzeichen auf","` without success flag"},
  // T__auf
  {"` auf","`"},
  // T_Zahl_der_empfangenen_Faxe
  {"       Zahl der empfangenen Faxe: ","       Number of received faxes: "},
  // T_von
  {" von "," from "},
  // T_nicht_erkannt
  {" nicht erkannt!"," not identified!"},
  // T_Fehlermeldung_beim_Loeschversuch_eines_Hyla_Faxes_mit_faxrm
  {"Fehlermeldung beim Loeschversuch eines Hyla-Faxes mit `faxrm ","Error while trying to delete a hyla-fax with `faxrm "},
  // T_Zahl_der_nicht_geloeschten_Dateien
  {"Zahl der nicht geloeschten Dateien: ","No. of not deleted files: "},
  // T_hylanr
  {", hylanr: ",", hylano.: "},
  // T_Capispooldatei
  {", Capispooldatei: ",", capispoolfile: "},
  // T_Gesamt
  {"Gesamt: ","Total: "},
  // T_Verwende
  {"Verwende: ","Using: "},
  // T_Hylafax_ohne_plus_entdeckt_muss_ich_loeschen
  {"Hylafax ohne + entdeckt, muss ich loeschen ...","hylafax without plus found, must delete it ..."},
  // T_Versuch
  {" Versuch: "," try no.: "},
  // T_nicht_gefunden_kein_Datenbankeintrag
  {"' nicht gefunden, kein Datenbankeintrag!"," not found, no database entry!"},
  // T_setzhylastat
  {"setzhylastat()","sethylastat()"},
  // T_empfangene_Dateien_loeschen_die_nicht_verarbeitet_werden_koennen
  {"empfangene Dateien loeschen, die nicht verarbeitet werden koennen","delete received files that could not be processed"},
  // T_hylafehlt
  {"hylafehlt: ","hylamissing: "},
  // T_capilaeuft
  {"capilaeuft: ","capirunning: "},
  // T_spruef_sname
  {"spruef(), sname: ","sprove(), sname: "},
  // T_nicht_gefunden_versuche_ihn_einzurichten
  {" nicht gefunden, versuche ihn einzurichten"," not found, trying to install it"},
  // T_gefunden
  {" gefunden."," found."},
  // T_Kein_Modem_gefunden
  {"Kein Modem gefunden.","No modem found."},
  // T_kein_Faxprogramm_verfuegbar
  {"(kein Faxprogramm verfuegbar)","(no fax program available)"},
  // T_Capisuite_gestartet
  {"Capisuite gestartet.","capisuite started."},
  // T_schreibe_Konfigurkation
  {"schreibe Konfiguration!","writing configuration!"},
  // T_Dateien
  {"Dateien ","Files "},
  // T_nicht_verarbeitbar_Verschiebe_sie_nach
  {" nicht verarbeitbar. Verschiebe sie nach "," not processable. Moving them to "},
  // T_Fuehre_aus_Dp
  {"Fuehre aus: ","Executing: "},
  // T_falls_es_hier_haengt_bitte_erneut_aufrufen
  {" (falls es hier haengt, bitte erneut aufrufen)"," (if it hangs, please invoke again)"},
  // T_pruefsamba
  {"pruefsamba()","checksamba()"},
  // T_Zufaxen
  {"Zufaxen","TobeFaxed"},
  // T_Warteauffax
  {"Warteauffax","WaitingFaxes"},
  // T_Nichtgefaxt
  {"Nichtgefaxt","NotFaxed"},
  // T_Faxempfang
  {"Faxempfang","FaxArrival"},
  // T_Gefaxt
  {"Gefaxt","Faxed"},
  // T_lief_schon
  {" lief schon."," ran already."},
  // T_zu_schreiben
  {"zu schreiben: ","must write: "},
  // T_Zahl_der_Verzeichnisse_fuer_erfolgreich_verschickte_Faxe
  {"Zahl der Verzeichnisse fuer erfolgreich verschickte Faxe","No of directories for successfully sent faxes"},
  // T_Verzeichnis
  {"Verzeichnis '","Directory '"},
  // T_nicht_als_Sambafreigabe_gefunden_wird_ergaenzt
  {"' nicht als Sambafreigabe gefunden, wird ergaenzt.","' not found as or under a samba share, amending it."},
  // T_zufaxenvz
  {"zufaxenvz: '","outgoing dir: '"},
  // VorgbSpeziell_Basis
  {"VorgbSpeziell_Basis()","specificprefs_base()"},
  // "Wollen Sie noch einen SQL-Befehl eingeben?"
  {"Wollen Sie noch einen SQL-Befehl eingeben?","Do You want to enter another sql command?"},
  // T_Strich_ist_SQL_Befehl_loeschen_faxnr_wird_ersetzt_mit_der_Faxnr_vollnr_faxnr_mit_deren_Zusammenziehlung
  {" ('-'=SQL-Befehl loeschen,'&&faxnr&&' wird ersetzt mit der Faxnr, 'vollnr('&&faxnr&&')' mit deren Zusammenziehung)",
   " ('-'=delete this sql command,'&&faxnr&&' will be replaces with the fax-no., 'vollnr('&&faxnr&&')' with its concatenation)"},
  // T_faxnr_wird_ersetzt_mit_der_Faxnr_vollnr_faxnr_mit_deren_Zusammenziehlung
  {" ('&&faxnr&&' wird ersetzt mit der Faxnr, 'vollnr('&&faxnr&&')' mit deren Zusammenziehung)",
   " ('&&faxnr&&' will be replaces with the fax-no., 'vollnr('&&faxnr&&')' with its concatenation)"},
  {"",""}
};


extern class Txkonsolecl Txk;
extern class Txdbcl Txd;

string pk = "8A490qdmjsaop4a89d0qÃ9m0943Ã09Ãax";
string ccapiconfdat="/etc/capisuite/capisuite.conf";
string cfaxconfdat="/etc/capisuite/fax.conf";

// Linux-System-Enum
enum lsysen:uchar {usys,sus,deb};

class lsyscl
{
  public:
    lsysen sys; 
    lsysen getsys(int obverb=0,int oblog=0)
    {
      if (sys==usys) {
        if (!systemrueck("cat /proc/version | grep SUSE",obverb,oblog)) return sus;
        if (!systemrueck("cat /proc/version | grep Ubuntu",obverb,oblog)) return deb;
      }
      return usys;
    }
} lsys;

extern class linstcl linst;

using namespace std;
const DBSTyp myDBS=MySQL;

#define autofaxcpp
#include "autofax.h"
constexpr const char *paramcl::moeglhvz[2];
// wird nur in VorgbSpeziell gebraucht:
zielmustercl::zielmustercl(const char * const vmuster,const char * const vziel):muster(vmuster),ziel(vziel)
{
 kompilier();
};  // zielmustercl

zielmustercl::zielmustercl() {
}

int zielmustercl::kompilier() {
  int reti=regcomp(&regex, muster.c_str(),REG_EXTENDED); 
  if (reti) {
    Log(string(Tx[T_Der_regulaere_Ausdruck])+drot+muster+schwarz+Tx[T_konnte_nicht_kompiliert_werden],1,0);
    return 1;
  }
  return 0;
} // zielmustercl::zielmustercl

int zielmustercl::setzemuster(const string& vmuster)
{
 muster=vmuster;
 return kompilier();
}
int zielmustercl::obmusterleer() {
 return muster.empty();
}

const string& zielmustercl::holmuster() {
 return muster;
}

// wird aufgerufen in: loeschefax, untersuchespool, capiwausgeb, setzhylastat, hylaausgeb
inline const char* FxStatS(FxStat *i) 
{
  enum FxStat:uchar {init,wartend,gesandt,gescheitert,fehlend,woasined};
  if (i) {
    switch (*i){
      case init: return "init";
      case wartend: return Tx[T_wartend];
      case gesandt: return Tx[T_gesandt];
      case gescheitert: return Tx[T_gescheitert];
      case fehlend: return Tx[T_nicht_in_der_Warteschlange];
      default:;
    }
  }
  return Tx[T_woasined];
} // FxStatS

// wird aufgerufen in: hservice_faxq_hfaxd, hservice_faxgetty
uchar servc::spruef(const string& sbez,uchar obfork,const string& sexec, const string& CondPath, const string& After, const string& wennnicht0,
                    int obverb,int oblog)
{
  Log(violetts+Tx[T_spruef_sname]+schwarz+sname,obverb,oblog);
  if (!wennnicht0.empty()) {
    servicelaeuft=!systemrueck(wennnicht0,obverb-1,oblog);
  }
  if (servicelaeuft) {
    Log(("Service ")+blaus+sname+schwarz+Tx[T_lief_schon],obverb,oblog);
  } else {
    for(uchar iru=0;iru<2;iru++) {
      if (!obslaeuft(obverb,oblog)) {
        if (serviceda) {
          servicelaeuft=!systemrueck(("killall ")+ename+" >/dev/null 2>&1; systemctl restart "+sname,obverb-1,oblog); // bei restart return value da 
          //          <<hblau<<"serviceda: "<<schwarz<<sname<<", servicelaeuft: "<<(int)servicelaeuft<<endl;
        } else {
          //          <<hblau<<"serviceda else: "<<schwarz<<sname<<endl;
          //  if (systemrueck("systemctl list-units faxq.service --no-legend | grep 'active running'",obverb-1,oblog)) KLA
          string systemd="/usr/lib/systemd/system/"+sname+".service";
          Log(blaus+systemd+Tx[T_nicht_gefunden_versuche_ihn_einzurichten]+schwarz,1,0);
          ofstream syst(systemd.c_str());
          if (syst.is_open()) {
            syst<<"[Unit]"<<endl;
            syst<<"Description="<<sbez<<" Service"<<endl;
            if (!CondPath.empty()) 
              syst<<"ConditionPathExists="<<CondPath<<endl;
            if (!After.empty())
              syst<<"After="<<After<<endl;
            syst<<endl;
            syst<<"[Service]"<<endl;
            if (obfork) 
              syst<<"Type=forking"<<endl;
            syst<<"User=root"<<endl;
            syst<<"Group=root"<<endl;
            syst<<"Restart=always"<<endl;
            syst<<"RestartSec=30"<<endl;
            // if (!spre.empty()) syst<<"ExecStartPre=source "<<spre<<endl;
            syst<<"ExecStart="<<sexec<<endl;
            syst<<endl;
            syst<<"[Install]"<<endl;
            syst<<"WantedBy=multi-user.target "<<endl;
            syst.close();
            systemrueck("systemctl daemon-reload",obverb-1,oblog);
          }
        }
      }
      if (servicelaeuft) { 
        if (systemrueck("systemctl is-enabled "+sname,obverb-1,oblog)) {
          systemrueck("systemctl enable "+sname,obverb,oblog);
        }
        break;
      }
    } //  for(uchar iru=0;iru<2;iru++) 
  } // if (servicelaeuft) else
  return servicelaeuft;
} // void servc::spruef() 

// wird aufgerufen in: pruefhyla, pruefcapi, spruef
int servc::obslaeuft(int obverb,int oblog)
{
  svec sysrueck;
  servicelaeuft=0;
  serviceda=0;
  systemrueck(("systemctl list-units ")+sname+".service --all --no-legend",obverb,oblog,&sysrueck);  // bei list-units return value immer 0
  if (!sysrueck.empty()) {
    Log(blau+sysrueck[0]+schwarz,obverb>1?obverb-1:0,oblog);
    if (sysrueck[0].find("active running")!=string::npos) {
      servicelaeuft=1; 
      serviceda=1;
    } else if (sysrueck[0].find("loaded")!=string::npos) {
      serviceda=1;
    }
  }
  return servicelaeuft;
} // int servc::obslaeuft(int obverb,int oblog)

int servc::restart(int obverb,int oblog)
{
  return systemrueck(string("systemctl restart ")+sname,1,oblog);
}

int servc::enableggf(int obverb,int oblog)
{
 return systemrueck(string("systemctl is-enabled ")+sname+" >/dev/null || systemctl enable >/dev/null"+sname,obverb,oblog);
}

//archiviert den Datensatz
// wird aufgerufen in: untersuchespool
void fsfcl::archiviere(DB *My, paramcl *pmp, struct stat *entryp, uchar obgescheitert, FaxTyp ftyp, uchar *geloeschtp, int obverb, int oblog)
{
  Log(violetts+Tx[T_archiviere]+schwarz+Tx[T_obgescheitert]+blau+ltoan((int)obgescheitert)+schwarz,obverb,oblog);
  // Voraussetzung: telnr, original, id; veraendert: geloescht
  static time_t jetzt = time(0);
  RS rins(My); 
  RS zs(My);
  string getname="",bsname="";
  if (!telnr.empty()) {
    getSender(pmp,telnr,&getname,&bsname);
  }
  for(int runde=0;runde<2;runde++) {
    vector<instyp> einf;
    if (runde==0) zs.Abfrage("SET NAMES 'utf8'");
    else if (runde==1) zs.Abfrage("SET NAMES 'latin1'");
    einf.push_back(instyp(My->DBS,"titel",&bsname));
    einf.push_back(instyp(My->DBS,"rcname",&getname));
    if (capi) {if (cdd.empty()) cdd="0000-00-00";} else {if (hdd.empty()) hdd="0000-00-00";}
    einf.push_back(instyp(My->DBS,"submt",ftyp==capi?&cdd:&hdd));
    einf.push_back(instyp(My->DBS,"submid",ftyp==capi?&capisd:&hylanr));
    string pid;
    if (!original.empty()) {
      const char* ocstr=original.c_str(); // da c_str() fluechtig sein koennte
      char* pidp = (char*)strcasestr(ocstr,"pid ");
      if (pidp) {
        if (pidp==ocstr || (pidp>ocstr && (strchr(" .,;",*(pidp-1))))){
          for(pidp+=4;*pidp && *pidp!=' ';pidp++) {
            if (strchr("0123456789",*pidp)) pid+=*pidp;
          } 
        }
      }
    }
    if (pid.empty()) pid="0";
    einf.push_back(instyp(My->DBS,"pid",&pid));
    einf.push_back(instyp(My->DBS,"Erfolg",(int)!obgescheitert));
    einf.push_back(instyp(My->DBS,"docname",&original));
    Log(string("original (docname): ")+blau+original+schwarz,obverb,oblog);
    einf.push_back(instyp(My->DBS,"transe",&jetzt));
    if (!telnr.empty()) einf.push_back(instyp(My->DBS,"rcfax",&telnr));

    einf.push_back(instyp(My->DBS,"fsize",entryp->st_size>4294967295?0:entryp->st_size)); // int(10)
    rins.insert(pmp->touta,einf, 1,0,ZDB?ZDB:!runde); 
    if (runde==1) zs.Abfrage("SET NAMES 'utf8'");
    if (!rins.fnr) break;
    if (runde==1) {
      Log(string(Tx[T_Fehler_af])+drot+ltoan(rins.fnr)+schwarz+Tx[T_bei]+tuerkis+rins.sql+schwarz+": "+blau+rins.fehler+schwarz,1,oblog);
      exit(0);
    }
  } // for(int runde=0;runde<2;runde++) 
  if (!rins.fnr) { 
    RS rsloe(My,string("DELETE FROM `")+pmp->spooltab+"` WHERE id = \""+id+"\"");
    *geloeschtp=1;
  } // if (!rins.fnr) 
} // archiviere

// wird aufgerufen in: loeschefax, untersuchespool
int fsfcl::loeschecapi(int obverb, int oblog)
{
  Log(violetts+Tx[T_loeschecapi]+schwarz,obverb,oblog);
  string stamm,exten;
  getstammext(&capisd,&stamm,&exten);
  int zdng=0; // Zahl der nicht Geloeschten
  if (!stamm.empty()) {
    for(uchar ru=0;ru<2;ru++) {
      string zuloe=cspf+vtz+stamm+(ru?".txt":".sff");
      zdng+=tuloeschen(zuloe,obverb,oblog);
    }
  } else {
    zdng=1;
  }
  return zdng;
} // void fsfcl::loeschecapi(int obverb, int oblog)

// Rueckgabe: Zahl der nicht geloeschten Eintraege
// wird aufgerufen in: loeschefax, untersuchespool
int fsfcl::loeschehyla(paramcl *pmp,int obverb, int oblog)
{
  Log(violetts+Tx[T_loeschehyla]+schwarz,obverb,oblog);
  if (hylanr!="0" && !hylanr.empty()) {
    svec rmerg;
    uchar vmax=5;
    svec findrueck;
    // wenn Datei nicht mehr in sendq, sondern in doneq, sei es gelungen oder gescheitert, dann ist loeschen sinnlos
    systemrueck(("find '")+pmp->hsendqvz+"' -name q"+hylanr,obverb,oblog,&findrueck);
    if (!findrueck.size()) {
      return 0;
    }
    for(uchar iru=0;iru<vmax;iru++) {
      if (iru) {
        systemrueck(string("systemctl restart ")+pmp->sfaxgetty->sname+" "+pmp->shfaxd->sname+" "+pmp->sfaxq->sname,obverb-1,oblog);
      }
      systemrueck(string("faxrm ")+hylanr+" 2>&1",obverb,oblog,&rmerg);
      if (rmerg.size()) {
        if (rmerg[0].find(" removed")!=string::npos || rmerg[0].find("job does not exist")!=string::npos) {
          return 0;
        }
        Log(rots+Tx[T_Fehlermeldung_beim_Loeschversuch_eines_Hyla_Faxes_mit_faxrm]+hylanr+"`:\n    "+schwarz+rmerg[0],1,1);
      }
    }
  }
  return 1;
} // int fsfcl::loeschehyla(int obverb, int oblog)

paramcl::paramcl(int argc, char** argv)
{
  for(int i=1;i<argc;i++)
    if (argv[i][0])
      argcmv.push_back(argcl(i,argv)); 
  ich=argv[0];
  tstart=clock();
  cklingelzahl="1";
  hklingelzahl="2"; // muss mindestens 2 sein, um die Nr. des anrufenden zu uebertragen
  konfdatname.clear();
#ifdef obhp
  sfaxq=new servc("hylafax-faxq","faxq");
  shfaxd=new servc("hylafax-hfaxd","hfaxd");
#else
  sfaxq=new servc("","faxq");
  shfaxd=new servc("","hfaxd");
#endif
  scapisuite=new servc("","capisuite");
} // paramcl::paramcl()

paramcl::~paramcl()
{
  if (My) delete My;
  if (sfaxq) delete sfaxq;
  if (shfaxd) delete shfaxd;
  if (sfaxgetty) delete sfaxgetty;
  if (scapisuite) delete scapisuite;
} // paramcl::~paramcl()

// wird aufgerufen in: bereinigewv
string paramcl::getzielvz(const string& qdatei)
{
  for(zielmustercl *zmakt=zmp;1;zmakt++){
    if (!regexec(&(zmakt->regex),qdatei.c_str(),0,NULL,0))
      return zmakt->ziel;
    if (zmakt->obmusterleer()) break;
  }
  return "";
} // getzielvz

// wird aufgerufen in: getcommandline
int paramcl::setzegcp(const string& name, string *wert)
{
  for(size_t ind=0;ind<gcs;ind++) {
    if (cgconfp[ind].name==name) {
      cgconfp[ind].wert=*wert;
      return 0;
    }
  }
  return 1;
} // int paramcl::setzegcp(const string& name, string *wert)

// passt einen Dateinamen gemaess der vorhandenen Dateien in allen moegenlichen Zielverzeichnissen so an
// dass er beim Umwandeln in PDF und Verschieben als erfolgreiches oder nicht erfolgreiches Fax sich nicht ueberschneidet
// wird aufgerufen in: DateienHerricht
string paramcl::neuerdateiname(const string& qpfad)
{
  string dateiname=qpfad, altdname, stamm, exten, extu;
  loeschealleaus(&dateiname,"'");
  loeschealleaus(&dateiname,"\"");
  for(unsigned runde=0;runde<2;runde++) {
    for(unsigned aru=0;1;aru++) {
      dateiname=base_name(dateiname);
      if (dateiname==altdname) break;
      altdname = dateiname;
      dateiname=zielname(dateiname,wvz,1,(string*)0,obverb,oblog);
      dateiname=zielname(dateiname,gvz,1,(string*)0,obverb,oblog);
      dateiname=zielname(dateiname,zmp,1,(string*)0,obverb,oblog);
    }
    getstammext(&dateiname,&stamm,&exten);
    if (!runde) extu=exten; 
    else break;
    string pdf=stamm+".pdf";
    if (pdf==dateiname) break;
    else dateiname=pdf;
  }
  return base_name(stamm+"."+extu);
} // neuerdateiname

// wird aufgerufen von DateienHerricht
// die frisch ins Warteverzeichnis verschobenen Dateien werden in spooltab eingetragen
void paramcl::WVZinDatenbank(vector<fxfcl> *fxvp)
{
  Log(violetts+Tx[T_WVZinDatenbank]+schwarz,obverb,oblog);
  RS rins(My); 
  RS zs(My);
  string spoolid="";
  for (unsigned nachrnr=0; nachrnr<fxvp->size(); ++nachrnr) {
    for(int runde=0;runde<2;runde++) {
      if (runde==0) { zs.Abfrage("SET NAMES 'utf8'");
      } else if (runde==1) zs.Abfrage("SET NAMES 'latin1'");
      rins.clear();
      vector<instyp> einf; // fuer alle Datenbankeinfuegungen
      string odatei = base_name(fxvp->at(nachrnr).spdf);
      einf.push_back(instyp(My->DBS,"original",&odatei));
      if (nachrnr<fxvp->size()) {
        string oudatei = base_name(fxvp->at(nachrnr).npdf);
        einf.push_back(instyp(My->DBS,"origvu",&oudatei));
      }
      einf.push_back(instyp(My->DBS,"prio",fxvp->at(nachrnr).prio));
      rins.insert(spooltab,einf, 1,0,ZDB?ZDB:!runde,&spoolid);
      if (runde==1) zs.Abfrage("SET NAMES 'utf8'");
      if (spoolid!="null") break;
      if (runde==1) {
        Log(string(Tx[T_Fehler_af])+drot+ltoan(rins.fnr)+schwarz+Tx[T_bei]+tuerkis+rins.sql+schwarz+": "+blau+rins.fehler+schwarz,1,oblog);
        exit(0);
      }
    }   // for(int runde=0;runde<2;runde++)
    Log(drots+string("  Spool-ID: ")+schwarz+spoolid,obverb,oblog);
  } // for (unsigned nachrnr=0; nachrnr<spdfp->size(); ++nachrnr) 
  for (uchar tr=0;tr<2;tr++) {
    char ***cerg;
    RS tellen(My,string("select max(length(gettel3(")+(tr?"origvu":"original")+",'"+anfaxstr+"','"+ancfaxstr+"','"+anhfaxstr+"')))"
        " from `" +spooltab+"`");
    while (cerg=tellen.HolZeile(),cerg?*cerg:0) {
      if (*(*cerg+0))
        My->tuerweitern(spooltab,"telnr",atol(*(*cerg+0)),!obverb);
      break;
    }
    RS tel(My,string("update `")+spooltab+"` "
        "set telnr=gettel3("+(tr?"origvu":"original")+",'"+anfaxstr+"','"+ancfaxstr+"','"+anhfaxstr+"') "
        "where isnull(telnr) or telnr=''",ZDB);
  }
} // WVZinDatenbank

// in lieskonfein, getcommandl0, getcommandline, rueckfragen
void paramcl::lgnzuw()
{
  if (langu=="d" || langu=="D" || langu=="deutsch" || langu=="Deutsch") {
    Txd.lgn=Txk.lgn=Tx.lgn=deutsch;
  } else if (langu=="e" || langu=="E" || langu=="english" || langu=="english" || langu=="English" || langu=="Englisch") {
    Txd.lgn=Txk.lgn=Tx.lgn=englisch;
  } else {
    Txd.lgn=Txk.lgn=Tx.lgn=deutsch;
  }
} // void paramcl::lgnzuw()


// wird aufgerufen in: main
void paramcl::logvorgaben()
{
#ifdef _WIN32
  logvz = "C:\\Dokumente und Einstellungen\\All Users\\Anwendungsdaten";
#elif linux
  logvz = "/var/log";
#endif
  logdname = "autofax.log";
  loggespfad = logvz+vtz+logdname;
  logdt=&loggespfad.front();
} // void paramcl::logvorgaben


// wird aufgerufen in: main
void paramcl::getcommandl0()
{
  Log(violetts+"getcommandl0()"+schwarz,obverb,oblog);
  uchar plusverb=0;
  //  for(int i=argc-1;i>0;i--) KLA if (argv[i][0]==0) argc--; KLZ // damit fuer das Compilermakro auch im bash-script argc stimmt
  for(unsigned iru=0;iru<3;iru++) {
    switch (iru) {
      case 0:
        opts.push_back(optioncl("lg","language", &Tx,T_sprachstr,psons,&langu));
        opts.push_back(optioncl("langu","sprache", &Tx,-1,psons,&langu));
        opts.push_back(optioncl("lang","lingue", &Tx,-1,psons,&langu));
        break;
      case 1:
        opts.push_back(optioncl("v","verbose", &Tx, T_Bildschirmausgabe_gespraechiger,&plusverb,1));
        loggespfad = logvz+vtz+logdname;
        opts.push_back(optioncl("l","log",&Tx, T_protokolliert_ausfuehrlich_in_Datei, &loggespfad, T_sonst_knapper, &oblog,1));
        opts.push_back(optioncl("lvz","logvz", &Tx, T_waehlt_als_Logverzeichnis_pfad_derzeit, pverz,&logvz));
        opts.push_back(optioncl("ld","logdname", &Tx, T_logdatei_string_im_Pfad, &logvz, T_wird_verwendet_anstatt, &logdname, psons));
        opts.push_back(optioncl("ldn","logdateineu", &Tx, T_logdatei_vorher_loeschen, &logdateineu, 1));
        break;
      case 2:
        opts.push_back(optioncl("kd","konfdat", &Tx, T_verwendet_Kofigurationsdatei_string_anstatt,pfile,&konfdatname));
        break;
    }
    for(;optslsz<opts.size();optslsz++) {
      for(size_t i=0;i<argcmv.size();i++) {
        string altlogdname(logdname);
        string altlogvz(logvz);
        if (opts[optslsz].pruefp(&argcmv,&i,&hilfe)) {
          if (iru==1) {
            if (plusverb) {obverb++;plusverb=0;}
            if (!obcapi) hylazuerst=1; else if (!obhyla) hylazuerst=0;
            if (altlogdname!=logdname || altlogvz!=logvz) {
              if (!logdname.empty()) {
                loggespfad = logvz+vtz+logdname;
                logdt = &loggespfad.front();
              }
            }
          }
          if (opts[optslsz].kurz!="v") break;
        }
      }
    }
    optslsz=opts.size();
    if (!iru) lgnzuw();
  } // for(unsigned iru=0;iru<3;iru++)
} // void paramcl::getcommandl0(int argc, char** argv)

// wird aufgerufen in: main
void paramcl::pruefhardware()
{
  Log(violetts+Tx[T_pruefhardware]+schwarz,obverb,oblog);
  svec rueck;
  cmd="lspci | grep -i 'isdn'";
  logdt=&loggespfad.front();
  systemrueck(cmd, obverb,oblog,&rueck);
  // <<"pruefhardware 1 vor  obcapi: "<<(int)obcapi<<endl;
  if (rueck.size()) {
    obcapi=obfcard=1;
  } else {
    Log(rots+Tx[T_Keine_ISDN_Karte_gefunden]+schwarz+Tx[T_mitCapi]+rot+Tx[T_auf]+schwarz+"0.",1,oblog);
    obcapi=obfcard=0;
  }
  // <<"pruefhardware 1 nach obcapi: "<<(int)obcapi<<endl;
  obhyla=obmodem=0;
  rueck.clear();
  cmd="find /sys/class/tty";
  systemrueck(cmd, obverb,oblog,&rueck);
  for(size_t i=0;i<rueck.size();i++) {
    struct stat entrydriv;
    if (!lstat(((rueck[i])+"/device/driver").c_str(),&entrydriv)) {
      string tty=base_name(rueck[i]);
      // ttyS0 erscheint auf Opensuse und Ubuntu konfiguriert, auch wenn kein Modem da ist
      if (tty!="ttyS0") {
        svec rue2;
        if (!systemrueck(("stty -F /dev/")+tty+" >/dev/null 2>&1",-1,oblog,&rue2)) {
          obhyla=obmodem=1;
          modems<<tty;
          Log(string("Modem: ")+blau+Tx[T_gefunden],obverb,oblog);
        }
      }
    }
  }
  if (!obmodem) Log(rots+Tx[T_Kein_Modem_gefunden]+schwarz,obverb,oblog);
  // wvdialconf oder schneller: setserial -a /dev/tty*, mit baud_base: <!=0>  als Kriterium
} // void paramcl::pruefhardware()

// wird aufgerufen in: main, pruefhyla
// koennte auch im Fall eines entfernten Modems oder hylafax-Programms benoetigt werden
// sucht das Hylaverzeichnis und setzt varsphylavz darauf, return 0, wenn nicht gefunden dann varsphylavz="", return 1
// wird aufgerufen in: pruefhyla, main
int paramcl::setzhylavz()
{
  Log(violetts+Tx[T_setzhylavz]+schwarz,obverb,oblog);
  // wird fruehestens in korrerfolgszeichen benoetigt
  // varsphylavz wird benoetigt in: korrerfolgszeichen, untersuchespool, hfaxsetup(pruefhyla), pruefhyla, hylaausgeb(untersuchespool,zeigweitere)
  // hsendqvz wird benoetigt in: loescheallewartende, loeschewaise, zeigweitere, inDBh(faxemitH)
  struct stat entryhyla;
  const char* testcmd="/bin/faxrcvd";
  uchar weiter=0; // nur damit die Zeilenrueckungen in vi richtig durchgefuehrt werden
  // 28.3.16: Die Datei /etc/init.d/hylafax+ taugt nicht fuer die Fallunterscheidung, da sie selbst eine Fallunterscheidung enthaelt
#ifdef obhp
  svec hrueck;
  systemrueck("grep /var /usr/lib/systemd/system/*fax*.service | head -n 1 | cut -d'=' -f2 | awk -F'/etc' '{print $1}'",obverb,oblog,&hrueck);
  if (hrueck.size()) {
    varsphylavz=hrueck[0];
  } else {
    varsphylavz="/var/spool/hylafax";
    weiter=1;
  }
#else
  // in der OpenSuse-Datei bekam das Verzeichnis den Namen "SPOOL",
  // in Ubuntu "HYLAFAX_HOME"; dort bekam die Variable "SPOOL" einen anderen Inhalt
  cppSchluess hylaconf[]={{"SPOOL"},{"HYLAFAX_HOME"}};
  size_t cs=sizeof hylaconf/sizeof*hylaconf;
  string hylacdat="/etc/init.d/hylafax";
  confdat hylac(hylacdat,hylaconf,cs,obverb);
  if (!hylaconf[1].wert.empty()) {
    //  if (cpplies(hylacdat,hylaconf,cs)) KLA
    varsphylavz=hylaconf[1].wert;
  } else if (!hylaconf[0].wert.empty()) {
    varsphylavz=hylaconf[0].wert;
  } else {
    varsphylavz="/var/spool/fax";
    weiter=1;
  }
#endif
  if (weiter) {
    //  if (lsys.getsys(obverb,oblog)==sus) varsphylavz="/var/spool/fax";
    //  else if (lsys.getsys(obverb,oblog)==deb) varsphylavz="/var/spool/hylafax";
    for(unsigned iru=0;iru<(sizeof moeglhvz/sizeof *moeglhvz)+1;iru++) {
      if (lstat((varsphylavz+testcmd).c_str(),&entryhyla)) 
        break;
      else if (iru==sizeof moeglhvz/sizeof *moeglhvz) {
        varsphylavz="";
        // obhyla=0 hier noch nicht, da setzhylavz auch einmal vor der Installation schon aufgerufen wird
        return 1; // kein Verzeichnis gefunden
      }
      varsphylavz=moeglhvz[iru];
    }
  }
  kuerzevtz(&varsphylavz);
  hsendqvz=varsphylavz+"/sendq";
  kuerzevtz(&hsendqvz);
  return 0;
} // int paramcl::setzhylavz()

// Musterfunktion, die von einer Funktion in gesonderter Datei vorgaben.cpp ueberschrieben werden kann
void paramcl::VorgbSpeziell() 
{
  Log(violetts+Tx[VorgbSpeziell_Basis]+schwarz,obverb,oblog);
  dbq="autofax";
  muser="user";
  citycode="8131";
  msn="999999";
  LocalIdentifier="MeiLocId";
  cFaxUeberschrift="Mei FaxUeberschrift";
  sqlz=sqlvz="2";
}


// wird aufgerufen in: main
// allgemeine Vorgaben, fuer Deutschland
void paramcl::VorgbAllg()
{
  static cppSchluess capiconf[]={{"spool_dir"},{"fax_user_dir"},{"send_tries"},{"send_delays"},{"outgoing_MSN"},
                     {"dial_prefix"},{"fax_stationID"},{"fax_headline"},{"fax_email_from"},{"outgoing_timeout"}};
  ccs=sizeof capiconf/sizeof*capiconf;
  capiconfp= capiconf;
  confdat cfaxconf(cfaxconfdat,capiconf,ccs,obverb);
  if (1) {
    //  if (cpplies(cfaxconfdat,capiconf,ccs,&rest)) KLA
    spoolcapivz=capiconfp[0].wert;
    cfaxuservz=capiconfp[1].wert;
  }  else {           
    spoolcapivz="/var/spool/capisuite";
    cfaxuservz=spoolcapivz+"/users";
  }
  cdonevz = mitvtz(spoolcapivz)+"done";
  cfailedvz = mitvtz(spoolcapivz)+"failed";

  hylazuerst=0;
  // hmodemstr="ACM";
  maxcapinr="3";
  maxhylanr="3";
  gleichziel=1;
  anfaxstr="an Fax";
  ancfaxstr="an cFax";
  anhfaxstr="an hFax";
  anstr=" an ";
  undstr="und";
  countrycode="49";
  LongDistancePrefix="0";
  InternationalPrefix="00";
   // um z.B. spezielle Vorgaben (s. VorgbSpeziell) vom Server abhaengig machen zu koennen
#ifdef _WIN32
  // char lpszUsername[255]; DWORD dUsername = sizeof(lpszUsername); GetUserName(lpszUsername, &dUsername);
  dcpt = sizeof(cpt);
  GetComputerName(cpt,&dcpt);
#elif linux
  // uid_t userId = getuid(); passwd* pw = getpwuid(userId); pw->pw_name<<endl;
  cptlen = MAXHOSTNAMELEN;
  gethostname(cpt, cptlen);
#endif
  cronminut="2";

  cuser="";
  for(size_t i=cfaxconf.zn.size();i>0;) {
    int erg;
    char buf[250];
    if ((erg=sscanf(cfaxconf.zn[--i].c_str(),"[%[^]]]",buf))>0) 
      if (strcasecmp(buf,"global")) {
        cuser=buf; // nehme den letzten besten user
        break;
      }
  }
  if (cuser.empty()) hylazuerst=1;
  pruefcvz();
} // void paramcl::VorgbAllg()

// wird aufgerufen in: konfcapi, VorgbAllg
void paramcl::pruefcvz()
{
  kuerzevtz(&cfaxuservz);
  cfaxusersqvz=cfaxuservz+vtz+cuser+"/sendq"; //  "/var/spool/capisuite/users/<user>/sendq";
  cfaxuserrcvz=cfaxuservz+vtz+cuser+"/received";
  systemrueck(string("sudo mkdir -p ")+cfaxusersqvz,obverb,oblog);
  systemrueck(string("sudo mkdir -p ")+cfaxuserrcvz,obverb,oblog);
} // paramcl::pruefcvz

// wird aufgerufen in: main
void paramcl::lieskonfein()
{
  Log(violetts+Tx[T_lieskonfein]+schwarz,obverb,oblog);
  static cppSchluess gconf[]={{"langu"},{"obcapi"},{"obhyla"},{"hylazuerst"},{"maxcapinr"},{"maxhylanr"},{"cuser"},
    {"countrycode"},{"citycode"},{"msn"},{"LongDistancePrefix"},{"InternationalPrefix"},{"LocalIdentifier"},
    {"cFaxUeberschrift"},{"cklingelzahl"},{"hmodem"},{"hklingelzahl"},
    {"gleichziel"},{"zufaxenvz"},{"wartevz"},{"nichtgefaxtvz"},{"empfvz"},{"cronminut"},{"anfaxstr"},{"ancfaxstr"},{"anhfaxstr"},
    {"anstr"},{"undstr"},{"host"},{"muser"},{"mpwd"},{"datenbank"},{"logvz"},{"logdname"},{"sqlz"},{"musterzahl"}};
  cgconfp=gconf;
  gcs=sizeof gconf/sizeof*gconf;
  if (konfdatname.empty()) konfdatname = aktprogverz()+".conf";
  confdat afconf(konfdatname,gconf,gcs,obverb); // hier werden die Daten aus der Datei eingelesen
  if (1) {
    //  if (cpplies(konfdatname,gconf,gcs)) KLA
    // sodann werden die Daten aus gconf den einzelenen Klassenmitgliedsvariablen zugewiesen 
    rzf=0;
    unsigned lfd=0;
    if (langu.empty()) {
      if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&langu); else rzf=1; // Sprache aus der Commandline geht vor Konfiguration
      lgnzuw();
    } 
    lfd++;
  // <<"lieskonfein vor  obcapi: "<<(int)obcapi<<endl;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&obcapi); else rzf=1; lfd++;
  // <<"lieskonfein nach obcapi: "<<(int)obcapi<<endl;
    konfobcapi=obcapi;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&obhyla); else rzf=1; lfd++;
    konfobhyla=obhyla;
    if (obcapi) if (obhyla) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&hylazuerst); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&maxcapinr); else rzf=1;} lfd++;
    if (obcapi) if (obhyla) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&maxhylanr); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&cuser); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&countrycode); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&citycode); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&msn); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&LongDistancePrefix); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&InternationalPrefix); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&LocalIdentifier); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&cFaxUeberschrift); else rzf=1;} lfd++;
    if (obcapi) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&cklingelzahl); else rzf=1;} lfd++;
    if (obhyla) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&hmodem); else rzf=1;} lfd++;
    if (obhyla) {if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&hklingelzahl); else rzf=1;} lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&gleichziel); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&zufaxenvz); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&wvz); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&gvz); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&empfvz); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&cronminut); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&anfaxstr); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&ancfaxstr); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&anhfaxstr); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&anstr); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&undstr); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&host); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&muser); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) mpwd=XOR(string(gconf[lfd].wert),pk); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&dbq); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&logvz); else rzf=1; lfd++;
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&logdname); else rzf=1; lfd++;
    loggespfad = logvz+vtz+logdname;
    logdt = &loggespfad.front();
    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&sqlz); else rzf=1; lfd++;
    sqlzn=atol(sqlz.c_str());
    // Vorgaben uebernehmen
    if (sqlzn) {
      sqlconfp= new cppSchluess[sqlzn];
      for(size_t i=0;i<sqlzn;i++) {
        sqlconfp[i].name=string("SQL_")+ltoan(i+1);
        if (i<sqlvzn)
          sqlconfp[i].wert=sqlconfvp[i].wert;
      }
    }
    // jetzt erst stehen die Variablen fuer alle SQL-Befehle zur Verfuegung
    afconf.auswert(sqlconfp,sqlzn,obverb);

    if (!gconf[lfd].wert.empty()) gconf[lfd].hole(&zmz); else rzf=1; lfd++;
    if (!zmvzn || !zmvp) {
     zmvp= new zielmustercl{"","/var/autofax/ziel"};
     zmvzn=1;
    }
    zmvz=ltoan(zmvzn); // aus VorgbSpeziell

    uchar zmda=0; // 1= Zielmuster in der Konfigurationsdatei schon festgelegt
    for(int iru=0;;iru++) {
      zmzn=atol(zmz.c_str());
      if (!iru) if (zmzn>0) zmda=1;
      if (zmzn) break;
    // Vorgaben uebernehmen
      zmz=zmvz;
      zmzukonf=1;
    }
    
    zmconfp = new cppSchluess[zmzn+zmzn];
    zmp=new zielmustercl[zmzn];
    // Vorgaben uebernehmen
    for(size_t i=0;i<(zmzn<zmvzn?zmzn:zmvzn);i++) {
     zmp[i].setzemuster(zmvp[i].holmuster());
     zmp[i].ziel=zmvp[i].ziel;
    }
    for (size_t i=0;i<zmzn;i++) {
     zmconfp[2*i].name=string("ZMMuster_")+ltoan(i+1);
     zmconfp[2*i].setze(&zmp[i].holmuster());
     zmconfp[2*i+1].name=string("ZMZiel_")+ltoan(i+1);
     zmconfp[2*i+1].setze(&zmp[i].ziel);
    }
    if (zmda) {
    // jetzt stehen die Variablen fuer die Zielverzeichnisse zur Verfuegung
      afconf.auswert(zmconfp,zmzn+zmzn,obverb);
      for(size_t i=0;i<zmzn;i++) {
        if (i==zmzn-1) zmconfp[2*i].wert=""; // das letzte Muster muss "" sein, damit jedes Fax beruecksichtigt wird
        zmp[i].setzemuster(zmconfp[2*i].wert);
        zmp[i].ziel=zmconfp[2*i+1].wert;
      }
    }
  } else {
    rzf=1;
  } // (1) else
} // void paramcl::lieskonfein()

// wird aufgerufen in: main
int paramcl::getcommandline()
{
  Log(violetts+"getcommandline()"+schwarz,obverb,oblog);
  //  uchar plusverb=0;
  opts.push_back(optioncl("zvz","zufaxenvz", &Tx, T_faxt_die_Dateien_aus_pfad_anstatt,pverz,&zufaxenvz));
  opts.push_back(optioncl("wvz","wartevz", &Tx, T_Dateien_warten_in_pfad_anstatt,pverz,&wvz));
  opts.push_back(optioncl("evz","empfvz", &Tx, T_Empfangsverzeichnis_fuer_Faxempfang,pverz,&empfvz));
  opts.push_back(optioncl("cm","cronminut", &Tx, T_Alle_wieviel_Minuten_soll_Autofax_aufgerufen_werden_0_ist_gar_nicht, pzahl, &cronminut));
  // <<"getcommandline 1 vor  obcapi: "<<(int)obcapi<<endl;
  opts.push_back(optioncl("kc","keincapi", &Tx, T_Capisuite_nicht_verwenden,&obcapi,0));
  // <<"getcommandline 1 nach obcapi: "<<(int)obcapi<<endl;
  opts.push_back(optioncl("kh","keinhyla", &Tx, T_hylafax_nicht_verwenden,&obhyla,0));
  opts.push_back(optioncl("cz","capizuerst", &Tx, T_versuche_faxe_zuerst_ueber_Capisuite_wegzuschicken,&hylazuerst,0));
  opts.push_back(optioncl("hz","hylazuerst", &Tx, T_versuche_faxe_zuerst_ueber_hylafax_wegzuschicken,&hylazuerst,1));
  //  opts.push_back(optioncl("hms","hylamodemstring",&Tx, T_sucht_nach_dev_tty_string_als_Modem_mit_string_anstatt,&hmodemstr,psons));
  opts.push_back(optioncl("mod","modem",&Tx, T_Fuer_Hylafax_verwendetes_Modem,psons,&hmodem));
  opts.push_back(optioncl("mc","maxcapinr",&Tx, T_nach_zahl_Versuchen_Capisuite_wird_Hylafax_versucht,pzahl,&maxcapinr));
  opts.push_back(optioncl("mh","maxhylanr",&Tx, T_nach_zahl_Versuchen_Hylafax_wird_Capisuite_verwendet,pzahl,&maxhylanr));
  opts.push_back(optioncl("ckzl","capiklingelzahl",&Tx, T_Zahl_der_Klingeltoene_bis_Capisuite_den_Anruf_annimmt_anstatt,pzahl,&cklingelzahl));
  opts.push_back(optioncl("cuser","cuser",&Tx, T_verwendet_fuer_Capisuite_den_Linux_Benutzer_string_anstatt,psons,&cuser));
  opts.push_back(optioncl("hkzl","hylaklingelzahl",&Tx, T_Zahl_der_Klingeltoene_bis_Hylafax_den_Anruf_annimmt_anstatt,pzahl,&hklingelzahl));
  opts.push_back(optioncl("gz","gleichziel", &Tx, T_FAxe_werden_auch_ohne_Faxerfolg_ins_Zielverzeichnis_kopiert,&gleichziel,1));
  opts.push_back(optioncl("afs","anfaxstring",&Tx, T_faxnr_wird_hinter_string_erwartet_statt_hinter,psons,&anfaxstr));
  opts.push_back(optioncl("acfs","ancfaxstring",&Tx, T_faxnr_fuer_primaer_Capisuite_wird_hinter_string_erwartet_statt_hinter,psons,&ancfaxstr));
  opts.push_back(optioncl("ahfs","anhfaxstring",&Tx, T_faxnr_fuer_primaer_hylafax_wird_hinter_string_erwartet_statt_hinter,psons,&anhfaxstr));
  opts.push_back(optioncl("as","anstring",&Tx, T_Adressatenname_wird_hinter_string_erwartet_statt_hinter,psons,&anstr));
  opts.push_back(optioncl("us","undstring",&Tx, T_Trennstring_string_fuer_mehrere_Adressaten_Telefonnummern_statt,psons,&undstr));
  opts.push_back(optioncl("host","host",&Tx, T_verwendet_die_Datenbank_auf_Host_string_anstatt_auf,psons,&host));
  opts.push_back(optioncl("muser","muser",&Tx, T_verwendet_fuer_MySQL_MariaDB_den_Benutzer_string_anstatt,psons,&muser));
  opts.push_back(optioncl("mpwd","mpwd",&Tx, T_verwendet_fuer_MySQL_MariaDB_das_Passwort_string_anstatt,psons,&mpwd));
  opts.push_back(optioncl("db","db",&Tx, T_verwendet_die_Datenbank_string_anstatt,psons,&dbq));
  //  opts.push_back(optioncl("l","log", &Tx, T_protokolliert_ausfuehrlich_in_Datei+drot+loggespfad+schwarz+Tx[T_sonst_knapper],&oblog,1));
  //  opts.push_back(optioncl("lvz","logvz",&Tx, T_waehlt_als_Logverzeichnis_pfad_anstatt,&logvz,pverz));
  //  opts.push_back(optioncl("ld","logdname",&Tx, T_logdatei_string_im_Pfad+drot+logvz+schwarz+Tx[T_wird_verwendet_anstatt],&logdname,psons));
  //  opts.push_back(optioncl("ldn","logdateineu", &Tx, T_logdatei_vorher_loeschen,&logdateineu,1));
  //  opts.push_back(optioncl("v","verbose", &Tx, T_Bildschirmausgabe_gespraechiger,&plusverb,1));
  opts.push_back(optioncl("sqlv","sql-verbose", &Tx, T_Bildschirmausgabe_mit_SQL_Befehlen,&ZDB,255));
  opts.push_back(optioncl("rf","rueckfragen", &Tx, T_alle_Parameter_werden_abgefragt_darunter_einige_hier_nicht_gezeigte,&rzf,1));
  opts.push_back(optioncl("loef","loeschefax", &Tx, T_ein_Fax_nach_Rueckfrage_loeschen,&loef,1));
  opts.push_back(optioncl("loew","loeschewaise", &Tx, T_Eintraege_aus, &spooltab, 
        T_loeschen_zu_denen_kein_Datei_im_Wartevz_und_kein_Capi_oder_Hylafax_nachweisbar_ist, &loew,1));
  opts.push_back(optioncl("loea","loescheallew", &Tx, T_alle_wartenden_Faxe_und_zugehoerige_Eintraege_aus, &spooltab, T_loeschen, &loea,1));
  opts.push_back(optioncl("loee","loescheempf", &Tx, T_empfangene_Dateien_loeschen_die_nicht_verarbeitet_werden_koennen,&loee,1));
  opts.push_back(optioncl("kez","korrerfolgszeichen", &Tx, T_in_der_Datenbanktabelle, &touta, T_wird_das_Erfolgszeichen_korrigiert, &kez,1));
  opts.push_back(optioncl("bwv","bereinigewv", &Tx, T_Dateien_aus_Warteverzeichnis_gegen, &touta, T_pruefen_und_verschieben, &bwv,1));
  opts.push_back(optioncl("lista","listarchiv", &Tx, T_listet_Datensaetze_aus, &touta, T_mit_Erfolgskennzeichen_auf, &lista,1));
  opts.push_back(optioncl("listf","listfailed", &Tx, T_listet_Datensaetze_aus, &touta, T_ohne_Erfolgskennzeichen_auf, &listf,1));
  opts.push_back(optioncl("listi","listinca", &Tx, T_listet_Datensaetze_aus, &tinca, T__auf, &listi,1));
  opts.push_back(optioncl("n","dszahl", &Tx, T_Zahl_der_aufzulistenden_Datensaetze_ist_zahl_statt,pzahl, &dszahl));
  opts.push_back(optioncl("h","hilfe", &Tx, T_Zeigt_diesen_Bildschirm_an, &hilfe,1));
  opts.push_back(optioncl("?","help", &Tx, -1, &hilfe,1));

  string altlogdname(logdname);
  string altlogvz(logvz);
  string altckzl(cklingelzahl);
  string altcuser(cuser);
  string althkzl(hklingelzahl);
  for(;optslsz<opts.size();optslsz++) {
    for(size_t i=0;i<argcmv.size();i++) {
      if (opts[optslsz].pruefp(&argcmv,&i,&hilfe)) {
        break;
      }
    }
  }
  for(size_t i=0;i<argcmv.size();i++) {
    if (!argcmv[i].agef) {
      Log(rots+"Parameter: "+violett+argcmv[i].argcs+rot+Tx[T_nicht_erkannt]+schwarz,1,1);
      hilfe=1;
    }
  }
  if (!obcapi) hylazuerst=1; else if (!obhyla) hylazuerst=0;
  if (altckzl!=cklingelzahl || rzf) {
    setzegcp("cklingelzahl",&cklingelzahl); // zum Schreiben in die /usr/local/sbin/autofax.conf in autokonfschreib
    capizukonf=1;
  }
  if (altcuser!=cuser || rzf) {
    setzegcp("cuser",&cuser);
    capizukonf=1;
  }
  if (altlogdname!=logdname || altlogvz!=logvz) {
    if (!logdname.empty()) {
      loggespfad = logvz+vtz+logdname;
      logdt = &loggespfad.front();
    }
  }
  if (althkzl!=hklingelzahl || rzf) {
    setzegcp("hklingelzahl",&hklingelzahl);
    hylazukonf=1;
  }

  lgnzuw();
  Log(string(Tx[T_Fertig_mit_Parsen_der_Befehlszeile]),obverb>1,oblog);
  if (hilfe){
    cout<<blau<<Tx[T_Gebrauch]<<drot<<ich<<" [-<opt>|--<longopt> [<content>]] ..."<<schwarz<<endl; 
    cout<<blau<<Tx[T_Faxt_Dateien_aus_Verzeichnis_pfad_die]<<anfaxstr<<
      Tx[T_faxnr_enthalten_und_durch_soffice_in_pdf_konvertierbar_sind_und_traegt_sie]
      <<drot<<dbq<<blau<<Tx[T_Tabellen]<<drot<<touta<<blau<<"`,`"<<drot<<spooltab<<blau<<Tx[T_aein]<<endl;
    for(size_t j=0;j<opts.size();j++) {
      opts[j].hilfezeile(Tx.lgn);
    }
    return 0;
  }
  return 1;
} // int paramcl::getcommandline(int argc, char** argv)

// wird aufgerufen in: main
void paramcl::rueckfragen()
{
  Log(violetts+Tx[T_rueckfragen]+schwarz,obverb,oblog);
  //  static Schluessel gconf[]=KLA KLA"hylazuerst"KLZ,KLA"maxcapinr"KLZ,KLA"maxhylanr"KLZ,
  //                                KLA"gleichziel"KLZ,KLA"zufaxenvz"KLZ,KLA"wartevz"KLZ KLZ;
  char erg;
  if (rzf) {
    int lfd=-1;
    char *locale = setlocale(LC_CTYPE,"");
    if (locale) if (strchr("defi",locale[0])) langu=locale[0];
    vector<string> sprachen={"e","d","f","i"};
    if (cgconfp[++lfd].wert.empty() || rzf) {
      langu=holstrings("Language/Sprache/Lingue/Lingua",&sprachen, &langu);
      lgnzuw();
      cgconfp[lfd].setze(&langu);
    }
  // <<"rueckfragen 1 vor  obcapi: "<<(int)obcapi<<endl;
    if (cgconfp[++lfd].wert.empty() || rzf) {
      if (obfcard) {
        erg=holbuchst(Tx[T_Soll_die_Capisuite_verwendet_werden],string(Tx[T_j_af])+"n",0,"jJyYoOsSnN",obcapi?Tx[T_j_af]:"n");
        obcapi= strchr("jyJYoOsS",(int)erg)?1:0;
      } else {
        obcapi=0;
      }
      cgconfp[lfd].setze(&obcapi);
    }
  // <<"rueckfragen 1 nach obcapi: "<<(int)obcapi<<endl;
    if (cgconfp[++lfd].wert.empty() || rzf) {
      if (obmodem) {
        erg=holbuchst(Tx[T_Soll_Hylafax_verwendet_werden],string(Tx[T_j_af])+"n",0,"jJyYoOsSnN",obhyla?Tx[T_j_af]:"n");
        obhyla= strchr("jyJYoOsS",(int)erg)?1:0;
      } else {
        obhyla=0;
      }
      cgconfp[lfd].setze(&obhyla);
    }
    if (obcapi) {
      if (obhyla) {
        if (cgconfp[++lfd].wert.empty() || rzf) {
          const char *ferkl[]={"hylafax","capisuite"};
          erg=holbuchst(Tx[T_Soll_vorrangig_hylafax_gewaehlt_werden],string(Tx[T_j_af])+"n",ferkl,"jJyYoOsSnN",hylazuerst?Tx[T_j_af]:"n");
          hylazuerst= strchr("jyJYoOsS",(int)erg)?1:0;
          cgconfp[lfd].setze(&hylazuerst);
        }
        if (cgconfp[++lfd].wert.empty() || rzf) {
          maxcapinr=holzahl(Tx[T_Zahl_der_Versuche_in_Capisuite_bis_hylafax_eingeschaltet_wird],maxcapinr.c_str());
          cgconfp[lfd].setze(&maxcapinr);
        }
        if (cgconfp[++lfd].wert.empty() || rzf) {
          maxhylanr=holzahl(Tx[T_Zahl_der_Versuche_in_hylafax_bis_Capisuite_eingeschaltet_wird],maxhylanr.c_str());
          cgconfp[lfd].setze(&maxhylanr);
        }
        // also: obcapi, aber nicht obhyla
      } else {
        lfd+=3;
        hylazuerst=0;
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        //        string bliste, tmpcuser;
        vector<string> benutzer;
        cmd="cat /etc/passwd | grep /home/ | cut -d':' -f 1";
        systemrueck(cmd,obverb,oblog,&benutzer);
        for(size_t i=0;i<benutzer.size();i++) {
          //          bliste+=benutzer[i];
          //          if (i<benutzer.size()-1) bliste+=",";
          if (!i) if (cuser.empty()) cuser=benutzer[i]; // Vorgabe
        }
        /*
           string Frage=string("Linux-Benutzer fuer Capisuite (")+bliste+"):";
           do KLA
           tmpcuser=holstring(Frage.c_str(),&cuser);
           KLZ while (benutzer.size() && bliste.find(tmpcuser)==string::npos && 
           tmpcuser.find(',')==string::npos); // nur vorhandene User akzeptieren
           cuser=tmpcuser;
         */
        cuser=holstrings(Tx[T_Linux_Benutzer_fuer_Capisuite],&benutzer,&cuser);
        cgconfp[lfd].setze(&cuser);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        countrycode=holstring(Tx[T_Hylafax_eigene_Landesvorwahl_ohne_plus_oder_00],&countrycode);
        cgconfp[lfd].setze(&countrycode);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        citycode=holstring(Tx[T_Hylafax_eigene_Ortsvorwahl_ohne_0],&citycode);
        cgconfp[lfd].setze(&citycode);
      }
    } else {
      lfd+=6; // nicht obcapi, aber obhyla kaeme hier
      hylazuerst=1;
    }

    if (obcapi || obhyla) {
      if (cgconfp[++lfd].wert.empty() || rzf) {
        msn=holstring(Tx[T_Hylafax_und_Capisuite_eigene_MSN_Faxnummer_ohne_Vorwahl],&msn);
        cgconfp[lfd].setze(&msn);
      }
    } else {
      lfd++; 
    }

    if (obcapi) {
      if (cgconfp[++lfd].wert.empty() || rzf) {
        LongDistancePrefix=holstring(Tx[T_Hylafax_Praefix_fuer_ausserorts_zB_0],&LongDistancePrefix);
        cgconfp[lfd].setze(&LongDistancePrefix);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        InternationalPrefix=holstring(Tx[T_Hylafax_Praefix_fuer_das_Ausland_zB_00],&InternationalPrefix);
        cgconfp[lfd].setze(&InternationalPrefix);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        LocalIdentifier=holstring(Tx[T_Hylafax_bis_10_Buchstabe_fuer_eigenen_Namen],&LocalIdentifier);
        cgconfp[lfd].setze(&LocalIdentifier);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        cFaxUeberschrift=holstring(Tx[T_Capisuite_bis_20_Buchstaben_fuer_eigenen_Namen],&cFaxUeberschrift);
        cgconfp[lfd].setze(&cFaxUeberschrift);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        cklingelzahl=holzahl(Tx[T_Zahl_der_Klingeltoene_bis_Capisuite_den_Anruf_annimmt],cklingelzahl.c_str());
        cgconfp[lfd].setze(&cklingelzahl);
      }
    } else {
      lfd+=5; // nicht obcapi, aber obhyla kaeme hier
    }

    if (obhyla) {
      if (cgconfp[++lfd].wert.empty() || rzf) {
        hmodem=holstrings(Tx[T_Fuer_Hylafax_verwendetes_Modem],&modems,hmodem.empty()?&modems[0]:&hmodem);
        cgconfp[lfd].setze(&hmodem);
      }
      if (cgconfp[++lfd].wert.empty() || rzf) {
        hklingelzahl=holzahl(Tx[T_Zahl_der_Klingeltoene_bis_Hylafax_den_Anruf_annimmt],hklingelzahl.c_str());
        cgconfp[lfd].setze(&hklingelzahl);
      }
    } else {
      lfd+=2;
      hylazuerst=0;
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      erg=holbuchst(Tx[T_Sollen_die_Dateien_unabhaengig_vom_Faxerfolg_im_Zielverzeichnis_gespeichert_werden],
          string(Tx[T_j_af])+"n",0,"jJyYoOsSnN",gleichziel?Tx[T_j_af]:"n");
      gleichziel= strchr("jyJYoOsS",(int)erg)?1:0;
      cgconfp[lfd].setze(&gleichziel);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      zufaxenvz=holverz(Tx[T_Verzeichnis_mit_zu_faxenden_Dateien],&zufaxenvz);
      cgconfp[lfd].setze(&zufaxenvz);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      wvz=holverz(Tx[T_Verzeichnis_mit_wartenden_Dateien],&wvz);
      cgconfp[lfd].setze(&wvz);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      gvz=holverz(Tx[T_Verzeichnis_mit_gescheiterten_Dateien],&gvz);
      cgconfp[lfd].setze(&gvz);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      empfvz=holverz(Tx[T_Verzeichnis_fuer_empfangene_Faxe],&empfvz);
      cgconfp[lfd].setze(&empfvz);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      cronminut=holzahl(Tx[T_Alle_wieviel_Minuten_soll_Autofax_aufgerufen_werden_0_ist_gar_nicht],&cronminut);
      cgconfp[lfd].setze(&cronminut);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      anfaxstr=holstring(Tx[T_Buchstabenfolge_vor_erster_Faxnummer],&anfaxstr);
      cgconfp[lfd].setze(&anfaxstr);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      ancfaxstr=holstring(Tx[T_Buchstabenfolge_vor_erster_Fax_Nummer_primaer_fuer_Capisuite],&ancfaxstr);
      cgconfp[lfd].setze(&ancfaxstr);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      anhfaxstr=holstring(Tx[T_Buchstabenfolge_vor_erster_Fax_Nummer_primaer_fuer_Hylafax],&anhfaxstr);
      cgconfp[lfd].setze(&anhfaxstr);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      anstr=holstring(Tx[T_Buchstabenfolge_vor_erstem_Adressaten],&anstr);
      cgconfp[lfd].setze(&anstr);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      undstr=holstring(Tx[T_Buchstabenfolge_vor_weiterem_Adressaten_sowie_weiterer_Faxnummer],&undstr);
      cgconfp[lfd].setze(&undstr);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      host=holstring(Tx[T_Host_fuer_MySQL_MariaDB_Datenbank],&host);
      cgconfp[lfd].setze(&host);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      string Frage=Tx[T_Benutzer_fuer_MySQL_MariaDB];
      muser=holstring(Frage.c_str(),&muser);
      cgconfp[lfd].setze(&muser);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      string Frage=string(Tx[T_Passwort_fuer_MySQL_MariaDB])+Tx[T_fuer_Benutzer]+drot+muser+schwarz+"'";
      do {
        mpwd=holstring(Frage.c_str(),&mpwd);
      }
      while (mpwd.empty());
      string pwdstr=XOR(mpwd,pk);
      cgconfp[lfd].setze(&pwdstr);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      dbq=holstring(string(Tx[T_Datenbankname_fuer_MySQL_MariaDB_auf])+host+"'",&dbq);
      cgconfp[lfd].setze(&dbq);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      logvz=holverz(Tx[T_Logverzeichnis],&logvz);
      cgconfp[lfd].setze(&logvz);
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      logdname=holstring(Tx[T_Logdateiname],&logdname);
      cgconfp[lfd].setze(&logdname);
    }
    loggespfad = string(logvz)+vtz+logdname;
    logdt = &loggespfad.front();
    if (cgconfp[++lfd].wert.empty() || rzf) {
      size_t nsqlzn=0;
      vector<cppSchluess*> sqlv; 
      size_t aktsp=0;
      for(size_t akt=0;;akt++) {
       const string& nix="";
       const string *vorgabe=
           akt<sqlzn?
           (sqlconfp[akt].wert.empty()?&sqlconfvp[akt].wert:&sqlconfp[akt].wert): // wird in auswert zurueckgesetzt
           akt<sqlvzn?
           &sqlconfvp[akt].wert:
           &nix;
       string zwi=holstring(string(Tx[T_SQL_Befehl_Nr])+ltoan(akt+1)+(vorgabe->empty()?
       Tx[T_faxnr_wird_ersetzt_mit_der_Faxnr_vollnr_faxnr_mit_deren_Zusammenziehlung]:
       Tx[T_Strich_ist_SQL_Befehl_loeschen_faxnr_wird_ersetzt_mit_der_Faxnr_vollnr_faxnr_mit_deren_Zusammenziehlung]),
           vorgabe);
       if (zwi=="-") zwi.clear();
       if (zwi.empty()) {
        if (akt>sqlzn && akt >sqlvzn) akt--;
       } else {
         // hier Sql-Dateien pruefen
         svec dben=holdbaussql(zwi);
         for(size_t i=0;i<dben.size();i++) {
          cout<<"i: "<<blau<<i<<rot<<": "<<dben[i]<<schwarz<<endl;
          cout<<"pruefDB: "<<pruefDB(dben[i])<<endl;
         }
         cppSchluess* neuS=new cppSchluess;
         neuS->name=string("SQL_")+ltoan(++aktsp);
         neuS->wert=zwi;
         sqlv.push_back(neuS);
         nsqlzn++;
       }
       if (akt>=sqlzn && akt >= sqlvzn) {
          erg=holbuchst(Tx[T_Wolle_Sie_noch_einen_SQL_Befehl_eingeben],string(Tx[T_j_af])+"n",0,"jJyYoOsSnN",Tx[T_j_af]);
          if (!strchr("jyJYoOsS",(int)erg)) break;
       }
      }
      string nsqlz=ltoan(nsqlzn);
      cgconfp[lfd].setze(&nsqlz);
      sqlzn=nsqlzn;
      // Vektor in Array umwandeln
      static cppSchluess *nsqlconfp = new cppSchluess[sqlv.size()];
      for(size_t sqli=0;sqli<sqlv.size();sqli++) {
       nsqlconfp[sqli]=*sqlv[sqli];
      }
      sqlconfp=nsqlconfp;
    }
    if (cgconfp[++lfd].wert.empty() || rzf) {
      string  nzmz=holzahl(Tx[T_Zahl_der_Verzeichnisse_fuer_erfolgreich_verschickte_Faxe],&zmz);
      size_t nzmzn;
      while (1) {
        nzmzn=atol(nzmz.c_str());
        if (nzmzn>0) break;
        nzmz="1";
      }
      cgconfp[lfd].setze(&nzmz);
      static cppSchluess *nzmconfp= new cppSchluess[nzmzn+nzmzn];
      for(size_t akt=0;akt<(nzmzn<zmzn?nzmzn:zmzn);akt++) {
       if (nzmconfp[2*akt].wert.empty())
        nzmconfp[2*akt].wert=zmp[akt].holmuster();
       if (nzmconfp[2*akt+1].wert.empty())
        nzmconfp[2*akt+1].wert=zmp[akt].ziel;
      }
      while (1) {
        int fehler=0;
        for(size_t akt=0;akt<nzmzn;akt++) {
          string zwi=holstring(string("Zielmuster Nr. ")+ltoan(akt+1),&nzmconfp[2*akt].wert);
          nzmconfp[2*akt].name=string("ZMMuster_")+ltoan(akt+1);
          if (akt==nzmzn-1) zwi.clear(); // das letzte Muster muss leer sein, damit jede Datei irgendwo hinkommt
          nzmconfp[2*akt].setze(&zwi);
          zwi=holstring(string("Zielverzeichnis Nr. ")+ltoan(akt+1),&nzmconfp[2*akt+1].wert);
          nzmconfp[2*akt+1].name=string("ZMZiel_")+ltoan(akt+1);
          nzmconfp[2*akt+1].setze(&zwi);
        }
        zmp=new zielmustercl[nzmzn];
        for(uint imu=0;imu<nzmzn;imu++) {
          zmp[imu].setzemuster(nzmconfp[2*imu].wert);
          fehler+=zmp[imu].kompilier();
          zmp[imu].ziel=nzmconfp[2*imu+1].wert;
        }
       if (!fehler) break;
      }
      zmconfp=nzmconfp;
      zmz=nzmz;
      zmzn=nzmzn;
    }
  } // if (rzf)
  // die Verzeichnisnamen standardisieren
  kuerzevtz(&spoolcapivz);
  kuerzevtz(&cdonevz);
  kuerzevtz(&cfailedvz);
  kuerzevtz(&logvz);
  kuerzevtz(&zufaxenvz);
  kuerzevtz(&wvz);
  kuerzevtz(&gvz);
  kuerzevtz(&empfvz);
  for(zielmustercl *zmakt=zmp;1;zmakt++){
    kuerzevtz(&zmakt->ziel);
    if (zmakt->obmusterleer()) break;
  }
} // void paramcl::rueckfragen()

// wird aufgerufen in: main
void paramcl::autofkonfschreib()
{
//  <<"obverb in autokonfschreib: "<<(int)obverb<<endl;
  Log(violetts+Tx[T_autokonfschreib]+schwarz+", "+Tx[T_zu_schreiben]+((rzf || capizukonf || hylazukonf || zmzukonf)?Tx[T_ja]:Tx[T_nein]),obverb,oblog);
  if (rzf || capizukonf || hylazukonf || zmzukonf) {
    Log(violetts+Tx[T_schreibe_Konfigurkation]+schwarz,obverb,oblog);
    for (size_t i=0;i<gcs;i++) {
     if (cgconfp[i].name=="obhyla") cgconfp[i].setze(&obhyla);
     else if (cgconfp[i].name=="obcapi") cgconfp[i].setze(&obcapi);
    }
    cppSchluess *schlp[3]={cgconfp,sqlconfp,zmconfp};
    size_t groe[sizeof schlp/sizeof *schlp]={gcs,sqlzn,zmzn+zmzn};
    //    for(size_t i=0;i<sqlzn;i++) KLA _out<<"i: "<<i<<sqlconfp[i].name<<endl; KLZ
    multicppschreib(konfdatname, schlp, groe, sizeof schlp/sizeof *schlp);
  }
  //    ::cppschreib(konfdatname, cgconfp, gcs);
} // void paramcl::autofkonfschreib()


// wird  aufgerufen in: pruefcapi
void paramcl::konfcapi()
{
  Log(violetts+Tx[T_konfcapi]+schwarz,obverb,oblog);
  // Zahl der Klingeltoene in capisuite einstellen
  if (obcapi) {
    cppSchluess cconf[]={{"incoming_script"}};
    size_t cs=sizeof cconf/sizeof*cconf;
    confdat ccapic(ccapiconfdat,cconf,cs,obverb);
    if (1) {
      //    if (cpplies(ccapiconfdat,cconf,cs)) KLA
      ifstream f(cconf[0].wert.c_str()); // /usr/lib64/capisuite/incoming.py
      if (f.is_open()) {
        string zeile;
        const char* suchstr="faxInfo=capisuite.connect_faxG3(call,stationID,headline,";
        while(getline(f,zeile)) {
          size_t nk=zeile.find(suchstr);
          if (nk!=string::npos) {
            nk+=strlen(suchstr);
            size_t klap=zeile.find(')',nk);
            if (klap!=string::npos) {
              string nkz=zeile.substr(nk,klap-nk);
              if (nkz!=cklingelzahl) {
                string neuzeile=zeile.substr(0,nk)+cklingelzahl+zeile.substr(nk+nkz.length());
                string neudatei=string(cconf[0].wert)+"_neu";
                f.close();
                ofstream fneu(neudatei.c_str());
                ifstream falt(cconf[0].wert.c_str());
                if (falt.is_open()) if (fneu.is_open()) {
                  while(getline(falt,zeile)) {
                    nk=zeile.find(suchstr);
                    if (nk!=string::npos) {
                      fneu<<neuzeile<<endl;
                    } else {
                      fneu<<zeile<<endl;
                    } 

                  } // while(getline(falt,zeile)) 
                  struct stat entryorig;
                  string origdatei=string(cconf[0].wert)+"_orig";
                  if (lstat(origdatei.c_str(),&entryorig)) {
                    rename(cconf[0].wert.c_str(),origdatei.c_str());
                  } else {
                    remove(cconf[0].wert.c_str());
                  }            
                  rename(neudatei.c_str(),cconf[0].wert.c_str()); 
                }  // if (falt.is_open()) if (fneu.is_open()) 
                break;
              } // if (nkz!=cklingelzahl) 
            } // if (klap!=string::npos) 
          } // if ((nkz=strstr(zeile,suchstr))) 
        } // while(getline(f,zeile)) 
      } // if (f.is_open()) 
    } // if (cpplies(ccapiconfdat,cconf,cs)) 
    //  static cppSchluess capiconf[]=KLA KLA"spool_dir"KLZ,KLA"fax_user_dir"KLZ,KLA"send_tries"KLZ,KLA"send_delays"KLZ,
    //         KLA"outgoing_MSN"KLZ,KLA"dial_prefix"KLZ,KLA"fax_stationID"KLZ,KLA"fax_headline"KLZ,KLA"fax_email_from"KLZ KLZ;
    // fax_stationID
    uchar capicffehlt=0;
    if (capiconfp[6].wert.find("000 0000")!=string::npos || capiconfp[6].wert.empty()) {
      //    if (cfax_stationID.find("000 0000")!=string::npos) 
      //    Log(string("Capisuite ist offenbar noch nicht konfiguriert(")+blau+"fax_stationID"+schwarz+" enthaelt '000 0000').\n"
      //        "Die Einstellung koennen spaeter in "+blau+cfaxconfdat+schwarz+" geaendert werden.",1,0);
      Log(string(Tx[T_Capisuite_ist_offenbar_noch_nicht_richtig_konfiguriert])+blau+"'fax_stationID'"+schwarz+Tx[T_ist_Apostroph]+blau+
          capiconfp[6].wert+schwarz+"')."+ Tx[T_Die_Einstellungen_koennen_spaeter_in]+blau+cfaxconfdat+schwarz+Tx[T_geaendert_werden],1,1);
      // fax_stationID
      capicffehlt=1;
    }
    if (rzf || capicffehlt) {
      while (capiconfp[6].wert.find("000 0000")!=string::npos || !istelnr(capiconfp[6].wert)) {
        capiconfp[6].wert=string("+")+countrycode+" "+citycode+" "+msn;
        capiconfp[6].wert=holstring(string("fax_stationID: ")+Tx[T_Faxnr_die_zum_Adressaten_gesandt_wird_bis_20_Zeichen_nur_plus_und_Ziffern],
            &capiconfp[6].wert);
      }
      vector<string> tok;
      aufSplit(&tok,&capiconfp[6].wert,' ');
      // outgoing_MSN
      if (capiconfp[4].wert.empty()) capiconfp[4].wert=tok[tok.size()-1].c_str();
      capiconfp[4].wert=msn;
      while (capiconfp[4].wert.empty() || !isnumeric(capiconfp[4].wert)) {
        capiconfp[4].wert=holstring(string("outgoing_MSN: ")+
            Tx[T_ausgehende_Multiple_Subscriber_Number_Faxnummer_ohne_Vorwahl],&capiconfp[4].wert);
      } 
      // dial_prefix
      capiconfp[5].wert=holstring(string("dial_prefix: ")+Tx[T_Amtsholung_ueblicherweise_kk_oder_0],&capiconfp[5].wert);
      // fax_headline
      capiconfp[7].wert=cFaxUeberschrift;
      capiconfp[7].wert=holstring(string("fax_headline: ")+Tx[T_Faxueberschrift],&capiconfp[7].wert);
      // fax_email_from
      capiconfp[8].wert=holstring(string("fax_email_from: ")+
          Tx[T_Adressat_empfangener_Faxe_die_ueber_Email_verteilt_werden],&capiconfp[8].wert);
      // send_tries
      capiconfp[2].wert="15";
      do {
        capiconfp[2].wert=holstring(string("send_tries: ")+Tx[T_Zahl_der_Sendeversuche],&capiconfp[2].wert);
      } while (capiconfp[2].wert.empty() || !isnumeric(capiconfp[2].wert));
      // send_delays
      capiconfp[3].wert="60,60,60,300,300,300,300,300,3600,3600,3600,3600,18000,36000";
      do {
        capiconfp[3].wert=holstring(string("send_delays: ")+Tx[T_kommagetrennte_Liste_mit_Sekundenabstaenden_zwischen_Sendeversuchen],
            &capiconfp[3].wert);
      } while (capiconfp[3].wert.empty() || !isnumeric(capiconfp[3].wert));
      // outgoing_timeout
      capiconfp[9].wert="60";
      capiconfp[9].wert=holstring(string("outgoing_timeout: ")+Tx[T_Geduld_bis_zum_Verbindungsaufbau_in_s],&capiconfp[9].wert);
      if (obverb>0) {
        for(unsigned snr=0;snr<ccs;snr++) {
          Log(string("snr: ")+blau+ltoan(snr)+schwarz+" "+capiconfp[snr].wert+", "+capiconfp[snr].wert,obverb,oblog);
        }
      }
    } // if (cfax_stationID.find("000 0000")!=string::npos) 
    string suchcuser=string("[")+cuser+"]";
    // es gibt zwei moegliche Gruende zum Neuschreiben der Datei: 1) Parameter diffierieren, 2) noch kein User angelegt
    uchar cuserda=0, paramdiff=0, neuschreiben=0;
    string zeile, neudatei;
    uchar geschrieben;
    size_t kommpos;
    // iru=0 => pruefen, ob Datei geaendert werden muss; iru=1 => aendern
    for(uchar iru=0;iru<2;iru++) {
      ofstream *fneu=0;
      ifstream f(cfaxconfdat.c_str()); // /etc/capisuite/fax.conf
      if (f.is_open()) {
        if (iru) {
          neudatei=cfaxconfdat+"_neu";
          fneu=new ofstream(neudatei.c_str());
          if (!fneu->is_open()) break;
        }
        while(f.is_open() && getline(f,zeile)) {
          kommpos=zeile.find('#');
          geschrieben=0;
          //          if (kommpos!=string::npos) zeile.erase(kommpos);
          //          if (!zeile.length()) continue;
          if (!cuserda) 
            if (zeile.find(suchcuser)!=string::npos) 
              cuserda=1;
          if (iru || !paramdiff) {
            size_t nkz=zeile.find('=');
            string lzeile,rzeile;
            if (nkz!=string::npos && nkz<kommpos) {
              lzeile=zeile.substr(0,nkz); 
              rzeile=zeile.substr(nkz+1);
              for(unsigned snr=0;snr<ccs;snr++) {
                if (lzeile.find(capiconfp[snr].name)!=string::npos) {
                  // _out<<"snr: "<<snr<<", lzeile: "<<tuerkis<<lzeile<<schwarz<<", rzeile: "<<blau<<rzeile<<schwarz<<endl;
                  string altwert=rzeile;
                  gtrim(&altwert);
                  // Anfuehrungszeichen entfernen
                  if (altwert[0]==altwert[altwert.length()-1] && strchr("\"'",altwert[0])) {
                    if (altwert.length()) altwert.erase(altwert.length()-1);
                    if (altwert.length()) altwert.erase(0,1);
                  }
                  if (snr==0 || snr==1) capiconfp[snr].wert=altwert; // spool_dir und fax_user_dir hier nicht konfigurierbar
                  Log(string("capiconfp[")+ltoan(snr)+"].name: "+tuerkis+capiconfp[snr].name+schwarz+Tx[T_komma_wert]+
                      (capiconfp[snr].wert==altwert?blau:rot)+capiconfp[snr].wert+schwarz+Tx[T_komma_Altwert]+
                      blau+altwert+schwarz,obverb+iru,oblog+iru);
                  if (capiconfp[snr].wert!=altwert) {
                    if (!iru) {
                      neuschreiben=1;
                      paramdiff=1;
                    } else {
                      *fneu<<capiconfp[snr].name<<" = \""<<capiconfp[snr].wert<<"\""<<endl;
                      geschrieben=1;
                    }
                  }
                  break; // wenn richtige Zeile gefunden, nicht mehr weitersuchen, unabhaengig vom Ergebnis
                } // if ((nkz=strstr(zeile,capiconfp[snr].name))) 
              } // if ((nkz=zeile.find(capiconfp[snr].wert))!=string::npos) 
            } // for(unsigned snr=0;snr<ccs;snr++) 
          } // if (!paramdiff) 
          if (iru)
            if (!geschrieben) 
              *fneu<<zeile<<endl;
        } // while( fgets(p=zeile,sizeof zeile,f) ) 
        if (!iru) {
          if (!cuserda) neuschreiben=1;
          if (!neuschreiben) break; // dann fertig
        } else { // if (iru)
          if (!cuserda)  {
            *fneu<<suchcuser<<endl;
            *fneu<<"fax_numbers=\""<<capiconfp[4].wert<<"\""<<endl;
            *fneu<<"fax_stationID=\""<<capiconfp[6].wert<<"\""<<endl;
            *fneu<<"fax_headline=\""<<capiconfp[7].wert<<"\""<<endl;
            *fneu<<"fax_email_from=\""<<capiconfp[8].wert<<"\""<<endl;
            *fneu<<"fax_action=\"MailAndSave\""<<endl;
          } // if (!cuserda)
          if (fneu) delete fneu;
          string origdatei=cfaxconfdat+"_orig";
          struct stat entryorig;
          if (lstat(origdatei.c_str(),&entryorig)) {
            rename(cfaxconfdat.c_str(),origdatei.c_str());
          } else {
            remove(cfaxconfdat.c_str());
          }            
          rename(neudatei.c_str(),cfaxconfdat.c_str());
        } // if (iru)
        cfaxuservz=capiconfp[1].wert;
      } // if (f.is_open())
    } // for(uchar iru=0;iru<2;iru++) 
    pruefcvz();

    // dieser Abschnitt war zuvor richtcapiher
    unsigned long nextnr=0;
    struct stat entrynextnr;
    string ndatei=cfaxusersqvz+"/fax-nextnr";
    if (!lstat(cfaxusersqvz.c_str(),&entrynextnr)) {
      ifstream nextstr(ndatei.c_str());
      if (nextstr.is_open()) {
        string zeile;
        if (getline(nextstr,zeile)) {
          nextnr=atol(zeile.c_str());
        }
      }
    }
    if (!nextnr) {
      cmd=string(" echo $(( `find ")+spoolcapivz+
        " -type f -name '*-fax-*.sff' | cut -d '-' -f3 | cut -d '.' -f1 | sort -rn | head -n1` + 1 )) > '"+ndatei+"'";
      systemrueck(cmd,obverb,oblog);
    }

  } // obcapi
  capizukonf=0;
} // void paramcl::konfcapi()

// wird aufgerufen in: main
void paramcl::verzeichnisse()
{
  Log(violetts+Tx[T_verzeichnisse],obverb,oblog);
  systemrueck(string("sudo mkdir -p ")+zufaxenvz,obverb,oblog);
  systemrueck(string("sudo mkdir -p ")+wvz,obverb,oblog);
  systemrueck(string("sudo mkdir -p ")+gvz,obverb,oblog);
  systemrueck(string("sudo mkdir -p ")+empfvz,obverb,oblog);
  for(zielmustercl *zmakt=zmp;1;zmakt++){
    systemrueck(string("sudo mkdir -p ")+zmakt->ziel,obverb,oblog);
    if (zmakt->obmusterleer()) break;
  }
  for(uint imu=0;imu<this->zmzn;imu++) {
    char *imus = ltoan(imu);
    Log(string(Tx[T_Muster])+imus+": '"+rot+this->zmp[imu].holmuster()+schwarz+"'",this->obverb>1,this->oblog);
    Log(string(Tx[T_Ziel])+imus+":   '"+rot+this->zmp[imu].ziel+schwarz+"'",this->obverb>1,this->oblog);
  }
} // paramcl:: verzeichnisse()

// wird aufgerufen in: main
void paramcl::pruefcron()
{
  Log(violetts+Tx[T_pruefcron],obverb,oblog);
  //  svec rueck;
  int cronda;
  int cronzuplanen = (cronminut!="0");

  for (uchar runde=0;runde<2;runde++) {
    cronda = !systemrueck("which crontab > /dev/null 2>&1",obverb-1,0);
    if (cronda) break;
    // systemrueck("which zypper 2>/dev/null && zypper -n in cron || 
    //              KLA which apt-get 2>/dev/null && apt-get --assume-yes install cron; KLZ",1,1);
    linst.doinst("cron",1,1);
    //  int obionice=!systemrueck("which ionice > /dev/null 2>&1",0,0);
  }
  if (cronda) {
    int nochkeincron = systemrueck("crontab -l > /dev/null 2>&1",obverb-1,0);
    const string tmpc="meincrontab";
    string cb0 = " /usr/bin/ionice -c2 -n7 /usr/bin/nice -n19 /usr/bin/ps -A | /usr/bin/grep -q autofax || "+meinpfad();
    string cbef  =string("*/")+cronminut+" * * * *"+cb0; // "-"-Zeichen nur als cron
    string cbeesc=string("\\*/")+cronminut+" \\* \\* \\* \\*"+cb0; // hier vorne \\- gestrichen
#ifdef uebersichtlich
    string befehl;
    if (!cronzuplanen) {
      if (nochkeincron) {
      } else {
        befehl=("bash -c 'grep \"autofax\" -q <(crontab -l)' && (crontab -l | sed '/autofax/d'>\"")+tmpc+"\"; crontab \""+tmpc+"\")";
      }
    } else {
      if (nochkeincron) {
        befehl=("rm -f ")+tmpc+";";
      } else {
        befehl = ("bash -c 'grep \"")+cbeesc+"\" -q <(crontab -l)' || (crontab -l | sed '/autofax/d'>\""+tmpc+"\";";
      }
      befehl+=("echo \"")+cbef+"\">>\""+tmpc+"\"; crontab \""+tmpc+"\"";
      if (!nochkeincron)
        befehl+=")";
    }
#else
    string befehl=cronzuplanen?
      (nochkeincron?("rm -f ")+tmpc+";":
       ("bash -c 'grep \"")+cbeesc+"\" -q <(crontab -l)' || (crontab -l | sed '/autofax/d'>\""+tmpc+"\"; ")+
      "echo \""+cbef+"\">>\""+tmpc+"\"; crontab \""+tmpc+"\""+(nochkeincron?"":")")
      :
      (nochkeincron?"":("bash -c 'grep \"autofax\" -q <(crontab -l)' && (crontab -l | sed '/autofax/d'>\"")+tmpc+"\"; crontab \""+tmpc+"\")")
      ;
#endif      
    systemrueck(befehl,obverb,oblog);
  }
//  systemrueck(string("mv -i '")+mpfad+"' /root/bin",1,0);
} // pruefcron

// wird aufgerufen in: main
void paramcl::pruefsamba()
{
  Log(violetts+Tx[T_pruefsamba],obverb,oblog);
  const char* const smbdatei="/etc/samba/smb.conf";
  confdat smb(smbdatei,obverb);
  smb.Abschn_auswert(obverb);
  vector<string*> vzn;
  const char* const VSambaName[4]={Tx[T_Zufaxen],Tx[T_Warteauffax],Tx[T_Nichtgefaxt],Tx[T_Faxempfang]};
  //={&zufaxenvz,&wvz,&gvz,&empfvz};
  vzn.push_back(&zufaxenvz);
  vzn.push_back(&wvz);
  vzn.push_back(&gvz);
  vzn.push_back(&empfvz);
  for(zielmustercl *zmakt=zmp;1;zmakt++){
    vzn.push_back(&zmakt->ziel);
    if (zmakt->obmusterleer()) break;
  }
  uchar gef[vzn.size()];
  for(unsigned k=0;k<vzn.size();k++) {
    gef[k]=0;
  }
  for(size_t i=0;i<smb.abschv.size();i++) {
   if (smb.abschv[i].aname!="global") {
    const string& pfad = smb.abschv[i].suche("path");
    if (!pfad.empty()) {
      for(unsigned k=0;k<vzn.size();k++) {
        if (!gef[k]) if (!vzn[k]->empty()) {
          if (!vzn[k]->find(pfad)) {
            gef[k]=1;
          }
        } // if (!gef[k]) if (!vzn[k]->empty()) 
      } // for(unsigned k=0;k<sizeof vzn/sizeof *vzn;k++) 
    } // if (!pfad.empty()) 
   } // if (smb.abschv.aname!="global") 
  } // for(size_t i=0;i<smb.abschv.size();i++) 
  uchar smbrestart=0;
  for(unsigned k=0;k<vzn.size();k++) {
    if (!gef[k]) {
      smbrestart=1;
      ofstream sapp(smbdatei,ifstream::app);
      Log(rots+Tx[T_Verzeichnis]+blau+*vzn[k]+rot+Tx[T_nicht_als_Sambafreigabe_gefunden_wird_ergaenzt]+schwarz,1,oblog);
      if (sapp.is_open()) {
       if (k<4) {
         sapp<<"["<<VSambaName[k]<<"]"<<endl;
         sapp<<"  comment = autofax "<<VSambaName[k]<<endl;
       } else {
         sapp<<"["<<Tx[T_Gefaxt]<<"_"<<(k-4)<<"]"<<endl;
         sapp<<"  comment = autofax "<<Tx[T_Gefaxt]<<"_"<<(k-4)<<endl;
       }
       sapp<<"  path = "<<*vzn[k]<<endl;
       sapp<<"  directory mask = 0660"<<endl;
       sapp<<"  browseable = Yes"<<endl;
       sapp<<"  vfs objects = recycle"<<endl;
       sapp<<"  recycle:versions = Yes"<<endl;
       sapp<<"  recycle:keeptree = Yes"<<endl;
       sapp<<"  recycle:repository = Papierkorb"<<endl;
      }
    }
  } // for(unsigned k=0;k<sizeof vzn/sizeof *vzn;k++) 
  if (smbrestart) {
    systemrueck(string("systemctl restart smb nmb"),obverb-1,oblog);
  } else {
    servc smbsvc("smb","");
    if (!smbsvc.obslaeuft(obverb,oblog)) {
      smbsvc.restart(obverb,oblog);
      smbsvc.enableggf(obverb,oblog);
    }
    servc nmbsvc("nmb","");
    if (!nmbsvc.obslaeuft(obverb,oblog)) {
      nmbsvc.restart(obverb,oblog);
      nmbsvc.enableggf(obverb,oblog);
    }
  }
} // pruefsamba

// wird aufgerufen in: main
int paramcl::initDB()
{
  Log(violetts+"initDB()"+schwarz,obverb,oblog);
  My = new DB(myDBS,host,muser,mpwd,dbq,0,0,0,obverb,oblog);
  if (My->fehnr) {
    Log(rots+Tx[Verbindung_zur_Datenbank_nicht_herstellbar]+schwarz+ltoan(My->fehnr)+rot+Tx[T_Breche_ab]+schwarz,1,1);
    return 1;
  }
  return 0;
} // initDB

int paramcl::pruefDB(const string& db)
{
  Log(violetts+"pruefDB("+db+")"+schwarz,obverb,oblog);
  My = new DB(myDBS,host,muser,mpwd,db,0,0,0,obverb,oblog);
  return (My->fehnr); 
}

// Parameter -kez
// wird aufgerufen in: main
void paramcl::korrerfolgszeichen()
{
  Log(violetts+Tx[T_korrerfolgszeichen]+schwarz,obverb,oblog);
  // geht wegen Loeschens der Protokolldateien nur (noch) fuer Gefundene, s.u.
  if (1) {
    for(uchar runde=1;runde<2;runde++) {
      set<string>::iterator fit; // Iterator dafuer
      vector<string> rueck;
      set<string> fdn; // Fax-Dateien
      size_t ruecki;
      switch (runde) {
        case 0: // capi
          cmd=string("find '")+cdonevz+"' -maxdepth 1 -type f -iname '*-fax-*.sff'"; //  -printf '%f\\n'"; // cfailedvz weniger wichtig
          systemrueck(cmd,obverb,oblog,&rueck);
          // sortieren
          for(ruecki=0;ruecki<rueck.size();ruecki++) {
            fdn.insert(rueck[ruecki]);
          } 
          break;
        default: // hyla
          //        fdn.clear();
          cmd=string("find ")+varsphylavz+" -name 'q*' -print0 | /usr/bin/xargs -0 grep -l ^state:7 ";
          rueck.clear();
          systemrueck(cmd,obverb,oblog,&rueck);
          for(ruecki=0;ruecki<rueck.size();ruecki++) {
            size_t pos;
            if ((pos=rueck[ruecki].rfind("/q"))!=string::npos) fdn.insert(rueck[ruecki].substr(pos+2));
          } 
      }
      string sql=string("select titel p0, tsid p1, submt p2, submid p3, oscht p4, subject p5, docname p6, id p7, fsize p8, pages p9, ")+
        "devname p10, retries p11, prio p12, rcfax p13, rcname p14, csid p15, sender p16, transs p17, transe p18, Pid p19, eind p20, Erfolg p21 "
        "from `"+touta+"` where submid "+(runde?"rlike '^[0-9]+$'":"like '%fax-%.sff'")+" order by submt";
      RS routa(My,sql);
      if (!routa.obfehl) {
        char ***cerg;
        while (cerg=routa.HolZeile(),cerg?*cerg:0) {
          if (*(*cerg+3)) if (*(*cerg+20)) if (*(*cerg+21)) {
            string farbe=rot;
            if (runde)
              fit=fdn.find(*(*cerg+3));
            else
              fit=fdn.find(cdonevz+vtz+cuser+"-"+*(*cerg+3));
            if ((fit!=fdn.end()) != (**(*cerg+21)=='0')) farbe=blau;
            else {
              stringstream ausg;
              if (fit!=fdn.end()) {
              ausg<<Tx[T_gefunden]<<": "<<(int)(fit!=fdn.end())<<" "<<
                    Tx[T_Falschals]<<(**(*cerg+21)=='0'?Tx[T_gescheitert]:Tx[T_gesandt])<<Tx[T_eingeordnet]<<farbe<<setw(14)<<
                    *(*cerg+3)<<schwarz<<" "<<*(*cerg+2)<<" "<<blau<<*(*cerg+6)<<schwarz;
              Log(ausg.str(),1,1);
              RS vs(My,string("UPDATE `")+touta+"` SET Erfolg= !Erfolg WHERE submid = '"+*(*cerg+3)+"'",255);
              }
            } // ((fit!=fdn.end()) != (**(*cerg+21)=='0')) else
          } // if (*(*cerg+3)) if (*(*cerg+20)) if (*(*cerg+21)) 
        } // while (cerg=routa.HolZeile(),cerg?*cerg:0) 
      } // if (!routa.obfehl) 
    } // for(uchar runde=1;runde<2;runde++) 
  } // if (0) 

} // korrerfolgszeichen

// Parameter -bwv
// wird aufgerufen in: main
void paramcl::bereinigewv()
{
  Log(violetts+Tx[T_bereinigewv]+schwarz,obverb,oblog);
  set<string> fdn; // Fax-Dateien
  set<string>::iterator fit; // Iterator dafuer
  //  cmd=string("find \"")+wvz+"\" -maxdepth 1 -type f -iname \"*.pdf\" -printf '%f\n'";
  cmd=string("find \"")+wvz+"\" -maxdepth 1 -type f -printf '%f\n'";
  vector<string> rueck;
  systemrueck(cmd,obverb,oblog,&rueck);
  for(size_t i=0;i<rueck.size();i++) {
    fdn.insert(rueck[i]);
  } 
  //  for(fit=fdn.begin();fit!=fdn.end();++fit) _out<<blau<<*fit<<schwarz<<endl;
  string vgl="Beispiel 997.doc"; // nicht existente Datei
  Log(string(Tx[T_DateienzahlimVz])+blau+ltoan(fdn.size())+schwarz,1,1);
  for(fit=fdn.begin();fit!=fdn.end();++fit) {
    if (*fit==vgl) {
      Log(blaus+*fit+schwarz,1,1);
    }
  }
  for(unsigned runde=0;runde<2;runde++) {
    string sql;
    switch (runde) {
      case 0: sql=string("select id p0, original p1, origvu p2 from `")+spooltab+"`"; break;
      case 1: sql=string("select eind p0, docname p1, Erfolg p2 from `")+touta+"`"; break;
    }
    RS rsp(My,sql,ZDB);
    char ***cerg;
    while (cerg=rsp.HolZeile(),cerg?*cerg:0) {
      for(unsigned cnr=1;cnr<3;cnr++) {
        if (cnr==1 || !runde) { // nur die erste Tabelle hat zwei Spalten mit Dateinamen
          if (*(*cerg+cnr)) {
            string dbdocname=*(*cerg+cnr);
            for (unsigned aru=0;aru<5;aru++) {
              string stamm,exten;
              getstammext(&dbdocname,&stamm,&exten);
              if (!aru || !exten.empty()) {
                if (aru) {
                  switch (aru) {
                    case 1: dbdocname=stamm+".doc"; break;
                    case 2: dbdocname=stamm+".xls"; break;
                    case 3: dbdocname=stamm+".jpg"; break;
                    case 4: dbdocname=stamm+".tif"; break;
                  }
                }
                if ((fit=fdn.find(dbdocname))!=fdn.end()) { // wenn Datenbankeintrag in der sortierten Mengen der Dateinamen enthalten
                  struct stat entryfit;
                  string quel=wvz+vtz+*fit;
                  if (!lstat(quel.c_str(),&entryfit)) { // duerfte immer erfuellt sein
                    if (entryfit.st_size) { // keine 0-Bytes-Dateien kopieren
                      if (runde) { // wenn nicht in der Spool-Tabelle
                        string zlvz;
                        //                        if (runde==1) zlvz=getzielvz(quel); else if (runde==2) zlvz=gvz;
                        if (runde==1) {
                          if (**(*cerg+2)=='1') zlvz=getzielvz(quel); else if (**(*cerg+2)=='0') zlvz=gvz;
                        }
                        unsigned vfehler=0;
                        if (!zlvz.empty()) {
                          string zdt=zlvz+vtz+*fit;
                          dorename(quel,zdt,&vfehler,1,1);
                          if (vfehler) {
                            Log(rots+Tx[T_Fehler_beim_Verschieben_Ausrufezeichen]+": "+ltoan(vfehler)+schwarz,1,1);
                            exit(2);
                          }
                          fdn.erase(fit);
                          break;
                        } // if (!zlvz.empty()) 
                      } else {// if (runde) 
                        fdn.erase(fit); // die in der Spool-Tabelle genannten Dateien stehen lassen
                      }
                      //                      Log(string(ltoan(entryfit.st_size))+" "+(runde==2?hblau:runde==1?blau:rot)+*fit+schwarz,1,1);
                      Log(string(ltoan(entryfit.st_size))+" "+(runde?(**(*cerg+2)=='1'?blau:hblau):rot)+*fit+schwarz,1,1);
                    } else {
                      Log(string(Tx[T_0Bytes])+violett+wvz+vtz+*fit+schwarz+"'",1,1);
                    } // if (!lstat(quel.c_str(),&entryfit)) else
                  } // if (!lstat(quel.c_str(),&entryfit)) 
                } // if ((fit=fdn.find(dbdocname))!=fdn.end()) 
                if (!runde) break;
              } // if (!aru || !exten.empty()) 
            } // for (unsigned aru=0;aru<3;aru++) 
          } // if (*(*cerg+cnr)) 
        } // if (cnr==1 || !runde) 
      } // for(unsigned cnr=1;cnr<3;cnr++) 
    } // while (cerg=rsp.HolZeile(),cerg?*cerg:0) 
  } // for(unsigned runde=0;runde<3;runde++) 
} // bereinigewv

// wird aufgerufen in: main
int paramcl::loeschefax(int obverb, int oblog)
{
  Log(blaus+Tx[T_loeschefax]+schwarz,obverb,oblog);
  vector<fsfcl> fsfv;
  size_t nr=0;
  string ergnr,erg;
  char buchst;
  char*** cerg;
  RS zul(My,string("select concat_ws(' ',left(concat(if(isnull(original),'NULL',original),space(50)),50),"
        "right(concat(space(15),if(isnull(capispooldatei),'NULL',capispooldatei)),15),")+
      "concat('Capidials:',right(concat(space(4),if(isnull(capidials),'NULL',capidials)),4)),"
      "concat('Hyla:',right(concat(space(5),if(isnull(hylanr),'NULL',hylanr)),5)), "
      "concat('Hyladials:',right(concat(space(4),if(isnull(hyladials),'NULL',hyladials)),4))) p0,"
      "id p1,"
      "if(isnull(capispooldatei),'NULL',capispooldatei) p2,"
      "if(isnull(capispoolpfad),'"+cfaxusersqvz+"',capispoolpfad) p3,"
      "hylanr p4 from `"+spooltab+"` order by id",ZDB);
  while (cerg=zul.HolZeile(),cerg?*cerg:0) {
    if (*(*cerg+0) && *(*cerg+1)) {
      Log(string("Fax ")+blau+ltoan(++nr)+schwarz+": "+*(*cerg+0),1,1);
      fsfv.push_back(fsfcl(*(*cerg+1),*(*cerg+2),*(*cerg+4),*(*cerg+3)));
    }
  }
  if (fsfv.size()) {
    ergnr=holzahl(Tx[T_Welches_Fax_soll_geloescht_werden]);
    if (ergnr=="0") return 0;
    buchst=holbuchst(string(Tx[T_Soll_das_Fax_geloescht_werden_0_ist_Abbruch])+violett+ergnr+schwarz+Tx[T_wirklich_geloescht_werden],
        string(Tx[T_j_af])+"n",0,"jJyYoOsSnN","n");
    if (strchr("jyJYoOsS",buchst)) {
      nr=atol(ergnr.c_str())-1;
      if (nr>=0 && nr<fsfv.size()) {
        int zdng=0; // Zahl der nicht geloeschten
        if (fsfv[nr].capisd!="NULL" && !fsfv[nr].capisd.empty()) {
          zdng+=fsfv[nr].loeschecapi(obverb,oblog);
          Log(blaus+"capi: "+Tx[T_Zahl_der_nicht_geloeschten_Dateien]+schwarz+ltoan(zdng)+blau+Tx[T_Capispooldatei]+
              schwarz+fsfv[nr].capisd,obverb,oblog);
        }
        if (fsfv[nr].hylanr!="NULL" && !fsfv[nr].hylanr.empty()) {
          zdng+=fsfv[nr].loeschehyla(this,obverb,oblog);
          Log(blaus+"hyla: "+Tx[T_Zahl_der_nicht_geloeschten_Dateien]+schwarz+ltoan(zdng)+blau+Tx[T_hylanr]+schwarz+fsfv[nr].capisd,obverb,oblog);
        }
        Log(blaus+Tx[T_Gesamt]+Tx[T_Zahl_der_nicht_geloeschten_Dateien]+schwarz+ltoan(zdng),obverb,oblog);
        struct stat entrysend;
        fsfv[nr].setzcapistat(this,&entrysend);
        string protdakt;
        uchar hyla_uverz_nr=1;
        int obsfehlt=0;
        /*fsfv[nr].*/setzhylastat(&fsfv[nr], &protdakt, &hyla_uverz_nr, &obsfehlt, 0, obverb, oblog); // hyla_uverz_nr, obsfehlt
        Log(violetts+"capistat: "+schwarz+FxStatS(&fsfv[nr].capistat)+violett+", hylastat: "+schwarz+FxStatS(&fsfv[nr].hylastat),obverb,oblog);
        if (!zdng || (fsfv[nr].capistat==fehlend && fsfv[nr].hylastat==fehlend)) {
          RS loe(My,string("delete from `")+spooltab+"` where id="+fsfv[nr].id,-obverb);
        }
      }
    }
  } else {
    Log(Tx[T_Kein_Fax_zum_Loeschen_vorhanden],1,oblog);
  }
  return 1;
} // int paramcl::loeschefax(int obverb, int oblog)

// wird aufgerufen in: main
int paramcl::loeschewaise(int obverb, int oblog)
{
  Log(blaus+Tx[T_loeschewaise]+schwarz,obverb,oblog);
  vector<string> allec;
  vector<string> ids;
  char*** cerg;
  RS su(My,string("select original p0, capispooldatei p1, hylanr p2, id p3 from `")+spooltab+"`");
  while (cerg=su.HolZeile(),cerg?*cerg:0) {
    if (*(*cerg+0)) {
      struct stat entryo;
      if (!lstat((wvz+vtz+*(*cerg+0)).c_str(),&entryo)) continue; // Wenn es die Datei im Warteverzeichnis gibt
      if (*(*cerg+1)) if (!lstat((cfaxusersqvz+vtz+*(*cerg+1)).c_str(),&entryo)) continue; // wenn eine Capispooldatei drinsteht und es sie gibt
      if (*(*cerg+2)) if (!lstat((hsendqvz+"/q"+*(*cerg+2)).c_str(),&entryo)) continue; // wenn eine Hylaspooldatei drinsteht und es sie gibt
      if (*(*cerg+3)) ids.push_back(*(*cerg+3));
    }
  }
  for(size_t i=0;i<ids.size();i++) {
    RS loe(My,string("delete from `")+spooltab+"` where id="+ids[i]);
  }
  return 0;
} // int paramcl::loeschewaise(int obverb, int oblog)

// wird aufgerufen in: main
int paramcl::loescheallewartende(int obverb, int oblog)
{
  Log(blaus+Tx[T_loescheallewartenden]+schwarz,obverb,oblog);
  int erg=0;
  vector<string> allec;
  struct stat entryvz;
  if (!lstat(cfaxusersqvz.c_str(),&entryvz)) {
    cmd=string("find '")+cfaxusersqvz+"/' -maxdepth 1 -type f -iname 'fax-*.*'";
    systemrueck(cmd,obverb,oblog,&allec);
    erg+=allec.size();
    for(size_t i=0;i<allec.size();i++) {
      remove(allec[i].c_str());
      if (allec[i].find(".sff")!=string::npos) {
        string fname=base_name(allec[i]);
        RS loe(My,string("delete from `")+spooltab+"` where capispooldatei='"+fname+"'");
      }
    }
  }
  //  cmd=string("rm ")+cfaxuservz+vtz+cuser+vtz+"sendq"+vtz+"fax-*.*"; //  "/var/spool/capisuite/users/<user>/sendq";
  if (!lstat(hsendqvz.c_str(),&entryvz)) {
    cmd=string("find '")+hsendqvz+"' -maxdepth 1 -type f -iname 'q*' -printf '%f\\n'";
    vector<string> alled;
    systemrueck(cmd,obverb,oblog, &alled);
    erg+=alled.size();
    for(size_t i=0;i<alled.size();i++) {
      string transalle=alled[i];
      ersetzAlle(&transalle,"q","");  
      cmd=string("faxrm ")+transalle;
      if (systemrueck(cmd,obverb,oblog)) {
        RS loe(My,string("delete from `")+spooltab+"` where hylanr="+transalle);
      }
    }
  }
  return erg;
} // int paramcl::loescheallewartende(int obverb, int oblog)

// wird aufgerufen in: main (2x)
void paramcl::tu_lista(const string& oberfolg)
{
  Log(violetts+Tx[T_tu_lista]+schwarz,obverb,oblog);
  char ***cerg;
  RS lista(My,string("select Ueberm p0, Submid p1, Faxname p2, Empfaenger p3, Fax p4, Erfolg p5 from (")+
      "select date_format(transe,'%d.%m.%y %H:%i:%s') Ueberm, Submid, right(concat(space(75),left(Docname,75)),75) Faxname, "
      "right(concat(space(30),left(rcname,30)),30) Empfaenger, rcfax Fax, Erfolg, transe from `"+
      touta+"` where Erfolg = "+oberfolg+" order by eind desc limit "+dszahl+
      ") i order by transe",ZDB);
  while (cerg=lista.HolZeile(),cerg?*cerg:0) {
    cout<<blau<<setw(17)<<*(*cerg+0)<<"|"<<violett<<setw(14)<<*(*cerg+1)<<schwarz<<"|"<<blau<<setw(65)<<*(*cerg+2)<<"|"
      <<schwarz<<setw(30)<<*(*cerg+3)<<"|"<<blau<<*(*cerg+4)<<schwarz<<endl;
  }
} // tu_lista

// wird aufgerufen in: main
void paramcl::tu_listi()
{
  Log(violetts+Tx[T_tu_listi]+schwarz,obverb,oblog);
  char ***cerg;
  RS listi(My,string("SELECT * from (SELECT date_format(transe,'%d.%m.%y %H:%i:%s') p0,right(concat(space(85),left(titel,85)),85) p1,"
        "fsize p2,tsid p3,id p4 FROM `")+tinca+"` i order by transe desc limit "+dszahl+") i order by p0",ZDB);
  while (cerg=listi.HolZeile(),cerg?*cerg:0) {
    cout<<blau<<setw(17)<<*(*cerg+0)<<"|"<<violett<<setw(85)<<*(*cerg+1)<<schwarz<<"|"<<blau<<setw(17)<<*(*cerg+2)<<"|"
      <<schwarz<<setw(17)<<*(*cerg+3)<<"|"<<blau<<*(*cerg+4)<<schwarz<<endl;
  }
} // tu_listi

// zufaxenvz = zufaxen-Verzeichnis
// 1a. die (Nicht-PDF- und PDF-) Dateien in dem Verzeichnis ermitteln und im Fall mehrerer Zielfaxnummern aufteilen ...
// 1b. die Nicht-PDF-Dateien in dem Verzeichnis zum PDF-Druck ermitteln, in Warteverzeichnis verschieben und in die PDF-Liste spdf eintragen ...
// 2a. ... und in im Warteverzeichnis in PDF umwandeln, falls erfolgreich und gleichziel => auch in ziel kopieren
// 2b. Die originalen PDF-Dateien ins Warteverzeichnis verschieben, falls erfolgreich, nicht schon registriert und gleichziel => auch in ziel kopieren
// wird aufgerufen in: main
void paramcl::DateienHerricht() 
{
  Log(violetts+Tx[T_DateienHerricht],obverb,oblog);
  static uchar sofficegeprueft=0, convertgeprueft=0;
  struct stat entrynpdf;
  //vector<string> npdf, spdf, *npdfp=&npdf, *spdfp=&spdf;  vector<uchar> prios;
  vector<fxfcl> fxv;
  vector<string> anfxstrvec;
  anfxstrvec.push_back(anfaxstr);
  if (!ancfaxstr.empty()) anfxstrvec.push_back(ancfaxstr);
  if (!anhfaxstr.empty()) anfxstrvec.push_back(anhfaxstr);
  for(uchar iprio=0;iprio<anfxstrvec.size();iprio++) {
    // 1a. die (Nicht-PDF- und PDF-) Dateien in dem Verzeichnis ermitteln und im Fall mehrerer Zielfaxnummern aufteilen ...
    cmd=string("find \"")+zufaxenvz+"\" -maxdepth 1 -type f -iregex \".*"+anfxstrvec.at(iprio)+" [ -,/;:\\\\\\.\\+]*[0123456789]+.*\"";
    vector<string> alled;
    systemrueck(cmd,obverb,oblog, &alled);
    for(size_t i=0;i<alled.size();i++) {
      string stamm,exten;
      getstammext(&(alled.at(i)),&stamm,&exten);
      Log(string(Tx[T_Endung])+tuerkis+exten+schwarz,obverb>1,oblog);
      Log(string(Tx[T_Stamm])+tuerkis+stamm+schwarz,obverb>1,oblog);
      vector<string> tok, toknr, toktxt, tokname;
      Log(string(Tx[T_trennenach])+blau+anfxstrvec.at(iprio)+schwarz+"'",obverb>1,oblog);
      aufiSplit(&tok,&stamm,anfxstrvec.at(iprio).c_str());
      if (tok.size()>1) { // sollte immer sein
        for(unsigned k=0;k<tok.size();k++) {
          Log(blaus+tok[k]+schwarz,obverb>1,oblog);
        }
        aufiSplit(&toknr,&tok[1],undstr.c_str());
        for(unsigned k=0;k<toknr.size();k++) {
          Log(tuerkiss+"toknr["+ltoan(k)+"]: "+toknr[k]+schwarz,obverb>1,oblog);
        }
        aufiSplit(&toktxt,&tok[0],anstr.c_str());
        for(unsigned k=0;k<toktxt.size();k++) {
          Log(blaus+"toktxt["+ltoan(k)+"]: "+toktxt[k]+schwarz,obverb>1,oblog);
        }
        if (toktxt.size()>1) {
          aufiSplit(&tokname,&toktxt[1],undstr.c_str());
          for(unsigned i=0;i<tokname.size();i++) {
            Log(tuerkiss+"tokname["+ltoan(i)+"]: "+tokname[i]+schwarz,obverb>1,oblog);
          }
        }
        for(unsigned j=0;j<toknr.size();j++) { // alle bis auf die letzte Adresse
          string tmp;
          gtrim(&toknr[j]);
          gtrim(&toktxt[0]);
          if (toktxt.size()==1) {                          // keine Arztliste
            tmp= string(toktxt[0]+" "+anfxstrvec.at(iprio)+" "+toknr[j]+'.'+exten);
          } else if (tokname.size()!=toknr.size()) {         // Arztliste nicht gleich lang wie Faxnummernliste
            gtrim(&(toktxt[1]));
            tmp= string(toktxt[0]+anstr+toktxt[1]+" "+anfxstrvec.at(iprio)+" "+toknr[j]+'.'+exten);
          } else {                                       // alles optimal
            gtrim(&(tokname[j]));
            tmp= string(toktxt[0]+anstr+tokname[j]+" "+anfxstrvec.at(iprio)+" "+toknr[j]+'.'+exten);
          }
          Log(tuerkiss+toknr[j]+schwarz,obverb>1,oblog);
          uint kfehler=0;
          if (j<toknr.size()-1) {
            kopiere(alled.at(i),tmp,&kfehler,1);
            if (kfehler) break;
            Log(string(Tx[T_ErstelledurchKopieren])+rot+tmp+schwarz,1,oblog);
          } else {
            if (alled.at(i)!=tmp) {
              int result=rename((alled.at(i)).c_str(),tmp.c_str());
              if ( result != 0 )
                Log(rots+Tx[T_FehlerbeimUmbenennen]+schwarz,1,1);
              Log(string(Tx[T_ErstelledurchBenennen])+rot+tmp+schwarz,1,oblog);
            }
          }
        } // for(unsigned j=0;j<toknr.size();j++) 
      } // if (tok.size()>1) 
    } // for(size_t i=0;i<alled.size();i++) 
  } // for(unsigned iprio=0;iprio<anfxstrvec.size();iprio++) 

  // 1b. die Nicht-PDF-Dateien in dem Verzeichnis zum PDF-Druck ermitteln, 
  //    in Warteverzeichnis verschieben und in die PDF-Liste spdfp eintragen ...
    /*
       cmd=string("find \"")+zufaxenvz+"\" -maxdepth 1 -type f -iregex \".*\\("+
       anfaxstr+
       (ancfaxstr.empty()?"":string("\\|")+ancfaxstr))+
       (anhfaxstr.empty()?"":string("\\|")+anhfaxstr+
       "\\) [ -,/;:\\\\\\.\\+]*[0123456789]+.*\" -not -iname \"*.pdf\"";
     */
    for(unsigned iprio=0;iprio<anfxstrvec.size();iprio++) {
      if (!anfxstrvec.at(iprio).empty()) {
        cmd= string("find \"")+zufaxenvz+"\" -maxdepth 1 -type f -iregex \".*"+anfxstrvec.at(iprio)+" [ -,/;:\\\\\\.\\+]*[0123456789]+.*\""
          " -not -iname \"*.pdf\"";
        vector<string> npdfd;
        systemrueck(cmd, obverb,oblog, &npdfd);
        for(size_t i=0;i<npdfd.size();i++) {
          uint vfehler=0;
          string ndname=zufaxenvz+vtz+neuerdateiname(npdfd.at(i));
          if (ndname!=npdfd.at(i)) {
            dorename(npdfd.at(i),ndname,&vfehler,obverb,oblog);
            if (vfehler) {
              cerr<<rot<<Tx[T_autofaxabgebrochen]<<schwarz<<vfehler<<Tx[T_FehlerbeimUmbenennenbei]<<endl<<
                blau<<npdfd.at(i)<<schwarz<<" ->\n"<<
                blau<<ndname<<schwarz<<endl;
              exit(1);
            }
            npdfd.at(i)=ndname;
          }
          string wartedatei=verschiebe(npdfd.at(i),wvz,&vfehler,1,obverb,oblog);
          if (!vfehler) {
            string stamm,exten;
            //          npdfp->push_back(wartedatei);
            getstammext(&wartedatei,&stamm,&exten);
            string zupdf=stamm+".pdf"; 
            //          spdfp->push_back(zupdf);
            //          prios.push_back(iprio);
            fxv.push_back(fxfcl(wartedatei,zupdf,iprio));
          } else {
            cerr<<rot<<Tx[T_autofaxabgebrochen]<<schwarz<<vfehler<<Tx[T_FehlerbeimUmbenennenbei]<<endl<<
              blau<<npdfd.at(i)<<schwarz<<" ->\n"<<
              blau<<wvz<<schwarz<<endl;
            exit(1);
          } // if (!vfehler) 
        } // for(size_t i=0;i<npdfd.size();i++) 
      } // if (!anfxstrvec.at(iprio).empty()) 
    } // for(unsigned iprio=0;iprio<anfxstrvec.size();iprio++) 

  for(unsigned i=0;i<fxv.size();i++) {
    Log(string("npdf[")+rot+ltoan(i)+schwarz+"]: "+rot+fxv[i].npdf+schwarz,obverb,oblog);
    Log(string("spdf[")+rot+ltoan(i)+schwarz+"]: "+rot+fxv[i].spdf+schwarz,obverb,oblog);
    Log(string("prio[")+rot+ltoan(i)+schwarz+"]: "+rot+ltoan(fxv[i].prio)+schwarz,obverb,oblog);
  }
  // zufaxenvz = zufaxen-Verzeichnis
  Log(string(Tx[T_aus])+drot+zufaxenvz+schwarz+vtz+Tx[T_verarbeitete_Nicht_PDF_Dateien]+drot+ltoan(fxv.size())+schwarz,1,oblog);
  //  geszahl+=npdfp->size(); // 1b

  // 2a. ... und im Warteverzeichnis in PDF umwandeln, falls erfolgreich und gleichziel => auch in ziel kopieren
  for (int nachrnr=fxv.size()-1; nachrnr>=0; --nachrnr) {
    unsigned erg=-1;
    if (lstat((fxv[nachrnr].npdf.c_str()), &entrynpdf)) {
      // ZPKT(" nach lstat: "hh;
      Log(string(Tx[T_lstatfehlgeschlagen]) + strerror(errno) + Tx[T_beiDatei]+ fxv[nachrnr].npdf,1,1,1);
      continue;
    } // (lstat((*pfad + vtz + dirEntry->d_name).c_str(), &entrynpdf)) 
    for(unsigned runde=1;runde<=2;runde++) {
      switch (runde) {
        case 1: 
          if (!sofficegeprueft) {
            if (systemrueck("which soffice",obverb,oblog)) {
              //              systemrueck("which zypper 2>/dev/null && zypper -n in soffice || "
              //                          "{ which apt-get 2>/dev/null && apt-get --assume-yes install soffice; }",obverb,oblog);
              linst.doinst("soffice",obverb,oblog);
            }
            sofficegeprueft=1;
          }
          cmd=string("soffice --headless --convert-to pdf --outdir \"")+wvz+"\" \""+fxv[nachrnr].npdf+"\"";
          break; // Ergebnis immer 0
        case 2: 
          if (!convertgeprueft) {
            if (systemrueck("which convert",obverb,oblog)) {
              //              systemrueck("which zypper 2>/dev/null && zypper -n in convert || "
              //                          "{ which apt-get 2>/dev/null && apt-get --assume-yes install convert; }",obverb,oblog);
              linst.doinst("convert",obverb,oblog);
            }
            convertgeprueft=1;
          }
          cmd=string("convert \""+fxv[nachrnr].npdf+"\" \""+fxv[nachrnr].spdf+"\""); 
          break;
      }
      vector<string> umwd;
      systemrueck(cmd, obverb,oblog,&umwd);
      erg=lstat(fxv[nachrnr].spdf.c_str(),&entrynpdf); 
      Log(string(Tx[T_Umwandlungvon])+blau+fxv[nachrnr].npdf+Tx[T_inPDFmit]+tuerkis+(runde==1?"soffice":"convert")+schwarz+
          Tx[T_beendetErgebnis]+rot+(erg?Tx[T_misserfolg]:Tx[T_Erfolg_af])+schwarz, 1||erg,(erg?1:oblog));
      if (!erg) break; 
    }
    if (!erg) {
      if (gleichziel) {
        uint kfehler=0;
        kopiere(fxv[nachrnr].npdf, zmp, &kfehler, 1, obverb, oblog);
        kopiere(fxv[nachrnr].spdf, zmp, &kfehler, 1, obverb, oblog);
      }
    } else {
      //      spdfp->erase(spdfp->begin()+nachrnr);
      fxv.erase(fxv.begin()+nachrnr);
    }
  } // for (int nachrnr=npdfp->size()-1; nachrnr>=0; --nachrnr)  // 2.

  // 2b. Die originalen PDF-Dateien ins Warteverzeichnis verschieben, falls erfolgreich, nicht schon registriert und gleichziel 
  //      => auch in ziel kopieren
    for(unsigned iprio=0;iprio<anfxstrvec.size();iprio++) {
      if (!anfxstrvec.at(iprio).empty()) {
        cmd=string("find \"")+zufaxenvz+"\" -maxdepth 1 -type f -iregex \".*"+anfxstrvec.at(iprio)+" [ -,/;:\\\\\\.\\+]*[0123456789]+.*\""
          " -iname \"*.pdf\"";
        vector<string> spdfd;
        systemrueck(cmd,obverb, oblog, &spdfd);
        for(size_t i=0;i<spdfd.size();i++) {
          uint vfehler=0;
          string ndname=zufaxenvz+vtz+neuerdateiname(spdfd.at(i));
          if (ndname!=spdfd.at(i)) {
            dorename(spdfd.at(i),ndname,&vfehler,obverb,oblog);
            if (vfehler) {
              cerr<<rot<<Tx[T_autofaxabgebrochen]<<schwarz<<vfehler<<Tx[T_FehlerbeimUmbenennenbei]<<endl<<
                blau<<spdfd.at(i)<<schwarz<<" ->\n"<<
                blau<<ndname<<schwarz<<endl;
              exit(1);
            }
            spdfd.at(i)=ndname;
          }
          string wartedatei=verschiebe(spdfd.at(i),wvz,&vfehler,1,obverb,oblog);
          if (!vfehler) {
            int vorhanden=0; // 1= Datei schon zuvor als nicht-PDF-Datei eingetragen
            for(unsigned i=0;i<fxv.size();i++) {
              if (i>=spdfd.size()) break;
              if (fxv[i].spdf==spdfd.at(i)) {vorhanden=1;break;} 
            }
            if (!vorhanden) {
              fxv.push_back(fxfcl(wartedatei,iprio));
              if (gleichziel) {
                uint kfehler=0;
                kopiere(wartedatei, zmp, &kfehler, 1, obverb, oblog);
              }
            } // if (!vorhanden)
          } else {
            cerr<<rot<<Tx[T_autofaxabgebrochen]<<schwarz<<vfehler<<Tx[T_FehlerbeimUmbenennenbei]<<endl<<
              blau<<spdfd.at(i)<<schwarz<<" ->\n"<<
              blau<<wvz<<schwarz<<endl;
            exit(1);
          } // if (!vfehler) else 
        } // for(size_t i=0
      } // if (!anfxstrvec.at(iprio).empty()) 
    } // for(unsigned iprio=0;iprio<anfxstrvec.size();iprio++) 
  for(unsigned i=0;i<fxv.size();i++) {
    Log(string("npdf[")+rot+ltoan(i)+schwarz+"]: "+rot+fxv[i].npdf+schwarz,obverb,oblog);
    Log(string("spdf[")+rot+ltoan(i)+schwarz+"]: "+rot+fxv[i].spdf+schwarz,obverb,oblog);
    Log(string("prio:       ")+rot+ltoan(fxv[i].prio)+schwarz,obverb,oblog);
  }

  Log(string(Tx[T_aus])+drot+zufaxenvz+schwarz+vtz+Tx[T_verarbeitete_PDF_Dateien]+drot+ltoan(fxv.size())+schwarz,1,oblog);
  geszahl=fxv.size();
  WVZinDatenbank(&fxv);
} // void DateienHerricht(DB *My, string *wvz, string *zufaxenvz, zielmustercl zmp, unsigned long *geszahl,uchar gleichziel, int obverb=0,int oblog=0)

// wird aufgerufen in: main
void paramcl::faxealle()
{
  Log(violetts+Tx[T_faxealle]+schwarz+", "+blau+Tx[T_obcapimitDoppelpunkt]+schwarz+(obcapi?Tx[T_ja]:Tx[T_nein])+", "
      +blau+Tx[T_obhylamitDoppelpunkt]+schwarz+(obhyla?Tx[T_ja]:Tx[T_nein]),obverb,oblog);
  vector<fsfcl> fsfv;
  char ***cerg;
  unsigned long dbszahl=0; // Zahl der Datenbanksaetze
  string hzstr=ltoan(hylazuerst);
  RS r0(My,string("select id p0, origvu p1, original p2, telnr p3, prio p4, "
        "if(isnull(capispooldatei),'',capispooldatei) p5, if(isnull(capidials),'',capidials) p6, "
        "if(isnull(hylanr),'',hylanr) p7, if(isnull(hyladials),'',hyladials) p8, "
        "((isnull(capispooldatei)or capispooldatei='') and (isnull(hyladials) or hyladials>")+maxhylanr+" or hyladials=-1 or "
      "    (isnull(prio) or prio=1 or (prio=0 and not "+hzstr+")))) p9, "
      "((isnull(hylanr)or hylanr='') and (isnull(capidials) or capidials>" +maxcapinr+" or capidials=-1 or "
      "      (isnull(prio) or prio=2 or (prio=0 and     "+hzstr+")))) p10 "
      "from `"+spooltab+"` "
      "where original>''",ZDB);
  if (r0.obfehl) {
    cerr<<rots<<Tx[T_Fehler_af]<<schwarz<<r0.obfehl<<rot<<Tx[T_beiSQLAbfrage]<<schwarz<<r0.sql<<endl;
  } else {
    while (cerg=r0.HolZeile(),cerg?*cerg:0) {
      (dbszahl)++;
      if (!*(*cerg+9)) *(*cerg+9)=(char*)"";
      if (!*(*cerg+10)) *(*cerg+10)=(char*)"";
      if (*(*cerg+0) && *(*cerg+1) && *(*cerg+2) && *(*cerg+3) && *(*cerg+4) && *(*cerg+5) && 
          *(*cerg+6) && *(*cerg+7) && *(*cerg+8) && *(*cerg+9) && *(*cerg+10)) {
        // obcapi = *(*cerg+9), obhyla=*(*cerg+10)
        fsfv.push_back(fsfcl(*(*cerg+0), *(*cerg+1), *(*cerg+2), *(*cerg+3), atoi(*(*cerg+4)), *(*cerg+5),
              atoi(*(*cerg+6)), *(*cerg+7), atoi(*(*cerg+8)), (binaer)atoi(*(*cerg+9)), (binaer)atoi(*(*cerg+10))));
      }
    }
    Log(string(Tx[T_ZahldDSmwegzuschickendenFaxenin])+spooltab+"`: "+ltoan(fsfv.size()),obverb,oblog);
    for(unsigned i=0;i<fsfv.size();i++) {
      Log(string(" i: ")+blau+ltoan(i)+schwarz+Tx[T_PDFDatei]+blau+fsfv[i].spdf+schwarz+
          " ."+Tx[T_obcapimitDoppelpunkt]+blau+(fsfv[i].fobcapi?Tx[T_ja]:Tx[T_nein])+schwarz+
          " ."+Tx[T_obhylamitDoppelpunkt]+blau+(fsfv[i].fobhyla?Tx[T_ja]:Tx[T_nein])+schwarz,obverb,oblog);
      if (fsfv[i].fobcapi) if (obcapi) faxemitC(My, spooltab, &fsfv[i],this,obverb,oblog);  
      if (fsfv[i].fobhyla) if (obhyla) faxemitH(My, spooltab, &fsfv[i],this,obverb,oblog);  
      //      _out<<fsfv[i].id<<" "<<rot<<fsfv[i].npdf<<" "<<schwarz<<(int)fsfv[i].obcapi<<" "<<(int)fsfv[i].obhyla<<endl;
    }
  }
} // faxealle

// wird aufgerufen in: untersuchespool, loeschefax
void fsfcl::setzcapistat(paramcl *pmp, struct stat *entrysendp)
{
  if (capisd.empty()) {
    capistat=fehlend;
  } else {
    string aktuser;
    sendqgespfad = cspf+vtz+capisd;
    size_t p1,p2;
    if ((p1=cspf.rfind(vtz))) if ((p2=cspf.rfind(vtz,p1-1))) {
      aktuser=cspf.substr(p2+1,p1-p2-1);
      if (!lstat(sendqgespfad.c_str(),entrysendp)) {
        capistat=wartend;
      } else {
        // gesandte und gescheiterte Faxe wurden von capisuite entsprechend umbenannt
        for(capistat=gesandt;capistat<=gescheitert;capistat=static_cast<FxStat>(capistat+1)) { 
          // entspr. gefaxte/gescheiterte Datei in capisuite
          sendqgespfad = (capistat==gescheitert?pmp->cfailedvz:pmp->cdonevz)+vtz+aktuser+"-"+capisd; 
          if (!lstat((sendqgespfad.c_str()), entrysendp)) break; 
        } 
        // hier koennte capistat auch fehlend sein
      } 
    }
  }
} // fsfcl::setzcapistat

// hylafax: bei zu faxenden Dateien stehen die Steuerdateien in /var/spool/fax/sendq/q105, benannt nach /var/spool/fax/etc/xferfaxlog, dort steht in der 6. Spalte die hyla-Index-Nummer z.B. 105, die als Rueckmeldung von sendfax erscheint ("request id is 105 (group id 105) for host localhost (1 file)")
// in der 4. Spalte steht dazu die Dokumentnummer 00000133, mit der das Dokument im Spool in /var/spool/fax/docq/doc133.pdf wartet
// hylafax-Befehl: sendfax -n -A -d  08141305952 "/DATA/Patientendokumente/warteauffax/Klupp Harals Korrekturschema an Fax 08141305952.pdf"
// sendfax -n -A -d   764881 "/DATA/Patientendokumente/warteauffax/Jäle Erika, MR-Befund an Dr. Neteler an Fax 764881.pdf""  ( (mit utf8)

// Dateien in Spool-Tabelle nach inzwischen verarbeiteten durchsuchen, Datenbank- und Dateieintraege korrigieren 
// wird aufgerufen in: main
void paramcl::untersuchespool() // faxart 0=capi, 1=hyla
{
  // Schaue nach, welche der gespoolten schon weggeschickt sind, Anpassung der Primaerdateien und des Datenbankeintrags
  Log(violetts+Tx[T_untersuchespool]+schwarz,obverb,oblog);
  char ***cerg;
  RS rs(My,string("select id p0,capispooldatei p1,capispoolpfad p2,original p3,cdateidatum p4,"
        " telnr p5,origvu p6,hylanr p7,capidials p8,hyladials p9,hdateidatum p10 "
        "from `")+spooltab+"`",ZDB);
  if (!rs.obfehl) {
    faxord=0;
    while (cerg=rs.HolZeile(),cerg?*cerg:0) {
      faxord++;
      if (*(*cerg+0)) if (*(*cerg+3)) {
        (dbzahl)++;
        stringstream ausg; //      ausg.str(std::string()); ausg.clear();
        fsfcl fsf(*(*cerg+0),*(*cerg+3)); // id, original
        if (*(*cerg+1)) fsf.capisd =*(*cerg+1); // capispooldatei
        if (*(*cerg+2)) fsf.cspf   =*(*cerg+2); // capispoolpfad
        if (*(*cerg+4)) fsf.cdd    =*(*cerg+4); // cdateidatum
        if (*(*cerg+5)) fsf.telnr  =*(*cerg+5); // telnr
        if (*(*cerg+6)) fsf.origvu =*(*cerg+6); // origvu
        if (*(*cerg+7)) fsf.hylanr =*(*cerg+7); // hylanr
        if (*(*cerg+8)) fsf.cdials =*(*cerg+8); // capidials
        if (*(*cerg+9)) fsf.hdials =*(*cerg+9); // hyladials
        if (*(*cerg+10)) fsf.hdd   =*(*cerg+10); // hdateidatum
        Log(string("id: ")+fsf.id+": ",obverb?-2:0,oblog); // -2: schreibt ohne Zeilenwechsel
        ausg<<blau<<faxord<<") "<<rot<<wvz<<vtz<<fsf.original<<schwarz<<": "; // ab hier Neue-Zeile-Zeichen immer am Anfang der naechsten Zeile
        // a) ueber capisuite
        // den Status in Capi der aus spool geholten Zeile untersuchen, dort aktualisieren
        //   und ggf. in hylafax stoppen
        struct stat entrysend;
        if (obcapi) {
          if (faxord==1) this->pruefcapi(); // in der ersten Runde, in der Capi verwendet werden soll, Capi pruefen
          fsf.setzcapistat(this, &entrysend);
          string ctries;
          fsf.capiwausgeb(&ausg, &capiconfp[2].wert, obverb, &ctries, oblog);

          if (fsf.capistat==wartend) {
            RS rupd(My); 
            vector<instyp> einf; // fuer alle Datenbankeinfuegungen
            einf.push_back(instyp(My->DBS,"capidials",&ctries));
            string bedingung=string("id=")+fsf.id;
            rupd.update(spooltab,einf,ZDB,bedingung);

          } else if (fsf.capistat==gesandt) {
            // ... und ggf. in hylafax loeschen
            fsf.loeschehyla(this,obverb, oblog);
          } else if (fsf.capistat==gescheitert) {
          } else if (fsf.capistat==fehlend) {
          }
        } // if (obcapi) 

        // b) ueber hylafax
        if (obhyla) {
          string protdakt;
          uchar hyla_uverz_nr; // kleine Runde
          int obsfehlt=-1;
          string hylastate, hyladials, hylastatus, hylastatuscode;
          /*fsf.*/
          setzhylastat(&fsf, &protdakt, &hyla_uverz_nr, &obsfehlt, 0, obverb, oblog, &hylastate, &hyladials, &hylastatus, &hylastatuscode);
          fsf.hylaausgeb(&ausg, this, obsfehlt, obverb, 0, oblog);
          if (!obsfehlt) { // Protokolldatei vorhanden
            RS rupd(My); 
            vector<instyp> einf; // fuer alle Datenbankeinfuegungen
            einf.push_back(instyp(My->DBS,"hylastate",&hylastate));
            if (!hyla_uverz_nr) { // wenn fertig
              if (fsf.hylastat==gescheitert) { // (hylastate=="8") // 8, status gescheitert, evtl. unzureichend dokumentiert, aber wahr
                einf.push_back(instyp(My->DBS,"hylanr","0",(uchar)1));
                //                  einf.push_back(instyp(My->DBS,"hyladials","-1",(uchar)1));
                hyladials="-1";
                einf.push_back(instyp(My->DBS,"hylastatus",&hylastatus));
                einf.push_back(instyp(My->DBS,"hylastatuscode",&hylastatuscode));
              } else if (fsf.hylastat==gesandt) { // (hylastate=="7") // 7, status erfolgreich
                // ... und ggf. in capisuite loeschen
                fsf.loeschecapi(obverb,oblog);
              }
            }
            einf.push_back(instyp(My->DBS,"hyladials",&hyladials));
            string bedingung=string("id=")+fsf.id;
            rupd.update(spooltab,einf,ZDB,bedingung);
            ausg<<Tx[T_bzw]<<blau<<protdakt<<schwarz;
          } // if (!warteirgendwo)
        } // if (!obsfehlt) ... else
        if (obcapi || obhyla) {
          // im Erfolgsfall zugrundeliegende Dateien verschieben
          if (fsf.capistat==gesandt || fsf.hylastat==gesandt) {
            (ezahl)++;

            // Fax gelungen, Dateien in warteauffax nach zielmuster verschieben bzw. loeschen ...
            for(unsigned iru=0;iru<2;iru++) {
              string *datei=iru?&fsf.origvu:&fsf.original;
              if (!datei->empty()) {
                string zuloe = wvz+ vtz + *datei;
                if (gleichziel) { 
                  tuloeschen(zuloe,obverb,oblog);
                } else {
                  uint vfehler=0;
                  verschiebe(zuloe, zmp, &vfehler, 1, obverb, oblog);
                }
              }
            }
          } else if ((!obhyla && fsf.capistat==gescheitert) || (!obcapi && fsf.hylastat==gescheitert) || 
                     (fsf.capistat==gescheitert && fsf.hylastat==gescheitert)) {
            (gzahl)++; 
          } else if ((!obhyla && fsf.capistat==fehlend) || (!obcapi && fsf.hylastat==fehlend) || 
                     (fsf.capistat==fehlend && fsf.hylastat==fehlend)) {
            (fzahl)++;
          } else if (fsf.capistat==wartend || fsf.hylastat==wartend) {
            (wzahl)++;
          }
          // Aktionen, wenn in beiden gescheitert oder fehlend
          if (obverb>0) {
            Log(violetts+"Capistat: "+schwarz+FxStatS(&fsf.capistat)+violett+", Hylastat: "+schwarz+FxStatS(&fsf.hylastat),obverb,oblog);
          }
          uchar allegesch = ((!obcapi || fsf.capistat==gescheitert) && (!obhyla || fsf.hylastat==gescheitert));
          uchar nimmer = ((!obcapi || fsf.capistat==fehlend || fsf.capistat==gescheitert) && 
              (!obhyla || fsf.hylastat==fehlend || fsf.hylastat==gescheitert));
          uchar ogibts[2] = {0,0};
          string odatei[2];
          if (nimmer) {
            for(unsigned iru=0;iru<2;iru++) {
              string *datei=iru?&fsf.origvu:&fsf.original;
              if (!datei->empty()) {
                odatei[iru] = wvz+ vtz + *datei;
                struct stat ostat;
                ogibts[iru]=!lstat(odatei[iru].c_str(),&ostat);
                Log(blaus+"odatei["+(iru?"1":"0")+"]: "+odatei[iru],obverb,oblog);
                Log(blaus+"ogibts["+(iru?"1":"0")+"]: "+(ogibts[iru]?"1":"0"),obverb,oblog);
              } // if (!datei->empty()
            } // for(unsigned iru=0
          } // if (nimmer)
          if (fsf.capistat==gesandt || fsf.hylastat==gesandt || allegesch || (nimmer /* && !ogibts[0] */) ) {
            uchar geloescht=0;
            fsf.archiviere(My, this, &entrysend, allegesch , fsf.capistat==gesandt?capi:fsf.hylastat==gesandt?hyla:fsf.capisd.empty()?capi:hyla, 
                &geloescht, obverb, oblog);
          } 
          if (allegesch || (nimmer && !ogibts[0])) {
            // Fax gescheitert, Dateien von warteauffax nach nichtgefaxt verschieben
            for(unsigned iru=0;iru<2;iru++) {
              uint vfehler=0;
              if (ogibts[iru])
                verschiebe(odatei[iru],gvz,&vfehler, 1, obverb,oblog);
            }
          } // if (allegesch || (nimmer && !ogibts[0]))
        } // if (obcapi || obhyla)
        Log(ausg.str(),1,oblog);
      } // if (*(*cerg+0)) if (*(*cerg+3))
    } // while (cerg=rs.HolZeile(),cerg?*cerg:0) 
  } // if (!rs.obfehl) 
} // untersuchespool

// Zeige Dateien im Spool an, die nicht in der Spool-Tabelle stehen
// wird aufgerufen in: main
void paramcl::zeigweitere()
{
  Log(violetts+Tx[T_zeigweitere]+schwarz,obverb,oblog);
  static int obtitel=0;
  stringstream ausg; //      ausg.str(std::string()); ausg.clear();
  vector<string> rueck;
  struct stat entryvz;
  if (obcapi) {
    if (!lstat(cfaxusersqvz.c_str(),&entryvz)) {
      // 7.2.16: alte *.lock-Dateien loeschen
      cmd=string("find '")+cfaxusersqvz+"' -maxdepth 1 -type f -iname 'fax*.lock'"; //  -printf '%f\\n'";
      systemrueck(cmd,obverb,oblog,&rueck);
      for(size_t i=0;i<rueck.size();i++) {
        string stamm,exten;
        getstammext(&rueck[i],&stamm,&exten);
        string zugeh=stamm+".sff";
        if (lstat(zugeh.c_str(),&entryvz)) {
          remove(rueck[i].c_str());
        }
      }
      rueck.clear();
      // 20.1.16: wenn dort eine .txt-steht ohne zugehoerige .sff-Datei, dann haelt sie den ganzen Betrieb auf
      cmd=string("find '")+cfaxusersqvz+"' -maxdepth 1 -type f -iname 'fax*.txt'"; //  -printf '%f\\n'";
      systemrueck(cmd,obverb,oblog,&rueck);
      for(size_t i=0;i<rueck.size();i++) {
        string stamm,exten;
        getstammext(&rueck[i],&stamm,&exten);
        string zugeh=stamm+".sff";
        if (lstat(zugeh.c_str(),&entryvz)) {
          string base=base_name(zugeh);
          RS inouta(My,string("select submid from `")+touta+"` where submid = '"+base+"'",ZDB);
          if (inouta.num_rows) {
            Log(blaus+Tx[T_Verwaiste_Datei]+violett+rueck[i]+schwarz+Tx[T_geloescht_Fax_schon_in]+violett+touta+schwarz+
                Tx[T_archiviert_Ausrufezeichen],1,1);
            remove(rueck[i].c_str());
            break;
          } else {
            // 31.1.16: ... und wenn diese sich nicht in outa findet ...
            string waisen = cfaxusersqvz+"/waisen";
            systemrueck(string("sudo mkdir -p ")+waisen);
            uint vfehler=0;
            verschiebe(rueck[i],waisen,&vfehler,1,obverb,oblog);
          } // if (inouta.num_rows) else 
        } // if (lstat(zugeh.c_str(),&entryvz)) 
      } // for(size_t i=0;i<rueck.size();i++) 
      cmd=string("find '")+cfaxusersqvz+"' -maxdepth 1 -type f -iname 'fax*.sff'"; //  -printf '%f\\n'";
      rueck.clear();
      systemrueck(cmd,obverb,oblog,&rueck);
      for(size_t i=0;i<rueck.size();i++) {
        uchar indb=0;
        char ***cerg;
        ZDB=0;
        RS rs(My,string("select id from `")+spooltab+"` where concat(capispoolpfad,'/',capispooldatei)='"+rueck[i]+"'",ZDB);
        if (cerg=rs.HolZeile(),cerg?*cerg:0) indb=1;
        if (!indb) {
          if (!obtitel) {
            ausg<<rot<<Tx[T_Weitere_Spool_Eintraege]<<schwarz;
            obtitel=1;
          }
          fsfcl fsf(rueck[i],wartend);
          fsf.capiwausgeb(&ausg, &capiconfp[2].wert, obverb, 0, oblog, ++faxord);
        }
      } // for(size_t i=0
    }
  }
  if (obhyla) {
    if (!lstat(hsendqvz.c_str(),&entryvz)) {
      cmd=string("find '")+hsendqvz+"' -maxdepth 1 -type f -iname 'q*' -printf '%f\\n'";
      rueck.clear();
      systemrueck(cmd,obverb,oblog,&rueck);
      for(size_t i=0;i<rueck.size();i++) {
        uchar indb=0;
        char ***cerg;
        RS rs(My,string("select id from `")+spooltab+"` where concat('q',hylanr)='"+rueck[i]+"'",ZDB);
        if (cerg=rs.HolZeile(),cerg?*cerg:0) indb=1;
        if (!indb) {
          if (!obtitel) {
            ausg<<rot<<Tx[T_Weitere_Spool_Eintraege]<<schwarz;
            obtitel=1;
          }
          fsfcl fsf(rueck[i]);
          string protdakt=hsendqvz+vtz+rueck[i];
          uchar hyla_uverz_nr=1;
          int obsfehlt=0;
          /*fsf.*/
          setzhylastat(&fsf, &protdakt, &hyla_uverz_nr, &obsfehlt, 2, obverb, oblog);
          fsf.hylaausgeb(&ausg, this, 0, obverb, 0, oblog);
        } // if (!indb)
      } // for(size_t i=0;i<rueck.size();i++) 
    } // if (!lstat(hsendqvz.c_str(),&entryvz))
  } // if (obhyla) 
  if (obtitel) Log(ausg.str(),1,oblog);
} // void zeigweitere(DB *My, paramcl *pmp, int obverb=0,int oblog=0)

// wird aufgerufen in: main
void paramcl::empfarch()
{
  Log(violetts+Tx[T_empfarch]+schwarz,obverb,oblog);
  // hyla
  string hempfavz=varsphylavz+"/autofaxarch"; // /var/spool/capisuite/empfarch/
  // Faxe in der Empfangswarteschlange auflisten, ...
  cmd=string("find \"")+varsphylavz+"/recvq\" -name \"fax*.tif\"";
  vector<string> rueck;
  systemrueck(cmd,obverb,oblog, &rueck);
  char tbuf[255];
  for(size_t i=0;i<rueck.size();i++) {
    if (!i) {
      cmd=string("sudo mkdir -p ")+hempfavz;
      systemrueck(cmd,obverb,oblog);
    }
    // ..., Informationen darueber einsammeln, ...
    string zeit;
    string absdr,tsid,callerid,devname;
    ulong seiten=0;
    string stamm,exten,ganz=rueck[i];
    getstammext(&ganz,&stamm,&exten);
    string base=base_name(stamm);
    string fnr=base.substr(3);
    fnr=fnr.substr(fnr.find_first_not_of("0"));
    vector<string> tok; // fuer imagedescription
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    struct stat entrylog;
    memset(&entrylog,0,sizeof entrylog);
    if (!lstat(rueck[i].c_str(),&entrylog))  {
      /*
         memcpy(&tm, localtime(&entrylog.st_mtime),sizeof(tm));
         char buf[255];
         strftime(buf, sizeof(buf), "%d.%m.%Y %H.%M.%S", &tm);
         <<"Buf: "<<buf<<endl;
       */
    }
    if (TIFF* tif = TIFFOpen(rueck[i].c_str(), "r")) {
      ankzahl++;
      char *rdesc=0;
      if (TIFFGetField(tif, TIFFTAG_DATETIME, &rdesc)) {
        // <<"Datetime: \n"<<rdesc<<endl;
        strptime(rdesc,"%Y:%m:%d %H:%M:%S",&tm);
        /*
           char buf[255];
           strftime(buf, sizeof(buf), "%d.%m.%Y %H.%M.%S", &tm);
           <<"Buf (2): "<<buf<<endl;
         */
      }
      seiten=TIFFNumberOfDirectories(tif);
      rdesc=0;
      if (TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &rdesc)) {
        //          printf("Beschreibung: %s\n",beschreib);
        //  out<<"Beschreibung: \n"<<rdesc<<endl;
        tok.clear();
        aufSplit(&tok,rdesc,'\n');
        if (tok.size()) {
          callerid=tok[0];
          if (tok.size()>1) {
            tsid=tok[1];
            if (tok.size()>2)
              absdr=tok[2];
          }
        }
      }
      // rdesc=0;
      // if (TIFFGetField(tif, TIFFTAG_MODEL, &rdesc))
      rdesc=0;
      if (TIFFGetField(tif, TIFFTAG_MAKE, &rdesc)) {
        //          printf("Beschreibung: %s\n",beschreib);
        devname=rdesc;
      }

      TIFFClose(tif);
    }

    strftime(tbuf, sizeof(tbuf), "%d.%m.%Y %H.%M.%S", &tm);
    if (absdr.length()>187) absdr.erase(187);
    string hdatei = "Fax h"+fnr+","+Tx[T_von]+absdr+", T."+tsid+", "+Tx[T_vom]+tbuf+".tif";
    string hpfad=empfvz+vtz+hdatei;
    Log(blaus+base+schwarz+" => "+hblau+hdatei+schwarz,1,1);
    // ..., die empfangene Datei in hpfad kopieren ...
    cmd=string("cp -ai \"")+rueck[i]+"\" \""+hpfad+"\"";
    systemrueck(cmd,obverb,oblog);
    struct stat entrynd;
    if (!lstat(hpfad.c_str(),&entrynd)) {
      cmd=string("chmod 777 \"")+hpfad+"\"";
      systemrueck(cmd,obverb,oblog);
      cmd=string("mv -i \"")+rueck[i]+"\" \""+hempfavz+"\"";
      systemrueck(cmd,obverb,oblog);
      RS zs(My);
      // ... und falls erfolgreich in der Datenbanktabelle inca eintragen
      for(int runde=0;runde<2;runde++) {
        if (runde==0) { zs.Abfrage("SET NAMES 'utf8'");
        } else if (runde==1) zs.Abfrage("SET NAMES 'latin1'");
        RS rins(My); 
        vector<instyp> einf; // fuer alle Datenbankeinfuegungen
        einf.push_back(instyp(My->DBS,"fsize",entrylog.st_size));
        einf.push_back(instyp(My->DBS,"pages",seiten));
        einf.push_back(instyp(My->DBS,"titel",&absdr));
        einf.push_back(instyp(My->DBS,"tsid",&tsid));
        einf.push_back(instyp(My->DBS,"callerid",&callerid));
        einf.push_back(instyp(My->DBS,"devname",&callerid));
        einf.push_back(instyp(My->DBS,"id",&base));
        einf.push_back(instyp(My->DBS,"transe",&tm));
        rins.insert(tinca,einf, 1,0,ZDB?ZDB:!runde); 
        if (runde==1) zs.Abfrage("SET NAMES 'utf8'");
        if (!rins.fnr) break;
        if (runde==1) {
          Log(string(Tx[T_Fehler_af])+drot+ltoan(rins.fnr)+schwarz+Tx[T_bei]+tuerkis+rins.sql+schwarz+": "+blau+rins.fehler+schwarz,1,oblog);
          exit(0);
        }
      } // for(int runde=0;runde<2;runde++) 
    } // if (!lstat(hpfad.c_str(),&entrynd)) 
  } // for(size_t i=0;i<rueck.size();i++) 

  // capi
  int erg;
  string cempfavz=spoolcapivz+"/autofaxarch"; // /var/spool/capisuite/empfarch/
  cppSchluess umst[]={{"filename"},{"call_from"},{"call_to"},{"time"},{"cause"}};
  size_t cs=sizeof umst/sizeof*umst;
  struct stat entryvz;
  if (!lstat(cfaxuserrcvz.c_str(),&entryvz)) /* /var/spool/capisuite/users/~/received */ {
    // Faxe in der Empfangswarteschlange auflisten, ...
    cmd=string("find \"")+cfaxuserrcvz+"\" -maxdepth 1 -type f -iname \"*.txt\"";
    vector<string> rueck;
    systemrueck(cmd,obverb,oblog, &rueck);
    for(size_t i=0;i<rueck.size();i++) {
      if (!i) {
        cmd=string("sudo mkdir -p ")+cempfavz;
        systemrueck(cmd,obverb,oblog);
      }
      ankzahl++;
      // ..., Informationen darueber einsammeln, ...
      string stamm,exten;
      getstammext(&(rueck.at(i)),&stamm,&exten);
      Log(string("txt: ")+tuerkis+(rueck.at(i))+schwarz,obverb,oblog);
      string sffname=stamm+".sff";
      struct stat entrysff;
      uchar verschieb=0;
      confdat empfconf(rueck.at(i),umst,cs,obverb);
      //    if (cpplies(rueck.at(i),umst,cs)) KLA
      struct tm tm;
      memset(&tm, 0, sizeof(struct tm));
      strptime(umst[3].wert.c_str(), "%a %b %d %H:%M:%S %Y", &tm);
      strftime(tbuf, sizeof(tbuf), "%d.%m.%Y %H.%M.%S", &tm);
      Log(rots+"   "+umst[0].name+": "+schwarz+umst[0].wert,obverb,oblog);
      Log(rots+"   "+umst[1].name+": "+schwarz+umst[1].wert,obverb,oblog);
      Log(rots+"   "+umst[2].name+": "+schwarz+umst[2].wert,obverb,oblog);
      Log(rots+"   "+umst[3].name+": "+schwarz+tbuf,obverb,oblog);
      Log(rots+"   "+umst[4].name+": "+schwarz+umst[4].wert,obverb,oblog);

      string base=base_name(stamm);
      string fnr=base.substr(4);
      tm.tm_isdst=-1; // sonst wird zufaellig ab und zu eine Stunde abgezogen
      time_t modz=mktime(&tm);
      string getname="",bsname="";
      if (!(umst[1].wert.empty())) {
        getSender(this,umst[1].wert,&getname,&bsname);
      }
      getname+=", ";
      getname+=bsname;
      if (getname.length()>187) getname.erase(187);
      string cdatei = "Fax c"+fnr+","+Tx[T_von]+getname+", T."+umst[1].wert+","+Tx[T_vom]+tbuf+".tif";
      string cpfad= empfvz + vtz+cdatei; // Tx[T_Fax_von]+umst[1].wert+Tx[T_an]+umst[2].wert+Tx[T_vom]+tbuf+".tif";
      Log(blaus+stamm+schwarz+" => "+hblau+cdatei+schwarz,1,1);
      // ..., die empfangene Datei in hpfad kopieren ...
      if (!lstat(sffname.c_str(),&entrysff)) {
        if (entrysff.st_size) {
          cmd=string("sfftobmp -f -t ")+stamm+".sff -o \""+cpfad+"\"";
          if (!(erg=systemrueck(cmd,obverb,oblog))) {
            struct utimbuf ubuf;
            ubuf.modtime = modz;
            ubuf.actime = modz;
            if (utime(cpfad.c_str(),&ubuf)) {
              Log(rots+Tx[T_Fehler_beim_Datumsetzen_von]+cpfad+rot+"'"+schwarz,1,1);
            } else {
              cmd=string("mv -i \"")+stamm+".sff\" \""+cempfavz+vtz+cuser+"-"+base+".sff\"";
              systemrueck(cmd,obverb,oblog);
              cmd=string("mv -i \"")+(rueck.at(i))+"\" \""+cempfavz+vtz+cuser+"-"+(base_name(rueck[i]))+"\"";
              systemrueck(cmd,obverb,oblog);
            } // if (utime(cpfad.c_str(),&ubuf))  else
          } else {// if (!(erg=systemrueck(cmd,obverb,oblog)))
            if (loee) {
              Log(string(Tx[T_Dateien])+rot+stamm+".* "+schwarz+Tx[T_nicht_verarbeitbar_Verschiebe_sie_nach]+rot+"./falsche"+schwarz+".",1,1);
              verschieb=2;
            }
          }
        } else {
          // empfangenes Fax mit 0 Bytes, vermutlich abgefangen von anderem System, samt Textdatei nach 'falsche' verschieben
          verschieb=2;
        }
      } else {
        // .txt nach falsche verschieben
        verschieb=1;
      }
      if (verschieb) {
        string falsche = cfaxuserrcvz+"/falsche";
        systemrueck(string("sudo mkdir -p ")+falsche);
        uint vfehler=0;
        verschiebe(rueck.at(i),falsche,&vfehler,1,obverb,oblog);
        if (verschieb==2) {
          verschiebe(sffname,falsche,&vfehler,1,obverb,oblog);
        }
      } // if (verschieb) 
      RS zs(My);
      for(int runde=0;runde<2;runde++) {
        if (runde==0) { zs.Abfrage("SET NAMES 'utf8'");
        } else if (runde==1) zs.Abfrage("SET NAMES 'latin1'");
        RS rins(My); 
        vector<instyp> einf; // fuer alle Datenbankeinfuegungen
        einf.push_back(instyp(My->DBS,"titel",getname+", "+bsname));
        einf.push_back(instyp(My->DBS,"tsid",&umst[1].wert));
        einf.push_back(instyp(My->DBS,"transe",&modz));
        einf.push_back(instyp(My->DBS,"id",&base));
        einf.push_back(instyp(My->DBS,"fsize",entrysff.st_size));
        einf.push_back(instyp(My->DBS,"csid",&umst[2].wert));
        rins.insert(tinca,einf, 1,0,ZDB?ZDB:!runde); 
        if (runde==1) zs.Abfrage("SET NAMES 'utf8'");
        if (!rins.fnr) break;
        if (runde==1) {
          Log(string(Tx[T_Fehler_af])+drot+ltoan(rins.fnr)+schwarz+Tx[T_bei]+tuerkis+rins.sql+schwarz+": "+blau+rins.fehler+schwarz,1,oblog);
          exit(0);
        }
      }
    } // for(size_t i=0;i<rueck.size();i++) 
  }

} // void paramcl::empfarch()

// wird aufgerufen in: main
void paramcl::schlussanzeige(const char* const progname)
{
  Log(violetts+Tx[T_schlussanzeige]+schwarz,obverb,oblog);
  tende = clock();
  //  ausg.str(std::string()); ausg.clear();
  //  ausg<<schwarz<<"Pfad: "<<tuerkis<<zufaxenvz<<schwarz<<"; Dateien: "<<tuerkis<<geszahl<<schwarz<<"; Zeit: "<<drot<<setprecision(7)<<fixed<<((tende-tstart)/CLOCKS_PER_SEC)<<schwarz<<setprecision(0)<<" s";
  if (obverb>0) {
    stringstream ausg;
    ausg<<Tx[T_Zeit_Doppelpunkt]<<drot<<setprecision(7)<<fixed<<((tende-tstart)/CLOCKS_PER_SEC)<<schwarz<<setprecision(0)<<" s";
    Log(ausg.str(),1,oblog); 
  }
  Log(string(Tx[T_Zahl_der_empfangenen_Faxe])+drot+ltoan(ankzahl)+schwarz,1,oblog);
  Log(string(Tx[T_Zahl_der_ueberpruefen_Datenbankeintraege])+drot+ltoan(dbzahl)+schwarz,1,oblog);
  Log(string(Tx[T_davon_gescheiterte_Faxe])+drot+ltoan(gzahl)+schwarz,1,oblog);
  Log(string(Tx[T_davon_erfolgreiche_Faxe])+drot+ltoan(ezahl)+schwarz,1,oblog);
  Log(string(Tx[T_davon_noch_wartende_Faxe])+drot+ltoan(wzahl)+schwarz,1,oblog);
  Log(string(Tx[T_davon_nicht_in_Warteschlange])+drot+ltoan(fzahl)+schwarz,1,oblog);
  Log(string(Tx[T_Fertig_mit])+blau+progname+schwarz+" !",obverb,oblog);
} // paramcl::schlussanzeige


// ermittelt fuer eine in ein Zielverzeichnis zu kopierende Datei den dortigen Namen, falls gewuenscht unter Beruecksichtigung 
// dort evtl. schon vorhandener Dateien
// wird aufgerufen von verschiebe (Version 1) und kopiere (Version 1), neuerdateiname, zielname (Version 2)
string zielname(const string& qdatei, const string& zielverz, uchar wieweiterzaehl=0, string* zieldatei=0, int obverb=0, int oblog=0)
{
  //  Log(violetts+Tx[T_zielname]+schwarz,obverb,oblog);
  // wieweiterzaehl: 0: auf *_1_1 nach *_1, 1: auf *_2 nach *_1, 2: gar nicht
  struct stat entryziel;
  string dateiname=base_name(qdatei);
  string ziel = zielverz + (zielverz[zielverz.length()-1]==vtz?"":vtzs)+ dateiname;
  Log(string(Tx[T_zielname_erstes_Ziel])+rot+ziel+schwarz+"'",obverb,oblog);
  if (wieweiterzaehl<2) {
    unsigned long ausweich=0;
    string stamm,exten;
    getstammext(&dateiname,&stamm,&exten);
    // wenn von einer vorhandenen Ausweichnummerierung aus weitergezaehlt werden soll (anstatt dateiname_1_1 ...)
    if (wieweiterzaehl) {
      size_t pos = stamm.rfind('_');
      if (pos!=string::npos) {
        string auswstr=stamm.substr(pos+1);
        if (isnumeric(auswstr)) { 
          ausweich=atol(auswstr.c_str());
          if (ausweich) if (stamm.length()>pos) stamm.erase(pos);
        }
      } // if (pos!=string::npos) 
    } // if (wieweiterzaehl) 
    uchar ausgewichen=0;
    for(;1;) {
      if (lstat(ziel.c_str(), &entryziel)) break;
      ausweich++;
      ausgewichen=1;
      dateiname=stamm+"_"+ltoan(ausweich)+"."+exten;
      ziel=zielverz + (zielverz[zielverz.length()-1]==vtz?"":vtzs) + dateiname;
    } // for(;1;) 
    if (ausgewichen) Log(string(Tx[T_zielname_Ausweichziel])+rot+ziel+schwarz+"'",obverb,oblog);
  } // if (wieweiterzaehl<2) 
  if (zieldatei) *zieldatei=dateiname;
  return ziel;
} // zielname 

// wird aufgerufen in: verschiebe (Version 2), kopiere (Version 2), neuerdateiname, 
string zielname(const string& qdatei, zielmustercl *zmp, uchar wieweiterzaehl=0, string* zieldatei=0, int obverb=0, int oblog=0)
{
  //  Log(violetts+Tx[T_zielname]+schwarz,obverb,oblog);
  for(zielmustercl *zmakt=zmp;1;zmakt++){
    int reti=regexec(&(zmakt->regex),qdatei.c_str(),0,NULL,0);
    Log(string(Tx[T_Datei])+rot+qdatei+schwarz+Tx[T_entspricht]+(reti?Tx[T_entsprichtnicht]:Tx[T_entsprichtdoch])+
        Tx[T_Muster_Doppelpunkt]+rot+zmakt->holmuster()+schwarz+"'",obverb,oblog);
    if (!reti){
      return zielname(qdatei,zmakt->ziel,wieweiterzaehl,zieldatei,obverb,oblog);
    }
    if (zmakt->obmusterleer()) break;
  }
  return "";
} // zielname


// wird aufgerufen in: verschiebe (Version 1), verschiebe (Version 2), DateienHerricht
void dorename(const string& quelle, const string& ziel, uint *vfehler, int obverb, int oblog)
{
  Log(string(Tx[T_Verschiebe])+rot+quelle+schwarz+"'\n         -> '"+rot+ziel+schwarz+"'",obverb,oblog);
  if (rename(quelle.c_str(),ziel.c_str())) {
    perror(Tx[T_Fehler_beim_Verschieben]);
    string cmd=string("mv \"")+quelle+"\" \""+ziel+"\"";
    if (vfehler) *vfehler+=systemrueck(cmd,obverb,1);
  }
} // dorename


// wird aufgerufen von Dateienherricht und untersuchespool; Vorsicht, wenn qdateip ein Verzeichnisname ist!
string verschiebe(const string& qdatei, const string& zielvz, uint *vfehler, uchar wieweiterzaehl, int obverb=0,int oblog=0)
{
  // wieweiterzaehl: 0: auf *_1_1 nach *_1, 1: auf *_2 nach *_1, 2: gar nicht
  string ziel=zielname(qdatei,zielvz,wieweiterzaehl,0,obverb,oblog);
  if (!ziel.empty()) dorename(qdatei,ziel,vfehler,obverb,oblog);
  return ziel;
} // string verschiebe

// wird aufgerufen von untersuchespool; Vorsicht, wenn qdateip ein Verzeichnisname ist!
void verschiebe(const string& qdatei, zielmustercl *zmp, uint *vfehler, uchar wieweiterzaehl, int obverb=0, int oblog=0) 
{
  // wieweiterzaehl: 0: auf *_1_1 nach *_1, 1: auf *_2 nach *_1, 2: gar nicht
  string ziel=zielname(qdatei,zmp,wieweiterzaehl,0,obverb,oblog);
  if (!ziel.empty()) dorename(qdatei,ziel,vfehler,obverb,oblog);
} // verschiebe

// wird aufgerufen in Dateienherricht
string kopiere(const string& qdatei, const string& zield, uint *kfehler, uchar wieweiterzaehl, int obverb, int oblog)
{
  // wieweiterzaehl: 0: auf *_1_1 nach *_1, 1: auf *_2 nach *_1, 2: gar nicht
  string dir=dir_name(zield);
  string base=base_name(zield);
  string ziel;
  if (!base.empty() && !dir.empty()) {
    ziel=zielname(base,dir,wieweiterzaehl,0, obverb,oblog);
    Log(string(Tx[T_Kopiere_Doppelpunkt])+rot+qdatei+schwarz+"'\n         -> '"+rot+ziel+schwarz+"'",obverb,oblog);
    string cmd=string("cp -a \"")+qdatei+"\" \""+ziel+"\"";
    kfehler+= systemrueck(cmd,obverb>1,oblog);
  } else {
    cerr<<rot<<Tx[T_Fehler_beim_Kopieren]<<Tx[T_Dateiname]<<blau<<zield<<schwarz<<Tx[T_schlechtgeformt]<<endl;
  }
  return ziel;
} // string kopiere

// wird aufgerufen in Dateienherricht
void kopiere(const string& qdatei, zielmustercl *zmp, uint *kfehler, uchar wieweiterzaehl, int obverb, int oblog) 
{
  // wieweiterzaehl: 0: auf *_1_1 nach *_1, 1: auf *_2 nach *_1, 2: gar nicht
  string ziel=zielname(qdatei,zmp,wieweiterzaehl,0, obverb,oblog);
  if (!ziel.empty()) {
    Log(string(Tx[T_Kopiere_Doppelpunkt])+rot+qdatei+schwarz+"'\n         -> '"+rot+ziel+schwarz+"'",obverb,oblog);
    string cmd=string("cp -a \"")+qdatei+"\" \""+ziel+"\"";
    kfehler+= systemrueck(cmd,obverb>1,oblog);
  }
} // string kopiere

// wird aufgerufen in pruefhyla
void hfaxsetup(paramcl *pmp,int obverb=0, int oblog=0)
{
  Log(violetts+"hfaxsetup()"+schwarz,obverb,oblog);
  struct stat entrybuf;
  const char *faxsu="/usr/sbin/faxsetup";
  if (!lstat(faxsu, &entrybuf)) {
#ifdef autofaxsetup
    // das Skript faxsetup soll leicht veraendert als 'autofaxsetup' abgespeichert und dann aufgerufen werden
    const char *afaxsu="/usr/local/sbin/autofaxsetup";
    ifstream alt(faxsu);
    if (alt.is_open()) {
      ofstream neu(afaxsu);
      if (neu.is_open()) {
        string zeile, ergzeile;
        size_t znr=0,promptz=0;
        string vorgabe;
        while(getline(alt,zeile)) {
          size_t p0,p1,kla,klz,einr=0;
          uchar zukomm=0;
          if (zeile.find("INTERACTIVE=yes")!=string::npos) {
            zeile="INTERACTIVE=no" # Aenderung G.Schade 25.5.16;
          } else if (zeile.find("ok=prompt")!=string::npos) {
            // Interaktivitaet streichen
            zeile="     ok=skip    # prompt for parameters # Aenderung G.Schade 1.1.16";
          } else if (!zeile.find("PATH_EGETTY=")) {
            // suche das aktuelle Fax-Empfangsprogramm; 'which faxgetty' geht nicht immer, da z.B. /usr/lib/fax/ nicht im Standard-Befehlssuchpfad,
            // aber unter Opensuse 42.1 der Vorgabeort fuer faxgetty
            zeile="PATH_EGETTY=$(find /usr/lib/fax /usr/sbin /usr/bin /root/bin /sbin -perm /111 -name faxgetty) "
              "# /bin/egetty # pathname for external getty program # Aenderung G.Schade 29.2./ 23.3.16";
          } else if (zeile.find("$MV $DIR_LIBDATA/hyla.conf.tmp $DIR_LIBDATA/hyla.conf")!=string::npos) {
            // wenn zuvor keine der Veraenderungen von hyla.conf erfolgt, dann gibt es auch hyla.conf.tmp nicht;
            // dies soll ohne Fehlermeldung gehen
            zeile=" test -f \"$DIR_LIBDATA/hyla.conf.tmp\" && "+zeile;
          } else {
            if (zeile[0]!='#' && (p0=zeile.find("prompt "))!=string::npos) {
              einr=p0+2;
              if ((kla=zeile.find('[',p0))!=string::npos) if ((klz=zeile.find(']',kla))!=string::npos) {
                // faxaddmodem soll hier nicht aufgerufen werden
                promptz=znr; 
                if (zeile.find("run faxaddmodem")==string::npos) 
                  vorgabe=zeile.substr(kla+1,klz-kla-1);
                else
                  vorgabe="no";
              }
              zukomm=1;
            }
            // wenn in der oder der naechsten Zeile read steht
            // (z.B. in den Funktionen promptFor...parameter()), dann soll ohne Rueckfrage die Vorgabe verwendet werden
            if ((znr==promptz || znr==promptz+1) && zeile[0]!='#' && (p1=zeile.find("read "))!=string::npos) {
              ergzeile=string(einr,' ')+zeile.substr(p1+5);
              if ((p1=ergzeile.find(' ',einr))!=string::npos) ergzeile=ergzeile.substr(0,p1);
              ergzeile=ergzeile+"=\""+vorgabe+"\" # G.Schade 1.1.16";
              zukomm=1;
            }
            if (zukomm) zeile=string("# ")+zeile+" # Kommentar G.Schade 1.1.16";
          } // if (zeile.find("INTERACTIVE=yes")!=string::npos) else else else else
          neu<<zeile<<endl;
          if (!ergzeile.empty()) {
            neu<<ergzeile<<endl;
            ergzeile.clear();
          }
          znr++;
        } // while(getline(alt,zeile)) 
        neu.close();
        systemrueck(string("chmod 770 '")+afaxsu+"'",0,1);
        systemrueck(string("systemctl stop ")+pmp->sfaxgetty->sname+" "+pmp->shfaxd->sname+" "+pmp->sfaxq->sname,obverb,oblog);
        systemrueck(string("killall ")+pmp->sfaxgetty->ename+" "+pmp->shfaxd->ename+" "+pmp->sfaxq->ename,obverb,oblog);
        Log(blaus+Tx[T_Fuehre_aus_Dp]+schwarz+afaxsu+blau+Tx[T_falls_es_hier_haengt_bitte_erneut_aufrufen]+schwarz,1,oblog);
        system((string("/usr/bin/sh ")+afaxsu+(obverb?" -verbose":"")).c_str()); 
        //        systemrueck(string("source ")+afaxsu+(obverb?" -verbose":""),obverb,oblog,0,falsch); // haengt am Schluss, geht nicht mit unbuffer, unbuffer /usr/local/sbin/autofaxsetup -verbose, loeschen von exit 0 am schluss, exec, stty -echo -onlcr usw., nohup,
        Log(blaus+Tx[T_Fertig_mit]+schwarz+afaxsu,1,oblog);
        systemrueck(string("systemctl daemon-reload"),0,1);
        //        systemrueck(string("rm ")+afaxsu,1,1);
      }
    }
#else
    systemrueck(string("systemctl stop ")+pmp->sfaxgetty->sname+" "+pmp->shfaxd->sname+" "+pmp->sfaxq->sname,obverb,oblog);
    systemrueck(string("killall ")+pmp->sfaxgetty->ename+" "+pmp->shfaxd->ename+" "+pmp->sfaxq->ename,obverb,oblog);
    Log(blaus+Tx[T_Fuehre_aus_Dp]+schwarz+faxsu+" -nointeractive"+blau+Tx[T_falls_es_hier_haengt_bitte_erneut_aufrufen]+schwarz,1,oblog);
    system((string("/usr/bin/sh ")+faxsu+" -nointeractive"+(obverb?" -verbose":"")).c_str()); 
    // systemrueck(string("source ")+faxsu+(obverb?" -verbose":""),obverb,oblog,0,falsch); // haengt am Schluss, geht nicht 
    // mit unbuffer, unbuffer /usr/local/sbin/autofaxsetup -verbose, loeschen von exit 0 am schluss, exec, stty -echo -onlcr usw., nohup,
    Log(blaus+Tx[T_Fertig_mit]+schwarz+faxsu,1,oblog);
    systemrueck(string("systemctl daemon-reload"),0,1);

#endif
  } //   if (!lstat(faxsu, &entrybuf)) KLA

  svec rueckf;
  struct stat entryfaxgt;
  pmp->faxgtpfad.clear();
  //  pmp->faxgtpfad="/usr/lib/fax/faxgetty";
  //    pmp->faxgtpfad="/usr/sbin/faxgetty";
  if (!systemrueck("which faxgetty",obverb-1,oblog,&rueckf)) {
    if (rueckf.size()) {
      pmp->faxgtpfad=rueckf[0];
    }
  }
  if (pmp->faxgtpfad.empty() || lstat(pmp->faxgtpfad.c_str(),&entryfaxgt)) {
    rueckf.clear();
    pmp->faxgtpfad.clear();
    if (!systemrueck("find /usr/lib/fax /usr/sbin /usr/bin /root/bin /sbin -perm /111 -name faxgetty",obverb,oblog,&rueckf)) {
      if (rueckf.size()) 
        pmp->faxgtpfad=rueckf[0];
    }
  }
  // violett<<"pmp->faxgtpfad 4: "<<pmp->faxgtpfad<<schwarz<<endl;
} // hfaxsetup

// wird aufgerufen in: pruefhyla
void hconfig(paramcl *pmp,int obverb=0, int oblog=0)
{
  string confd=pmp->varsphylavz+"/etc/config";
  ofstream conf(confd.c_str());
  if (conf.is_open()) {
    conf<<"LogFacility:    daemon"<<endl;
    conf<<"CountryCode:    "<<pmp->countrycode<<endl;
    conf<<"AreaCode:   "<<pmp->citycode<<endl;
    conf<<"LongDistancePrefix: "<<pmp->LongDistancePrefix<<endl;
    conf<<"InternationalPrefix:  "<<pmp->InternationalPrefix<<endl;
    conf<<"DialStringRules:  \"etc/dialrules\""<<endl;
    conf<<"ServerTracing:    1"<<endl;
    conf<<"SessionTracing:   0xffffffff"<<endl;
    {
      struct stat entryfaxsd;
      svec rueckf;
      string faxsdpfad=pmp->varsphylavz+"/bin/faxsend";
      if (lstat(faxsdpfad.c_str(),&entryfaxsd)) {
        faxsdpfad="/usr/sbin/faxsend";
        if (lstat(faxsdpfad.c_str(),&entryfaxsd)) {
          faxsdpfad.clear();
          if (systemrueck("find /usr /root/bin /sbin -perm /111 -name faxsend",obverb,oblog,&rueckf)) {
            if (rueckf.size()) 
              faxsdpfad=rueckf[0];
          }
        }
      }
      if (!faxsdpfad.empty())
        conf<<"SendFaxCmd:   "<<faxsdpfad<<endl;
    }
    conf<<"Use2D:      \"no\""<<endl;
  } else {
    cerr<<Tx[T_Datei]<<confd<<Tx[T_nichtbeschreibbar]<<endl;
    exit(0);
  }
} // void hconfig(paramcl *pmp,int obverb=0, int oblog=0)


// wird aufgerufen in: pruefhyla (2x)
void hconfigtty(paramcl *pmp,int obverb=0, int oblog=0)
{
  string modconfdat;
  modconfdat=pmp->varsphylavz+"/etc/config"+"."+pmp->hmodem; 
  ofstream hci(modconfdat.c_str());
  if (hci.is_open()) {
    time_t tim=time(0);
    struct tm *tm=localtime(&tim);
    char buf[80];
    strftime(buf, sizeof(buf), "%d.%m.%y %H:%M:%S", tm);
    hci<<"# Konfiguration von hylafax durch autofax vom "<<buf<<endl;
    hci<<"CountryCode:    "<<pmp->countrycode<<endl;
    hci<<"AreaCode:   "<<pmp->citycode<<endl;
    hci<<"FAXNumber:    \""<<pmp->countrycode<<"."<<pmp->citycode<<"."<<pmp->msn<<"\""<<endl;
    hci<<"LongDistancePrefix: "<<pmp->LongDistancePrefix<<endl;
    hci<<"InternationalPrefix:  "<<pmp->InternationalPrefix<<endl;
    hci<<"DialStringRules:  \"etc/dialrules\""<<endl;
    hci<<"ServerTracing:    0xFFFFF"<<endl;
    hci<<"SessionTracing:   0xFFFFF"<<endl;
    hci<<"RecvFileMode:   0600"<<endl;
    hci<<"LogFileMode:    0600"<<endl;
    hci<<"DeviceMode:   0600"<<endl;
    hci<<"RingsBeforeAnswer:  "<<pmp->hklingelzahl<<" #muss mindestens 2 sein zur Uebergabe der Nummer des Anrufenden"<<endl;
    hci<<"CIDNumber: NMBR="<<endl;
    hci<<"CIDName:   NAME="<<endl;
    hci<<"SpeakerVolume:    off"<<endl;
    hci<<"GettyArgs:    \"-h %l dx_%s\""<<endl;
    hci<<"LocalIdentifier:  "<<pmp->LocalIdentifier<<endl;
    hci<<"LogFacility:    daemon"<<endl;
#ifdef obhp
    hci<<"TagLineFont:    etc/LiberationSans-25.pcf"<<endl;
#else
    hci<<"TagLineFont:    etc/lutRS18.pcf"<<endl;
#endif
    hci<<"TagLineFormat:    \"Von %%l|%c|Seite %%P of %%T\""<<endl;
    hci<<"AdaptiveAnswer:   yes"<<endl;
    hci<<"AnswerRotary:   \"voice fax data\""<<endl;
    hci<<"MaxRecvPages:   100"<<endl;
    hci<<"JobReqBusy:   120"<<endl;
    hci<<"# Modem-related stuff: should reflect modem command interface"<<endl;
    hci<<"# and hardware connection/cabling (e.g. flow control)."<<endl;
    hci<<"ModemType:    Class1    # use class 1 interface"<<endl;
    hci<<"ModemRate:    38400   # rate for DCE-DTE communication"<<endl;
    hci<<"ModemFlowControl:  rtscts # hardware flow control # xonxoff   # software flow control"<<endl;
    hci<<"ModemSetupDTRCmd: ATS13=1&D3 # ATS13=1&D2 # setup so DTR drop resets modem"<<endl;
    hci<<"ModemSetupDCDCmd: AT&C1   # setup so DCD reflects carrier (or not)"<<endl;
    hci<<"ModemNoFlowCmd:   AT&H0 # AT&H0&I0&R1 # setup modem for no flow control"<<endl;
    hci<<"ModemHardFlowCmd: AT&H1 # AT&H1&I0&R2 # setup modem for hardware flow control"<<endl;
    hci<<"ModemSoftFlowCmd: AT&H2 # AT&H2&I2&R1 # setup modem for software flow control"<<endl;

    hci<<"ModemResultCodesCmd:  ATQ0X4 #CID=1    # enable result codes"<<endl;
    hci<<"ModemMfrQueryCmd: !USR5637 # !USR"<<endl;
    hci<<"ModemModelQueryCmd: ATI3"<<endl;
    hci<<"ModemRevQueryCmd: ATI7    # XXX returns a multi-line result"<<endl;
    hci<<"# When AT+FCLASS=1 is issued the modem automatically switches"<<endl;
    hci<<"# to software flow control; these parameters let the fax software"<<endl;
    hci<<"# reset flow control as needed after entering Class 1."<<endl;
    hci<<"Class1NFLOCmd:    AT+FLO=0 # AT&H0&I0&R1  # setup modem for no flow control"<<endl;
    hci<<"Class1HFLOCmd:    AT+FLO=2 # AT&H1&I0&R2  # setup modem for hardware flow control"<<endl;
    hci<<"Class1SFLOCmd:    AT+FLO=1 # ""   # modem does this automatically"<<endl;
    hci<<"# This should resolve \"DIS/DTC received 3 times\" errors:"<<endl;
    hci<<"Class1ResponseWaitCmd:  AT+FRS=1  # wait after sending TCF for response"<<endl;
    hci<<"# The remainder of this configuration is included so that the"<<endl;
    hci<<"# modem \"idles\" in Class 0 while not sending or receiving facsimile."<<endl;
    hci<<"ModemSetupAACmd:  AT+FCLASS=0 # leave modem idling in class 0"<<endl;
    hci<<"ModemAnswerCmd:   AT+FCLASS=1A  # answer in Class 1"<<endl;
    hci<<" die folgenden sind aus dem Internet"<<endl;
    hci<<"ModemResetCmds:   ATS19=255"<<endl;
    hci<<"ModemAnswerFaxCmd:  ATS27=8S56=0+FCLASS=1;A"<<endl;
    hci<<"ModemAnswerDataCmd: ATS27=1S56=16+FCLASS=0;A"<<endl;
    hci<<"Class1RecvIdentTimer: 10000"<<endl;
    hci<<""<<endl;
    hci<<"# When using AT+FRS=n we see USR modems reset themselves in the middle of sessions"<<endl;
    hci<<"# this is not good.  So, we seem to work-around that problem by not using the"<<endl;
    hci<<"# command.  Unfortunately, this isn't an ideal thing."<<endl;
    hci<<"Class1SwitchingCmd: \"<delay\0727>\""<<endl;
    hci<<"# FaxRcvdCmd: ./schreibe.sh"<<endl;
  } else {
    cerr<<Tx[T_Datei]<<modconfdat<<Tx[T_nichtbeschreibbar]<<endl;
    exit(0);
  }
} // void hconfigtty(paramcl *pmp,int obverb=0, int oblog=0)


// wird aufgerufen in: pruefhyla (2x)
int hservice_faxq_hfaxd(paramcl *pmp,int obverb=0, int oblog=0)
{
  int hylafehler=0;
  Log(violetts+"hservice_faxq_hfaxd()"+schwarz,obverb,oblog);
  hylafehler+=!pmp->sfaxq->spruef("Faxq",0/*1*/,"/usr/sbin/faxq -D",
      pmp->varsphylavz+"/etc/setup.cache", pmp->shfaxd->sname, "",pmp->obverb,pmp->oblog);
  hylafehler+=!pmp->shfaxd->spruef("HFaxd",0/*1*/,"/usr/lib/fax/hfaxd -i hylafax -s 444",
      pmp->varsphylavz+"/etc/setup.cache", "", "", pmp->obverb,pmp->oblog);
  return hylafehler;

} // void hservice_faxq_hfaxd(paramcl *pmp,int obverb=0, int oblog=0)

// wird aufgerufen in: pruefhyla (1x)
int hservice_faxgetty(paramcl *pmp,int obverb=0, int oblog=0)
{
  obverb=1;
  Log(violetts+"hservice_faxgetty()"+schwarz,obverb,oblog);
  return !pmp->sfaxgetty->spruef(("HylaFAX faxgetty for ")+pmp->hmodem,0,
      pmp->faxgtpfad+" "+pmp->hmodem,"","","",pmp->obverb,pmp->oblog);
  // /etc/inittab werde von systemd nicht gelesen
  /*,"cat /etc/inittab 2>/dev/null | grep -E '^[^#].*respawn.*faxgetty' >/dev/null 2>&1"*/ 
} // void hservice_faxgetty(paramcl *pmp,int obverb=0, int oblog=0)

// wird aufgerufen in main
int paramcl::pruefhyla()
{
  Log(violetts+Tx[T_pruefhyla]+schwarz,obverb?obverb:obverb,oblog);
  long br=0; // baud rate
  string brs; // Baud-Rate-String
  int hylalaeuftnicht=0;
  static uchar hylafehlt=1;
  uchar falscheshyla=0;
  uchar modemlaeuftnicht=1;
  uchar frischkonfiguriert=0;
  vector<string> ruecki;
  // Baud rate ermitteln ...
  systemrueck(("stty -F /dev/")+this->hmodem+"| head -n 1 | cut -f2 -d' '",obverb,oblog,&ruecki);
  if (ruecki.size()>0) {
    brs=ruecki[0];
    br=atol(brs.c_str());
  }
  // .. und anzeigen
  if (br<=0) {
    Log(string(Tx[T_Baudratevon])+blau+"/dev/"+this->hmodem+schwarz+Tx[T_mit_af]+rot+brs+schwarz+Tx[T_zugeringVerwendeHylafaxnicht],1,1);
    //    return 1;
  } else {
    Log(string("Modem '")+blau+"/dev/"+this->hmodem+schwarz+Tx[T_mit_Baudrate]+violett+brs+schwarz+Tx[T_wird_verwendet],obverb,oblog);
  }
  if (!this->sfaxgetty) this->sfaxgetty=new servc("hylafax-faxgetty-"+this->hmodem,"faxgetty");
  for(unsigned versuch=0;versuch<2;versuch++) {
    //    hstart=clock();
    //    hylalaeuftnicht= !(PIDausName("hfaxd")>=0);
    //    hende=clock();
    //    <<"Zeit fuer pidausname: "<<(hende-hstart)<<endl;
    //    if (systemrueck("systemctl status hylafax | grep ' active (running)' >/dev/null 2>&1",obverb,oblog)) hylalaeuftnicht=1;
    //    if (hylalaeuft) hylalaeuft= !systemrueck("systemctl is-active hylafax",obverb,oblog);
    // pruefen, ob hylafax.service laeuft
    Log(Tx[T_Pruefe_ob_Hylafax_gestartet],-1,oblog);
    if (!this->sfaxq->obslaeuft(obverb-1,oblog) || !this->shfaxd->obslaeuft(obverb-1,oblog)) {
      hylalaeuftnicht=1;
      hylafehlt=1;

      // falls nein, dann schauen, ob installiert
#ifdef obhp      
      char *hfr=(char*)"hylafax+", *hfcr=(char*)"hylafax+-client", *hff=(char*)"hylafax", *hfcf=(char*)"hylafax-client";
      string hfftext=Tx[T_Hylafax_ohne_plus_entdeckt_muss_ich_loeschen];
#else
      char *hfr="hylafax", *hfcr="hylafax-client", *hff="hylafax+", *hfcf="hylafax+-client";
      string hfftext=Tx[T_Hylafaxplus_entdeckt_muss_ich_loeschen];
#endif
      if (!linst.obfehlt(hff) || !linst.obfehlt(hfcf)) {
        Log(hfftext,-1,1);
        linst.douninst(string(hff)+" "+hfcf,obverb,oblog);
        falscheshyla=1;
      }
      hylafehlt=linst.obfehlt(hfr,obverb,oblog) || linst.obfehlt(hfcr,obverb,oblog);
      string vstring=ltoan(versuch);
      Log(violetts+Tx[T_hylafehlt]+schwarz+ltoan(hylafehlt)+Tx[T_Versuch]+vstring,obverb,oblog);
      // falls nein, dann installieren
      if (hylafehlt || versuch) {
        //        Log(string(Tx[(T_)T_Installation_von_Hylafax_nicht_feststellbar_versuche_es_zu_installieren]),-2,1);
        if (falscheshyla) {
          systemrueck(string(
                "grep -rl 'faxcron\\|faxqclean' /etc/cron* | /usr/bin/xargs ls -l;")+ // in hylafax: /etc/cron.daily/suse.de-faxcron, 
              // in ~+: cron.daily/hylafax und cron.hourly/hylafax
              "cd init.d; rm "+hff+";"
#ifdef obhp      
              "cd /usr/lib/systemd/system; rm faxq.service hfaxd.service faxgetty*.service;"
              "cd /etc/systemd/system; rm faxq.service hfaxd.service faxgetty*.service;"
              //            "cd /usr/lib/fax; rm -f pagesizes pagerules faxmail.ps typerules;"  // duerfte nicht geloescht werden muessen
#else
              "cd /usr/lib/systemd/system; rm hylafax*.service;"
#endif
              "cd /usr/local/bin; rm -f faxalter faxcover faxmail faxrm faxstat sendfax sendpage;" // fehlt alles in rpm -ql hylafax / ~+

              "cd /usr/sbin; rm -f choptest cqtest dialtest edit-faxcover faxabort faxaddmodem faxadduser faxanswer faxconfig faxcron faxdeluser "
              "   faxinfo faxlock faxmodem faxmsg faxq faxqclean faxquit faxsetup faxsetup.linux faxstate faxwatch probemodem rchylafax*"
              "   recvstats tagtest tiffcheck tsitest typetest xferfaxstats"
#ifdef obhp      
              "   faxsetup.bsdi faxsetup.irix;"
#else
              "   faxfetch faxgetty faxsend hfaxd hylafax lockname ondelay pagesend textfmt;"
#endif
#ifdef obhp      
              "cd /usr/lib/fax; rm faxgetty faxsend hfaxd lockname ondelay pagesend;" 
#endif

              ,-2,oblog); 
        }
        //        string cmd("which zypper 2>/dev/null && zypper -n in hylafax hylafax-client || "
        //            "{ which apt-get 2>/dev/null && apt-get --assume-yes install hylafax-server hylafax-client; }; systemctl daemon-reload; ");
        //        hylafehlt=systemrueck(cmd,1+obverb,oblog,0,wahr,wahr,Tx[T_hylafax_faxmodem_nicht_gefunden_Versuche_es_zu_installieren_mit]); 
        if (!(hylafehlt=linst.doinst(string(hfr)+" "+string(hfcr),obverb,oblog))) {
          if (this->setzhylavz()) {
  // "pruefhyla 1 vor  obhyla: "<<(int)this->obhyla<<endl;
            this->obhyla=0;
  // "pruefhyla 1 nach obhyla: "<<(int)this->obhyla<<endl;
            return 1;
          }
          struct stat entryxfer, entryxfer0;
          memset(&entryxfer0,0,sizeof entryxfer0);
          string d1=this->varsphylavz+"/etc/xferfaxlog",d0=d1+".rpmsave";
          if (!lstat(d1.c_str(),&entryxfer)) {
            if (entryxfer.st_size<10) { // wenn neu
              if (!lstat(d0.c_str(),&entryxfer0) && entryxfer0.st_size>10) {
                systemrueck("cp -a "+d0+" "+d1,obverb,oblog);
              } else {
                if (falscheshyla)  {
                  char* fspoolvz=0;
                  for(unsigned iru=0;iru<sizeof this->moeglhvz/sizeof this->moeglhvz;iru++) {
                    if (this->moeglhvz[iru]!=this->varsphylavz) {
                      fspoolvz=(char*)this->moeglhvz[iru];
                      break;
                    } // if (this->moeglhvz[iru]
                  } // for(unsigned iru=0
                  if (fspoolvz) {
                    /*
                       wenn /var/spool/hylafax/etc/xferfaxlog 1 Byte hat und /var/spool/fax/etc/xferfaxlog mehr dann kopieren:
                       etc/xferfaxlog sendq recvq log doneq docq archive
                     */
                    /* // muss noch getestet werden
                       systemrueck(("cp ")+fspoolvz+"/etc/xferfaxlog "+this->varsphylavz+"/etc/",obverb,oblog);
                       systemrueck(("cp ")+fspoolvz+"/sendq "+this->varsphylavz+"/",obverb,oblog);
                       systemrueck(("cp ")+fspoolvz+"/recvq "+this->varsphylavz+"/",obverb,oblog);
                       systemrueck(("cp ")+fspoolvz+"/log "+this->varsphylavz+"/",obverb,oblog);
                       systemrueck(("cp ")+fspoolvz+"/doneq "+this->varsphylavz+"/",obverb,oblog);
                       systemrueck(("cp ")+fspoolvz+"/docq "+this->varsphylavz+"/",obverb,oblog);
                       systemrueck(("cp ")+fspoolvz+"/archive "+this->varsphylavz+"/",obverb,oblog);
                     */
                  }
                } // if (falscheshyla)
              } // !lstat(d0.c_str()
            } // if (entryfer.st_size<10

          }
#ifdef obhp      
          systemrueck(string("chown uucp:uucp -R ")+this->varsphylavz,obverb,oblog);
#else
          systemrueck(string("chown fax:uucp -R ")+this->varsphylavz,obverb,oblog);
#endif
        }
      }
      Log(string(Tx[T_StarteHylafax]),1,oblog);
      // falls ja, dann starten (falls erst installiert werden muss, dann bei versuch==1
#ifdef obhp
#else
      hylalaeuftnicht=hservice_faxq_hfaxd(this,obverb,oblog);
#endif
      //     if (systemrueck("systemctl list-units faxq.service --no-legend | grep ' active running'",-1,oblog) 
    } else {
      Log(Tx[T_Hylafax_laeuft],obverb,oblog);
    } //     if (systemrueck("systemctl list-units faxq.service --no-legend | grep ' active running'",-1,oblog) 
    hylalaeuftnicht=0;
    hylafehlt=0;
    for (uchar iru=0;iru<3;iru++) {
      int fglaeuftnicht=!this->sfaxgetty->obslaeuft(obverb,oblog);
      modemlaeuftnicht=systemrueck(("faxstat | grep ")+this->hmodem+" 2>&1",obverb,oblog) + fglaeuftnicht;
      if (!modemlaeuftnicht) break;
#ifdef obhp
#else
      hylalaeuftnicht=hservice_faxq_hfaxd(this,obverb,oblog)+fglaeuftnicht;
#endif
      if (iru>1) {
        systemrueck(("chmod 660 ")+this->varsphylavz+"/FIFO*",obverb,oblog);
        hfaxsetup(this,obverb,oblog);
        hconfig(this,obverb,oblog);
        hconfigtty(this,obverb,oblog);
        frischkonfiguriert=1;
      }
      if (iru>0) {
        hservice_faxgetty(this,obverb,oblog);
      }
      if (!hylalaeuftnicht && !modemlaeuftnicht) break;
    }
    if (!hylalaeuftnicht && !modemlaeuftnicht) break;
  } // for(unsigned versuch=0;versuch<3;versuch++) 

  //          cmd=string("zypper -n in uucp"); // fuer cu
  //          systemrueck(cmd,1,1,0,wahr,wahr,"installiere cu"); 
  //          cmd=string("usermod -a -G dialout root");
  //          systemrueck(cmd,1,1,0,wahr,wahr,"berechtige mich"); 
  //          cmd=string("chmod 666 ")+this->hmodem;
  //          systemrueck(cmd,1,1,0,wahr,wahr,"berechtige mich"); 
  if (this->hylazukonf || frischkonfiguriert) {
    //    hconfig(this,obverb,oblog); // countrycode, citycode/areacode, longdistancepraefix, internationalprefix
    if (!frischkonfiguriert) hconfigtty(this,obverb,oblog);
    if (!systemrueck(string("systemctl stop ")+this->sfaxgetty->sname+" "+this->shfaxd->sname+" "+this->sfaxq->sname,obverb,oblog)) {
      systemrueck("systemctl stop hylafax",obverb,oblog);
      systemrueck("systemctl disable hylafax",obverb,oblog);
      systemrueck(string("killall ")+this->sfaxgetty->ename+" "+this->shfaxd->ename+" "+this->sfaxq->ename,obverb,oblog);
    } // if (!systemrueck(string("systemctl stop ")+this->sfaxgetty->sname+" "+this->shfaxd->sname+" "+this->sfaxq->sname,obverb,oblog)) 
    if (!systemrueck(string("systemctl start ")+this->sfaxgetty->sname+" "+this->shfaxd->sname+" "+this->sfaxq->sname,obverb,oblog)) {
      systemrueck(string("systemctl enable ")+this->sfaxgetty->sname+" "+this->shfaxd->sname+" "+this->sfaxq->sname,obverb,oblog);
      this->hylazukonf=0;
    } // if (!systemrueck(string("systemctl start ")+this->sfaxgetty->sname+" "+this->shfaxd->sname+" "+this->sfaxq->sname,obverb,oblog)) 
  } // if (this->hylazukonf && !frischkonfiguriert) 
  // systemrueck("grep -rl 'faxcron\\|faxqclean' /etc/cron* | /usr/bin/xargs ls -l;",obverb,oblog); 
  // // in hylafax: /etc/cron.daily/suse.de-faxcron, 
  if (hylalaeuftnicht || modemlaeuftnicht) {
    Log(string(Tx[T_hylafaxspringtnichtan]),1,1);
    this->obhyla=0;
    if (this->konfobhyla) this->hylazukonf=1;
    return 1;
  }
  // Archivierung ggf. aktivieren
  systemrueck("! grep -q 'faxqclean *$' /etc/cron.hourly/hylafax || sed -i.bak 's/faxqclean *$/faxqclean -a -A/g' /etc/cron.hourly/hylafax",
      obverb,oblog);
  return 0;
} // int paramcl::pruefhyla()

/*
long GetFileSize(string filename)
{
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc ? -1 : stat_buf.st_size;
} // GetFileSize

void clear_kb(void) // statt fflush(stdin) nach getchar
{
  char junk[255];
  fgets (junk,sizeof junk,stdin);
} // clear_kb
*/

// wird aufgerufen in: paramcl::setzhylavz, pruefcvz, rueckfragen
void kuerzevtz(string *vzp)
{
  if (!vzp->empty()) {
    if (vzp->at(vzp->length()-1)==vtz)
      vzp->resize(vzp->length()-1);
  }
} // kuerzevtz

// von fkn-systems
// wird aufgerufen in: paramcl::pruefcapi
void pruefrules(int obverb, int oblog) 
{
  Log(violetts+Tx[T_pruefrules]+schwarz,obverb?obverb-1:0,oblog);
  const char *rulesd="/etc/udev/rules.d/46-FKN_isdn_capi.rules";
  struct stat entrybuf;
  if (lstat(rulesd, &entrybuf)) { 
    ofstream rules(rulesd);
    if (rules.is_open()) {
      rules<<"# Symlink (capi20 -> capi) zu"<<endl;
      rules<<"# Kompatibilitaetszwecken erstellen"<<endl;
      rules<<"KERNEL==\"capi\", SYMLINK=\"capi20\""<<endl;
    }
  }
} // void pruefrules() {

// wird aufgerufen in: pruefcapi
void pruefblack(int obverb, int oblog) 
{
  Log(violetts+Tx[T_pruefblack]+schwarz,obverb?obverb-1:0,oblog);
  const string blackd="/etc/modprobe.d/50-blacklist.conf";
  const char* vgl[]={"blacklist avmfritz", "blacklist mISDNipac"};
  uchar obda[]={0,0}, obeinsfehlt=0;
  fstream blacki(blackd,fstream::in);
  if (blacki.is_open()) {
    string zeile;
    while(getline(blacki,zeile)) {
      for(unsigned i=0;i<sizeof vgl/sizeof *vgl;i++) {
        if (zeile.find(vgl[i])!=string::npos) obda[i]=1;
      }
    }
    blacki.close();
    for(unsigned i=0;i<sizeof vgl/sizeof *vgl;i++) {
      if (!obda[i]) {obeinsfehlt=1;break;}
    }
    if (obeinsfehlt) {
      fstream blacka(blackd,fstream::out|fstream::app);
      if (blacka.is_open()) {
        for(unsigned i=0;i<sizeof vgl/sizeof *vgl;i++) {
          if (!obda[i]) {
            Log(string(Tx[T_haengean])+blau+blackd+schwarz+Tx[T_an_mdpp]+violetts+vgl[i]+schwarz,obverb,oblog);
            blacka<<vgl[i]<<endl;
          }
        }
      }
    }
  } else {
  }
} // void pruefblack(int obverb, int oblog) 

// wird aufgerufen in: pruefcapi
void pruefmodcron(int obverb, int oblog)
{
  Log(violetts+Tx[T_pruefmodcron]+schwarz,obverb?obverb-1:0,oblog);
  const string mp="@reboot /sbin/modprobe ";
  const string mps[]={mp+"capi",mp+"fcpci"};
  const string tmpc="meincrontab";
  for(uchar ru=0;ru<sizeof mps/sizeof *mps;ru++) {
    svec rueck;
    systemrueck(string("bash -c 'grep \"")+mps[ru]+"\" -q <(crontab -l)' || "
        "(crontab -l>\""+tmpc+"\"; echo \""+mps[ru]+"\">>\""+tmpc+"\"; crontab \""+tmpc+"\")",obverb,oblog,&rueck);
    for(size_t znr=0;znr<rueck.size();znr++) {
      Log(rueck[znr],1+obverb,oblog);
    }
  }
} // void pruefmodcron(int obverb, int oblog)


// wird aufgerufen in: untersuchespool, main
int paramcl::pruefcapi()
{
  Log(violetts+Tx[T_pruefcapi]+schwarz,obverb?obverb:obverb,oblog);
  static uchar capischonerfolgreichinstalliert=0;
  int capilaeuft=0;
  unsigned versuch=0;
  for(;versuch<5;versuch++) {
    // capi4linux muss zum Laufen der Capisuite installiert sein
    // fuer fcpci muss in driver.c eingefuegt werden:
    // #if !defined(IRQF_DISABLED)
    // #  define IRQF_DISABLED 0x00
    // #endif
    //    capilaeuft=(PIDausName("capisuite")>=0);
    capilaeuft = this->scapisuite->obslaeuft(obverb-1,oblog);
    Log(violetts+Tx[T_capilaeuft]+schwarz+ltoan(capilaeuft)+schwarz,obverb,oblog);
    if (!capilaeuft) {
      //      pid_t pid = GetPIDbyName("capisuite") ; // If -1 = not found, if -2 = proc fs access error
      uchar fcpcida=0, capida=0, capidrvda=0;
      vector<string> rueck;
      systemrueck("lsmod",obverb,oblog,&rueck);
      for(size_t i=0;i<rueck.size();i++){
        if (!fcpcida) {if (!rueck[i].find("fcpci")) {fcpcida=1;continue;}}
        if (!capidrvda) {if (!rueck[i].find("capidrv")) {capidrvda=1;continue;}}
        if (!capida) {if (!rueck[i].find("capi")) {capida=1;continue;}}
        if (fcpcida && capida && capidrvda) break;
      }
      if (!fcpcida || !capida || !capidrvda) {
        Log("Lade Capi-Module ...",-1,0);
        systemrueck("modprobe -r avmfritz mISDNipac",obverb,oblog);
        for(uchar ivers=0;ivers<2;ivers++) {
          if (systemrueck("modprobe -v fcpci",obverb-1,oblog)) {
            if (ivers) {
              Log(rots+Tx[T_KannFcpciNInstVerwCapiNicht]+schwarz,1,1);
              return 1;
            }
            utsname unbuf;
            uname(&unbuf);
            Log(string(Tx[T_Kernelversion])+blau+unbuf.release+schwarz,obverb,oblog);
            struct stat entryfc;
            string fcpciko = string("/lib/modules/")+unbuf.release+"/kernel/extras/fcpci.ko";
            if (lstat(fcpciko.c_str(), &entryfc)) {
              Log(string(Tx[T_Datei])+blau+fcpciko+schwarz+Tx[T_nichtgefFcpciMfdKinstallierwerden],obverb,1);
              //              systemrueck("which zypper 2>/dev/null && zypper -n in kernel-source || "
              //                  "{ which apt-get 2>/dev/null && apt-get --assume-yes install linux-source; }", 1+obverb,oblog);
              linst.doinst("kernel-source",1+obverb,oblog);
              const string qvz="/usr/src";
              const string versi="fcpci-3.10.0";
              const string srcf=string("fritz-")+versi+".tar.bz2";
              systemrueck(string("sudo mkdir -p ")+qvz);
              struct stat entrysrc;
              if (lstat((qvz+vtz+srcf).c_str(),&entrysrc)) {
                systemrueck(string("cd ")+qvz+";wget https://belug.de/~lutz/pub/fcpci/"+srcf+" --no-check-certificate",1+obverb,oblog);
              }
              string srcvz=qvz+vtz+versi+"/src";
              if (lstat(srcvz.c_str(),&entrysrc)) {
                systemrueck(string("cd ")+qvz+";tar xjf "+srcf,obverb,oblog);
                //                sed -e '/#include <linux\/isdn\/capilli.h>/ a\#include <linux\/utsname.h>' -e '/NOTE("(%s built on %s at %s)\\n", TARGET, __DATE__, __TIME__);/ c    NOTE("(%s built on release %s, version %s)\\n", TARGET, utsname()->release, utsname()->version);' main.c >main_neu.c
                // fuer Kernel 4.3.3-3-default und gcc muss jetzt noch 1) , __DATE__ und __TIME__ aus main.c Zeile 365 entfernt werden,
                // 2) in driver.c Zeile 373 IRQF_DISABLED durch 0x00 ersetzt werden, dann kompilier- und installierbar
              }
              systemrueck(string("cd ")+srcvz+";test -f driver.c.bak || sed -i.bak '/request_irq/i#if !defined(IRQF_DISABLED)\\n"
                  "# define IRQF_DISABLED 0x00\\n#endif' driver.c;"
                  "sed -e '/#include <linux\\/isdn\\/capilli.h>/a #include <linux\\/utsname.h>' "
                  "-e '/NOTE(\"(%s built on %s at %s)\\\\n\", TARGET, __DATE__, __TIME__);/"
                  "c NOTE(\"(%s built on release %s, version %s)\\\\n\", TARGET, utsname()->release, utsname()->version);' "
                  "main.c >main_neu.c;mv -n main.c main.c.bak;mv -n main_neu.c main.c;"
                  "make clean",1+obverb,oblog);
              svec rueck;
              systemrueck(string("rm -f /root/bin/xargs"),1+obverb,oblog);
              systemrueck(string("cd ")+srcvz+";make all",1+obverb,oblog);
              systemrueck(string("cd ")+srcvz+";make install",1+obverb,oblog);
            }
          }
        }
        systemrueck("modprobe capi",obverb,oblog);
        systemrueck("modprobe capidrv",obverb,oblog);
      }
      pruefrules(obverb,oblog);
      pruefblack(obverb,oblog);
      capischonerfolgreichinstalliert=!linst.obfehlt("capisuite capi4linux i4l-isdnlog");
      if (!capischonerfolgreichinstalliert) {
        Log(string(Tx[T_Konnte])+blau+"capisuite"+schwarz+Tx[T_nichtstarten],1,oblog);
        // if (systemrueck("which zypper",-1,-1)) KLA
        if (linst.checkinst(-1,-1)!=zyp) {
          Log(rots+Tx[T_Kann_Capisuite_nicht_installieren_verwende_Capi_nicht],1,1);
          this->obcapi=0;
          return 1;
        }
        systemrueck("zypper lr | grep 'kkeil factory development project' || "
            "sudo zypper ar -f http://download.opensuse.org/repositories/home:/kkeil:/Factory/openSUSE_Factory/home:kkeil:Factory.repo",
            1,1);
        // i4l-isdnlog scheint nicht wirklich noetig zu sein
        //        capischonerfolgreichinstalliert=!systemrueck("zypper -n --gpg-auto-import-keys in capisuite capi4linux i4l-isdnlog", 1+obverb,oblog); 
        // i4l-base geloescht
        capischonerfolgreichinstalliert=!linst.doinst("capisuite capi4linux i4l-isdnlog",obverb+1,oblog);
        // capisuite unter Kernel 4: 
        // git clone https://github.com/larsimmisch/capisuite.git
        // cd capisuite
        // zypper in sfftobmp libcapi20-2
        //        // scp linux2:/usr/include/capiutils.h /usr/include
        //        // scp linux2:/usr/include/capicmd.h /usr/include
        // zypper in libcapi20-3 python-devel capi4linux capi4linux-devel
        // ln -s /usr/lib64/libcapi20.so.3.0.6 libcapi20.so
        // in ./src/application/pythonscript.cpp Zeile 104: (Py_ssize_t*)&length statt &length
        // in /usr/include/capiutils.h eine dritte Zeile einfuegen: #define CAPI_LIBRARY_V2
        // in src/backend/connection.cpp eine Zeile 26 einfuegen: #include <cstring>

      }
      systemrueck("systemctl daemon-reload",obverb,oblog);
    }
    if (versuch>0 || this->capizukonf) {
      this->konfcapi();
    } //     if (versuch>0) KLA
    // das folgende verhindert zwar den Programmabbruch bei active (exit), das nuetzt aber nichts. In dem Fall fcpci aktualisieren! 23.5.14
    //    capilaeuft = !systemrueck("systemctl status capisuite | grep ' active (running)' >/dev/null 2>&1",0,obverb,oblog);
    //     capilaeuft  = !systemrueck("systemctl is-active capisuite",0,obverb,oblog);
    capilaeuft = this->scapisuite->obslaeuft(obverb-1,oblog);
    if (capilaeuft) {
      break;
    } else {
      systemrueck("systemctl daemon-reload");
      systemrueck("systemctl stop isdn 2>/dev/null",obverb>0?obverb:-1,oblog);
      //      systemrueck("systemctl start isdn",obverb,oblog);
      Log(string(Tx[T_StarteCapisuite]),-1,oblog);
      systemrueck("systemctl stop capisuite 2>/dev/null",-1,oblog);
      if (!systemrueck("systemctl start capisuite 2>/dev/null",-1,oblog)) {
        if (!systemrueck("systemctl enable capisuite 2>/dev/null",-1,oblog)) {
        }
        Log(Tx[T_Capisuite_gestartet],1,oblog);
      } else {
        //       Log("Capisuite konnte nicht gestartet werden.",1,1);
      }
    }
  } //  for(unsigned versuch=0;1;versuch++) (3.)
  if (capilaeuft) {
    if (this->obcapi) pruefmodcron(obverb,oblog);
  } else {
    Log(rots+Tx[T_konntecapisuiteservice]+violett+ltoan(versuch)+rot+Tx[T_malnichtstartenverwN]+schwarz,1,1);
    return 1;
  }
  return 0;
} // pruefcapi()


// wird aufgerufen in: faxemitC
void inDbc(DB *My, const string& spooltab, const string& spoolg, fsfcl *fsfp, char* telnr, int obverb, int oblog)
{
  Log(violetts+Tx[T_inDbc]+schwarz,obverb?obverb-1:0,oblog);
  string spooldir, spoolfil;
  spooldir=dir_name(spoolg);
  spoolfil=base_name(spoolg);
  uint affr=0;
  Log(string(Tx[T_SpoolDateierstellt])+rot+spoolg+schwarz+"'",obverb,oblog);
  Log(string(Tx[T_SpoolDatei])+rot+spoolfil+schwarz+"'",obverb,oblog);
  Log(string(Tx[T_SpoolPfad])+rot+spooldir+schwarz+"'",obverb,oblog);
  RS zs(My);
  struct stat entryspool;
  if (!lstat((spoolg.c_str()), &entryspool)) {
    for(int runde=0;runde<2;runde++) {
      if (runde==0) { zs.Abfrage("SET NAMES 'utf8'");
      } else if (runde==1) zs.Abfrage("SET NAMES 'latin1'");
      RS rupd(My); 
      rupd.clear();
      vector<instyp> einf; // fuer alle Datenbankeinfuegungen
      einf.push_back(instyp(My->DBS,"capispoolpfad",&spooldir));
      einf.push_back(instyp(My->DBS,"capispooldatei",&spoolfil));
      einf.push_back(instyp(My->DBS,"cdateidatum",&entryspool.st_mtime));
      einf.push_back(instyp(My->DBS,"cdateizeit",entryspool.st_mtime));
      einf.push_back(instyp(My->DBS,"telnr",telnr));
      string bedingung=string("id=")+fsfp->id;
      rupd.update(spooltab,einf,ZDB,bedingung);

      if (runde==1) zs.Abfrage("SET NAMES 'utf8'");
      affr=My->affrows();
      if (affr>0) break;
      if (runde==1) {
        Log(string(Tx[T_Fehler_af])+drot+ltoan(rupd.fnr)+schwarz+Tx[T_bei]+tuerkis+rupd.sql+schwarz+": "+blau+rupd.fehler+schwarz,1,oblog);
        exit(0);
      }
    }   // for(int runde=0;runde<2;runde++)
    Log(drots+string("  affected_rows(): ")+schwarz+ltoan(affr),obverb,oblog);
  }   // if (!lstat((*spoolgp->c_str()), &entryspool)) 
} // inDbc

// wird aufgerufen in: faxealle
void faxemitC(DB *My, const string& spooltab, fsfcl *fsfp, paramcl *pmp, int obverb, int oblog)
  // 3. wartende Dateien bestimmen
  // 4. falls welche gefunden, capisuite pruefen
  // 5. wegfaxen
  //      und wenn erfolgreich im spool, dann jeweils in Datenbank eintragen
{
  Log(violetts+Tx[T_faxemitC]+schwarz,obverb,oblog);
  // 5. wegfaxen und wenn erfolgreich im spool, dann in Datenbank eintragen
  if (fsfp->telnr.empty()) {
    obverb=oblog=1; 
    Log(string(Tx[T_DieFaxnrausTabelle])+tuerkis+spooltab+schwarz+"`, id `"+tuerkis+fsfp->id+schwarz+"` "+
        drot+fsfp->spdf+schwarz+Tx[T_istleerfaxeesdahernicht],1,1);
  } else {
    Log(string(Tx[T_DieFaxnrvon])+drot+fsfp->spdf+schwarz+Tx[T_ist]+blau+fsfp->telnr+schwarz,obverb,oblog);
    struct stat entryff;
    string ff=pmp->wvz+vtz+fsfp->spdf;
    if (lstat(ff.c_str(), &entryff)) {
      Log(rots+Tx[T_faxemitCFehler]+schwarz+Tx[T_Faxdatei]+blau+ff+rot+ Tx[T_fehlt]+schwarz,1,1);
      // archiviere
    } else if (!entryff.st_size) {
      Log(rots+Tx[T_faxemitCFehler]+schwarz+Tx[T_Faxdatei]+blau+ff+rot+ Tx[T_hat0Bytes]+schwarz,1,1);
    } else {
      string cmd=string("capisuitefax -n -u ")+pmp->cuser+" -d "+fsfp->telnr+" \""+pmp->wvz+vtz+fsfp->spdf+"\" 2>&1";
      vector<string> faxerg;
      systemrueck(cmd,obverb,oblog,&faxerg,wahr,wahr,Tx[T_Faxbefehl]);
      if (faxerg.size()) {
        const char* tz1=" successful enqueued as ", // muss sprachlich so falsch bleiben wie im python-Script
              *tz2=" for ";
        if (char *z1=strstr((char*)faxerg.at(0).c_str(),tz1)) {
          if (char *z2=strstr(z1,tz2)) {
            string spoolg(z1+strlen(tz1),z2-z1-strlen(tz1));
            //            inDatenbankc(My, &spoolg, idsp, npdfp, spdfp, nachrnr, z2+strlen(tz2), obverb, oblog);
            inDbc(My, spooltab, spoolg, fsfp, z2+strlen(tz2), obverb, oblog);
          }   // if (char *z2=strstr(z1,tz2)) 
          // if (char *z1=strstr((char*)faxerg.at(0).c_str(),tz1))
        } else if (faxerg.at(0).find("can't open")==0) {
          Log(rots+Tx[T_Datei]+blau+pmp->wvz+vtz+fsfp->spdf+rot+"' (id: "+blau+fsfp->id+rot+
              Tx[T_nichtgefundenloeschesieausDB]+schwarz,1,1);
          RS rsloe(My,string("delete from `")+spooltab+"` where id = "+fsfp->id,ZDB);
        }
      } else {
        Log(rots+string(Tx[T_KeinErgebnisbeimFaxen])+schwarz,1,1);
      }
    }
  }
} // faxemitC

// wird aufgerufen in faxemitH
void inDBh(DB *My, const string& spooltab, paramcl *pmp, const string& hylaid, fsfcl *fsfp,string *tel,int obverb, int oblog)
{
  Log(violetts+Tx[T_inDBh]+schwarz,obverb?obverb-1:0,oblog);
  string spoolfil=string("q")+hylaid;
  string spoolg= pmp->hsendqvz + vtz + spoolfil;
  uint affr=0;
  Log(string(Tx[T_SpoolDateierstellt])+rot+spoolg+schwarz+"'",pmp->obverb,pmp->oblog);
  Log(string(Tx[T_SpoolDatei])+rot+spoolfil+schwarz+"'",pmp->obverb,pmp->oblog);
  Log(string(Tx[T_SpoolPfad])+rot+pmp->hsendqvz+schwarz+"'",pmp->obverb,pmp->oblog);
  RS zs(My);
  struct stat entryspool;
  string spoolid="";
  if (!lstat(spoolg.c_str(), &entryspool)) {
    for(int runde=0;runde<2;runde++) {
      if (runde==0) { zs.Abfrage("SET NAMES 'utf8'");
      } else if (runde==1) zs.Abfrage("SET NAMES 'latin1'");
      RS rupd(My); 
      rupd.clear();
      vector<instyp> einf; // fuer alle Datenbankeinfuegungen
      einf.push_back(instyp(My->DBS,"hylanr",hylaid));
      einf.push_back(instyp(My->DBS,"hdateidatum",&entryspool.st_mtime));
      einf.push_back(instyp(My->DBS,"hdateizeit",entryspool.st_mtime));
      einf.push_back(instyp(My->DBS,"telnr",tel));
      string bedingung=string("id=")+fsfp->id;
      rupd.update(spooltab,einf,ZDB,bedingung);
      if (runde==1) zs.Abfrage("SET NAMES 'utf8'");
      affr=My->affrows();
      if (affr>0) break;
      if (runde==1) {
        Log(string(Tx[T_Fehler_af])+drot+ltoan(rupd.fnr)+schwarz+Tx[T_bei]+tuerkis+rupd.sql+schwarz+": "+blau+rupd.fehler+schwarz,1,pmp->oblog);
        exit(0);
      }
    }   // for(int runde=0;runde<2;runde++)
    Log(drots+"  affected_rows(): "+schwarz+ltoan(affr),pmp->obverb,pmp->oblog);
  } else {
    Log(drots+Tx[T_SpoolDatei]+spoolg+Tx[T_nicht_gefunden_kein_Datenbankeintrag],1,1);
  }   // if (!lstat((*spoolgp->c_str()), &entryspool)) 
} // inDBh

// wird aufgerufen in: faxealle
void faxemitH(DB *My, const string& spooltab, fsfcl *fsfp, paramcl *pmp, int obverb, int oblog)
{
  // wenn in capi maxversuch ueberschritten, dann mit hylafax wegfaxen und wenn erfolgreich im spool, dann in Datenbank aktualisieren
  // 3. wartende Dateien bestimmen
  Log(violetts+Tx[T_faxemitH]+schwarz,obverb,oblog);
  // 5. wegfaxen und wenn erfolgreich im spool, dann in Datenbank eintragen
  string tel="";
  char* pt=strcasestr((char*)fsfp->spdf.c_str(),(char*)pmp->anfaxstr.c_str());
  if (!pt) if (!pmp->ancfaxstr.empty()) pt=strcasestr((char*)fsfp->spdf.c_str(),(char*)pmp->ancfaxstr.c_str());
  if (!pt) if (!pmp->anhfaxstr.empty()) pt=strcasestr((char*)fsfp->spdf.c_str(),(char*)pmp->anhfaxstr.c_str());
  if (pt) pt+=5;
  for(;pt;pt++){
    if (!*pt || *pt=='_') break; // '_' ist wegen Ausweichzielen mit Unterstrichen
    if (strchr("0123456789",*pt)) tel+=*pt;
    else if (*pt=='+') tel+="00";
  }
  if (tel.empty()) {
    Log(string(Tx[T_DieFaxnrvon])+drot+fsfp->spdf+schwarz+Tx[T_istleerfaxeesdahernicht],1,1);
  } else {
    Log(string(Tx[T_DieFaxnrvon])+drot+fsfp->spdf+schwarz+Tx[T_ist]+blau+tel+schwarz,obverb,oblog);
    // 27.3.16: Uebernacht wurden die Berechtigungen so eingeschraenkt, dass Faxsenden nicht mehr ging, evtl. durch faxqclean
    systemrueck("find "+pmp->varsphylavz+" -name seqf -exec chmod 660 {} \\;"" -exec chown fax:uucp {} \\;",obverb,oblog);
    const char* tz1="request id is ", *tz2=" (";
    string cmd=string("sendfax -n -A -d ")+tel+" \""+pmp->wvz+vtz+fsfp->spdf+"\" 2>&1";
    svec faxerg;
    // <<rot<<"Achtung: faxemith: "<<endl<<schwarz<<cmd<<endl;
    systemrueck(cmd,1,1,&faxerg,wahr,wahr,Tx[T_HylafaxBefehl]);
    for(size_t i=0;i<faxerg.size();i++) {
      Log(string(Tx[T_RueckmlgZeile])+ltoan(i)+": "+faxerg.at(i),obverb-1,oblog);
      if (char *z1=strstr((char*)faxerg.at(i).c_str(),tz1)) {
        if (char *z2=strstr(z1,tz2)) {
          string hylaid(z1+strlen(tz1),z2-z1-strlen(tz1));
          //            inDatenbankh(My, pmp->hsendqvz, &hylaid, idsp, npdfp, spdfp, nachrnr, &tel, obverb, oblog);
          inDBh(My, spooltab, pmp, hylaid, fsfp,&tel,obverb,oblog);
        }   // if (char *z2=strstr(z1,tz2)) 
        break;
      }    // if (char *z1=strstr((char*)faxerg.at(0).c_str(),tz1))
    } // faxerg.size
  } // tel.empty() else
} // faxemitH

// wird aufgerufen in: archiviere, empfarch
void getSender(paramcl *pmp,const string& faxnr, string *getnamep, string *bsnamep,int obverb,int oblog) 
{
  Log(violetts+"getSender()"+schwarz,obverb,oblog);
  pmp->My->usedb(pmp->My->db);
  string trimfaxnr=faxnr;
  ltrim(&trimfaxnr);
  //  char* trimfaxnr=(char*)faxnrc; 
  //  while(*trimfaxnr==32) trimfaxnr++;
  for(unsigned snr=0;snr<pmp->sqlzn;snr++) {
    Log(string(rot)+"snr: "+schwarz+ltoan(snr),obverb,oblog);
    Log(string(rot)+"sql: "+schwarz+pmp->sqlconfp[snr].wert,obverb,oblog);
    RS rhae(pmp->My,ersetzAllezu(pmp->sqlconfp[snr].wert,"&&faxnr&&",trimfaxnr.c_str())); // (const char*)trimfaxnr));
    if (!rhae.obfehl) {
      Log(string("obfehl: ")+ltoan((int)rhae.obfehl),obverb,oblog);
      char ***cerg;
      while (cerg=rhae.HolZeile(),cerg?*cerg:0) {
        Log(string("cerg: ")+ltoan((bool)*cerg),obverb,oblog);
        if (*(*cerg+0)) {
          Log(string("P0: ")+blau+*(*cerg+0)+schwarz,obverb,oblog);
          if (*getnamep=="") if (*(*cerg+0)) *getnamep = *(*cerg+0);
        }
        if (*(*cerg+1)) {
          Log(string("P1: ")+blau+*(*cerg+1)+schwarz,obverb,oblog);
          if (*bsnamep=="") if (*(*cerg+1)) *bsnamep = *(*cerg+1);
        }
        break;
      }
    }
    if (!obverb) if (*getnamep!="") break;
    //    if (!obverb) if (*getnamep!="" && bsname !="") break;
  }
} // void getSender(paramcl *pmp,const char* faxnrc, string *getnamep, string *bsnamep,int obverb=0,int oblog=0) 

// wird aufgerufen in: main
const string& pruefspool(DB *My,const string& spooltab, int obverb, int oblog, uchar direkt=0)
{
  Log(violetts+Tx[T_prufespool]+schwarz,obverb,oblog);
  if (!direkt){
    Feld felder[] = {
      Feld("ID","int","10","",Tx[T_eindeutige_Identifikation],1,1),
      Feld("original","varchar","1","",Tx[T_Originalname_der_Datei],1,0,1),
      Feld("origvu","varchar","1","",Tx[T_Originalname_der_Datei_vor_Umwandlung_in_PDF],0,0,1),
      Feld("telnr","varchar","1","",Tx[T_zu_senden_an],0,0,1),
      Feld("prio","int","1","",Tx[T_Prioritaet_aus_Dateinamen],0,0,1),
      Feld("capidials","int","10","",Tx[T_Zahl_der_bisherigen_Versuche_in_Capisuite],0,0,1),
      Feld("hyladials","int","10","",Tx[T_Zahl_der_bisherigen_Versuche_in_Hylafax],0,0,1),
      Feld("capispooldatei","varchar","1","",Tx[T_Spooldatei_in_Capisuite],0,0,1),
      Feld("capispoolpfad","varchar","1","",Tx[T_Pfad_zur_Spooldatei_in_Capisuite_ohne_abschliessendes_Verzeichnistrennzeichen],0,0,1),
      Feld("cdateidatum","datetime","0","0",Tx[T_Aenderungszeit_der_CapiSpoolDatei],0,0,1),
      Feld("cdateizeit","double","0","0",Tx[T_Aenderungszeit_der_CapiSpooldatei_im_time_Format],0,0,1),
      Feld("hylanr","int","10","","jobid in hylafax",0,0,1),
      Feld("hdateidatum","datetime","0","0",Tx[T_Aenderungszeit_der_Hylaspooldatei],0,0,1),
      Feld("hdateizeit","double","0","0",Tx[T_Aenderungszeit_der_Hylaspooldatei_im_Time_Format],0,0,1),
      Feld("althylanr","int","10","",Tx[T_jobid_in_letztem_gescheitertem_hylafax],0,0,1),
      Feld("hylastate","int","2","",Tx[T_state_Feld_in_hylafax],0,0,1),
      Feld("hylastatuscode","int","10","",Tx[T_statuscode_in_letztem_gescheitertem_hylafax],0,0,1),
      Feld("hylastatus","varchar","80","",Tx[T_status_in_letztem_gescheitertem_hylafax],0,0,1),
    };
    Feld ifelder0[] = {Feld("capispooldatei")}; Index i0("capispooldatei",ifelder0,sizeof ifelder0/sizeof* ifelder0);
    Feld ifelder1[] = {Feld("cdateidatum")};    Index i1("cdateidatum",ifelder1,sizeof ifelder1/sizeof* ifelder1);
    Feld ifelder2[] = {Feld("hdateidatum")};    Index i2("hdateidatum",ifelder2,sizeof ifelder2/sizeof* ifelder2);
    //    Feld ifelder3[] = {Feld("original")};       Index i3("original",ifelder3,sizeof ifelder3/sizeof* ifelder3);
    Index indices[]={i0,i1,i2/*,i3*/};
    // auf jeden Fall ginge "binary" statt "utf8" und "" statt "utf8_general_ci"
    Tabelle tab(spooltab,felder,sizeof felder/sizeof* felder,indices,sizeof indices/sizeof *indices,Tx[T_capispooldateien_der_Capisuite],"InnoDB","utf8","utf8_general_ci","DYNAMIC");
    if (My->prueftab(&tab, obverb)) {
      Log(string(Tx[T_Fehler_beim_Pruefen_von])+spooltab,1,1);
      return NULL;
    }
  }
  return spooltab;
} // const char* pruefspool(DB *My,const char* spooltab, int obverb, int oblog, uchar direkt=0)

// wird aufgerufen in: main
void pruefouttab(DB *My, const string& touta, int obverb, int oblog, uchar direkt=0)
{
  Log(violetts+Tx[T_pruefouta]+schwarz,obverb,oblog);
  if (!direkt){
    Feld felder[] = {
      Feld("eind","int","10","",Tx[T_eindeutige_Identifikation],1,1),
      Feld("Erfolg","int","1","",Tx[T_1_ist_erfolgreiche_Uebertragung_0_ist_fehlgeschlagen],0,0,1),
      Feld("rcname","varchar","1","",Tx[T_Name_des_Adressaten],0,0,1),
      Feld("submt","datetime","0","0",Tx[T_Datum_des_Losschickens],0,0,1),
      Feld("transe","datetime","0","0",Tx[T_Ende_der_Uebertragung],0,0,1),
      Feld("submid","varchar","1","",Tx[T_Kennung_im_Faxsystem_hylafax_Nr_Capisuite_Datei_MSFax_Fax_Dateiname],0,0,1),
      Feld("docname","varchar","1","",Tx[T_Dateiname],0,0,1),
      Feld("fsize","int","10","",Tx[T_Dateigroesse],0,0,1),
      Feld("pages","int","10","",Tx[T_Seitenzahl],0,0,1),
      Feld("retries","int","10","",Tx[T_Zahl_der_Anwahlen],0,0,1),
      Feld("rcfax","varchar","1","",Tx[T_FAxnummer_des_Adressaten],0,0,1),
      Feld("titel","varchar","1","",Tx[T_Titel_des_Adressaten],0,0,1),
      Feld("pid","int","10","",Tx[T_PID_falls_aus_Dateinamen_ermittelt],0,0,1),
      Feld("tsid","varchar","1","",Tx[T_Kurzbezeichnung_der_eigenen_Faxstelle],0,0,1),
      Feld("devname","varchar","1","",Tx[T_Geraetename],0,0,1),
      Feld("oscht","datetime","0","0",Tx[T_outgoing_schedule_time_oder_so_aehnlich_nur_MSFax],0,0,1),
      Feld("subject","varchar","1","",Tx[T_Hinweis_auf_Dateiinhalt_oder_Adressat_nur_MSFax],0,0,1),
      Feld("id","varchar","1","",Tx[T_identisch_zu_submid_nur_MSFax],0,0,1),
      Feld("prio","int","10","",Tx[T_Prioritaet_nur_MSFax],0,0,1),
      Feld("csid","varchar","1","",Tx[T_FAxnummer_des_Adressaten_nur_MSFax],0,0,1),
      Feld("sender","varchar","1","",Tx[T_Faxnummer_des_Senders_nur_MSFax],0,0,1),
      Feld("transs","datetime","0","0",Tx[T_Beginn_der_Uebertragung_nur_MSFax],0,0,1),
    };
    Feld ifelder0[] = {Feld("submt"),Feld("Erfolg")};   Index i0("submt",ifelder0,sizeof ifelder0/sizeof* ifelder0);
    Feld ifelder1[] = {Feld("Erfolg"),Feld("submt")};   Index i1("Erfolg",ifelder1,sizeof ifelder1/sizeof* ifelder1);
    Feld ifelder2[] = {Feld("docname"),Feld("Erfolg")}; Index i2("docname",ifelder2,sizeof ifelder2/sizeof* ifelder2);
    Feld ifelder3[] = {Feld("pid"),Feld("Erfolg")};     Index i3("pid",ifelder3,sizeof ifelder3/sizeof* ifelder3);
    Index indices[]={i0,i1,i2,i3};
    // auf jeden Fall ginge "binary" statt "utf8" und "" statt "utf8_general_ci"
    Tabelle taba(touta,felder,sizeof felder/sizeof* felder,indices,sizeof indices/sizeof *indices,
        Tx[T_Archiv_fuer_die_erfolgreichen_Faxe],"InnoDB","utf8","utf8_general_ci","DYNAMIC");
    if (My->prueftab(&taba, obverb)) {
      Log(string(Tx[T_Fehler_beim_Pruefen_von])+touta,1,1);
    }
  }
} // int pruefouttab(DB *My, string touta, int obverb, int oblog, uchar direkt=0)

// wird aufgerufen in: main
void pruefinctab(DB *My, const string& tinca, int obverb, int oblog, uchar direkt=0)
{
  Log(violetts+Tx[T_pruefinca]+schwarz,obverb,oblog);
  if (!direkt){
    Feld felder[] = {
      Feld("eind","int","10","",Tx[T_eindeutige_Identifikation],1,1),
      Feld("Erfolg","int","1","",Tx[T_1_ist_erfolgreiche_Uebertragung_0_ist_fehlgeschlagen],0,0,1),
      //      Feld("rcname","varchar","1","",Tx[T_Name_des_Adressaten],0,0,1),
      //      Feld("submt","datetime","0","0",Tx[T_Datum_des_Losschickens],0,0,1),
      Feld("transe","datetime","0","0",Tx[T_Ende_der_Uebertragung],0,0,1),
      //      Feld("submid","varchar","1","",Tx[T_Kennung_im_Faxsystem_hylafax_Nr_Capisuite_Datei_MSFax_Fax_Dateiname],0,0,1),
      //      Feld("docname","varchar","1","",Tx[T_Dateiname],0,0,1),
      Feld("fsize","int","10","",Tx[T_Dateigroesse],0,0,1),
      Feld("pages","int","10","",Tx[T_Seitenzahl],0,0,1),
      Feld("retries","int","10","",Tx[T_Zahl_der_Anwahlen],0,0,1),
      //      Feld("rcfax","varchar","1","",Tx[T_FAxnummer_des_Adressaten],0,0,1),
      Feld("titel","varchar","1","",Tx[T_Titel_des_Adressaten],0,0,1),
      //      Feld("pid","int","10","",Tx[T_PID_falls_aus_Dateinamen_ermittelt],0,0,1),
      Feld("tsid","varchar","1","",Tx[T_Kurzbezeichnung_der_eigenen_Faxstelle],0,0,1),
      Feld("devname","varchar","1","",Tx[T_Geraetename],0,0,1),
      //      Feld("oscht","datetime","0","0",Tx[T_outgoing_schedule_time_oder_so_aehnlich_nur_MSFax],0,0,1),
      //      Feld("subject","varchar","1","",Tx[T_Hinweis_auf_Dateiinhalt_oder_Adressat_nur_MSFax],0,0,1),
      Feld("id","varchar","1","",Tx[T_identisch_zu_submid_nur_MSFax],0,0,1),
      //      Feld("prio","int","10","",Tx[T_Prioritaet_nur_MSFax],0,0,1),
      Feld("csid","varchar","1","",Tx[T_FAxnummer_des_Adressaten_nur_MSFax],0,0,1),
      //      Feld("sender","varchar","1","",Tx[T_Faxnummer_des_Senders_nur_MSFax],0,0,1),
      Feld("transs","datetime","0","0",Tx[T_Beginn_der_Uebertragung_nur_MSFax],0,0,1),
    };
    Feld ifelder0[] = {Feld("transe"),Feld("Erfolg")};   Index i0("transe",ifelder0,sizeof ifelder0/sizeof* ifelder0);
    Feld ifelder1[] = {Feld("Erfolg"),Feld("transe")};   Index i1("Erfolg",ifelder1,sizeof ifelder1/sizeof* ifelder1);
    Feld ifelder2[] = {Feld("titel"),Feld("Erfolg")}; Index i2("titel",ifelder2,sizeof ifelder2/sizeof* ifelder2);
    Feld ifelder3[] = {Feld("tsid"),Feld("Erfolg")}; Index i3("tsid",ifelder3,sizeof ifelder3/sizeof* ifelder3);
    Feld ifelder4[] = {Feld("id"),Feld("Erfolg")}; Index i4("id",ifelder4,sizeof ifelder4/sizeof* ifelder4);
    Index indices[]={i0,i1,i2,i3,i4};
    // auf jeden Fall ginge "binary" statt "utf8" und "" statt "utf8_general_ci"
    Tabelle taba(tinca,felder,sizeof felder/sizeof* felder,indices,sizeof indices/sizeof *indices,
        Tx[T_Archiv_fuer_die_erfolgreichen_Faxe],"InnoDB","utf8","utf8_general_ci","DYNAMIC");
    if (My->prueftab(&taba, obverb)) {
      Log(string(Tx[T_Fehler_beim_Pruefen_von])+tinca,1,1);
    }
  }
} // int pruefinctab(DB *My, string tinca, int obverb, int oblog, uchar direkt=0)


// wird aufgerufen in: main
void pruefprocgettel3(DB *Myp, const string& usr, const string& pwd, const string& host, int obverb, int oblog)
{
  Log(violetts+Tx[T_pruefprocgettel3]+schwarz,obverb,oblog);
  string body;
  body+= "begin \n";
  body+= " declare pos int;\n";
  body+= " declare ch char default '0';\n";
  body+= " declare tel varchar(100) default '';\n";
  body+= " set pos = instr(dname,anfaxstr);\n";
  body+= " if pos>0 then\n";
  body+= "  set pos=pos+length(anfaxstr);\n";
  body+= " else\n";
  body+= "  if ancfaxstr > '' then\n";
  body+= "   set pos = instr(dname,ancfaxstr);\n";
  body+= "   if pos>0 then\n";
  body+= "    set pos=pos+length(ancfaxstr);\n";
  body+= "   else\n";
  body+= "    if anhfaxstr > '' then\n";
  body+= "     set pos = instr(dname,anhfaxstr);\n";
  body+= "     if pos>0 then\n";
  body+= "      set pos=pos+length(anhfaxstr);\n";
  body+= "     end if;\n";
  body+= "    end if;\n";
  body+= "   end if;\n";
  body+= "  end if;\n";
  body+= " end if;\n";
  body+= " if pos>0 then\n";
  body+= "  wlab: loop\n";
  body+= "   set ch = substring(dname,pos,1);\n";
  body+= "   if ch = '_' then leave wlab; end if;\n";
  body+= "   if instr('0123456789',ch) then set tel=concat(tel,ch);\n";
  body+= "   else if ch='+' then set tel=concat(tel,'00'); end if; end if;\n";
  body+= "   set pos=pos+1;\n";
  body+= "   if pos>length(dname) then leave wlab; end if;\n";
  body+= "  end loop;\n";
  body+= " end if;\n";
  body+= " return tel;\n";
  body+= "end";
  string mhost = host=="localhost"?host:"%";
  string owner=string("`")+usr+"`@`"+mhost+"`";
  for(uchar runde=0;runde<2;runde++) {
    uchar fehlt=1;
    char ***cerg;
    RS rs(Myp,"SHOW CREATE FUNCTION gettel3",2-obverb);
    while (cerg=rs.HolZeile(),cerg?*cerg:0) {
      for(uint i=1;i<=2;i++) {
        if (*(*cerg+i)) if (strstr(*(*cerg+i),body.c_str())) if (strstr(*(*cerg+i),owner.c_str())) {
          fehlt=0;
          break;
        }
      }
      break;
    } 
    //   RS rs(Myp,string("select definer from mysql.proc where definer like '`")+usr+"`@`"+mhost+"`'",255);
    if (fehlt) {
      DB *aktMyp;
      if (!runde) aktMyp=Myp; else {
        DB MySup(myDBS,Myp->host.c_str(),"root",Myp->rootpwd.c_str(),Myp->db.c_str(),0,0,0,obverb,oblog);
        aktMyp=&MySup;
      }
      string proc= "DROP FUNCTION IF EXISTS `gettel3`";
      RS rs0(aktMyp, proc);
      proc = "CREATE DEFINER="+owner+" FUNCTION `gettel3`\n"
        "(dname VARCHAR(1000), anfaxstr VARCHAR(100), ancfaxstr VARCHAR(100), anhfaxstr VARCHAR(100)) \n"
        "RETURNS VARCHAR(1000) CHARSET latin1 COLLATE latin1_german2_ci DETERMINISTIC\n";
      proc+=body;
      RS rs1(aktMyp, proc);
    } else 
      break; // runde
  }
} // void pruefprocgettel3(DB *Myp, string usr, string pwd, string host, int obverb, int oblog)


// ermittelt die letzten Sendedaten zu sendqgespfad mit fsf.capistat, schreibt die Zahl der Versuche in ctries zurueck und ergaenzt den 
// Anzeigezeiger ausgp
// wird aufgerufen in: untersuchespool, zeigweitere
void fsfcl::capiwausgeb(stringstream *ausgp, string *maxtries, int obverb, string *ctriesp, int oblog,unsigned long faxord)
{
  Log(violetts+Tx[T_capiwausgeb]+schwarz+"  capistat: "+blau+FxStatS(&capistat)+schwarz+ " maxtries: "+blau+*maxtries+schwarz,obverb,oblog);
  *ausgp<<blau<<endl;
  if (faxord) *ausgp<<faxord<<")";
  else *ausgp<<"  ";
  *ausgp<<"Capi: "<<schwarz;
  if (capistat==wartend) {
    *ausgp<<schwarz<<" "<<Tx[T_wartend]<<schwarz;
  } else if (capistat==gesandt) {
    *ausgp<<blau<<" "<<Tx[T_gesandt]<<schwarz;
  } else if (capistat==gescheitert) {
    *ausgp<<drot<<" "<<Tx[T_gescheitert]<<schwarz;
  } else if (capistat==fehlend) {
    //    *ausgp<<hgrau<<" "<<FxStatS(&capistat)<<schwarz; // bringt zur Zeit keinen Vorteil
    *ausgp<<hgrau<<" "<<Tx[T_nicht_in_der_Warteschlange]<<schwarz;
  }
  if (capistat!=fehlend) {
    size_t p1=sendqgespfad.rfind('.');
    if (p1) {
      string suchtxt = sendqgespfad.substr(0,p1)+".txt";
      cppSchluess cconf[]={{"tries"},{"starttime"},{"dialstring"}};
      size_t cs=sizeof cconf/sizeof*cconf;
      confdat capiconf(suchtxt,cconf,cs,obverb);
      if (1) {
        //    if (cpplies(suchtxt,cconf,cs)) KLA
        // RS rmod(My,string("update spool set capidials=")+cconf[0].val+" where id = "+*(*cerg+0),ZDB);
        if (ctriesp) *ctriesp=cconf[0].wert;
        char buf[255];
        int versuzahl=atol(cconf[0].wert.c_str());
        snprintf(buf,4,"%3d",versuzahl);
        *ausgp<<", "<<blau<<buf<<"/"<<*maxtries<<schwarz<<Tx[T_Anwahlen];
        //                      if (versuzahl>12) ausg<<"zu spaet, ";
        struct tm tm;
        memset(&tm, 0, sizeof(struct tm));
        strptime(cconf[1].wert.c_str(), "%a %b %d %H:%M:%S %Y", &tm);
        strftime(buf, sizeof(buf), "%d.%m.%y %H:%M:%S", &tm);
        *ausgp<<blau<<buf<<schwarz; 
        *ausgp<<", T.: "<<blau<<setw(12)<<cconf[2].wert<<schwarz; 
        *ausgp<<Tx[T_kommaDatei]<<rot<<sendqgespfad<<schwarz;
        *ausgp<<Tx[T_bzw]<<blau<<"*.txt"<<schwarz;
      }
    }
    if (ctriesp) if (ctriesp->empty()) *ctriesp="0";
  }
} // void fsfcl::capiwausgeb(stringstream *ausgp, string *maxtries, int obverb, string *ctriesp, int oblog,unsigned long faxord)


// wird aufgerufen in: setzhylastat
int paramcl::xferlog(const string& jobid, string *erg, int obverb, int oblog)
{
  // mit grep braucht er fuer eine 400 kb Datei ca. 170 clock()-Einheiten (vorne und hinten)
  // rueckwaerts braucht er fuer eine 400 kb Datei bis zum letzten Satz 93 clock()-Einheiten, bis zum ersten 220000.
  // vorwaerts braucht er fuer diese Datei ca. 9000 clock()-Einheiten
  // #define profiler
#ifdef profiler
  perfcl prf("xferlog");
#endif
  int gefunden=0;
  const char sep = 9; // geht auch: "[[:blank:]]"
  string datei=varsphylavz + "/etc/xferfaxlog"; 
#define mitgrep
#ifdef mitgrep
  svec grueck;
#define direkt
#ifdef direkt
  systemrueck(string("tac \"")+datei+"\" | grep -m 1 \""+this->hmodem+sep+jobid+sep+"\" | cut -f 14",obverb,oblog,&grueck); // ggf. broken pipe error
  if (grueck.size()) {
    gefunden=1;
    *erg=grueck[0];
#else
    systemrueck(string("tac \"")+datei+"\" | grep -m 1 \""+this->hmodem+sep+jobid+sep+"\"",obverb,oblog,&grueck); // ggf. broken pipe error
    if (grueck.size()) KLA
      vector<string> tok;
    aufSplit(&tok,&grueck[grueck.size()-1],sep);
#endif // direkt else
#else // mitgrep else
    ifstream f(datei.c_str(),ios::binary); 
    string zeile;
#define rueckwaerts
#ifdef rueckwaerts
    char ch;
    string rzeile;
    if (f.is_open()) KLA
      f.seekg(-1,ios::end); 
    for(;f.get(ch);f.seekg(-2,ios::cur)) KLA
      if (ch==10) KLA
        zeile.clear();
    for(size_t index=rzeile.length()-1;index!=string::npos;index--) KLA
      zeile+=rzeile[index];
    KLZ
#else // rueckwaerts
      while(getline(f,zeile)) KLA
#endif // rueckwaerts
        vector<string> tok;
    aufSplit(&tok,&zeile,sep);
#endif // mitgrep else
#ifndef direkt
    if (tok.size()>15) KLA
      if (tok[4]==jobid) KLA
        gefunden=1;
    *erg=tok[13];
    if ((*erg)=="\"\"") erg->clear();
    //            if (erg->length()>1) if ((*erg)[0]=='"' && (*erg)[erg->length()-1]=='"') *erg=erg->substr(1,erg->length()-2);
    if (obverb) KLA
      Log(blaus+"tok[13]: "+schwarz+*erg,obverb,oblog);
    KLZ
#ifndef mitgrep
#ifdef rueckwaerts
      break;
    KLZ // if (tok[4]==jobid) KLA
      KLZ // if (tok.size()>15)
      rzeile.clear();
    KLZ else KLA
      rzeile+=ch;
#endif // rueckwaerts
#endif // not mitgrep
    KLZ // 
      KLZ // if (ch==10)
#endif  // not direkt
  } // if f.is_open() oder (grueck.size()
#ifdef profiler
  prf.ausgeb();
#endif
  return gefunden;
} // void xferlog(string varsphylavz, string jobid, string *erg)

// wird aufgerufen in setzhylastat
void paramcl::setzhconfp(string *protdaktp,int obverb)
{
  static cppSchluess hconf[]={{"state"},{"totdials"},{"status"},{"statuscode"},{"!pdf"},{"tts"},{"number"},{"maxdials"},{"pdf"}};
  static size_t cs=sizeof hconf/sizeof*hconf;
  static string *alt_protdaktp=0;
  if (alt_protdaktp!=protdaktp) {
    confdat hylconf(*protdaktp,hconf,cs,obverb,':');
    alt_protdaktp=protdaktp;
  }
  hconfp= hconf;
} // paramcl::setzhconfp

// wird aufgerufen in paramcl::loeschefax, paramcl::untersuchespool, paramcl::zeigweitere
void paramcl::setzhylastat(fsfcl *fsf, string *protdaktp, uchar *hyla_uverz_nrp, int *obsfehltp, uchar startvznr,
    int obverb, int oblog, string *hylastatep, string *hyladialsp, string *hylastatusp, string *hylastatuscodep) 
{
  string startvznrs = ltoan(startvznr);
  Log(violetts+"hylanr: "+schwarz+fsf->hylanr+violetts+" "+Tx[T_setzhylastat]+schwarz+
      " hyla_uverz_nrp: "+blau+ltoan(*hyla_uverz_nrp)+schwarz+" startvznr: "+blau+startvznrs+schwarz,
      obverb,oblog);
  // wenn in *hyla_uverz_nrp '1' uebergeben wird, nur in sendq suchen
  // Rueckgabe: 0 = in doneq oder archive gefunden
  struct stat entryprot;
  string cmd=string("find ")+this->varsphylavz+"/sendq "+(*hyla_uverz_nrp?" ":this->varsphylavz+"/doneq "+this->varsphylavz+"/archive ")
    +" -name 'q"+fsf->hylanr+"'";
  svec rueck;
  systemrueck(cmd,obverb,oblog,&rueck);
  if (rueck.size()) {
    *protdaktp=rueck[0];
    *obsfehltp=lstat(protdaktp->c_str(), &entryprot);
    *hyla_uverz_nrp=rueck.at(0).find("/doneq")==string::npos && rueck.at(0).find("/archive")==string::npos;
  }
  if (obverb) {
    Log(schwarzs+"*obsfehltp: "+blau+(*obsfehltp?"1":"0")+schwarz+", hyla_uverz_nr: "+blau+(*hyla_uverz_nrp?"1":"0")+schwarz,obverb,oblog);
  }
  if (*obsfehltp) {
    this->hconfp=0;
    // wenn also die Datenbankdatei weder im Spool noch bei den Erledigten nachweisbar ist
    if (this->xferlog(fsf->hylanr, &fsf->hgerg,obverb,oblog)) {
      if (fsf->hgerg.empty()) {
        fsf->hylastat=gesandt;
      } else {
        fsf->hylastat=gescheitert;
      }
    } else {
      fsf->hylastat=fehlend;
    }
  } else {
    this->setzhconfp(protdaktp,obverb);
    //  if (cpplies(*protdaktp,hconf,cs,0,':')) KLA
    if (hylastatep) *hylastatep=this->hconfp[0].wert;
    if (hyladialsp) *hyladialsp=this->hconfp[1].wert;
    if (hylastatusp) *hylastatusp=this->hconfp[2].wert;
    if (this->hconfp[3].wert.empty()) this->hconfp[3].wert="0";
    if (hylastatuscodep) *hylastatuscodep=this->hconfp[3].wert;
    vector<string> tok;
    string pdf=this->hconfp[4].wert==""?this->hconfp[8].wert:this->hconfp[4].wert;
    aufiSplit(&tok,&pdf,":");
    fsf->sendqgespfad = this->varsphylavz + vtz + tok[tok.size()-1];
    //    struct stat entryh;
    //    lstat(sendqgespfad.c_str(),&entryh); 
    // 8, status gescheitert, evtl. unzureichend dokumentiert, aber wahr
    if (*hyla_uverz_nrp) {
      fsf->hylastat=wartend;
    }  else { // !hyla_uverz_nrp
      if (this->hconfp[0].wert=="8") {  
        fsf->hylastat=gescheitert;
        // 7, status erfolgreich
      } else if (this->hconfp[0].wert=="7") { 
        fsf->hylastat=gesandt;
      } else {
        fsf->hylastat=woasined;
      }
    }
  }
  Log(violetts+"hylastat: "+blau+FxStatS(&fsf->hylastat)+schwarz,obverb,oblog);

} // setzhylastat

// wird aufgerufen in untersuchespool und zeigweitere
void fsfcl::hylaausgeb(stringstream *ausgp, paramcl *pmp, int obsfehlt, int obverb, uchar obzaehl, int oblog)
{
  Log(violetts+Tx[T_hylaausgeb]+schwarz+"  hylastat: "+blau+FxStatS(&hylastat)+schwarz,obverb,oblog);
  *ausgp<<blau<<"\n";
  if (obzaehl) *ausgp<<++pmp->faxord<<")";
  else *ausgp<<"  ";
  *ausgp<<"Hyla: "<<schwarz;
  if (obsfehlt) {
    // wenn also die Datenbankdatei weder im Spool noch bei den Erledigten nachweisbar ist
    if (hylastat==gesandt) {
      *ausgp<<blau<<" "<<Tx[T_gesandt]<<schwarz;
    } else if (hylastat==gescheitert) {
      *ausgp<<blau<<" "<<Tx[T_gescheitert]<<" ("<<hgerg<<")"<<schwarz;
    } else if (hylastat==fehlend) {
      *ausgp<<hgrau<<" "<<Tx[T_nicht_in_der_Warteschlange]<<schwarz;
    }
  } else {
    if (hylastat==wartend) {
      *ausgp<<schwarz<<" "<<Tx[T_wartend]<<schwarz;
    } else if (hylastat==gescheitert) {
      *ausgp<<blau<<Tx[T_gescheitert]<<schwarz;
    } else if (hylastat==gesandt) {
      *ausgp<<blau<<" "<<Tx[T_gesandt]<<schwarz;
    }
  }
  // wenn eine Protokolldatei auslesbar war
  if (pmp->hconfp) {
    *ausgp<<", ";
    char buf[100];
    int hversuzahl=atol(pmp->hconfp[1].wert.c_str()); // totdials
    snprintf(buf,4,"%3d",hversuzahl);
    *ausgp<<blau<<buf<<schwarz<<"/"<<pmp->hconfp[7].wert<<Tx[T_Anwahlen];
    // hier muss noch JobReqBusy, JobReqNoAnswer, JobReqNoCarrier, JobReqNoFCon, JobReqOther, JobReqProto dazu gerechnet werden
    time_t spoolbeg=(time_t)atol(pmp->hconfp[5].wert.c_str());
    strftime(buf, sizeof(buf), "%d.%m.%y %H:%M:%S", localtime(&spoolbeg));
    *ausgp<<blau<<buf<<schwarz; 
    //              if (hversuzahl>12) ausg<<", zu spaet";
    *ausgp<<", T.: "<<blau<<setw(12)<<pmp->hconfp[6].wert<<schwarz;
    *ausgp<<Tx[T_kommaDatei]<<rot<<sendqgespfad<<schwarz;
  }
} // void fsfcl::hylaausgeb(stringstream *ausgp, paramcl *pmp, int obsfehlt, int obverb, uchar obzaehl, int oblog)

// Rueckgabe: Zahl der nicht Geloeschten
// wird aufgerufen in: loeschecapi, untersuchespool
int tuloeschen(const string& zuloe,int obverb, int oblog)
{
  Log(violetts+Tx[T_tuloeschen]+schwarz,obverb,oblog);
  struct stat entryzuloe;
  if (!lstat(zuloe.c_str(),&entryzuloe)) {
    Log(string(Tx[T_Loesche_Ausrufezeichen])+rot+zuloe+schwarz,obverb,oblog);
    return remove(zuloe.c_str());
  } 
  Log(rot+zuloe+schwarz+Tx[T_nicht_geloescht_war_eh_nicht_mehr_da],obverb,oblog);
  return 0;
} // int tuloeschen(string zuloe,int obverb, int oblog)

int main(int argc, char** argv) 
{
  paramcl pm(argc,argv); // Programmparameter
  pm.logvorgaben();
  pm.getcommandl0(); // anfangs entscheidende Kommandozeilenparameter abfragen
  pm.pruefhardware();
  pm.VorgbAllg();
  pm.VorgbSpeziell();
  pm.lieskonfein();
  
  if (!pm.getcommandline()) 
    exit(1);
  pm.rueckfragen();
  pm.setzhylavz();
  pm.verzeichnisse();
  pm.pruefcron();
  pm.pruefsamba();

  if (pm.logdateineu) remove(logdt);
  kuerzelogdatei("/var/log/capisuite.log",pm.obverb); // screen
  Log(string(Tx[T_zufaxenvz])+drot+pm.zufaxenvz+schwarz+"'",pm.obverb,pm.oblog);
  Log(string(Tx[T_Logpfad])+drot+pm.loggespfad+schwarz+Tx[T_oblog]+drot+ltoan((int)pm.oblog)+schwarz+")",pm.obverb,pm.oblog);
  if (pm.initDB())
    return 10;
  // pruefe Tabelle <spooltab> und stelle sie ggf. her
  pruefspool(pm.My,pm.spooltab, pm.obverb,pm.oblog);
  pruefouttab(pm.My,pm.touta, pm.obverb,pm.oblog);
  pruefinctab(pm.My,pm.tinca, pm.obverb,pm.oblog);
  if (pm.kez) {
    pm.korrerfolgszeichen();
  } else if (pm.bwv) {
    pm.bereinigewv();
  } else if (pm.lista) {
    pm.tu_lista("1");
  } else if (pm.listf) {
    pm.tu_lista("0");
  } else if (pm.listi) {
    pm.tu_listi();
  } else {
    pruefprocgettel3(pm.My,pm.muser,pm.mpwd,pm.host,pm.obverb,pm.oblog);
    //  int qerg = mysql_query(My.conn,proc.c_str());
    // 1) nicht-pdf-Dateien in pdf umwandeln, 2) pdf-Dateien wegfaxen, 3) alle in warte-Verzeichnis kopieren, 4) in Spool-Tabelle eintragen
    //  vector<string> npdf, spdf;
    pm.DateienHerricht();  
    if (pm.obfcard) if (pm.obcapi) pm.obcapi= !pm.pruefcapi();
    if (pm.obmodem) if (pm.obhyla) pm.obhyla= !pm.pruefhyla();
    Log(Tx[T_Verwende]+blaus+(pm.obcapi?"Capisuite":"")+schwarz+(pm.obcapi&&pm.obhyla?", ":"")+blau+(pm.obhyla?"Hylafax":"")+schwarz+
        (!pm.obcapi&&!pm.obhyla?(blaus+Tx[T_kein_Faxprogramm_verfuegbar]+schwarz):""),1,pm.oblog);
    if (pm.loef || pm.loew || pm.loea) {
      if (pm.loef) pm.loeschefax(pm.obverb,pm.oblog);
      if (pm.loew) pm.loeschewaise(pm.obverb,pm.oblog);
      if (pm.loea) pm.loescheallewartende(pm.obverb,pm.oblog);
    } else {

      // hier stehen obcapi und obhyla fest
      pm.faxealle();
      // Dateien in Spool-Tabelle nach inzwischen verarbeiteten durchsuchen, Datenbank- und Dateieintraege korrigieren 
      pm.untersuchespool();
      pm.zeigweitere();
      pm.empfarch();
      Log(blaus+Tx[T_Ende]+schwarz,pm.obverb,pm.oblog);
      pm.schlussanzeige(argv[0]);
    } // if (pm.loef || pm.loew || pm.loea) else
  } // if (pm.kez) else else else
  pm.autofkonfschreib();
  return 0;
} // int main(int argc, char** argv) 

// Log-Datei ueberpruefen
// Netzwerk-Freigabe
// Hylafax mit Fritzcard
// convert installieren, falls weder convert noch soffice da
// Empfang pruefen


// ausgabestring fuer hylafax befuellen
// fuer update eine Unterfunktion mit Felderweiterung schreiben
// im Fall erfolgreich erledigter Hylafaxe DAtenbankeintrag fuer capi korrigieren, im Fall gescheiterer klaeren, wie erneut reingestellt
// update auch fuer datenbankeinterne Felder ermoeglichen
// bei hyla die dateigroesse der pdf-Datei nehmen
