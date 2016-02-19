#ifndef FILTERPIPE_H
#define FILTERPIPE_H

#include "masterHeaders.h"
#include "rawImage.h"

RawImage* process(vector<CommandLineArgModel*> args, RawImage* image);
void parseArguments(vector<CommandLineArgModel*> args);

#endif
