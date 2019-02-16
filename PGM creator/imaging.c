/**
 * @file image.c
 *
 * Implementierung der Bilddatenverarbeitung.
 * 
 * @author Andre Kloodt, Alexander Loeffler
 * 
 * gcc -ansi -pedantic -Wall -Wextra -Werror -DMAX_PIXEL_COUNT=255 -o ueb02 error.c imaging.c ueb02.c
 */

#include <assert.h>
#include <ctype.h>

#include "imaging.h"



Image imaging_createEmpty() 
{
  Image new;

  new.height = 0;
  new.width = 0;

  return new;
}



Image imaging_create(unsigned int width, unsigned int height, Grayscale background) 
{
  unsigned int i = 0;
  Image new;
  assert (width*height <= MAX_PIXEL_COUNT);

  new.height = height;
  new.width = width;
  
  for(i = 0; i < width*height; i++)
  {
	  new.pixels[i] = background;  
  }
  return new;
}



void imaging_delete(Image * img) 
{
  assert (img != NULL);
  
  (*img).height = 0;
  (*img).width = 0;
}



unsigned int imaging_getWidth(Image img) 
{
  return img.width;
}



unsigned int imaging_getHeight(Image img) 
{
  return img.height;
}



Grayscale imaging_getGrayscaleAt(Image img, unsigned int x, unsigned int y) 
{
  assert (x < img.width);
  assert (y < img.height);

  return img.pixels[y*img.width+x];
}



void imaging_setGrayscaleAt(Image * img, unsigned int x, unsigned int y, Grayscale gray) 
{
  assert (img != NULL);
  assert (x < (*img).width);
  assert (y < (*img).height);
 
  (*img).pixels[y*(*img).width+x] = gray;    
}



/** check the highest Grayscale value
 * 
 * @param img the picture
 * 
 * @return the length of the higest value
*/
unsigned char biggest_Grayscale(Image img)
{
  unsigned char bigGray, bigDigitCount = 0;
  unsigned int i = 0;
 
  for(i = 0; i < (img.height)*(img.width); i++)
  {
    if(img.pixels[i] > bigGray)
    {
      bigGray = img.pixels[i];
    }
  }
  if(bigGray >= 100)
  {
  
	  bigDigitCount = 3;
  }
  else if(bigGray >= 10) 
  {
	  bigDigitCount = 2;
  }
  else 
  {
	  bigDigitCount = 1;
  }
  return bigDigitCount;
}



/** check the length of the current value
 * 
 * @param value current value
 * 
 * @return the length of the current value
*/
unsigned char grayScale_Length(unsigned char value){
  unsigned char length = 0;
  
  if(value >= 100)
  {
	  length = 3;
  }
  else if(value >= 10) 
  {
	  length = 2;
  }
  else 
  {
	  length = 1;
  }
  return length;
}


Errorcode imaging_writePGM(Image img, FILE * stream, unsigned int * bytesWritten) 
{
  unsigned int x = 1,
               y = 0,
               z = 0,
               a = 0,
               tempBytes = 0;
  Errorcode err = ERR_NULL;
  unsigned char biggestGrey = biggest_Grayscale(img);
  unsigned int * tempWritten = &a;

  assert (stream != NULL);
  assert (bytesWritten != NULL);

  if(((*tempWritten) = fprintf(stream,"%s\n%d %d\n%d\n","P2",img.width,img.height,255)) > 0){
    (*bytesWritten) = (*tempWritten);

  for(y = 0;y < img.height*img.width; y++) 
  {  
    tempBytes = imaging_getGrayscaleAt(img,(y-z*img.width),z);
    /** sub the biggest GrayscaleValue and the current */
    if(biggestGrey - grayScale_Length(tempBytes) == 2)
    {
      if(((*tempWritten) = fprintf(stream,"  ")) > 0){
        (*bytesWritten) += (*tempWritten);
      }
	  }
	  else if (biggestGrey - grayScale_Length(tempBytes) == 1)
	  {
      if(((*tempWritten) = fprintf(stream," ")) > 0){
        (*bytesWritten) += (*tempWritten);
      }
	  }
    /** set the value with a following letter blank */
    if(((*tempWritten) = fprintf(stream,"%d ",tempBytes)) > 0){
      (*bytesWritten) += (*tempWritten);
    }

    if(y == img.width*x-1)
    {
      (*tempWritten) = fprintf(stream,"\n");
      if((*tempWritten) > 0){
        (*bytesWritten) += (*tempWritten);
      }
      x++;
      z++;
    }
  }
  }    
	return err;
}


Errorcode imaging_readPGM(Image * img, FILE * stream) 
{
  Errorcode err = ERR_NULL;
  unsigned char str1 = '\0',
                str2 = '\0';
  int tempWidth = 0,
      tempHeight = 0,
      lightValue = 0,
      greyValue = 0,
      x = 0,
      y = 0;

	assert (stream != NULL);
  assert (img != NULL);
    
  /** check the first line, if P2 */
  if(fscanf(stream,"%c%c",&str1,&str2) != 2 || str1 != 'P' || str2 != '2')
  {     
    err = ERR_PGM_INVALID_HEAD;
  }
  /** check the secound line, the Width and Height value */
  else if(fscanf(stream,"%d%d",&tempWidth,&tempHeight) != 2 || tempWidth*tempHeight > MAX_PIXEL_COUNT || 
          tempWidth <= 0 || tempHeight <= 0)
  {
    err = ERR_PGM_INVALID_HEAD;
  }
  /** check the third line, if the light value is 255 */ 
  else if(fscanf(stream,"%d",&lightValue) != 1 || lightValue != 255)
  {
    err = ERR_PGM_INVALID_HEAD;
  }
  else
  {  
    (*img) = imaging_create(tempWidth, tempHeight, 0);
    /** fill a new array with the new greyValues */
    while(x < tempWidth*tempHeight)
    {        
      if(fscanf(stream,"%d",&greyValue) != 1)
      {
        err = ERR_PGM_INVALID_DATA;
      }
      else if(greyValue < 0 || greyValue > 255 )
      {
        err = ERR_PGM_INVALID_DATA;
      }
      else
      {
        (*img).pixels[x] = greyValue;
        y++;
      }         
      x++;
    }

    if(x != y){
      err = ERR_IMAGE_SIZE;
    }

  }
  return err;
}
 
