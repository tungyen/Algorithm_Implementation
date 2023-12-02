import numpy as np
from numpy.linalg import norm
import cv2

def quantiz(training_set, partition, codebook):
    
    """
    Input:
        training_set: A np.array with size [N, 1], N is number of pixel of an image
        partition: list with size M
        codebook: list with length M + 1
    Output:
        indx: list with length N, indicating which zone is for a training sample
        distor: Scalar, the error of Lloyd-Max Quantization
    """

    indx = np.zeros((training_set.shape[0], 1))
    for i in range(len(partition)):
        indx = indx + (training_set > partition[i])
    distor = 0
    for i in range(len(codebook)):
        distor += norm(training_set[indx == i] - codebook[i])**2
    distor = distor / training_set.shape[0]
    return indx, distor
    
def get_rel_distortion(distor, last_distor, thres):
    
    """
    Input:
        distor: Scalar, the error of Lloyd-Max Quantization
        last_distor: Scalar, the previous error of Lloyd-Max Quantization
        thres: Scalar, threshold for changing the rel_distor
    Output:
        rel_distor: Scalar, the error of Lloyd-Max Quantization
    """
    
    if distor > thres:
        rel_distor = abs(distor - last_distor)/distor
    else:
        rel_distor = distor
    return rel_distor

def lloyds(training_set, ini_codebook, tol=1e-7):
    
    """
    Input:
        training_set: A np.array with size [N, 1], N is number of pixel of an image
        ini_codebook: int, number of partitions
        tol: Scalar, the convergence threshold for Lloyd_Max
    Output:
        partition: list with size M
        codebook: list with length M + 1
    """
    
    ## init codebook
    min_training = training_set.min()
    max_training = training_set.max()
    thres = np.spacing(1) * max_training
    
    int_training = (max_training - min_training)/ini_codebook
    codebook = np.linspace(min_training+int_training/2, max_training-int_training/2, ini_codebook)

    ## init partition
    partition = (codebook[1 : ini_codebook] + codebook[0 : ini_codebook-1]) / 2
    
    _, distor = quantiz(training_set, partition, codebook)
    last_distor = 0
    rel_distor = get_rel_distortion(distor, last_distor, thres)
    count = 0
    while (rel_distor > tol) and (rel_distor > thres):

        partition_aug = np.concatenate((np.array([min_training]), partition, np.array([max_training])))
        for i in range(len(partition_aug)-1):
            part_set = training_set[np.logical_and(training_set>=partition_aug[i], training_set<partition_aug[i+1]) ]
            if len(part_set) > 0:
                codebook[i] = part_set.mean()
            else:
                codebook[i] = (partition_aug[i] + partition_aug[i+1])/2
        
        # update t_hat: codebook
        partition = (codebook[1 : ini_codebook] + codebook[0 : ini_codebook-1]) / 2
        last_distor = 0 + distor
        _, distor = quantiz(training_set, partition, codebook)
        
        # get distortion
        rel_distor = get_rel_distortion(distor, last_distor, thres)
        count += 1

    return partition, codebook

def Lloyd(img, level):
    
    """
    Input:
        img: np.array, Input image for Lloyd-Max Quantization
        level: int, number of color zone for Lloyd-Max Quantization
    Output:
        img_new: np.array, Image after Lloyd-Max Quantization
    """
    
    img_new = np.copy(img)
    input = img_new.flatten().reshape((-1, 1))
    input = input.astype(np.float64)
    partition, codebook = lloyds(input, level)
    
    partition = np.round(partition)
    codebook = np.round(codebook)
    
    for i in range(len(partition)-1):
        l = partition[i]
        r = partition[i+1]
        img_new[np.where((img_new >=l) & (img_new <r))] = codebook[i+1]
    
    img_new[np.where((img_new <partition[0]))] = codebook[0]
    img_new[np.where((img_new >=partition[-1]))] = codebook[-1]
        
    img_new = img_new.astype(np.uint8)
    return img_new

def test_Lloyd_Max_Quantization(path, level, name):
    
    """
    Input:
        path: String, path of the image
        level: int, number of the color zone for Lloyd
        name: String, the name of the resulting image for saving
    """
    
    img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    img_new = np.copy(img)
    img_Lloyd = Lloyd(img_new, level)
    filename = "Lloyd_Max_Quantization_" + name + ".png"
    cv2.imwrite(filename, img_Lloyd)
    
test_Lloyd_Max_Quantization("diver.tif", 8, "diver")