import numpy as np
import cv2
import matplotlib.pyplot as plt


def padding(img, p):
    
    """
    Input:
        img: Input Image
        p: Total padding size
    Output:
        res: Image after padding
    """
    
    img_new = np.copy(img)
    top = int(p/2)
    bottom = p - top
    left = int(p/2)
    right = p - left
    res = np.pad(img_new, pad_width=[(top, bottom),(left, right)], mode='constant')
    return res

def gaussianFilter(img, kernel):
    
    """
    Input:
        img: Input Image
        kernel: The input Gaussian Kernel
    Output:
        img_new: Image after the gaussian blur
    """
    
    n = kernel.shape[0]
    p = int((n-1)/2)
    img_new = np.copy(img).astype(np.float64)
    img_padding = padding(img_new, n-1)
    h, w = img_padding.shape
    for i in range(h-2*p):
        for j in range(w-2*p):
            block = np.copy(img_padding[i:i+n, j:j+n])
            img_new[i, j] = np.sum(block * kernel)
    img_new = np.uint8(np.clip(img_new, 0, 255))
    return img_new

def getGradient(img, Sx, Sy):
    
    """
    Input: 
        img: Input Image
        Sx: The Sobel operator along with X-axis
        Sy: The Sobel operator along with Y-axis
    Output:
        G_res: The output gradient magnitute of the whole image
        G_theta_res: The output gradient direction of the whole image
    """
    
    G_res = np.zeros((img.shape[0], img.shape[1]))
    G_theta_res = np.zeros((img.shape[0], img.shape[1]))
    n = Sx.shape[0]
    img_new = np.copy(img).astype(np.float64)
    p = int((n-1)/2)
    img_padding = padding(img_new, n-1)
    h, w = img_padding.shape
    for i in range(h-2*p):
        for j in range(w-2*p):
            block = np.copy(img_padding[i:i+n, j:j+n])
            Gx = np.sum(block * Sx)
            Gy = np.sum(block * Sy)
            G = np.sqrt(Gx**2 + Gy**2)
            if Gx == 0.0:
                Gtheta = np.sign(Gy)*np.pi/2
            else:
                Gtheta = np.arctan(Gy/Gx)
            G_res[i, j] = G
            G_theta_res[i, j] = Gtheta
    return G_res, G_theta_res

def non_maximum_supression(gradient, gradient_theta):
    
    """
    Input: 
        gradient: Gradient magnitute of the whole image
        gradient_theta: Gradient direction of the whole image
    Output:
        gradient: Gradient magnitute of the whole image after NMS
    """
    
    h,w = gradient.shape
    for row in range(h):
        for col in range(w):
            G, G_theta = gradient[row, col], gradient_theta[row, col]
            theta_round = round(G_theta/(np.pi/4))*np.pi/4
            if theta_round == 0:
                block = gradient[row, max(0, col-1):min(col+2,w)]
                if G != np.amax(block):
                    gradient[row, col] = 0.0
                
            elif theta_round == np.pi/2 or theta_round == -np.pi/2:
                block = gradient[max(0, row-1):min(row+2,h), col]
                if G != np.amax(block):
                    gradient[row, col] = 0.0
                
            elif theta_round == np.pi/4:
                block = [G]
                if row != h-1 and col != 0:
                    block.append(gradient[row+1, col-1])
                if row != 0 and col != w-1:
                    block.append(gradient[row-1, col+1])
                if G != np.amax(block):
                    gradient[row, col] = 0.0
                    
            elif theta_round == -np.pi/4:
                block = [G]
                if row != h-1 and col != w-1:
                    block.append(gradient[row+1, col+1])
                if row != 0 and col != 0:
                    block.append(gradient[row-1, col-1])
                if G != np.amax(block):
                    gradient[row, col] = 0.0   
    return gradient

def thresholding(gradient, t_low, t_high):
    
    """
    Input:
        gradient: Gradient magnitute of the whole image
        t_low: threshold for filtering weak edge
        t_high: threshold for filtering strong edge
    Output:
        G_thres: Gradient magnitute of the whole image after thresholding
    """
    
    G_thres = np.copy(gradient)
    G_thres = G_thres / np.amax(G_thres)*255.0
    mask = gradient < t_low
    G_thres[mask] = 0.0
    
    h, w = gradient.shape
    for row in range(h):
        for col in range(w):
            if gradient[row, col] >= t_low and gradient[row, col] < t_high:
                block = gradient[max(0, row-1):min(row+2,h), max(0, col-1):min(col+2,w)]
                if np.amax(block) < t_high:
                    G_thres[row, col] = 0.0
    G_thres[G_thres != 0] = 255.0
    G_thres = G_thres.astype(np.uint8)
    return G_thres
                       
def canny(img, t_low, t_high, name):
    
    """
    Input:
        img: Image for finding edges
        t_low: threshold for filtering weak edge
        t_high: threshold for filtering strong edge
        name: Name of the picture for saving file
    """
    
    # Gaussian BLur
    img_new = np.copy(img)
    Gaussain_matrix = np.array([[2,4,5,4,2], [4,9,12,9,4], [5,12,15,12,5], [4,9,12,9,4], [2,4,5,4,2]])/159
    img_gaussian = gaussianFilter(img_new, Gaussain_matrix)
    filename = "Gaussian_Filter_" + name + ".png"
    cv2.imwrite(filename, img_gaussian)
    
    # Computing the gradient
    Mx = np.array([[-1,0,1], [-2,0,2], [-1,0,1]])
    My = np.array([[-1,-2,-1], [0,0,0], [1,2,1]])
    gradient_res, gradient_theta_res = getGradient(img_gaussian, Mx, My)
    filename = "Gradient_Magnitute_" + name + ".png"
    cv2.imwrite(filename, gradient_res)
    
    # Non Maximum Suppresion
    gradient_shrink = non_maximum_supression(gradient_res, gradient_theta_res)
    filename = "NMS_" + name + ".png"
    cv2.imwrite(filename, gradient_shrink)
    
    # Thresholding
    gradient_thres = thresholding(gradient_shrink, t_low, t_high)
    filename = "Gradient_thresholding_" + name + ".png"
    cv2.imwrite(filename, gradient_thres)
    
def test_canny(path, t_low, t_high, name):
    
    """
    Input:
        path: Path of the image for finding edges
        t_low: threshold for filtering weak edge
        t_high: threshold for filtering strong edge
        name: Name of the picture for saving file
    """
    
    gray = cv2.imread(path, 0)
    canny(gray, t_low, t_high, name)
    
test_canny("geisel.jpg", 50, 100, "Geisel")