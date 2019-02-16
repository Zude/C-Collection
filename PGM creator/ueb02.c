/**
 * @file ueb02.c 
 *
 * Hauptprogramm.
 * 
 * @author: andre kloodt, alexander loeffler
 * 
 */

#include <stdio.h>
#include <assert.h>

#include "error.h"
#include "imaging.h"

/**
 * Gibt die Hilfe in die uebergebene Datei beziehungsweise den uebergebenen Stream 
 * (stderr / stdout) aus.
 *
 * @param stream Datei bzw Stream in den geschrieben werden soll 
 * @pre stream != NULL
 */
void printUsage(FILE *stream)
{
  assert(stream != NULL);

  fprintf(stream, "%s\n", "Usage:");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb02 -h:");
  fprintf(stream, "%s\n", "    Show this help and exit.");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", " - or -");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb02 IMAGE_WIDTH IMAGE_HEIGHT BACKGROUND [OPERATION ...]");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "    IMAGE_WIDTH:  Image width (greater than zero)");
  fprintf(stream, "%s\n", "    IMAGE_HEIGHT: Image height (greater than zero)");
  fprintf(stream, "%s\n", "    BACKGROUND:   Background grayvalue (0..255)");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "  OPERATION(s):");
  fprintf(stream, "%s\n", "    -h HEIGHT SPACE GRAY");
  fprintf(stream, "%s\n", "       -> Create repeating horizontal lines with HEIGHT pixels in height,");
  fprintf(stream, "%s\n", "          SPACE pixels space and with grayvalue GRAY");
  fprintf(stream, "%s\n", "          HEIGHT must be greater than zero");
  fprintf(stream, "%s\n", "    -v WIDTH SPACE GRAY");
  fprintf(stream, "%s\n", "       -> Create repeating vertical lines with WIDTH pixels in width,");
  fprintf(stream, "%s\n", "          SPACE pixels space and with grayvalue GRAY");
  fprintf(stream, "%s\n", "          WIDTH must be greater than zero");
  fprintf(stream, "%s\n", "    -c HEIGHT HSPACE WIDTH VSPACE GRAY");
  fprintf(stream, "%s\n", "       -> Create horizontal and vertical lines with [WIDTH|HEIGHT] pixel");
  fprintf(stream, "%s\n", "          in width/height, [H|V]SPACE pixel space with grayvalue GRAY.");
  fprintf(stream, "%s\n", "          WIDTH and HEIGHT must be greater than zero");
  fprintf(stream, "%s\n", "    -o FILE");
  fprintf(stream, "%s\n", "       -> Output the current image into FILE");
  fprintf(stream, "%s\n", "          and print the amount of written bytes to stdout.");
  fprintf(stream, "%s\n", "    -r FILE");
  fprintf(stream, "%s\n", "       -> Read an image from the given FILE and write it repeatedly");
  fprintf(stream, "%s\n", "          into the current image.");
  fprintf(stream, "%s\n", "");
}

/**
 * Checks if the String is a valid integer
 *
 * @param[in] a String
 *
 * @return 1 if the String is a int, sscanf result if not
 */
int checkIfNumber(char *number)
{
  int x = 0;
  char y = '\0';

  return sscanf(number, "%d%c", &x, &y);
}



/**
 * Convert the givin String to an Integer, if possible
 *
 * @param[in] a String
 *
 * @return the integer, NULL if its not a Valid Int
 */
int convertToInt(char *stringer)
{
  int i = 0;
  char c = '\0';

  sscanf(stringer, "%d%c", &i, &c);
  return i;
}



/**
 * Checks if the givin String Array contains valid Integers
 *
 * @param[in] argv Array mit den Programmargumenten.
 * @param[in] argc Anzahl der Programmargumente.
 * @param[in] start Index where we like to start.
 * @param[in] end Index where we like to end.
 *
 * @return 1 if they are, 0 if they are not or arguments are missing
 */
int checkIfNumberArray(char *argv[], int argc, int start, int end)
{
  int returnValue = 1;

  if (argc > end)
  {
    for (; start <= end; start++)
    {
      if (checkIfNumber(argv[start]) != 1 || convertToInt(argv[start]) < 0)
      {
        returnValue = 0;
      }
    }
  }
  else
  {
    returnValue = 0;
  }

  return returnValue;
}



/**
 * Checks if the first 3 Arguments are Valid Image Data
 *
 * @param[in] argv[]
 * 
 * @return ERR_NULL if the String is a int, ERR_IMAGE_SIZE if not
 */
int checkArgImageInput(char *argv[])
{
  Errorcode err = ERR_NULL;

  if (checkIfNumber(argv[1]) != 1 || convertToInt(argv[1]) <= 0)
  {
    err = ERR_INVALID_ARG;
  }
  else if (checkIfNumber(argv[2]) != 1 || convertToInt(argv[2]) <= 0)
  {
    err = ERR_INVALID_ARG;
  }
  else if (checkIfNumber(argv[3]) != 1 || convertToInt(argv[3]) < 0 || convertToInt(argv[3]) > 255)
  {
    err = ERR_INVALID_ARG;
  }
  else if (convertToInt(argv[1]) * convertToInt(argv[2]) > MAX_PIXEL_COUNT)
  {
    err = ERR_IMAGE_SIZE;
  }

  return err;
}



/**
 * Add horizontal Lines to img
 *
 * @param[in] img, our image
 * @param[in] lineHeigth
 * @param[in] lineSpace
 * @param[in] grey, the grey value
 * 
 * @return return the img
 */
Image addHLines(Image *img, unsigned int lineHeigth, unsigned int lineSpace, unsigned char grey)
{
  unsigned int i = 0;
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int width = imaging_getWidth((*img));
  unsigned int height = imaging_getHeight((*img));

  for(; y < height; y++)
  {
    if(i < lineHeigth)
    {
      x=0;
      for(; x < width; x++)
      {
        imaging_setGrayscaleAt(img, x, y, grey);
      }
      i++;
    }
    else
    {
      if(lineSpace > 0){
        y+=(lineSpace-1);
      }
      else{
        y--;
      }
      i = 0;
    }
    
  }
  return (*img);
}



/**
 * Add vertival Lines to img
 *
 * @param[in] img, our image
 * @param[in] lineWidth
 * @param[in] lineSpace
 * @param[in] grey, the grey value
 * 
 * @return return the img
 */
Image addVLines(Image *img, unsigned int lineWidth, unsigned int lineSpace, unsigned char grey)
{
  unsigned int i = 0;
  unsigned int y = 0;
  unsigned int x = 0;
  unsigned int width = imaging_getWidth((*img));
  unsigned int height = imaging_getHeight((*img));

  for (; i < height; i++)
  {
    while (y < width)
    {
      for (; x < lineWidth; x++)
      {
        if (y < width)
        {
          imaging_setGrayscaleAt(img, y, i, grey);
          y++;
        }
      }

      x = 0;

      for (; x < lineSpace; x++)
      {
        if (y < width)
        {
        y++;
        }
      }

      x = 0;

    }

    y=0;

  }

  return (*img);
}


/**
 * Repeats writing the source Image source into the Image img
 *
 * @param[in] img, our image
 * @param[in] source, source image
 * 
 * @return void
 */
void repeat(Image * img, Image source){

    unsigned int a = 0,
                 b = 0,
                 k = 0,
                 p = 0;

      for(b = 0; b < imaging_getHeight((*img)); b++)
      {
        k=0;
        for(a = 0; a < imaging_getWidth((*img)); a++)
        {
          imaging_setGrayscaleAt(img,a,b, imaging_getGrayscaleAt(source,k,p));
          k++;
          if(k>=source.width)
          {
            k=0;
          }
        }
        p++;
        if(p>=source.height)
        {
          p=0;
        }
      }
}

/**
 * Processes all the Program Arguments
 *
 * @param[in] argc Anzahl der Programmargumente.
 * @param[in] argv Array mit den Programmargumenten.
 *
 * @return Errorcode.
 */
int processArguments(int argc, char *argv[])
{
  char x = '\0',
       y = '\0',
       z = '\0';
  int i = 0,
      writeChecker = 0;
  unsigned int ax = 3;
  unsigned int *ix = &ax;
  FILE * fp;
  Image new = imaging_createEmpty();
  Image new2 = imaging_createEmpty();
  Image *openImg = &new2;
  Image *img = &new;
  Errorcode err = ERR_NULL;

  (*img) = imaging_create(convertToInt(argv[1]), convertToInt(argv[2]), convertToInt(argv[3]));
      
  /** Check the Arguments and Process them **/
  for (i = 4; i < argc; i += 4)
  {
    x = '\0';
    y = '\0';
    z = '\0';

    /** Checks if the first Arguments is not like "-x" **/
    if (sscanf(argv[i], "%c%c%c", &x, &y, &z) != 2 || x != '-')
    {
      err = ERR_INVALID_ARG;
    }
    else
    {
      switch (y)
      {
      case 'h':
      {
        if (checkIfNumberArray(argv, argc, i + 1, i + 3) && convertToInt(argv[i + 1]) > 0 && convertToInt(argv[i + 3]) <= 255)
        {
          addHLines(img, convertToInt(argv[i + 1]), convertToInt(argv[i + 2]), convertToInt(argv[i + 3]));
        }
        else
        {
          err = ERR_INVALID_ARG;
        }
        break;
      }
      case 'v':
      {
        if (checkIfNumberArray(argv, argc, i + 1, i + 3) && convertToInt(argv[i + 1]) > 0 && convertToInt(argv[i + 3]) <= 255)
        {
          addVLines(img, convertToInt(argv[i + 1]), convertToInt(argv[i + 2]), convertToInt(argv[i + 3]));
        }
        else
        {
          err = ERR_INVALID_ARG;
        }
        break;
      }
      case 'c':
      {
        if (checkIfNumberArray(argv, argc, i + 1, i + 5) && convertToInt(argv[i + 1]) > 0 &&
            convertToInt(argv[i + 3]) > 0 && convertToInt(argv[i + 5]) <= 255)
        {
          addHLines(img, convertToInt(argv[i + 1]), convertToInt(argv[i + 2]), convertToInt(argv[i + 5]));
          addVLines(img, convertToInt(argv[i + 3]), convertToInt(argv[i + 4]), convertToInt(argv[i + 5])); 
        }
        else
        {
          err = ERR_INVALID_ARG;
        }

        i += 2;
        break;
      }
      case 'o':
      {
        if(argv[i+1] == NULL)
        {
          err = ERR_INVALID_ARG;
        }
        else
        {
          fp = fopen(argv[i+1], "wb");
          
          if (fp != NULL) {
            err = imaging_writePGM((*img), fp, ix);
            fprintf(stdout,"%d bytes written to file: %s.\n",(*ix),argv[i+1]);
            i -= 2;
            writeChecker = 1;
            fclose(fp);
          } else {
            err = ERR_INVALID_ARG;
          }
        }

        break;
      }
      case 'r':
      {
        if(argv[i+1] == NULL)
        {
          err = ERR_INVALID_ARG;
        }
        else
        {
          fp = fopen(argv[i+1], "rb");
          if(fp != NULL)
          {
            err = imaging_readPGM(openImg, fp);
            if(err == ERR_NULL){
              repeat(img, (*openImg));
            }
            i -= 2;
            fclose(fp);
          } else {
            err = ERR_INVALID_ARG;
          }
        }

        break;
      }
      default:
        err = ERR_INVALID_ARG;
      }
    }
    if (err != ERR_NULL)
    {
      i = argc;
    }
  }

  if(writeChecker == 0 && err == ERR_NULL)
  {
    err = imaging_writePGM((*img), stdout, ix);
  }

  return err;
}



/**
 * Hauptprogramm.
 *
 * @param[in] argc Anzahl der Programmargumente.
 * @param[in] argv Array mit den Programmargumenten.
 *
 * @return Fehlercode.
 */
int main(int argc, char *argv[])
{
  Errorcode error = ERR_NULL;
  char x = 0,
       y = 0,
       z = 0;

  /** Checks if the Argument is -h, otherwise continue **/
  if (argc == 2 && sscanf(argv[1], "%c%c%c", &x, &y, &z) == 2 && x == '-' && y == 'h')
  {
    printUsage(stdout);
  }
  else if (argc > 4) /** Was wenn nur aufruf 1 2 3 und kein operator ? **/
  {
    error = checkArgImageInput(argv);
    if (error == ERR_NULL)
    {
      error = processArguments(argc, argv);
    }
  }
  else
  {
    error = ERR_INVALID_ARG;
  }
  if(error != ERR_NULL)
  {
    printError(stderr, error);
    printUsage(stderr);
  }
  return error;
}
