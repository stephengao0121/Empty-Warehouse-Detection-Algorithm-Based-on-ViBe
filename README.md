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

## Update Log
### 2021.07.22
* Initial commit.
* Functionalizing the validation unit. Making the code more readable.

### 2021.07.23
* Adding the implementation of Bayes class, and getting ready for integrating bayes inference into the algorithm.

### 2021.07.28
* Adding some light compensation and removement methods, trying to eliminate the effect of variant illumination. After several experiments, gamma correction performs the best, and histogram performs well only on black background case, while others do not act well enough.
* Adding some annotations.

### 2021.07.29
* Trying to implement bayes inference on  
$$P(Y|X),$$  
where $Y$ stands for emptyness, and $X$ stands for door status (being open or not). However, the testset does not contain enough door opening cases, thus being unable to gather enough quantity to calculate believable probabilities.
* Adding some notations.

### 2021.07.30
* Adding some notations.
* Simplifying the code structure. Easier to operate testing.
* Rewriting destructor of Bayes class, fixing a memory bug.
* Creating a new file, called fd.cpp, which contains a implementation of frame difference method. This function is the preperation for adding bayes inference into the algorithm, making fd as the condition in conditional probability.
* Put function fd() into main.py. Remove fd.cpp.
