#ifndef INODESYSTEM_H
#define INODESYSTEM_H

#include <vector>
#include <string>
#include <unordered_map>
#include "INode.h"
#include "Data/DataBlock.h"

class INodeSystem {
public:
 INodeSystem() {
 };

 /**
  * @brief createSystem Erstellt ein neues System.
  * @param diskSize Die Festplattengröße.
  * @param blockSize Die Größe eines Blocks.
  * @return Das erstellte System.
  */
 static INodeSystem *createSystem(size_t diskSize, size_t blockSize);

 /**
  * @brief setNewSizeToFolder Setzt allen übergeordneten Ordnern die Größe eines neuen Files an.
  * @param path Pfad der Datei.
  * @param fileSize Größe der Datei.
  */
 void setNewSizeToFolder(string path, int fileSize);

 /**
  * @brief getDirectoryTree Methode zur Ermittlung des Ordner-Baums mit Dateien und Ordnern.
  * @param paths Ein Vector zur Speicherung der Pfade.
  * @param currentPath Der aktuelle Pfad.
  * @param currentINode Die Aktuelle INode.
  */
 void getDirectoryTree(std::vector<std::string> *paths, std::string currentPath, INode* currentINode);

 /**
  * @brief isFile prüft, ob es sich anhand der INode-Nummer um eine Datei handelt.
  * @param iNodeNumber Die Nummer der INode
  * @return true, wenn es eine Datei ist, andernfalls false
  */
 bool isFile(int iNodeNumber);

 /**
  * @brief addFileToDirectory Fügt einen Dateindex in einen Ordner ein.
  * @param path Der Pfad des Ordners.
  * @param iNodeFileNumber Der Datei-Index.
  */
 void addFileToDirectory(string path, int iNodeFileNumber);

 /**
  * @brief getFreeBlockCount Ermittelt die Anzahl freier Datenblöcke im System.
  * @return Anzahl der freien Blöcke.
  */
 int getFreeBlockCount();

 /**
  * @brief block_is_occupied prüft, ob es sich bei dem index um einen belegten Datenblock handelt.
  * @param data_block_index Der Index des Datenblocks.
  * @return Bei erfoglreicher Prüfung wird der Index der INode zurüfgeliefert, andernfalls -1.
  */
 int block_is_occupied(size_t data_block_index);

 /**
  * @brief defragDisk defragmentiert die Festplatte, sodass zusammengehörige Datenblöcke hintereinander gelegt werden.
  * @param callCount Die aktuelle Aufrufzahl der Mehtode.
  */
 void defragDisk(int callCount);

 /**
  * @brief getFragmentation Ermittelt die Fragmentierung der Datenblöcke.
  * @return einen Prozentwert der Fragmentierung.
  */
 float getFragmentation();

 /**
  * @brief getInodeDirFromPath Ermittelt anhand des Dateipfades, ob es sich um einen Ordner handelt.
  * @param path Der Dateipfad.
  * @return Bei Erfolg, das Ordner-Objekt, andernfalls nullptr.
  */
 INode *getInodeDirFromPath(string path);

 /**
  * @brief getInodeFileFromPath Ermittelt anhand des Dateipfades, ob es sich um eine Datei handelt.
  * @param path Der Dateipfad.
  * @return Bei Erfolg, das Datei-Objekt, andernfalls nullptr.
  */
 INode *getInodeFileFromPath(string path);

 /**
  * @brief getINodeByIndex Ermittelt anhand des Indexes das passende INode-Objekt.
  * @param searchIndex Der zu suchende INode-Index.
  * @return Bei Erfolg das INode-Objekt, andernfalls nullptr.
  */
 INode *getINodeByIndex(int searchIndex);

 /**
  * @brief getNameByIndex Ermittelt anhand des INode-Index den Namen der Datei.
  * @param index Der INode-Index.
  * @return Bei Erfolg den Namen der Datei, andernfalls leer string.
  */
 std::string getNameByIndex(int index);

 /**
  * @brief getIndexByName Ermittelt anhand des Datei/Ordner-Names den passenden Index.
  * @param name Der Datei/Ordner-Name.
  * @return Bei Erfolg den INode-Index, andernfalls -1.
  */
 int getIndexByName(string name);

 /**
  * @brief getIndexByName Ermittelt anhand des Datei/Ordner-Names aus Ordnereinträgen den passenden Index.
  * @param name Der Datei/Ordner-Name.
  * @param directory_entries Die Ordnereinträge.
  * @return Bei Erfolg den Datei/Ordner-Index, andernfalls -1.
  */
 int getIndexByName(string name, std::unordered_map<string, int> directory_entries);

 /**
  * @brief getRootDir Gibt das root-Verzeichnis zurück.
  * @return Das root-Verzeichnis.
  */
 INode* getRootDir();

 int nextINodeNumber = 0;
 std::vector<INode> iNodes;
 DataBlock *data_blocks;

 size_t diskSize; // Größe der gesamten Platte
 size_t blockSize; // Größe eines Blocks
 size_t blockCount; // Anzahl der Blöcke auf der Platte
};

#endif // INODESYSTEM_H
