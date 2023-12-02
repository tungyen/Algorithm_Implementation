import numpy as np
import cv2
import matplotlib.pyplot as plt
import random
import pandas as pd
from IPython.display import display

def createDataset(img):
    
    """
    Input:
        img: The input image
    Output:
        dataset: The N*3 table for pixel BGR value
    """
    
    img_new = np.copy(img)
    h, w, c = img_new.shape
    dataset = img_new.flatten().reshape(h*w, c)
    return dataset

def kMeansCluster(feature, centers):
    
    """
    Input:
        feature: The N*3 table for pixel BGR value
    Output:
        idx: A N * 1 table that record the corresponding label of each pixel
        centers: A k * 3 table that record the RGB value of K group
    """
    
    max_iteration = 50
    feature = feature.astype(np.float64)
    centers = centers.astype(np.float64)
    N = feature.shape[0]
    k, c = centers.shape
    diff = 1000
    cur_interate = 0
    thres = 4
    
    # Initialize the class assignent for the dataset
    idx = np.zeros((N, 1), dtype=np.int32)
    
    while(diff > thres and cur_interate < max_iteration):

        # Building up the assignment
        for i, pixel in enumerate(feature):
            dis = np.linalg.norm(centers-pixel, axis=1)
            idx[i] = np.argmin(dis)
        
        # Updating the centers 
        new_centers = np.zeros((k, c))
        for cluster in range(k):
            pixels = feature[np.squeeze(idx) == cluster]
            new_centers[cluster] = np.mean(pixels, axis=0)
            
        # Computing the difference between new and old centers
        diff = np.linalg.norm(new_centers-centers)
        centers = new_centers
        cur_interate += 1
    print("Iteration Time: ", cur_interate)
    print("Difference between new center and old center: ", diff)
    feature = feature.astype(np.uint8)
    centers = np.round(centers).astype(np.uint8)
    return idx, centers

def mapValues(img, idx, centers):
    
    """
    Input:
        img: The input image
        idx: A N * 1 table that record the corresponding label of each pixel
        centers: A k * 3 table that record the RGB value of K group 
    Output:
        img_new: The image after the K_mean segmentation
    """
    
    img_new = np.copy(img)
    h, w, _ = img_new.shape
    
    for row in range(h):
        for col in range(w):
            index = row * w + col
            Class = idx[index]
            color = centers[Class]
            img_new[row, col, :] = color
            
    return img_new

def test_K_mean_segmentation(path, K, name):
    
    """
    Input:
        path: The path of the input image
        K: The group number
        name: The name for the output image
    """

    # Loading the image
    img = cv2.imread(path)
    img_RGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    filename = "RGB_" + name + ".png"
    cv2.imwrite(filename, img_RGB)

    # Create the dataset
    dataset = createDataset(img_RGB)
    N, M = dataset.shape

    # Get the K-mean result of the image
    center_idx = random.sample(range(N), K)
    centers = np.array([dataset[i] for i in center_idx])
    idx, centers = kMeansCluster(dataset, centers)

    # Get the segmentation result
    img_segmentation = mapValues(img_RGB, idx, centers)
    filename = "K_mean_segmentation_" + name + ".png"
    cv2.imwrite(filename, img_segmentation)

    # Building up the center table
    centerTable = {}
    centerTable["ID"] = []
    centerTable["R"] = []
    centerTable["G"] = []
    centerTable["B"] = []

    for id, c in enumerate(centers):
        centerTable["ID"].append(id)
        centerTable["R"].append(c[0])
        centerTable["G"].append(c[1])
        centerTable["B"].append(c[2])

    LTable = pd.DataFrame.from_dict(centerTable)
    display(LTable)
    
test_K_mean_segmentation("white-tower.png", 7, "Tower")