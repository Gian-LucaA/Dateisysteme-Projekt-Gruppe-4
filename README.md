# BetriebssystemeProjekt - Dateisysteme
## Ziele
Innerhalb dieses Projekts soll ein Framework zur Manipulation von Dateisystems implementiert werden. Es soll eine Schnittstelle (API) definiert und implementiert werden mit deren Hilfe Entwickler leicht bel. Anderungen innerhalb des Dateisystems vornehmen können.

## Arbeitsbeschreibung
Wie bereits im Punkt ”Ziele”beschrieben soll innerhalb dieses Projekts ein Framework fur Zugriffe auf das FAT/Inode-basiertes Dateisystem konzipiert und auch implementiert werden. Diese Framework soll einem Benutzer ermöglichen beliebige Operationen auf einem FAT/Inode Dateisystem vornehmen zu können. Fur dieses Projekt können Sie Ihre Lösung des 4 Praktikumszettel als Basis nehmen. Fur dieses Projekt ist es unabdingbar, dass das entsprechende Kapitel aus dem Buch ’Moderne Betriebssysteme’ vollständig verstanden wurde. Zum konzipieren der Funktionalität des Frameworks ist es wichtig das FAT/Inode Dateisystem in technischer Hinsicht verstanden zu haben. Des Weiteren muss eine Simulation von einer CD-Laufwerk implementiert. Diese muss Datei-Lesen/Schreiben/Brennen auf die/ von der CD erm¨oglichen. Folgende Punkte können Ihnen bei der Gestaltung Ihres Projektes helfen:

- Aufteilung der Platte (Layout von Dateisystemen) – Master Boot Record; MBR – Zwei verschiedene Dateisysteme; FAT, I-Node basiert
- Speicher Belegung; Verkettete Listen
- Verzeichnisse; FAT und I-Nodes
- Defragmentierung
- Statistische Anzeige der Plattenbelegung (Fragmentieungsstatus)
- Datei auf den / von dem CD-ROM schreiben / lesen
