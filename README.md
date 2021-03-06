# Empty Warehouse Detection Algorithm Based on ViBe
  
Some warehouse detection programs based on ViBe algorithm and CNN model, intending to determine whether the warehouse is empty or not.  

## Structure

Bayesian version: A bayesian version that combines vibe and frame difference to produce the result.  
ViBe: A vibe based version.  
cnn: A convolutional neural network method.

## Revision compared to verbatim ViBe algorithm

1. Disabling the function that if a pixel is classified as frontground image for 50 consecutive iterations, it would be turned to background image.
2. Removing the counter that records the time of a pixel being in frontground continuously.
3. Decreasing the background updating rate to 1/32.
4. Adding a function that can process first 3 frames as the background model, by taking the pixels' average value.
5. Removing large chunks of comments.
6. Adding gamma correction to fight against light influences.
7. Adding a naive bayes classifier that takes account of both vibe and frame difference, two of the most accurate and robust methods.

## Update Log
### 2021.07.22
* Initial commit.
* Functionalizing the validation unit. Making the code more readable.

### 2021.07.23
* Adding the implementation of Bayes class, and getting ready for integrating bayes inference into the algorithm.

### 2021.07.28
* Adding some light compensation and removement methods, trying to eliminate the effect of variant illumination. After several experiments, gamma correction performs the best, and histogram performs well only on black background case, while others do not act well enough.
* After adding ganna correction, the update rate can be increased to 1/28 without worsening the accuracy result. The programs performs a better robustness on white backgrounds.
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
* Putting function fd() into main.py. Removing fd.cpp.

### 2021.08.02
* Rewriting bayes.cpp and bayes.h. Now they form a naive bayes classifier, used to improve robustness. The classifier is ready for intergation.
* Frame difference is integrated into main.cpp. Working well.

### 2021.08.03
* Integrating naive bayes into main.cpp. It takes both ViBe and fd as the condition to predict the result. First 2000 frames is decided by ViBe || fd, and the vector <vibe, fd, true_label> is fitted in naive bayes classifier. From the 2001th frame on, the decision is made by naive bayes classifier. New frames along with its vibe, fd and acutal label is put into naive bayes for further training.
* Rewriting validate functions, in order to be made compatible to naive bayes needs.

### 2021.08.04
* Revising and adding some notations.
* Adding the naive bayes classifier is no worse than simple vibe. Robustness needs to be further validated.

### 2021.08.05
* Revising some notations.
* Deleting some useless code blocks.
* Bayesian works no worse than ViBe, however, no evidence is discovered to prove that Bayesian is more robust.

### 2021.08.06
* Adjusting structure. Both bayesian version and simple vibe version is uploaded.

### 2021.08.09
* Adding a new directory *cnn*, including a cnn method. Basic cnn structure and training process is done. Prediction process needs to be completed later.

### 2021.08.11
* Completing prediction process.
* Expanding training dataset.
* Changing to categorical data.

### 2021.08.20
* Removing cnn program. It does not work well.
* Adding some codes in vibe for some new testcases. A new door_emulation test is implemented. Functions called door_test0x is for those cases.
* Removing some useless test code and add notations.
* Adding macro definitions, for simplicity and readability.

### 2021.08.25
* Deleting some redundant notations.
* The final version.
