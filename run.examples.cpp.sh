./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -grayscale roi:10,5,100,250 -output output.grayscale.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -scale 0.75 -output output.scale.down.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -scale 1.9 -output output.scale.up.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -thresholdFilter threshold:0.6 roi:10,5,100,250 -output output.threshold.filter.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:false -output output.median.x.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:false y:true -output output.median.y.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:true -output output.median.xy.png

./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/slope.jpg" -grayhistogram -output output.grayhistogram.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/slope.jpg" -grayhistogramequalization -output output.grayhistogramequalization.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input output.grayhistogramequalization.png -grayhistogram -output output.grayhistogramequalization.grayhistogram.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/pen.jpg -histogramstretch -output output.histogramstretch.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/pen.jpg -histogramequalization -output output.histogramequalization.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:true s:true i:false -output output.hsihistogramequalization.hs.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:false s:true i:true -output output.hsihistogramequalization.si.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:true s:true i:true -output output.hsihistogramequalization.hsi.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:false s:false i:true -output output.hsihistogramequalization.i.png

./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/floor01.jpg" -sobeledgedetection roi:10,5,100,250 -output output.sobeledgedetection.gray.roi.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/floor01.jpg" -sobeledgedetection threshold:150 -output output.sobeledgedetection.gray.threshold.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input "assets/Gray Level/floor01.jpg" -sobeledgedetection direction:0 -output output.sobeledgedetection.gray.direction.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb -output output.sobeledgedetection.rgb.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb colorChannel1:true colorChannel2:false colorChannel3:false -output output.sobeledgedetection.rgb.r.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb colorChannel1:false colorChannel2:true colorChannel3:false -output output.sobeledgedetection.rgb.g.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb colorChannel1:false colorChannel2:false colorChannel3:true -output output.sobeledgedetection.rgb.b.png
./src/ImageProcessor/ImageProcessorCpp/Release/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:hsi colorChannel1:false colorChannel2:false colorChannel3:true -output output.sobeledgedetection.hsi.i.png
