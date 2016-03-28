#include "scaleFilter.h"

RawImage* ScaleFilter::scaleDown(double scale, RawImage* image)
{
	Size size = image->GetSize();

	int newHeight = (int)round((double)size.Height * scale);
	int newWidth = (int)round((double)size.Width * scale);
	RawImage* returnValue = new RawImage(newWidth, newHeight);
	Rectangle roi = fromSize(size);
	double scaleUp = 1.0 / scale;
	int scaleWindow = max((int)floor(scaleUp), 1);

	if (scaleWindow % 2 == 0) scaleWindow++;

	for (int y = 0; y < newHeight; y++)
		for (int x = 0; x < newWidth; x++)
		{
			int sourceX = (int)round((double)x * scaleUp);
			int sourceY = (int)round((double)y * scaleUp);

			returnValue->SetPixel(x, y, image->GetAverage(sourceX, sourceY, scaleWindow, scaleWindow, roi));
		}

	return returnValue;
}
RawImage* ScaleFilter::scaleUp(double scale, RawImage* image)
{
	Size size = image->GetSize();

	int newHeight = (int)round(size.Height * scale);
	int newWidth = (int)round(size.Width * scale);
	RawImage* returnValue = new RawImage(newWidth, newHeight);
	Rectangle roi = fromSize(size);
	double scaleDown = 1.0 / scale;

	for (int y = 0; y < newHeight; y++)
		for (int x = 0; x < newWidth; x++)
		{
			int sourceX = (int)round((double)x * scaleDown);
			int sourceY = (int)round((double)y * scaleDown);

			if (sourceX < roi.Width && sourceY < roi.Height)
				returnValue->SetPixel(x, y, image->GetPixel(sourceX, sourceY));
		}


	MedianFilterModel* model = new MedianFilterModel();
	model->hasRoi = true;
	model->Roi = fromSize(returnValue->GetSize());
	model->Window = (int)ceil(scale * 3.0);
	model->X = true;
	model->Y = true;

	CommandLineArgModel* command = new CommandLineArgModel();
	command->ParsedModel = model;

	return median->ProcessInput(command, returnValue);
}

RawImage* ScaleFilter::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	ScaleFilterModel* model = (ScaleFilterModel*)arg->ParsedModel;
	double value = model->Scale;

	if (value == 0) return new RawImage(0, 0);
	if (value < 1) return scaleDown(value, image);
	if (value > 1) return scaleUp(value, image);

	return image;
}
bool ScaleFilter::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("Scale", arg);
}
void ScaleFilter::ParseArgument(CommandLineArgModel* arg)
{
	ScaleFilterModel* model = new ScaleFilterModel();
	model->Scale = -1;

	for (unsigned int index = 0; index < arg->Parameters->size(); index++)
	{
		if (model->Scale < 0)
		{
			model->Scale = stod(trim(arg->Parameters->at(index)));
		}
	} 

	arg->ParsedModel = model;
}
