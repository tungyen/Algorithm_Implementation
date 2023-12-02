import numpy as np
import cv2


def findLongestBool(array):
    
    """
    Input:
        array: An array including only Bool value (True, False)
    Output:
        continuous_number: The longest number of continuous True in array
    """
    
    circular_array = array + array
    mx = 0
    cur = 0

    for ele in circular_array:
        if ele:
            cur += 1
        else:
            mx = max(mx, cur)
            cur = 0

    continuous_number = min(mx, len(array))
    return continuous_number


def neighbors(img, row, col):
    
    """
    Input:
        img: The original image
        row: The row of current pixel
        col: The col of current pixel
    Output:
        nei: The 16 neighbors of the current pixel
    """
    
    p1 = img[row-3, col]
    p2 = img[row-3, col+1]
    p3 = img[row-2, col+2]
    p4 = img[row-1, col+3]
    p5 = img[row, col+3]
    p6 = img[row+1, col+3]
    p7 = img[row+2, col+2]
    p8 = img[row+3, col+1]
    p9 = img[row+3, col]
    p10 = img[row+3, col-1]
    p11 = img[row+2, col-2]
    p12 = img[row+1, col-3]
    p13 = img[row, col-3]
    p14 = img[row-1, col-3]
    p15 = img[row-2, col-2]
    p16 = img[row-3, col-1]
    nei = [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16]
    
    return nei
    

def isCorner(img, row, col, thres):
    
    """
    Input:
        img: The original image
        row: The row of current pixel
        col: The col of current pixel
        thres: the threshold for determine bright and dark pixel neighbors
    Output:
        check: A boolean value that states Whether the pixel is a corner
    """
    
    sur = neighbors(img, row, col)
    top_four = [sur[0], sur[4], sur[8], sur[12]]
    
    bright = 0
    dark = 0
    
    for p in top_four:
        if p - img[row, col] > thres:
            bright += 1
        elif img[row, col] - p > thres:
            dark += 1
    
    if bright < 3 and dark < 3:
        return False
    
    bright_bool = (sur - img[row, col]) >= thres
    dark_bool = (img[row, col] - sur) >= thres
    
    if sum(bright_bool) < 12 and sum(dark_bool) < 12:
        return False
    
    check = findLongestBool(bright_bool) >= 12 or findLongestBool(dark_bool) >= 12
    return check
    
def detect(img, thres):
    
    """
    Input:
        img: The original image
        thres: the threshold for determine bright and dark pixel neighbors
    Output:
        corners: All corners of the input image
    """
    
    corners = []
    img_new = np.copy(img).astype(np.float64)
    h, w = img_new.shape
    
    for row in range(3, h-3):
        for col in range(3, w-3):
            if isCorner(img_new, row, col, thres):
                corners.append([row, col])
                
    return corners

def FASTscore(img, row, col):
    
    """
    Input:
        img: The original image
        row: The row of current pixel
        col: The col of current pixel
    Output:
        score: The score used for NMS of the whole image
    """
    
    img_new = np.copy(img).astype(np.float64)
    sur = neighbors(img_new, row, col)
    score = sum([abs(i-img_new[row, col]) for i in sur])
    return score

def NMS(img, corners, dis):
    
    """
    Input:
        img: The original image
        corners: All detected corners
        dis: The maximum distance that two corners could be neighbors
    Output:
        corners: All corners of the image after the NMS
    """
    
    cur = 1
    while cur < len(corners):
        rowPrev, colPrev = corners[cur-1]
        rowCur, colCur = corners[cur]
        if np.sqrt(((rowCur-rowPrev)**2) + ((colCur-colPrev)**2)) < dis:
            scoreCur = FASTscore(img, rowCur, colCur)
            scorePrev = FASTscore(img, rowPrev, colPrev)
            
            if scoreCur > scorePrev:
                del (corners[cur-1])
            else:
                del (corners[cur])  
        else:
            cur += 1
            continue
    return corners

def test_FAST(path, thres, dis, name):
    
    """
    Input:
        path: The path of the input image
        thres: the threshold for determine bright and dark pixel neighbors
        dis: The maximum distance that two corners could be neighbors
        name: name of the ouput image
    """
      
    img = cv2.imread(path)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    street_corners = detect(gray, thres)
    street_corners = NMS(gray, street_corners, dis)

    for row, col in street_corners:
        cv2.circle(img, (row, col), 2, (0,0,255), 2)
    
    filename = name + ".png"
    cv2.imwrite(filename, img)
    
test_FAST("tower.jpg", 30, 4, "Tower_FAST")

            