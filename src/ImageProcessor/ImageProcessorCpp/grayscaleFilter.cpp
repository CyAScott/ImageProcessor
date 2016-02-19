#include "grayscaleFilter.h"

RawImage* GrayscaleFilter::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	GrayscaleFilterModel* model = (GrayscaleFilterModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;

	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
			image->SetPixel(x, y, image->GetBrightness(x, y));

	return image;
}
bool GrayscaleFilter::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("Grayscale", arg);
}
void GrayscaleFilter::ParseArgument(CommandLineArgModel* arg)
{
	GrayscaleFilterModel* model = new GrayscaleFilterModel();
	model->hasRoi = false;
	
	for (unsigned int index = 0; index < arg->Parameters->size(); index++)
	{
		if (!model->hasRoi)
		{
			Rectangle roi = parseRoi(arg->Parameters->at(index));
			if (roi.Bottom != -1)
			{
				model->hasRoi = true;
				model->Roi = roi;
			}
		}
	}

	arg->ParsedModel = model;
}
