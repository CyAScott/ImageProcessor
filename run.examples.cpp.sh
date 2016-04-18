./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -grayscale roi:10,5,100,250 -output output.grayscale.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -scale 0.75 -output output.scale.down.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -scale 1.9 -output output.scale.up.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -thresholdFilter threshold:0.6 roi:10,5,100,250 -output output.threshold.filter.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:false -output output.median.x.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:false y:true -output output.median.y.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/11.jpg -median window:101 roi:10,5,100,250 x:true y:true -output output.median.xy.png

./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/slope.jpg" -grayhistogram -output output.grayhistogram.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/slope.jpg" -grayhistogramequalization -output output.grayhistogramequalization.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input output.grayhistogramequalization.png -grayhistogram -output output.grayhistogramequalization.grayhistogram.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -histogramstretch -output output.histogramstretch.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -histogramequalization -output output.histogramequalization.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:true s:true i:false -output output.hsihistogramequalization.hs.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:false s:true i:true -output output.hsihistogramequalization.si.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:true s:true i:true -output output.hsihistogramequalization.hsi.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -hsihistogramequalization h:false s:false i:true -output output.hsihistogramequalization.i.png

./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -sobeledgedetection roi:10,5,100,250 -output output.sobeledgedetection.gray.roi.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -sobeledgedetection threshold:150 -output output.sobeledgedetection.gray.threshold.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -sobeledgedetection direction:0 -output output.sobeledgedetection.gray.direction.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb -output output.sobeledgedetection.rgb.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb colorChannel1:true colorChannel2:false colorChannel3:false -output output.sobeledgedetection.rgb.r.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb colorChannel1:false colorChannel2:true colorChannel3:false -output output.sobeledgedetection.rgb.g.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:rgb colorChannel1:false colorChannel2:false colorChannel3:true -output output.sobeledgedetection.rgb.b.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:hsi colorChannel1:false colorChannel2:false colorChannel3:true -output output.sobeledgedetection.hsi.i.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:hsi colorChannel1:false colorChannel2:true colorChannel3:false -output output.sobeledgedetection.hsi.s.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:hsi colorChannel1:true colorChannel2:false colorChannel3:false -output output.sobeledgedetection.hsi.h.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/garden.jpg -sobeledgedetection color:hsi colorChannel1:true colorChannel2:true colorChannel3:true -output output.sobeledgedetection.hsi.png

./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/slope.jpg" -opencvhistogramequalization -output output.grayhistogramequalization.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input output.grayhistogramequalization.png -grayhistogram -output output.grayhistogramequalization.grayhistogram.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -opencvycrcbhistogramequalization y:true -output output.opencvycrcbhistogramequalization.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -opencvycrcbhistogramequalization y:true Cr:true Cb:false -output output.hsihistogramequalization.yCr.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -opencvycrcbhistogramequalization y:false Cr:true Cb:true -output output.hsihistogramequalization.CrCb.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -opencvycrcbhistogramequalization y:true Cr:true Cb:true -output output.hsihistogramequalization.yCrCb.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input assets/Color/pen.jpg -opencvycrcbhistogramequalization y:false Cr:false Cb:true -output output.hsihistogramequalization.Cb.opencv.png

./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -opencvsobeledgedetection roi:10,5,100,250 -output output.sobeledgedetection.gray.roi.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -opencvsobeledgedetection threshold:150 -output output.sobeledgedetection.gray.threshold.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -opencvsobeledgedetection direction:0 -output output.sobeledgedetection.gray.direction.opencv.png

./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -opencvcannyedgedetection roi:10,5,100,250 -output output.opencvcannyedgedetection.gray.roi.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -opencvcannyedgedetection threshold:150 -output output.opencvcannyedgedetection.gray.threshold.opencv.png
./src/ImageProcessor/ImageProcessorCpp/ImageProcessor -input "assets/Gray Level/floor01.jpg" -opencvcannyedgedetection direction:0 -output output.opencvcannyedgedetection.gray.direction.opencv.png
