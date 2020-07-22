# Hyperspectral_Image_Processing
It shows hyperspectral image processing. Hyperspectral processing technology is a powerful tool for detecting chemical substances, anomalies, and camouflaged objects, as well as for visual surveillance. These applications require low complexity and high throughput. 

## Basic Idea
Conventional image processing cannot detect an object embedded in complex background. But hyperspectral cameras can get multiple colors which provides more information about objects but requires high complexity to analysis. Bellow picture shows the type of Image data represented in dozens or hundreds of narrow and adjacent spectral bands.

![image](https://user-images.githubusercontent.com/52392004/88082589-eab14e00-cbbc-11ea-8361-7ad41d32f9fd.png)

The objectives of design are to assure high-speed operation for detection and update. Our algorithm has two kinds of data dependency. First, update and detection are sequential. The results of the update are the index of effective bands and refined libraries which is used in the detection. Thus the detection cannot start until the operation of the previous cube image is completed. The sequential property prevents the parallel operation of update and detection. Second, the update needs target and background samples, but the type of samples is decided after the detection. Therefore, update and detection require two different cube images.

#### Detection: Correlation: 75%

The main challenge of general hyperspectral image processing is the backside of its advantages: high volume and complexity of hyperspectral data. The performance
of detection depends on the quality of spectral information stored in the target library. The main operation in the hyperspectral processing for target detection is to compare the input cube with the target library to determine correlation in terms of spectra. The detection is based on perceptual segmentation where spectra contents for each subband are correlated with the spectra contents stored in the library. However, not all bands are necessary since some may contain redundant information where they are compared to the target library. The easiest approach is to reduce the number of bands and the amount of library for processing. 

![image](https://user-images.githubusercontent.com/52392004/88082931-614e4b80-cbbd-11ea-8233-c4b515a05461.png)

#### Algorithm

The main operation in the hyperspectral image processing for target detection is to compare input cube images with the target library, which determines the correlation coefficient in terms of spectrum contents.  Hence the main operation in hyperspectral image processing is the calculation of correlation. The correlation coefficient is a measure of similarity between two spectrum contents which are stored in the target library and obtained from input images. High values of the correlation coefficient between two spectrum contents indicate the high degree of similarity between the two spectrum contents.

![image](https://user-images.githubusercontent.com/52392004/88083106-9b1f5200-cbbd-11ea-8fd2-cbba9eede2b5.png)


### Reference
https://link.springer.com/content/pdf/10.1155/2007/82874.pdf
https://www.researchgate.net/publication/220058244_Real-Time_Target_Detection_Architecture_Based_on_Reduced_Complexity_Hyperspectral_Processing
