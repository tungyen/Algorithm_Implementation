import cv2
import numpy as np
import matplotlib.pyplot as plt


def UniformQuantizationPartition(img, level):
    
    """
    Input:
        img: Input image for Uniform Quantization
        level: The number of color zone you want to separate
    Output:
        partition: The separating edge for Uniform Quantization
    """
    
    img_new = np.copy(img)
    max_pixel = np.amax(img_new)
    min_pixel = np.amin(img_new)
    step = np.ceil((max_pixel-min_pixel)/level)
    partition = [min_pixel]
    for i in range(level-1):
        partition.append(min_pixel+step*(i+1))
    partition.append(max_pixel)
    
    return partition


def find_closest_color(c, partition):
    
    """
    Input:
        c: input color value (R or G or B)
        partition: the uniform quantization partition for a 2D image
    Output:
        res: The closest color of the input color in the image
    """
    
    for j in range(len(partition)-1):
        l = partition[j]
        r = partition[j+1]
        if c >= l and c<= r:
            res = round((l+r)/2)
            break
        
    return res

def UniformQuantization(img, level):
    
    """
    Input:
        img: Input image for Uniform Quantization, cannot be the gray scale image
        level: The number of color zone you want to separate
    Output:
        img_new: The image after Uniform Quantization
    """
    
    img_new = np.copy(img)
    c = img_new.shape[2]
    
    for i in range(c):
        img_c = np.copy(img_new[:, :, i])
        partition = UniformQuantizationPartition(img_c, level)
    
        for j in range(len(partition)-1):
            l = partition[j]
            r = partition[j+1]
            mask = np.where((img_c >=l) & (img_c <=r))
            img_c[mask] = round((l+r)/2)
        img_new[:, :, i] = img_c
        
    img_new = img_new.astype(np.uint8)
    return img_new


def errorCalculate(target, error, gain, minPixel, maxPixel):
    
    """
    Input:
        target: the pixel that error disperse to
        error: the difference between the old pixel and the pixel after find_closest_color
        gain: the weight for the error-dispersion on the target
        minPixel: the minimum pixel value in the original image
        maxPixel: the maximum pixel value in the original image
    Output:
        newTarget: New value for the target after dispersing the error
    """
    
    diff = error*gain
    newTarget = target + diff
    newTarget = np.uint8(np.clip(newTarget, minPixel, maxPixel))
    return newTarget


def FSD(img, level):
    
    """
    Input:
        img: Input image for Floyd-Steinberg Dithering, cannot be the gray scale image
        level: The number of color zone you want to separate
    Output:
        img_new: Image after the Floyd-Steinberg Dithering
    """
    
    img_new = np.copy(img)
    h, w, C = img_new.shape
    for c in range(C):
        img_c = img_new[:, :, c]
        partition = UniformQuantizationPartition(img_c, level)
        minPixel = np.amin(img_c)
        maxPixel = np.amax(img_c)
        for i in range(h):
            for j in range(w):
                ori = float(img_c[i, j])
                after = find_closest_color(ori, partition)
                img_c[i, j] = np.uint8(after)
                err = ori - after
                if j<w-1 and j>0 and i<h-1:
                    img_c[i, j+1] = errorCalculate(img_c[i, j+1], err, 7/16, minPixel, maxPixel)
                    img_c[i+1, j-1] = errorCalculate(img_c[i+1, j-1], err, 3/16, minPixel, maxPixel)
                    img_c[i+1, j] = errorCalculate(img_c[i+1, j], err, 5/16, minPixel, maxPixel)
                    img_c[i+1, j+1] = errorCalculate(img_c[i+1, j+1], err, 1/16, minPixel, maxPixel)
        img_new[:, :, c] = img_c  
    return img_new


def test_Uniform_Quantization(path, level, name):
    
    """
    path: Path of input image for Uniform Quantization, cannot be the gray scale image
    level: The number of color zone you want to separate
    name: the name of output image you want
    """
    
    img = cv2.imread(path)
    img_new = np.copy(img)
    img_UQ = UniformQuantization(img_new, level)
    filename = name + ".png"
    cv2.imwrite(filename, img_UQ)

def test_FSD(path, level, name):
    
    """
    path: Path of input image for Floyd-Steinberg Dithering, cannot be the gray scale image
    level: The number of color zone you want to separate
    name: the name of output image you want
    """
    
    img = cv2.imread(path)
    img_new = np.copy(img)
    img_FSD = FSD(img_new, level)
    filename = name + ".png"
    cv2.imwrite(filename, img_FSD)
    
test_Uniform_Quantization("geisel.jpg", 10, "Geisel_UQ")
test_FSD("geisel.jpg", 10, "Geisel_FSD")