masterHeaders.o: masterHeaders.cpp
	g++ -Wall -c -std=c++11 masterHeaders.cpp -o Release/masterHeaders.o
	
arguments.o: arguments.cpp
	g++ -Wall -c -std=c++11 arguments.cpp -o Release/arguments.o
	
rawImage.o: rawImage.cpp
	g++ -Wall -c -std=c++11 rawImage.cpp -o Release/rawImage.o

grayscaleFilter.o: grayscaleFilter.cpp
	g++ -Wall -c -std=c++11 grayscaleFilter.cpp -o Release/grayscaleFilter.o

medianFilter.o: medianFilter.cpp
	g++ -Wall -c -std=c++11 medianFilter.cpp -o Release/medianFilter.o

thresholdFilter.o: thresholdFilter.cpp
	g++ -Wall -c -std=c++11 thresholdFilter.cpp -o Release/thresholdFilter.o

scaleFilter.o: scaleFilter.cpp
	g++ -Wall -c -std=c++11 scaleFilter.cpp -o Release/scaleFilter.o
	
filterPipe.o: filterPipe.cpp
	g++ -Wall -c -std=c++11 filterPipe.cpp -o Release/filterPipe.o

program.o: program.cpp
	g++ -Wall -c -std=c++11 program.cpp -o Release/program.o

all : masterHeaders.o arguments.o rawImage.o grayscaleFilter.o medianFilter.o scaleFilter.o thresholdFilter.o filterPipe.o program.o
	g++ -Wall Release/masterHeaders.o Release/arguments.o Release/rawImage.o Release/grayscaleFilter.o Release/medianFilter.o Release/scaleFilter.o Release/thresholdFilter.o Release/filterPipe.o Release/program.o -o Release/ImageProcessor	
