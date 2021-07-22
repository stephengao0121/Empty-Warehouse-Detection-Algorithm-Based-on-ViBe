# Empty-Warehouse-Detection-Algorithm-Based-on-ViBe
  
A warehouse detection program based on ViBe algorithm, intending to determine whether the warehouse is empty or not.  

## Structure

Vibe.h: header file of vibe class.  
Vibe.cpp: definitions of useful methods of vibe class.  
main.cpp: main functions, including reading video, determining empty or not and gathering error information.  

## Revision compared to verbatim ViBe algorithm

1. Disabling the function that if a pixel is classified as frontground image for 50 consecutive iterations, it would be turned to background image.
2. Removing the counter that records the time of a pixel being in frontground continuously.
3. Decreasing the background updating rate to 1/32.
4. Adding a function that can process first 3 frames as the background model, by taking the pixels' average value.
5. Removing large chunks of comments.
