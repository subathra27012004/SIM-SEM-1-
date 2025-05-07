// Name : Subathra Sundarbabu 
// Student ID : 10263704
// Module Code : CM 2030
// Module Name : Graphics Programming
// Date : 10th March 2025

/** Commentary
1. Results of thresholding for RGB channels
Using sliders to apply thresholding independently to the red, green, and blue channels produced different results due to the RGB 
color model's representation of color. The red channel thresholding brought out the highlights of objects with strong red components, 
while the green and blue channels emphasized areas dominated by their respective colors. Depending on the dominant color, objects 
appear differently in each channel due to the varying intensity values.

2. Comparison of Thresholds 
Different RGB thresholding results were obtained using color space conversion between YCbCr, HSV, and HSL. By separating luminance 
from chrominance, YCbCr reduced sensitivity to lighting changes and enhanced contrast. Color segmentation and object distinction were 
enhanced by HSV, which prioritized hue over intensity. Edge detection and color separation were enhanced by HSL. These methods, which 
separated brightness from chromatic components, reduced noise and enhanced segmentation compared to RGB, leading to more consistent 
thresholding in various lighting scenarios.

3. Problems faced and Solutions
The inconsistencies in color thresholding were addressed with adjustable sliders. Background segmentation occasionally produced objects,
which were reduced by slightly blurring the segmentation mask. Another challenge was maintaining a visually structured and orderly 
display, which required careful calculations to ensure that the grid was positioned correctly. 

4. Project Completion and Areas for Improvement
The project was completed successfully. Overall, this project increased my understanding of image processing techniques, color space 
conversion, and thresholding. The experience has enhanced my skills in image processing and application development.

5. Extensions
  1. HSL colour space conversion and thresholding 
  The addition of thresholding and HSL color space conversion improved color-based image processing. With HSL, color and 
  brightness can be separated for more precise adjustments. By focusing on hue instead of total brightness, this enhancement 
  improved color filtering and object segmentation, ensuring more accurate results.

  2. Background segmentation with colour-changing feature
  Using the color wheel button, users can alter the background color of the background segmentation extension, which is based on the 
  background subtraction technique. Users can dynamically select a color to customize the image background.

  3. Load external images
  At first, image processing could only be done using live webcam input, but with this extension, users can upload their own photos and 
  use the same filters and adjustments.

  4. Capture button
  A capture button was included to facilitate users' ability to store their processed images. This feature captures the entire filtered 
  grid as a JPEG file, allowing users to save the applied transformations for later use.

  5. Zoom-in/zoom-out filter
  Users can use zoom-in and zoom-out features to examine individual filters. When multiple photos and transformations were displayed 
  in a grid, some details were hard to see. The zoom-in feature will allow users to click on a specific processed image to enlarge it,
  making it easier to see the effects of thresholding and color space conversion.

*/

// Global Variable
let video; // Store the webcam video
let scaled_width = 160; // Scale that image to 160 x 120 pixels 
let scaled_height = 120;
let Img_loaded = null; // Store the loaded Image
let c_index; // channel index
let red_slider , green_slider, blue_slider; // Slider for the color channels
let detector; // Variable for face detection
let classifier = objectdetect.frontalface; // Face detection classifier
var faces; // Store detected face
let mode = "original"; // Default mode
let visual; // Variable for visual
let show_info = false; // toggle state of info
let padding = 50; // padding

///////////////////////////////////////////////////////
///////////          Basic SetUp         //////////////
///////////////////////////////////////////////////////

/** Preload function */
function preload() {
  // body segmentation using ml5
  body_segmentation = ml5.bodySegmentation();
}

/** Set up function */
function setup() {
  // Create a canvas with windows width and height
  createCanvas(windowWidth, windowHeight);

  // Webcam function to display the webcam
  WebCam();

  // Style function for the buttons
  style();

  // Slider and position of the red channel
  red_slider = createSlider(0, 255, 125);
  red_slider.position((width / 4) - scaled_width - 70, (height / 2) + scaled_height * 2 - 170);
  
  // Slider and position of the green channel
  green_slider = createSlider(0, 255, 125);
  green_slider.position((width / 4) - 70, (height / 2) + scaled_height * 2 - 170);
  
  // Slider and position of the blue channel
  blue_slider = createSlider(0, 255, 125);
  blue_slider.position((width / 4) + scaled_width - 70, (height / 2) + scaled_height * 2 - 170);

  // Slider and position of color space conversion 1 
  ycrcb_slider = createSlider(0, 255, 125);
  ycrcb_slider.position((width / 4) - 230, (height / 2) + scaled_height * 2 - 100);

  // Slider and position of color space conversion 2
  hsv_slider = createSlider(0, 255, 20);
  hsv_slider.position((width / 4) + scaled_width - 230, (height / 2) + scaled_height * 2 - 100);

  // Slider and position of color space conversion 3
  hsl_slider = createSlider(0, 125, 20);
  hsl_slider.position((width / 4) + scaled_width - 70, (height / 2) + scaled_height * 2 - 100);

  // Detector for face detection
  detector = new objectdetect.detector(scaled_width, scaled_height, 1.2, classifier);

  // Start detecting segmentation on the webcam
  body_segmentation.detectStart(video, results);
}

/** Draw function */
function draw() {
  // Background color set to light blue
  background(173, 216, 230);

  push();
  fill(0); // fill black color
  textSize(42); // Set text size to be 42
  textAlign(CENTER, TOP); // Align text to center top
  text("Subathra's", width / 4, 340); // Position of the text
  text("Image Processing App", width / 4, 390); // Position of the text
  pop();

  // Push pop for the text
  push();
  fill(0); // fill black color
  textSize(20); // Set text size to be 20 
  textAlign(CENTER, TOP); // Align text to center top
  text("Student ID: 10263704", width / 4, 450); // Postion of the text
  text("Red", (width / 4) - scaled_width, (height / 2) + scaled_height * 2 - 200); // Postion of the label
  text("Green", (width / 4), (height / 2) + scaled_height * 2 - 200); // Postion of the label
  text("Blue", (width / 4) + scaled_width, (height / 2) + scaled_height * 2 - 200); // Postion of the label
  text("YCBRC ", (width / 4) + scaled_width - 320, (height / 2) + scaled_height * 2 - 130); // Postion of the label
  text("HSV", (width / 4) + scaled_width - 170, (height / 2) + scaled_height * 2 - 130); // Postion of the label
  text("HSL", (width / 4) + scaled_width, (height / 2) + scaled_height * 2 - 130); // Postion of the label
  pop();

  // Information to be displayed when the info button is clicked
  if (show_info) {
    fill(255); // White background for the box
    stroke(0); // Set stroke color to black
    rect(width / 4 - 530, 80, 510, 260, 10); // rounded rectangle box

    fill(0); // Black text
    noStroke(); // No stroke
    textSize(16); // Set text size to be 16
    text("Capture : Capture the grid visuals", 300, 120); // Position of the text
    text("Load : Load images from your desktop", 300, 150); // Position of the text
    text("Color Wheel : Change the background color of the visual", 300, 180); // Position of the text
    text("X Cancel : To remove the image loaded and return to webcam live", 300 , 210); // Position of the text
    text("Filter image : Press key 1 to 6 to activate different filters.", 300, 240); // Position of the text
    text("Press 0 to return back to live", 300, 270); // Position of the text
    text("Zoom In : Click on the individual filters", 300, 300); // Position of the text
    text("Zoom Out : Press ESC keyy", 300, 330); // Position of the text
  }

  if (zoom && filter_select) {
    visual_update(); // Update the visual when it is zoomed
    push();
    fill(0); // Fill with black color
    textSize(42); // Set text size to be 42
    textAlign(CENTER, TOP); // Align the text to center top
    text(filter_select.title, width / 2 + 500, 150); // Position of the title of the filter when it is zoomed
    pop();

    push(); // Push pop for the zoomed filter
    if (visual === Img_loaded) {
      translate(width / 2 - scaled_width * 1.5 / 2 + 200, height / 2 - scaled_height * 1.5 / 2 - 200); // translate the filter to the middle
      scale(5); // scale image to 5
    } else {
      translate(width / 2 - scaled_width * 1.5 / 2 + 1030, height / 2 - scaled_height * 1.5 / 2 - 200); // translate the filter to the middle
      scale(-5, 5); // If the visual is webcam, mirror the webcam when zoomed
    }

    if (filter_select.title === "Face Detection") { // when the title is face detection
      image(visual, 0, 0, scaled_width, scaled_height); // original web cam
      facedetection(visual, 0, 0); // apply face detection filter
    } else if (filter_select.title === "Red Channel") { // when the title is red channel
      image(extractColorChannel(visual, 'R'), 0, 0, scaled_width, scaled_height); // apply red channel filter
    } else if (filter_select.title === "Green Channel") { // when the title is green channel
      image(extractColorChannel(visual, 'G'), 0, 0, scaled_width, scaled_height); // apply green channel filter
    } else if (filter_select.title === "Blue Channel") { // when the title is blue channel
      image(extractColorChannel(visual, 'B'), 0, 0, scaled_width, scaled_height); // apply blue channel filter
    } else if (filter_select.title === "Threshold Red") { // when the title is threshold red
      image(thresholdFilter(visual, 'R', red_slider.value()), 0, 0, scaled_width, scaled_height); // apply red threshold filter
    } else if (filter_select.title === "Threshold Green") { // when the title is threshold greem
      image(thresholdFilter(visual, 'G', green_slider.value()), 0, 0, scaled_width, scaled_height); // apply green threshold filter
    } else if (filter_select.title === "Threshold Blue") { // when the title is threshold blue
      image(thresholdFilter(visual, 'B', blue_slider.value()), 0, 0, scaled_width, scaled_height); // apply blue threshold filter
    } else if (filter_select.title === "Grayscale") { // when the title is greyscale
      image(greyscaleFilter(visual), 0, 0, scaled_width, scaled_height); // apply greyscale filter
    } else if (filter_select.title === "YCbCr") { // when the title is ycbcr
      image(ycbcrConversion(visual), 0, 0, scaled_width, scaled_height); // apply ycbcr color space conversion filter
    } else if (filter_select.title === "HSV") { // when the title is hsv
      image(hsvConversion(visual), 0, 0, scaled_width, scaled_height); // apply hsv color space conversion filter
    } else if (filter_select.title === "FilteredImage") { // when the title is filteredimage
      image(Transformation(visual, mode), 0, 0, scaled_width, scaled_height); // apply the filtered image filters
      if (mode === "facedetection") facedetection(Transformation(visual, mode), 0, 0); // apply face detection for the filtered image when 1 is clicked
    } else if (filter_select.title === "ThresholdYCbCr") { // when the title is thresholdycbcr
      image(ycbcrThreshold(visual, 'YCbCr', ycrcb_slider.value()), 0, 0, scaled_width, scaled_height); // apply ycbcr threshold filter
    } else if (filter_select.title === "ThresholdHSV") { // when the title is thresholdhsv
      image(hsvThreshold(visual, 'HSV', hsv_slider.value()), 0, 0, scaled_width, scaled_height); // apply hsv threshold filter
    } else if (filter_select.title === "HSL") { // when the title is hsl
      image(hsl(visual), 0, 0, scaled_width, scaled_height); // apply hsl filter
    } else if (filter_select.title === "Threshold HSL") { // when the title is thresholdhsl
      image(hslThreshold(visual, hsl_slider.value(), hsl_slider.value(), hsl_slider.value()), 0, 0, scaled_width, scaled_height); // apply hsl threshold filter
    } else if (filter_select.title === "BG Color") { // when the title is bg color
      fill(bgColor); // fill the background with bg color
      rect(0, 0, scaled_width, scaled_height); // Draw a rectangle
      if (Img_loaded) { // If it is loaded image
        // Run segmentation separately for the loaded image
        body_segmentation.detect(Img_loaded, (result) => {
          let image_segmented = Img_loaded.get(); // Create a copy of the loaded image
          image_segmented.mask(result.mask); // Apply segmentation mask
          push();
          translate(width / 2 - (scaled_width * 5) / 2 + 500, height / 2 - (scaled_height * 5) / 2 + 13); // translate the image to center
          scale(5); // Scale the image
          image(image_segmented, 0, 0, scaled_width, scaled_height); // Show the segemented image
          pop();
        });
      } else if (mask_segmentation) { // If it is webcam live
        // Apply segmentation to the live webcam feed
        let visual_segmented = video.get(); // Get the current frame 
        visual_segmented.mask(mask_segmentation); // Apply segmentation mask
        image(visual_segmented, 0, 0, scaled_width, scaled_height); // Show the segemented image
      }}
    else {
      image(visual, 0, 0, scaled_width, scaled_height); // Basic visual
    }
    pop();
  } else if (!zoom) {
    grid(); // Draw the grid when there is no zoom filter
    grid_extensions(); // Draw the grid extensions when there is no zoom filter
    push();
    fill(0); // Fill black color
    textSize(40); // Set text size to be 40
    textAlign(CENTER, TOP); // Align the text center top
    text("BASIC", 1450, 70); // Position of the text
    text("EXTENSIONS", 2000, 70); // Position of the text
    pop();
  }
}

/** Helper function for the filters */
function prepareImage(img) {
  let imgOut = createImage(img.width, img.height); // Create new image
  imgOut.loadPixels(); // Get the pixel data of new image
  img.loadPixels(); // Get the pixel data of the input image
  return imgOut;
}

///////////////////////////////////////////////////////
////////////          Filters         /////////////////
///////////////////////////////////////////////////////

/** Basic WebCam */
function WebCam() {
  video = createCapture(VIDEO); // create video capture from the webcam
  video.size(scaled_width, scaled_height); // video size to scale width and height
  video.hide(); // hide the default
}

/** Greyscale filter */
function greyscaleFilter(img) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through width of image
    for (let y = 0; y < img.height; y++) { // Loop through height of image
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let r = img.pixels[index]; // Get the red value
      let g = img.pixels[index + 1]; // Get the green value
      let b = img.pixels[index + 2]; // Get the blue value

      let grey = r * 0.299 + g * 0.587 + b * 0.114; // calculation of the grey

      // Brightness increase to 20%
      // Added brightness not to exceed 255 pixel intensity
      grey = constrain(grey * 1.2, 0, 255);

      imgOut.pixels[index] = grey; // Set the red channel to grey
      imgOut.pixels[index + 1] = grey; // Set the green channel to grey
      imgOut.pixels[index + 2] = grey; // Set the blue channel to grey
      imgOut.pixels[index + 3] = 255; // Set the alpha channel to no transparency
    }
  }

  imgOut.updatePixels(); // Update the pixels
  return imgOut; // Greyscale image
}

/** 3 Different color channels filter */
function extractColorChannel(img, channel) {
  let imgOut = prepareImage(img);

  // Channel index based on R, G, B
  if (channel === 'R') {
    c_index = 0; // Red channel
  } else if (channel === 'G') {
    c_index = 1; // Green channel
  } else if (channel === 'B') {
    c_index = 2; // Blue channel
  }

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let value = img.pixels[index + c_index]; // Get the color value
      if (channel === 'R') {
        imgOut.pixels[index] = value; // red channel
        imgOut.pixels[index + 1] = 0; // green channel to 0
        imgOut.pixels[index + 2] = 0; // blue channel to 0
      } else if (channel === 'G') {
        imgOut.pixels[index] = 0; // red channel to 0
        imgOut.pixels[index + 1] = value; // green channel
        imgOut.pixels[index + 2] = 0; // blue channel to 0
      } else if (channel === 'B') {
        imgOut.pixels[index] = 0; // red channel to 0
        imgOut.pixels[index + 1] = 0; // green channel to 0
        imgOut.pixels[index + 2] = value; // blue channel
      }
      imgOut.pixels[index + 3] = 255; // Set the alpha channel to no transparency
    }
  }

  imgOut.updatePixels(); // Update the pixels
  return imgOut; // color channels
}

/** 3 Different color channels Threshold Filter */
function thresholdFilter(img, channel, threshold) {
  let imgOut = prepareImage(img);

  let Img_copy = img.get(); // Create a copy of the input image to prevent modifying the original
  Img_copy.filter(BLUR, 0.7); // Apply a slight blur to the image to remove noise
  Img_copy.loadPixels(); // Get the pixel data of the blurred image

  if (channel === 'G') {
    c_index = 1; // Green channel index
  } else if (channel === 'B') {
    c_index = 2; // Blue channel index
  } else {
    c_index = 0; // Red channel index
  }

  for (let i = 0; i < Img_copy.pixels.length; i += 4) { // Go through all pixel data
    let value = Img_copy.pixels[i + c_index]; // Get the value of R, G, or B
    if (value > threshold) { // pixel value greater than the threshold
      newValue = 255; // Set it to white
    } else {
      newValue = 0; // Set it to black
    }
    imgOut.pixels[i] = 0; // Set the red channel to 0
    imgOut.pixels[i + 1] = 0; // Set the green channel to 0
    imgOut.pixels[i + 2] = 0; // Set the blue channel to 0
    imgOut.pixels[i + 3] = 255; // Set the alpha to 255
    imgOut.pixels[i + c_index] = newValue; // Set the thresholded value for R,G,B
  }
  imgOut.updatePixels(); // Update the pixels
  return imgOut; // threshold image
}

/** Color Space conversion 1: ycbcr */
function ycbcrConversion(img) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let red = img.pixels[index]; // red channel value
      let green = img.pixels[index + 1]; // green channel value
      let blue = img.pixels[index + 2]; // blue channel value

      let y_Comp = 0.299 * red + 0.587 * green + 0.114 * blue; // Calculate y component
      let cb_Comp = -0.169 * red - 0.331 * green + 0.500 * blue + 128; // Calculate Cb component
      let cr_Comp = 0.500 * red - 0.419 * green - 0.081 * blue + 128; // Calculate Cr component

      imgOut.pixels[index] = y_Comp; // Set y component
      imgOut.pixels[index + 1] = cb_Comp;  // Set Cb component
      imgOut.pixels[index + 2] = cr_Comp;  // Set Cr component
      imgOut.pixels[index + 3] = 255; // Set the alpha to transparency
    }
  }
  imgOut.updatePixels(); // update the pixels
  return imgOut; // ycbcr converted image
}

/** Color Space conversion 2: hsv */
function hsvConversion(img) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let red = img.pixels[index] / 255; // red channel value
      let green = img.pixels[index + 1] / 255; // green channel value
      let blue = img.pixels[index + 2] / 255; // blue channel value

      let max = Math.max(red, green, blue); // Max value
      let min = Math.min(red, green, blue); // Min value
      let hue, saturation, value = max; // hue, saturation, value 
      let diff = max - min; // difference between max and min
      
      if (max === 0) { // If max is 0
        saturation = 0; // set saturation to 0
      } else {
        saturation = diff / max; // saturation calculation
      }      

      // Hue calculation
      if (max === min) { // If max and min are equal
        hue = 0; // hue is 0
      } else {
        switch (max) { 
          case red: // If red is the highest color value
            if (green < blue) { // check if green is smaller than blue
              hue = (green - blue) / diff + 6; // hue calculation with offset of 6
            } else {
              hue = (green - blue) / diff; // hue calculation
            }
            break;
          case green: // If green is the highest color value
            hue = (blue - red) / diff + 2; // hue calculation with offset of 2
            break;
          case blue: // If blue is the highest color value
            hue = (red - green) / diff + 4; // hue calculation with offset of 4
            break; // If blue is max
        }
        hue /= 6; // Normalize hue
      }
      imgOut.pixels[index] = hue * 255; // Set the hue
      imgOut.pixels[index + 1] = saturation * 255; // Set the saturation
      imgOut.pixels[index + 2] = value * 255; // Set the value
      imgOut.pixels[index + 3] = 255; // Set the alpha to transparency
    }
  }
  imgOut.updatePixels(); // Update the pixels
  return imgOut; // hsc converted image
}

/** Color Space threshold 1: ycbcr */
function ycbcrThreshold(img, y_Thresh, cb_Thresh, cr_Thresh) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let red = img.pixels[index]; // red channel value
      let green = img.pixels[index + 1]; // green channel value
      let blue = img.pixels[index + 2]; // blue channel value

      let y_Comp = 0.299 * red + 0.587 * green + 0.114 * blue; // Calculate y component
      let cb_Comp = -0.169 * red - 0.331 * green + 0.500 * blue + 128; // Calculate Cb component
      let cr_Comp = 0.500 * red - 0.419 * green - 0.081 * blue + 128; // Calculate Cr component

      // Apply thresholding
      if (y_Comp < y_Thresh || cb_Comp < cb_Thresh || cr_Comp < cr_Thresh) {
        imgOut.pixels[index] = 0; // Set red channel to black
        imgOut.pixels[index + 1] = 0; // Set green channel to black
        imgOut.pixels[index + 2] = 0; // Set blue channel to black
      } else {
        imgOut.pixels[index] = y_Comp; // Computed y component 
        imgOut.pixels[index + 1] = cb_Comp; // Computed Cb component
        imgOut.pixels[index + 2] = cr_Comp; // Computed Cr component
      }
      imgOut.pixels[index + 3] = 255; // Set the alpha to transparency
    }
  }
  imgOut.updatePixels(); // Update the pixels
  return imgOut; // ycbcr threshold image
}

/** Color Space threshold 2: hsv */
function hsvThreshold(img, hueThresh, saturationThresh, valueThresh) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let red = img.pixels[index] / 255; // red channel value
      let green = img.pixels[index + 1] / 255; // green channel value
      let blue = img.pixels[index + 2] / 255; // blue channel value

      let max = Math.max(red, green, blue); // Max value
      let min = Math.min(red, green, blue); // Min value
      let hue = 0; // hue
      let saturation = 0; // saturation
      let value = max; // Set value as max
      let diff = max - min; // Difference between max and min

      if (max !== 0) { // If max is not 0
        saturation = diff / max; // Calculate saturation
      }

      if (max !== min) { // If max is not equal to min
        if (max === red) { // If red channel is the highest color value
          hue = (green - blue) / diff; // calculate hue
          if (green < blue) {  // If green is less than blue
            hue = hue + 6; // offset hue by 6
          }
        } else if (max === green) { // If green channel is the highest color value
          hue = (blue - red) / diff + 2; // hue calculation with offset by 2
        } else if (max === blue) { // If blue channel is the highest color value
          hue = (red - green) / diff + 4; // huw calculation with offset by 4
        }
        hue = hue / 6; // Normalize hue
      }

      hue = hue * 255;
      saturation = saturation * 255;
      value = value * 255;

      // Apply thresholding
      if (hue < hueThresh || saturation < saturationThresh || value < valueThresh) {
        imgOut.pixels[index] = 0; // set red channel to black
        imgOut.pixels[index + 1] = 0; // set green channel to black
        imgOut.pixels[index + 2] = 0; // set blue channel to black
      } else {
        imgOut.pixels[index] = hue; // computed hue
        imgOut.pixels[index + 1] = saturation; // computed saturation
        imgOut.pixels[index + 2] = value; // computed value
      }
      imgOut.pixels[index + 3] = 255; // set alpha to transparency
    }
  }
  imgOut.updatePixels(); // Update the pixels
  return imgOut; // hsv threshold image
}

/** Face Detection Function using Bounding Box */
function facedetection(img, xPos, yPos) {
  // Detect faces in the current image
  faces = detector.detect(img.canvas);

  push();
  strokeWeight(2); // strokeweight os 2
  stroke(255, 0, 0); // Red color box
  noFill(); // no fill

  for (var i = 0; i < faces.length; i++) { // Loop thorugh each detected face
    var face = faces[i]; // Get the current face
    if (face[4] > 4) { // confidence level at index 4 is greater than 4
      rect(xPos + face[0], yPos + face[1], face[2], face[3]); // Draw a rectangle
    }
  }
  pop();
}

/** Apply the filters */
function Transformation(img, mode) {
  if (mode === "grayscale") { // when the mode is greyscale
    return greyscaleFilter(img); // apply the greyscale filter
  }
  if (mode === "faceHSV") { // when the mode is facehsv
    return HSV_Face(img); // apply the hsv filter to the face
  }
  if (mode === "blur") { // when the mode is blur
    return blur(img); // apply the blur filter to the face
  }
  if (mode === "pixelate") { // when the mode is pixelate
    return pixelate(img); // apply the pixelate filter to the face
  }
  return img; // Filtered image
}

/** Replace the detected face image with a blurred image */
function blur(img, blurLevel = 10) { 
  let imgOut = img.get(); // Copy the original image
  faces = detector.detect(img.canvas); // Detect faces

  for (let i = 0; i < faces.length; i++) { // Loop through all detected faces
    let face = faces[i]; // Current face 

    if (face[2] > 0 && face[3] > 0) {
      let x = Math.floor(face[0]); // x coordinate
      let y = Math.floor(face[1]); // y coordinate
      let width = Math.floor(face[2]); // width 
      let height = Math.floor(face[3]); // height

      // x and y coordinated are within the boundaries
      x = Math.max(0, x);
      y = Math.max(0, y);

      // width and height are within the boundaries
      width = Math.min(img.width - x, width);
      height = Math.min(img.height - y, height);

      if (width > 0 && height > 0) {
        let face_Img = img.get(x, y, width, height); // Extract the face region
        face_Img.filter(BLUR, blurLevel); // Apply blur effect

        imgOut.copy(face_Img, 0, 0, width, height, x, y, width, height); // Copy blurred face
      }
    }
  }
  return imgOut; // blur face image
}

/** Replace the detected face image with a colour converted image */
function HSV_Face(img) {
  let imgOut = img.get(); // Copy the original image
  faces = detector.detect(img.canvas); // Detect faces

  for (let i = 0; i < faces.length; i++) { // Loop through all detected faces
    let face = faces[i]; // Current face 

    if (face[2] > 0 && face[3] > 0) {
      let x = Math.floor(face[0]); // x coordinate
      let y = Math.floor(face[1]); // y coordinate
      let width = Math.floor(face[2]); // width 
      let height = Math.floor(face[3]); // height

      // x and y coordinated are within the boundaries
      x = Math.max(0, x);
      y = Math.max(0, y);

      // width and height are within the boundaries
      width = Math.min(img.width - x, width);
      height = Math.min(img.height - y, height);

      if (width > 0 && height > 0) {
        let face_Img = img.get(x, y, width, height); // Extract face region
        face_Img = hsvConversion(face_Img); // Apply hsv filter

        imgOut.copy(face_Img, 0, 0, width, height, x, y, width, height); // copy hsv face
      }
    }
  }
  return imgOut; // hsv face image
}

/**  Pixelate filter */
function pixelate(img) {
  let imgOut = img.get(); // Copy the original image
  faces = detector.detect(img.canvas); // Detect faces

  for (let i = 0; i < faces.length; i++) { // Loop through all detected faces
    let face = faces[i]; // Current face 

    if (face[2] > 0 && face[3] > 0) {
      let x = Math.floor(face[0]); // x coordinate
      let y = Math.floor(face[1]); // y coordinate
      let width = Math.floor(face[2]); // width 
      let height = Math.floor(face[3]); // height

      // x and y coordinated are within the boundaries
      x = Math.max(0, x);
      y = Math.max(0, y);

      // width and height are within the boundaries
      width = Math.min(img.width - x, width);
      height = Math.min(img.height - y, height);

      if (width > 0 && height > 0) {
        let face_Img = img.get(x, y, width, height); // Extract face region
        let block = 5; // 5x5 pixel blocks
        let outImage = createImage(face_Img.width, face_Img.height); // Output image
        outImage.loadPixels(); // Load pixels

        // Loop through the face region
        for (let bx = 0; bx < face_Img.width; bx += block) {
          for (let by = 0; by < face_Img.height; by += block) {
            let sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0; // variables 

            // Calculate average color of the block using image.get(x, y)
            for (let dx = 0; dx < block; dx++) {
              for (let dy = 0; dy < block; dy++) {
                let px = bx + dx; // pixel x position
                let py = by + dy; // pixel y position
                if (px < face_Img.width && py < face_Img.height) {
                  let c = face_Img.get(px, py); // Get pixel color
                  sumRed += red(c); // Add red channel value
                  sumGreen += green(c); // Add green channel value
                  sumBlue += blue(c); // Add blue channel value
                  count++;
                }
              }
            }
            let avgRed = sumRed / count; // Average red
            let avgGreen = sumGreen / count; // Average green
            let avgBlue = sumBlue / count; // Average blue

            // Use outImage.set(x, y, color(R, G, B)) to fill the block with avg color
            for (let dx = 0; dx < block; dx++) {
              for (let dy = 0; dy < block; dy++) {
                let px = bx + dx; // pixel x position
                let py = by + dy; // pixel y position
                if (px < face_Img.width && py < face_Img.height) {
                  outImage.set(px, py, color(avgRed, avgGreen, avgBlue)); // set block color to average
                }
              }
            }
          }
        }
        outImage.updatePixels(); // update the pixels
        imgOut.copy(outImage, 0, 0, width, height, x, y, width, height); // Copy pixelated face
      }
    }
  }
  return imgOut; // pixelated face image
}


///////////////////////////////////////////////////////
////////////     Webpage Functions      ///////////////
///////////////////////////////////////////////////////

/** Return to WebCam */
function return_To_Webcam() {
  Img_loaded = null; // Set load image to null
  mask_segmentation = null; // Set segmentation mask to null
  bgColor = [0, 0, 0]; // Set the background color back to black for the segmentation
  fileInput.elt.value = ''; // clear the file input

  if (video) {
    video.remove(); // Remove the video
  }
  WebCam(); // restart webcam
  video.elt.onloadeddata = () => {
    console.log("Webcam restarted, resetting segmentation..."); // console stetement
    body_segmentation.detectStart(video, results); // restart body segmentation
  };
}

/** Store the filters as grid */
function grid() {
  visual_update(); // update the visual
  let gridX = width / 2; // x axis of grid
  let gridY = (height / 2) - (scaled_height * 2) - 90; // y axis of grid
  let secondRowY = gridY + scaled_height + padding; // row 2
  let thirdRowY = secondRowY + scaled_height + padding; // row 3
  let fourthRowY = thirdRowY + scaled_height + padding; // row 4
  let fifthRowY = fourthRowY + scaled_height + padding; // row 5

  // First row: Original, grayscale, face detection
  drawTitle("Original", gridX, gridY); // title
  image(visual, gridX, gridY, scaled_width, scaled_height); // original visual
  drawTitle("Grayscale", gridX + scaled_width + padding, gridY); // title
  image(greyscaleFilter(visual), gridX + scaled_width + padding, gridY, scaled_width, scaled_height); // greyscale image
  drawTitle("Face Detection", gridX + (scaled_width + padding) * 2, gridY); // title
  image(visual, gridX + (scaled_width + padding) * 2, gridY, scaled_width, scaled_height); // visual
  facedetection(visual, gridX + (scaled_width + padding) * 2, gridY, scaled_width, scaled_height); // face detection

  // Second row: Color channels
  drawTitle("Red Channel", gridX, secondRowY); // title
  image(extractColorChannel(visual, 'R'), gridX, secondRowY, scaled_width, scaled_height); // red channel
  drawTitle("Green Channel", gridX + scaled_width + padding, secondRowY); // title
  image(extractColorChannel(visual, 'G'), gridX + scaled_width + padding, secondRowY, scaled_width, scaled_height); // green channel
  drawTitle("Blue Channel", gridX + (scaled_width + padding) * 2, secondRowY); // title
  image(extractColorChannel(visual, 'B'), gridX + (scaled_width + padding) * 2, secondRowY, scaled_width, scaled_height); // blue channel

  // Third row: Thresholds of color channels
  drawTitle("Threshold Red", gridX, thirdRowY); // title
  image(thresholdFilter(visual, 'R', red_slider.value()), gridX, thirdRowY, scaled_width, scaled_height); // red threshold
  drawTitle("Threshold Green", gridX + scaled_width + padding, thirdRowY); // title
  image(thresholdFilter(visual, 'G', green_slider.value()), gridX + scaled_width + padding, thirdRowY, scaled_width, scaled_height); // green threshold
  drawTitle("Threshold Blue", gridX + (scaled_width + padding) * 2, thirdRowY); // title
  image(thresholdFilter(visual, 'B', blue_slider.value()), gridX + (scaled_width + padding) * 2, thirdRowY, scaled_width, scaled_height); // blue threshold

  // Fourth row: original, ycbcr, hsv
  drawTitle("Original", gridX, fourthRowY); // title
  image(visual, gridX, fourthRowY, scaled_width, scaled_height); // original visual
  drawTitle("YCbCr", gridX + (scaled_width + padding), fourthRowY); // title
  image(ycbcrConversion(visual), gridX + (scaled_width + padding), fourthRowY, scaled_width, scaled_height); // ycbcr color space conversion
  drawTitle("HSV", gridX + (scaled_width + padding) * 2, fourthRowY); // title
  image(hsvConversion(visual), gridX + (scaled_width + padding) * 2, fourthRowY, scaled_width, scaled_height); // hsv color space conversion

  // Fifth row: threshold of ycbcr, hsv and filtered image
  drawTitle("Threshold YCbCr", gridX + scaled_width + padding, fifthRowY); // title
  image(ycbcrThreshold(visual, 'YCbCr', ycrcb_slider.value()), gridX + scaled_width + padding, fifthRowY, scaled_width, scaled_height); // ycbcr threshold
  drawTitle("Threshold HSV", gridX + (scaled_width + padding) * 2, fifthRowY); // title
  image(hsvThreshold(visual, 'HSV', hsv_slider.value()), gridX + (scaled_width + padding) * 2, fifthRowY, scaled_width, scaled_height); // hsv threshold
  drawTitle("Filtered Image", gridX, fifthRowY); // title
  let filteredVisual = Transformation(visual.get(), mode); // filtered image
  image(filteredVisual, gridX, fifthRowY, scaled_width, scaled_height); // filtered applied image
  if (mode === "facedetection") facedetection(visual, gridX, fifthRowY); // apply face detection
}

/** Store seperate grid for the extensions */
function grid_extensions(){
  visual_update(); // update the visual
  let gridX2 = width / 2; // x axis of grid 2
  let gridY2 = (height / 2) - (scaled_height * 2); // y axis of grid 2
  let secondRowY = gridY2 + scaled_height + padding; // row 2

  // First row: HSL, HSL threshold
  drawTitle("HSL", gridX2 + 650, gridY2 - 90, scaled_width, scaled_height); // title
  image(hsl(visual), gridX2 + 650, gridY2 - 90, scaled_width, scaled_height); // hsl conversion
  drawTitle("Threshold HSL", gridX2 + 640 + scaled_width + padding, gridY2 - 90, scaled_width, scaled_height); // title
  image(hslThreshold(visual, hsl_slider.value(), hsl_slider.value(), hsl_slider.value()), gridX2 + 640 + scaled_width + padding, gridY2 - 90, scaled_width, scaled_height); // hsl threshold

  // Second row: background segmentation
  fill(bgColor); // fill background with selected color by the user
  rect(gridX2 + 550 + scaled_width + padding, secondRowY - 90, scaled_width, scaled_height); // draw rectangle 
  drawTitle("BG color", gridX2 + 550 + scaled_width + padding, secondRowY - 90); // title

  if (Img_loaded) {
    // Run segmentation separately for the loaded image
    body_segmentation.detect(Img_loaded, (result) => {
      let image_segmented = Img_loaded.get(); // create a copy
      image_segmented.mask(result.mask); // apply segmentation mask
      image(image_segmented, gridX2 + 550 + scaled_width + padding, secondRowY - 90, scaled_width, scaled_height); // segmented image
    });
  } else if (mask_segmentation) {
    // Apply segmentation to the live webcam feed
    let visual_segmented = video.get(); // get the current frame
    visual_segmented.mask(mask_segmentation); // apply segmentation mask
    image(visual_segmented, gridX2 + 550 + scaled_width + padding, secondRowY - 90, scaled_width, scaled_height); // segmented visual
  }
}

/** Style for the buttons */
function style() {
  let capture_Img = createImg('images/capture.png'); // create image for capture button
  let capture_Text = createSpan('Capture'); // text label for capture button
  let load_Img = createImg('images/File_open.png'); // create image for load button
  let load_Text = createSpan('Load Image'); // text label for load button
  let colorWheel_Img = createImg('./Images/color_wheel.png'); // create image for color wheel button
  let color_text = createSpan('Color Wheel'); // text label for color wheel button
  let info = createImg('images/info.png'); // create image for info button
  let picker = createColorPicker('#ffffff'); // color picker

  // Capture button
  capture = createButton(''); // empty button
  capture.position((width / 4) - scaled_width - 200, (height / 2) + scaled_height * 2 - 280); // position of the capture button
  capture.style("font-size", "20px"); // font size to be 20
  capture.style("background-color", "beige"); // background color to be beige
  capture.style("color", "black"); // text color to be balck
  capture.style("font-weight", "BOLD"); // font weight to be bold 
  capture.style("padding", "10px 20px"); // padding
  capture.style("border-radius", "5px"); // border radius of 5
  capture.style("display", "flex"); // display to be flex
  capture.style("align-items", "center"); // align center
  capture.style("gap", "10px"); // gap of 10px between the icon and the text
  capture.mouseOver(() => capture.style("background-color", "grey")); // change background color when hover
  capture.mouseOut(() => capture.style("background-color", "beige")); // reset background color when not hover
  capture.mousePressed(capture_grid); // capture grid function when capture button is clicked
  capture_Img.size(30, 30); // image size
  capture_Img.parent(capture); // add image to button
  capture_Text.parent(capture); // add text to button

  // Load button
  load = createButton(''); // empty button
  load.position((width / 4) - scaled_width - 20, (height / 2) + scaled_height * 2 - 280); // position of load button
  load.style("font-size", "20px"); // font size to be 20
  load.style("background-color", "beige"); // background color to be beige
  load.style("color", "black"); // text color to be black
  load.style("font-weight", "BOLD"); // font weight to be bold
  load.style("padding", "10px 20px"); // padding
  load.style("border-radius", "5px"); // border radius to be 5
  load.style("display", "flex"); // display to be flex
  load.style("align-items", "center"); // align center
  load.style("gap", "10px"); // gap of 10px between the icon and the text
  load.mouseOver(() => load.style("background-color", "grey")); // change background color when hover
  load.mouseOut(() => load.style("background-color", "beige")); // reset background color when not hover
  load_Img.size(30, 30); // image size
  load_Img.parent(load); // add image to button
  load_Text.parent(load); // add text to button
  fileInput = createFileInput(Image_file); // create file input for loading images
  fileInput.hide(); // hide file input
  load.mousePressed(() => fileInput.elt.click()); // file input when load button is clicked

  // Color wheel button
  color_wheel = createButton(''); // empty button
  color_wheel.position((width / 4) - scaled_width + 200, (height / 2) + scaled_height * 2 - 280); // position of the color wheel button
  color_wheel.style("font-size", "20px"); // font size to be 20
  color_wheel.style("background-color", "beige"); // background color to be beige
  color_wheel.style("color", "black"); // text color to be black
  color_wheel.style("font-weight", "BOLD"); // font weight ot be bold
  color_wheel.style("padding", "10px 20px"); // padding
  color_wheel.style("border-radius", "5px"); // border radius to be 5
  color_wheel.style("display", "flex"); // display to be flex
  color_wheel.style("align-items", "center"); // align center
  color_wheel.style("gap", "10px"); // gap of 10px between the icon and the text
  color_wheel.mouseOver(() => color_wheel.style("background-color", "grey")); // change background color when hover
  color_wheel.mouseOut(() => color_wheel.style("background-color", "beige")); // reset background color when not hover
  colorWheel_Img.size(30, 30); // image size
  colorWheel_Img.parent(color_wheel); // add image to button
  color_text.parent(color_wheel); // add text to button
  picker.position((width / 4) - scaled_width + 320, (height / 2) + scaled_height * 2 - 220); // position of color picker
  picker.style('opacity', '0'); // opacity to be 0
  picker.style('width', '1px'); // width of 1
  picker.style('height', '1px'); // height of 1
  picker.style('border', 'none'); // no boarder
  picker.style('position', 'absolute'); // position absolute
  color_wheel.mousePressed(() => picker.elt.click()); // when color wheel button is clicked open the color picker
  picker.input(() => {bgColor = picker.color().levels;}); // update the background color as the user picks the color

  // Close button
  closeB = createButton('X Cancel'); // create button
  closeB.position((width / 4) - scaled_width + 430, (height / 2) + scaled_height * 2 - 280); // position of the cancel button
  closeB.style("font-size", "20px"); // font size to be 20
  closeB.style("background-color", "red"); // background color to be red
  closeB.style("color", "white"); // text color to be white
  closeB.style("font-weight", "BOLD"); // font weight to be bold
  closeB.style("padding", "12px 20px"); // padding
  closeB.style("border-radius", "5px"); // boarder radius to be 5px
  closeB.mouseOver(() => closeB.style("background-color", "grey")); // change background color when hover
  closeB.mouseOut(() => closeB.style("background-color", "red")); // reset background color when not hover
  closeB.mousePressed(return_To_Webcam); // calls the return to webcam function

  // Info button
  infob = createButton(''); // empty button
  infob.position((width / 4) - scaled_width - 400, (height / 2) + scaled_height * 2 - 750); // position of the info button
  infob.style("font-size", "20px"); // font size to be 20
  infob.style("background-color", "beige"); // background color to be beige
  infob.style("color", "black"); // text color to be black
  infob.style("font-weight", "BOLD"); // font weight to be bold
  infob.style("padding", "10px 20px"); // padding
  infob.style("border-radius", "5px"); // boarder radius of 5
  infob.style("display", "flex"); // display is flex
  infob.style("align-items", "center"); // align center
  infob.mouseOver(() => infob.style("background-color", "grey")); // change background color when hover
  infob.mouseOut(() => infob.style("background-color", "beige")); // reset background color when not hover
  infob.mousePressed(() => {show_info = !show_info;}); // when the button is clicked, show the info
  info.size(30, 30); // image size
  info.parent(infob); // add image to the button
}

/** Key press function */
function keyPressed() {
  if (key === '1') { // When 1 is clicked
    mode = "facedetection"; // apply face detection
  } else if (key === '2') { // when 2 is clicked
    mode = "grayscale"; // apply grayscale filter
  } else if (key === '3') { // when 3 is clicked
    mode = "blur"; // apply blue filter
  } else if (key === '4') { // when 4 is clicked
    mode = "faceHSV"; // apply HSV filter
  } else if (key === '5') { // when 5 is clicked
    mode = "pixelate"; // apply pixelate filter
  } else if (key === '0') { // when 0 is clicked
    mode = "original"; // remove all the filters
  } else if (keyCode === ESCAPE) { // when esc is clicked
    zoom = false; // zoom out the filter
    filter_select = null; // set the selected filter to null
  }
}

/** Title for the individual filters */
function drawTitle(title, x, y) {
  textSize(20); // Set font size to 20
  textAlign(CENTER, BOTTOM); // text align to center bottom
  fill(0); // set text color to black
  textStyle(BOLD); // bold the text
  text(title, x + scaled_width / 2, y - 5); // Draw text above the visual
}

/** Visual update when the picture is loaded */
function visual_update() {
  if (Img_loaded) { // if a image is loaded
    visual = Img_loaded; // set the visual with the loaded image
  } else {
    visual = video.get(); // set the visual with the webcam
  }
}

///////////////////////////////////////////////////////
////////////////     Extensions      //////////////////
///////////////////////////////////////////////////////

let zoom = false; // Variable to store the zoom 
let mask_segmentation; // variable to store the segmentation mask
let bgColor = [0,0,0]; // Variable to store the background color

/** Load external image */
function Image_file(file) {
  if (file.type === 'image') {
    loadImage(file.data, img => {
      Img_loaded = createImage(scaled_width, scaled_height); // create new image
      Img_loaded.copy(img, 0, 0, img.width, img.height, 0, 0, scaled_width, scaled_height); // copy the loaded image to the new image 
      Img_loaded.loadPixels(); // load pixels
      Img_loaded.updatePixels(); // update pixels
      backImg = null; // Reset background to prevent previous webcam background
    });
    video.stop(); // Stop webcam
  }
}

/** Segmentation results */
function results(result) {
  mask_segmentation = result.mask; // Store the segmentation mask from the result
}

/** Color space conversion 3: HSL */
function hsl(img) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let red = img.pixels[index] / 255; // red channel value
      let green = img.pixels[index + 1] / 255; // green channel value
      let blue = img.pixels[index + 2] / 255; // blue channel value

      let max = Math.max(red, green, blue); // Max value
      let min = Math.min(red, green, blue); // Min value
      let hue, saturation, light = (max + min) / 2; // hue, saturation, and lightness with default values
      let diff = max - min; // difference between max and min

      if (max === min) { // If the max and min values are equal
        hue = 0;  // Hue is 0
        saturation = 0;  // Saturation is 0
      } else {
        if (light > 0.5) { // lightness more than 0.5
          saturation = diff / (2 - max - min); // calculate saturation
        } else {
          saturation = diff / (max + min); // calculate saturation
        }

        if (max === red) { // If red channel is the highest color value
          if (green < blue) {
            hue = (green - blue) / diff + 6;  // calculate hue
          } else {
            hue = (green - blue) / diff;  // calculate hue
          }
        } else if (max === green) { // If green channel is the highest color value
          hue = (blue - red) / diff + 2;  // calculate hue
        } else {
          hue = (red - green) / diff + 4;  // calculate hue
        }

        hue /= 6; // Normalize the hue
      }

      imgOut.pixels[index] = hue * 255;  // set hue
      imgOut.pixels[index + 1] = saturation * 255;  // set saturation
      imgOut.pixels[index + 2] = light * 255;  // set lightness
      imgOut.pixels[index + 3] = 255;  // set alpha value to transparency
    }
  }
  imgOut.updatePixels(); // update the pixels
  return imgOut; // hsl converted image
}

/** Color Space threshold 2: HSL */
function hslThreshold(img, hueThresh, saturationThresh, lightThresh) {
  let imgOut = prepareImage(img);

  for (let x = 0; x < img.width; x++) { // Loop through the width
    for (let y = 0; y < img.height; y++) { // Loop through the height
      let index = (y * img.width + x) * 4; // Find the position of the pixel
      let red = img.pixels[index] / 255; // red channel value
      let green = img.pixels[index + 1] / 255; // green channel value
      let blue = img.pixels[index + 2] / 255; // blue channel value

      let max = Math.max(red, green, blue); // Max value
      let min = Math.min(red, green, blue); // Min value
      let hue, saturation, light = (max + min) / 2; // hue, saturation, and lightness with default values
      let diff = max - min; // difference between max and min

      if (max === min) {
        hue = 0;  // Hue is 0
        saturation = 0;  // Saturation is 0
      } else {
        if (light > 0.5) {
          saturation = diff / (2 - max - min);  // calculate Saturation
        } else {
          saturation = diff / (max + min);  // calclulate Saturation
        }

        if (max === red) { // If red channel is the highest color value
          hue = (green - blue) / diff;  // calculate hue
          if (green < blue) hue += 6; // If green is less than blue
        } else if (max === green) { // If green channel is the highest color value
          hue = (blue - red) / diff + 2;  // calculate hue
        } else {
          hue = (red - green) / diff + 4;  // calculate hue
        }
        hue /= 6; // Normalize the hue
      }

      hue *= 255;
      saturation *= 255;
      light *= 255;

      // Apply the threshold values
      if (hue < hueThresh || saturation < saturationThresh || light < lightThresh) {
        imgOut.pixels[index] = 0; // set to black
        imgOut.pixels[index + 1] = 0; // set to black
        imgOut.pixels[index + 2] = 0; // set to black
      } else {
        imgOut.pixels[index] = hue; // set hue
        imgOut.pixels[index + 1] = saturation; // set saturation
        imgOut.pixels[index + 2] = light; // set lightness
      }
      imgOut.pixels[index + 3] = 255; // Set alpha value to fully opaque
    }
  }
  imgOut.updatePixels(); // update pixels
  return imgOut; // hsl threshold image
}

/** Face Detection for Loaded Image */
function facedetectionImage(img) {
  let Img_detected = createGraphics(scaled_width, scaled_height); // Create graphics
  Img_detected.image(img, 0, 0, scaled_width, scaled_height); // original image on the new graphics
  let faces = detector.detect(img.canvas); // detect faces

  Img_detected.push(); // push
  Img_detected.strokeWeight(2); // stroke weight of 2
  Img_detected.stroke(255, 0, 0); // Red bounding box
  Img_detected.noFill(); // no fill

  for (let i = 0; i < faces.length; i++) { // Loop thorugh each detected face
    let face = faces[i]; // Get the current face
    if (face[4] > 4) { // confidence level at index 4 is greater than 4
      Img_detected.rect(face[0], face[1], face[2], face[3]); // Draw a rectangle
    }
  }
  Img_detected.pop();
  return Img_detected; // image with bounding box
}

/** Zoom In filter */
function mousePressed() {
  let gridX = width / 2; // x axis for the grid
  let gridY = (height / 2) - (scaled_height * 2) - 90; // y axis for the grid

  let filters = [
    { title: "Original", x: gridX, y: gridY }, // original filter
    { title: "Grayscale", x: gridX + scaled_width + padding, y: gridY }, // greyscale filter
    { title: "Face Detection", x: gridX + (scaled_width + padding) * 2, y: gridY }, // face dectection filter
    { title: "Red Channel", x: gridX, y: gridY + scaled_height + padding }, // red channel filter
    { title: "Green Channel", x: gridX + scaled_width + padding, y: gridY + scaled_height + padding }, // green channel filter
    { title: "Blue Channel", x: gridX + (scaled_width + padding) * 2, y: gridY + scaled_height + padding }, // blue channel filter
    { title: "Threshold Red", x: gridX, y: gridY + (scaled_height + padding) * 2 }, // red threshold filter
    { title: "Threshold Green", x: gridX + scaled_width + padding, y: gridY + (scaled_height + padding) * 2 }, // green threshold filter
    { title: "Threshold Blue", x: gridX + (scaled_width + padding) * 2, y: gridY + (scaled_height + padding) * 2 }, // blue threshold filter
    { title: "Original", x: gridX, y: gridY + (scaled_height + padding) * 3}, // original webcam
    { title: "YCbCr", x: gridX + scaled_width + padding, y: gridY + (scaled_height + padding) * 3 }, // ycbcr color space conversion
    { title: "HSV", x: gridX + (scaled_width + padding) * 2, y: gridY + (scaled_height + padding) * 3}, // hsv color space conversion
    { title: "FilteredImage", x: gridX, y: gridY + (scaled_height + padding) * 4}, // filtered image
    { title: "ThresholdYCbCr", x: gridX + scaled_width + padding, y: gridY + (scaled_height + padding) * 4 }, // ycbcr threshold
    { title: "ThresholdHSV", x: gridX + (scaled_width + padding) * 2, y: gridY + (scaled_height + padding) * 4}, // hsv threshold
    { title: "HSL", x: gridX + 650, y: gridY}, // hsl color space conversion
    { title: "Threshold HSL", x: gridX + 640 + scaled_width + padding, y: gridY }, // hsl treshold
    { title: "BG Color", x: gridX + 550 + scaled_width + padding, y: gridY + scaled_height + padding} // bg color change filter
  ];

  if (!zoom) {
    for (let filter of filters) { // loop through the filters
      if (
        mouseX > filter.x && mouseX < filter.x + scaled_width && // check if mouse is within the bounds
        mouseY > filter.y && mouseY < filter.y + scaled_height // check if mouse if within the bounds
      ) {
        filter_select = filter; // selected filter
        zoom = true; // activate zoom
        break; // exit when it is zoomed out
      }
    }
  }
}

/** Capture the grid as image */
function capture_grid() {
  visual_update(); // update the visual
  let padding = 10; // padding for the capture
  let img_Width = (scaled_width + padding) * 3; // width
  let img_Height = (scaled_height + padding) * 6; // height
  let Image = createGraphics(img_Width, img_Height); // create graphics

  // First row: Original, grayscale, face detection
  Image.image(visual, 0, 0, scaled_width, scaled_height); // original visual
  Image.image(greyscaleFilter(visual), scaled_width + padding, 0, scaled_width, scaled_height); // grey scale filter
  Image.image(facedetectionImage(visual, 0, 0), (scaled_width + padding) * 2, 0, scaled_width, scaled_height); // face detection image

  // Second row: 3 RGB channels
  Image.image(extractColorChannel(visual, 'R'), 0, scaled_height + padding, scaled_width, scaled_height); // red channel
  Image.image(extractColorChannel(visual, 'G'), scaled_width + padding, scaled_height + padding, scaled_width, scaled_height); // blue channel
  Image.image(extractColorChannel(visual, 'B'), (scaled_width + padding) * 2, scaled_height + padding, scaled_width, scaled_height); // green channel

  // Third row: Threshold channel filters
  Image.image(thresholdFilter(visual, 'R', red_slider.value()), 0, (scaled_height + padding) * 2, scaled_width, scaled_height); // red threshold
  Image.image(thresholdFilter(visual, 'G', green_slider.value()), scaled_width + padding, (scaled_height + padding) * 2, scaled_width, scaled_height); // green threshold
  Image.image(thresholdFilter(visual, 'B', blue_slider.value()), (scaled_width + padding) * 2, (scaled_height + padding) * 2, scaled_width, scaled_height); // blue threshold

  // Fourth row: original, YCbCr, HSV
  Image.image(visual, 0, (scaled_height + padding) * 3, scaled_width, scaled_height); // original visual
  Image.image(ycbcrConversion(visual), scaled_width + padding, (scaled_height + padding) * 3, scaled_width, scaled_height); // ycbcr color space conversion
  Image.image(hsvConversion(visual), (scaled_width + padding) * 2, (scaled_height + padding) * 3, scaled_width, scaled_height); // hsv color space conversion

  // Fifth row: Threshold YCbCr, HSV and Filtered Image
  let filteredVisual = Transformation(visual, mode); // filtered visual
  if (mode === "facedetection") { // when face mode is face detection
    filteredVisual = facedetectionImage(filteredVisual); // apply face detection
  } 
  Image.image(filteredVisual, 0, (scaled_height + padding) * 4, scaled_width, scaled_height); // filtered visual
  Image.image(ycbcrThreshold(visual, ycrcb_slider.value(), ycrcb_slider.value(), ycrcb_slider.value()), scaled_width + padding, (scaled_height + padding) * 4, scaled_width, scaled_height); // ycbcr threshold 
  Image.image(hsvThreshold(visual, hsv_slider.value(), hsv_slider.value(), hsv_slider.value()), (scaled_width + padding) * 2, (scaled_height + padding) * 4, scaled_width, scaled_height); // hsv threshold

  // Sixth row: HSL, HSL Threshold and BG Color
  Image.image(hsl(visual), 0, (scaled_height + padding) * 5, scaled_width, scaled_height); // hsl color space conversion
  Image.image(hslThreshold(visual, hsl_slider.value(), hsl_slider.value(), hsl_slider.value()), scaled_width + padding, (scaled_height + padding) * 5, scaled_width, scaled_height); // hsl threshold
  Image.fill(bgColor); // fill user selected background color
  Image.rect((scaled_width + padding) * 2, (scaled_height + padding) * 5, scaled_width, scaled_height); // draw rectangle
  if (Img_loaded) {
    body_segmentation.detect(Img_loaded, (result) => {
      let image_segmented = Img_loaded.get(); // create a copy
      image_segmented.mask(result.mask); // apply segmentation mask
      Image.image(image_segmented, (scaled_width + padding) * 2, (scaled_height + padding) * 5, scaled_width, scaled_height);  // segmented image
      Image.save('filtered_grid_snapshot.jpeg'); // save image
    });
  } else if (mask_segmentation) {
    let visual_segmented = video.get(); // get the current frame
    visual_segmented.mask(mask_segmentation); // apply segmentation
    Image.image(visual_segmented, (scaled_width + padding) * 2, (scaled_height + padding) * 5, scaled_width, scaled_height); // segmented visual
    Image.save('filtered_grid_snapshot.jpeg'); // save image
  } else {
    Image.save('filtered_grid_snapshot.jpeg'); // save image
  }
}