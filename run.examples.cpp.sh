./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -grayscale roi:10,5,100,250 -output output.grayscale.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -scale 0.75 -output output.scale.down.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -scale 1.9 -output output.scale.up.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -thresholdFilter threshold:0.6 roi:10,5,100,250 -output output.threshold.filter.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:false -output output.median.x.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:false y:true -output output.median.y.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:true -output output.median.xy.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/slope.jpg" -grayhistogram -output output.grayhistogram.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/slope.jpg" -grayhistogramequalization -output output.grayhistogramequalization.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/color/pen.jpg -histogramstretch -output output.histogramstretch.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/color/pen.jpg -histogramequalization -output output.histogramequalization.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/color/pen.jpg -hsihistogramequalization h:true s:true i:false -output output.hsihistogramequalization.hs.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/color/pen.jpg -hsihistogramequalization h:false s:true i:true -output output.hsihistogramequalization.si.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/color/pen.jpg -hsihistogramequalization h:true s:true i:true -output output.hsihistogramequalization.hsi.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/color/pen.jpg -hsihistogramequalization h:false s:false i:true -output output.hsihistogramequalization.i.png
