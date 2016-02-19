#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include "masterHeaders.h"

class RawImage
{
	private:
		vector<vector<byte>> rawBytes;
		byte getBrightness(int x, int y);
		Size size;

	public:
		RawImage(RawImage* rawImage);
		RawImage(string file);
		RawImage(int width, int height);
		void Save(string path);

		RawImage* Clone();
		Size GetSize();

		Color GetAverage(int x, int y, int width, int height, Rectangle roi);
		Color GetPixel(int x, int y);

		void SetPixel(int x, int y, Color color);
		void SetPixel(int x, int y, byte color);
		void SetPixel(int x, int y, byte r, byte g, byte b);

		byte GetBrightness(int x, int y);

		void Grayscale(int x, int y);

		byte ThresholdFilter(int x, int y, byte threshold);
};

#endif

