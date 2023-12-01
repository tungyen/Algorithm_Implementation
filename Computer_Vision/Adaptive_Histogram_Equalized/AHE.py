import cv2
import numpy as np

def AHE(img, win_size):
    """
    img : Input image
    win_size : Size of window in AHE, which require padding to prevent wrapping error
    """
    
    # Parameter preparing
    pad_size = int((win_size-1)/2)
    img_new = np.pad(img, (pad_size, pad_size), 'symmetric')
    h, w = img_new.shape
    output = np.zeros((h, w))
    
    # For each pixel, find the number of elements in the window that is less than the pixel
    for i in range(pad_size, h-pad_size):
        for j in range(pad_size, w-pad_size):
            block = img_new[i-pad_size:i+pad_size, j-pad_size:j+pad_size]
            ele = block.flatten()
            rank = len([d for d in ele if d < img_new[i, j]])      
            output[i, j] = rank * 255 / (win_size**2)
    output = output.astype(np.uint8)
    
    return output

def test_AHE(path, wSize, name):
    img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    img_new = np.copy(img)
    img_AHE = AHE(img_new, wSize)
    filename = name + ".png"
    cv2.imwrite(filename, img_AHE)
    cv2.imshow(name, img_AHE)

test_AHE("beach.png", 33, "Beach_AHE")