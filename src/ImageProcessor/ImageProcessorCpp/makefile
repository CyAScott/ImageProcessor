masterHeaders.o: masterHeaders.cpp
	g++ -Wall -c -std=c++11 masterHeaders.cpp -o Release/masterHeaders.o $(pkg-config opencv --cflags --libs)

arguments.o: arguments.cpp
	g++ -Wall -c -std=c++11 arguments.cpp -o Release/arguments.o $(pkg-config opencv --cflags --libs)

rawImage.o: rawImage.cpp
	g++ -Wall -c -std=c++11 rawImage.cpp -o Release/rawImage.o $(pkg-config opencv --cflags --libs)

grayscaleFilter.o: grayscaleFilter.cpp
	g++ -Wall -c -std=c++11 grayscaleFilter.cpp -o Release/grayscaleFilter.o $(pkg-config opencv --cflags --libs)

medianFilter.o: medianFilter.cpp
	g++ -Wall -c -std=c++11 medianFilter.cpp -o Release/medianFilter.o $(pkg-config opencv --cflags --libs)

thresholdFilter.o: thresholdFilter.cpp
	g++ -Wall -c -std=c++11 thresholdFilter.cpp -o Release/thresholdFilter.o $(pkg-config opencv --cflags --libs)

scaleFilter.o: scaleFilter.cpp
	g++ -Wall -c -std=c++11 scaleFilter.cpp -o Release/scaleFilter.o $(pkg-config opencv --cflags --libs)

grayHistogram.o: grayHistogram.cpp
	g++ -Wall -c -std=c++11 grayHistogram.cpp -o Release/grayHistogram.o $(pkg-config opencv --cflags --libs)

grayHistogramEqualization.o: grayHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 grayHistogramEqualization.cpp -o Release/grayHistogramEqualization.o $(pkg-config opencv --cflags --libs)

histogramStretch.o: histogramStretch.cpp
	g++ -Wall -c -std=c++11 histogramStretch.cpp -o Release/histogramStretch.o $(pkg-config opencv --cflags --libs)

histogramEqualization.o: histogramEqualization.cpp
	g++ -Wall -c -std=c++11 histogramEqualization.cpp -o Release/histogramEqualization.o $(pkg-config opencv --cflags --libs)

hsiHistogramEqualization.o: hsiHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 hsiHistogramEqualization.cpp -o Release/hsiHistogramEqualization.o $(pkg-config opencv --cflags --libs)

sobelEdgeDetection.o: sobelEdgeDetection.cpp
	g++ -Wall -c -std=c++11 sobelEdgeDetection.cpp -o Release/sobelEdgeDetection.o $(pkg-config opencv --cflags --libs)

openCvCannyEdgeDetection.o: openCvCannyEdgeDetection.cpp
	g++ -Wall -c -std=c++11 openCvCannyEdgeDetection.cpp -o Release/openCvCannyEdgeDetection.o $(pkg-config opencv --cflags --libs)

openCvGrayHistogramEqualization.o: openCvGrayHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 openCvGrayHistogramEqualization.cpp -o Release/openCvGrayHistogramEqualization.o $(pkg-config opencv --cflags --libs)

openCvHistogramEqualization.o: openCvHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 openCvHistogramEqualization.cpp -o Release/openCvHistogramEqualization.o $(pkg-config opencv --cflags --libs)

openCvSobelEdgeDetection.o: openCvSobelEdgeDetection.cpp
	g++ -Wall -c -std=c++11 openCvSobelEdgeDetection.cpp -o Release/openCvSobelEdgeDetection.o $(pkg-config opencv --cflags --libs)

openCvYCrCbHistogramEqualization.o: openCvYCrCbHistogramEqualization.cpp
	g++ -Wall -c -std=c++11 openCvYCrCbHistogramEqualization.cpp -o Release/openCvYCrCbHistogramEqualization.o $(pkg-config opencv --cflags --libs)

filterPipe.o: filterPipe.cpp
	g++ -Wall -c -std=c++11 filterPipe.cpp -o Release/filterPipe.o $(pkg-config opencv --cflags --libs)

program.o: program.cpp
	g++ -Wall -c -std=c++11 program.cpp -o Release/program.o $(pkg-config opencv --cflags --libs)

all : masterHeaders.o arguments.o rawImage.o grayscaleFilter.o medianFilter.o scaleFilter.o thresholdFilter.o grayHistogram.o grayHistogramEqualization.o histogramStretch.o histogramEqualization.o hsiHistogramEqualization.o sobelEdgeDetection.o openCvCannyEdgeDetection.o openCvGrayHistogramEqualization.o openCvHistogramEqualization.o openCvSobelEdgeDetection.o openCvYCrCbHistogramEqualization.o filterPipe.o program.o
	g++ -Wall Release/masterHeaders.o Release/arguments.o Release/rawImage.o Release/grayscaleFilter.o Release/medianFilter.o Release/scaleFilter.o Release/thresholdFilter.o Release/grayHistogram.o Release/grayHistogramEqualization.o Release/histogramStretch.o Release/histogramEqualization.o Release/hsiHistogramEqualization.o Release/sobelEdgeDetection.o Release/openCvCannyEdgeDetection.o Release/openCvGrayHistogramEqualization.o Release/openCvHistogramEqualization.o Release/openCvSobelEdgeDetection.o Release/openCvYCrCbHistogramEqualization.o Release/filterPipe.o Release/program.o -o Release/ImageProcessor $(pkg-config opencv --cflags --libs)
