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

grayHistogram.o: grayHistogram.cpp
	g++ -Wall -c -std=c++11 grayHistogram.cpp -o Release/grayHistogram.o

grayHistogramEqualization.o: grayHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 grayHistogramEqualization.cpp -o Release/grayHistogramEqualization.o

histogramStretch.o: histogramStretch.cpp
	g++ -Wall -c -std=c++11 histogramStretch.cpp -o Release/histogramStretch.o
	
histogramEqualization.o: histogramEqualization.cpp
	g++ -Wall -c -std=c++11 histogramEqualization.cpp -o Release/histogramEqualization.o

hsiHistogramEqualization.o: hsiHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 hsiHistogramEqualization.cpp -o Release/hsiHistogramEqualization.o
	
sobelEdgeDetection.o: sobelEdgeDetection.cpp
	g++ -Wall -c -std=c++11 sobelEdgeDetection.cpp -o Release/sobelEdgeDetection.o
	
filterPipe.o: filterPipe.cpp
	g++ -Wall -c -std=c++11 filterPipe.cpp -o Release/filterPipe.o

program.o: program.cpp
	g++ -Wall -c -std=c++11 program.cpp -o Release/program.o

all : masterHeaders.o arguments.o rawImage.o grayscaleFilter.o medianFilter.o scaleFilter.o thresholdFilter.o grayHistogram.o grayHistogramEqualization.o histogramStretch.o histogramEqualization.o hsiHistogramEqualization.o sobelEdgeDetection.o filterPipe.o program.o
	g++ -Wall Release/masterHeaders.o Release/arguments.o Release/rawImage.o Release/grayscaleFilter.o Release/medianFilter.o Release/scaleFilter.o Release/thresholdFilter.o Release/grayHistogram.o Release/grayHistogramEqualization.o Release/histogramStretch.o Release/histogramEqualization.o Release/hsiHistogramEqualization.o Release/sobelEdgeDetection.o Release/filterPipe.o Release/program.o -o Release/ImageProcessor	
