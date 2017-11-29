#ifndef MATRIX_ANIMATION_H
#define MATRIX_ANIMATION_H

#include <Arduino.h>
#include "RGBMatrix.h"

class MatrixAnimation {
 public:
  typedef enum {
    RGB24 = 0,
    RGB565_RLE = 1,
    RGB565 = 4,
#ifdef SUPPORTS_PALLETE_ENCODING
    INDEXED = 2,
    INDEXED_RLE = 3
#endif
  } Encoding;

  // Initialize the animation with no data. This is intended for the case
  // where the animation will be re-initialized from a memory structure in ROM
  // after the sketch starts.
  MatrixAnimation();

  // Initialize the animation
  // @param frameCount Number of frames in this animation
  // @param frameData Pointer to the frame data. Format of this data is encoding-specficic
  // @param encoding Method used to encode the animation data
  // @param ledCount Number of LEDs in the matrix
  // @param frameDelay Number of milliseconds to wait between frames
  MatrixAnimation(uint16_t frameCount,
            PGM_VOID_P frameData,
            Encoding encoding,
            uint16_t ledCount,
            uint16_t frameDelay);

  // Re-initialize the animation with new information
  // @param frameCount Number of frames in this animation
  // @param frameData Pointer to the frame data. Format of this data is encoding-specficic
  // @param encoding Method used to encode the animation data
  // @param ledCount Number of LEDs in the matrix
  // @param frameDelay Number of milliseconds to wait between frames
  void init(uint16_t frameCount,
            PGM_VOID_P frameData,
            Encoding encoding,
            uint16_t ledCount,
            uint16_t frameDelay);
 
  // Reset the animation, causing it to start over from frame 0.
  void reset();
  
  // Draw the next frame of the animation
  // @param matrix[] LED matrix to draw to.
  void draw(RGBMatrix &matrix);

  uint16_t getLedCount() const;
  uint16_t getFrameCount() const;
  uint16_t getFrameDelay() const;
  uint16_t getFrameIndex() const;
  void setFrameIndex(uint16_t frameIndex_);

  void decompress();

 private:
  uint16_t ledCount;              // Number of LEDs in the matrix
  uint16_t frameCount;            // Number of frames in this animation
  uint16_t frameDelay;            // Milliseconds to wait between frames

  Encoding encoding;              // Encoding type
  PGM_VOID_P frameData;           // Pointer to the begining of the frame data
  uint8_t bytesPerLED;            // number of bytes used to encode each LED's status
  
  uint16_t frameIndex;            // Current animation frame
  PGM_VOID_P currentFrameData;    // Pointer to the current position in the frame data

  uint8_t* decodedFrameData;      // frame data decoded from RLE

#ifdef SUPPORTS_PALLETE_ENCODING
  uint8_t colorTableEntries;      // Number of entries in the color table, minus 1 (max 255)
  struct CRGB colorTable[256];    // Color table

  void loadColorTable();          // Load the color table from memory
#endif

  typedef void (MatrixAnimation::*DrawFunction)(RGBMatrix &matrix);
  DrawFunction drawFunction;

  void drawRgb24(RGBMatrix &matrix);
  void drawRgb565_RLE(RGBMatrix &matrix);
  void drawRgb565(RGBMatrix &matrix);

#ifdef SUPPORTS_PALLETE_ENCODING
  void drawIndexed(RGBMatrix &matrix);
  void drawIndexed_RLE(RGBMatrix &matrix);
#endif
};

#endif
