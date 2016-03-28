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

		HsiImage ExportHsi(Rectangle roi);
		void ApplyHsi(Rectangle roi, HsiImage hsiImage);

		void Save(string path);

		RawImage* Clone();
		Size GetSize();

		Color GetAverage(int x, int y, int width, int height, Rectangle roi);
		Color GetPixel(int x, int y);
		SobelResult Sobel(int x, int y, Rectangle roi, vector<vector<int>> masks);
		SobelRgbColorResult SobelRgb(int x, int y, Rectangle roi, vector<vector<int>> masks);

		void SetPixel(int x, int y, Color color);
		void SetPixel(int x, int y, byte color);
		void SetPixel(int x, int y, byte r, byte g, byte b);
		void SetPixel(int x, int y, double h, double s, byte i);

		byte GetBrightness(int x, int y);

		void Grayscale(int x, int y);

		byte ThresholdFilter(int x, int y, byte threshold);

		HistogramResult GetHistogram(Rectangle roi);
};

#endif

