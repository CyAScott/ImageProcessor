./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -grayscale roi:10,5,100,250 -output output.grayscale.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -scale 0.75 -output output.scale.down.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -scale 1.9 -output output.scale.up.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -thresholdFilter threshold:0.6 roi:10,5,100,250 -output output.threshold.filter.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:false -output output.median.x.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:false y:true -output output.median.y.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:true -output output.median.xy.png
