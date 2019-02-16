#ifndef __imaging_H__
#define __imaging_H__

/**
 * @file image.h 
 *
 * Schnittstellendefinition der Bildverarbeitung.
 *
 * Diese Datei darf nicht veraendert werden. 
 * 
 * @author kar, mhe 
 */

#include <stdio.h>
#include "error.h"

/* Sofern nicht vom Compiler festgelegt, definiere eine maximale Laenge */
#ifndef MAX_PIXEL_COUNT
#define MAX_PIXEL_COUNT 10000
#endif

/** 
 *  Typdefintion fuer Graustufen
 */
typedef unsigned char Grayscale;


/**
 *  Typdefinition und Struktur fuer Bilder. 
 */
typedef struct {
  unsigned int width;
  unsigned int height;
  Grayscale pixels[MAX_PIXEL_COUNT];
} Image;

/**
 * Erzeugt ein leeres Bild mit den Abmessungen 0.
 *
 * @return Das leere Bild.
 */
Image imaging_createEmpty();
  
/**
 * Erzeugt ein Bild mit den uebergebenen Abmessungen (Breite, Hoehe) in
 * der angegegebenen Hintergrundfarbe.
 *
 * @param width Die Breite des zu erzeugenden Bildes
 * @param height Die Hoehe des zu erzeugenden Bildes
 * @param background Hintergrundfarbe.
 *
 * @pre width*height <= MAX_PIXEL_COUNT
 *
 * @return Das neu erzeugte Bild.
 */
Image imaging_create(unsigned int width, unsigned int height, Grayscale background);

/**
 * Loescht ein Bild, indem die Abmessungen auf 0 gesetzt werden.
 *
 * @param img Das zu loeschende Bild.
 * @pre img != NULL
 */
void imaging_delete(Image * img);



/**
 * Liefert die Breite des uebergebenen Bildes.
 *
 * @param img Das Bild.
 *
 * @return Die Breite des Bildes.
 */
unsigned int imaging_getWidth(Image img);

/**
 * Liefert die Hoehe des uebergebenen Bildes.
 *
 * @param img Das Bild.
 *
 * @return Die Hoehe des Bildes.
 */
unsigned int imaging_getHeight(Image img);

/**
 * Ermittelt den Grauwert an der uebergebenen Stelle des uebergebenen Bildes.
 *
 * @param img Das Bild.
 * @param x Die X-Koordinate.
 * @param y Die Y-Koordinate.
 *
 * @pre x < Bildbreite
 * @pre y < Bildhoehe
 *
 * @return Die ermittelte Graustufe.
 */
Grayscale imaging_getGrayscaleAt(Image img, unsigned int x, unsigned int y);

/**
 * Setzt einen Grauwert an der uebergebenen Stelle des uebergebenen Bildes.
 *
 * @param img Das Bild.
 * @param x Die X-Koordinate.
 * @param y Die Y-Koordinate.
 * @param gray Der zu setzende Grauwert.
 *
 * @pre img != NULL
 * @pre x < Bildbreite
 * @pre y < Bildhoehe
 */
void imaging_setGrayscaleAt(Image * img, unsigned int x, unsigned int y, Grayscale gray);

/**
 * Schreibt das uebergebene Bild in den uebergebenen Stream (z.B. eine Datei). 
 * Dabei wird zusaetzlich die Groesse des geschriebenen Dateistroms ermittelt 
 * und ueber den Parameter bytesWritten zurueckgegeben. Wenn bei der Verarbeitung
 * ein Fehler aufgetreten ist, so bleibt der Wert auf den bytesWritten zeigt 
 * unveraendert.
 *
 * Das Bild wird im Format P2 als PGM (Portable Graymap ASCII) geschrieben mit
 * einem Wert fuer die maximale Helligkeit von 255. 
 * Die Ausgabe der Bilddaten erfolgt zeilenweise und rechtsbuendig.
 *
 * @param img Das Bild.
 * @param stream Der Stream.
 * @param bytesWritten Zeiger auf die Anzahl der Bytes, die in die Dateistrom geschrieben 
 *                     wurden (Dateigroesse).
 *
 * @pre stream != NULL
 * @pre bytesWritten != NULL
 *
 * @return Errorcode ERR_NULL, wenn alles ok war
 *                   ERR_FILE_WRITE, falls nicht in den Dateistrom geschrieben werden konnte
 */
Errorcode imaging_writePGM(Image img, FILE * stream, unsigned int * bytesWritten);


/**
 * Liest ein PGM Bild im ASCII Format (P2) aus dem uebergebenen Dateistrom. Es
 * werden nur Bilder akzeptiert, deren Wert fuer die maximale Helligkeit 255 ist.
 *
 * @param img Zeiger auf das Bild.
 * @param stream Der Dateistrom.
 *
 * @pre img != NULL
 * @pre stream != NULL
 *
 * @return Errorcode ERR_NULL, wenn alles ok war
 *                   ERR_PPM_INVALID_HEAD, wenn der PGM Header ungueltig war
 *                   ERR_PPM_INVALID_DATA, wenn die Bilddaten ungueltig waren
 *                   ERR_IMAGE_SIZE, wenn das Bild in der Datei zu gross ist
 *                                   um gespeichert werden zu koennen 
 *                                   (Arraygroesse)
 */
Errorcode imaging_readPGM(Image * img, FILE * stream);


#endif
