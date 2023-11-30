import cv2
import numpy as np
import math

def Conv2(img, kernel):
    img_new = np.copy(img).astype(np.float64)
    res = np.copy(img_new)
    h_img, w_img = img_new.shape
    h_kernel, w_kernel = kernel.shape
    top = int((h_kernel-1)/2)
    bottom = h_kernel-1-top
    left = int((w_kernel-1)/2)
    right = w_kernel-1-left
    img_padding = np.pad(img_new, pad_width=[(top, bottom),(left, right)], mode='constant')
    for row in range(h_img):
        for col in range(w_img):
            block = img_padding[row:row+h_kernel, col:col+w_kernel]
            res[row, col] = np.sum(block * kernel)
    return res

def Harris_score_Cal(sq_Ix, sq_Iy, IxIy, param):
    score = np.copy(sq_Ix)
    for row in range(sq_Ix.shape[0]):
        for col in range(sq_Iy.shape[1]):
            M = np.array([[sq_Ix[row, col], IxIy[row, col]], [IxIy[row, col], sq_Iy[row, col]]])
            score[row, col] = np.linalg.det(M) - param * (np.trace(M)**2)
    return score

def cornerMarking(img, score, wSize, thres=None):
    if thres == None:
        thres =  1*math.fabs(score.mean())
    print(score)
    img_new = np.copy(img)
    h, w, _ = img_new.shape
    for row in range(h):
        for col in range(w):
            block = score[max(0, row-int(wSize/2)):min(score.shape[0], row+int(wSize/2)+1), max(0, col-int(wSize/2)):min(score.shape[1], col+int(wSize/2)+1)]
            if score[row, col] < np.amax(block):
                score[row, col] = 0.0
                continue
            if score[row, col] > thres:
                cv2.circle(img_new, (row, col), 3, (0, 0, 255), thickness=-5)
                
    return img_new

# Matrix initialization
Sobel_X = np.array([[2,2,4,2,2], [1,1,2,1,1], [0,0,0,0,0], [-1,-1,-2,-1,-1], [-2,-2,-4,-2,-2]])
Sobel_Y = np.array([[2,1,0,-1,-2], [2,1,0,-1,-2], [4,2,0,-2,-4], [2,1,0,-1,-2], [2,1,0,-1,-2]])
Gaussain_Filter = np.array([[2,4,5,4,2], [4,9,12,9,4], [5,12,15,12,5], [4,9,12,9,4], [2,4,5,4,2]])/159
board = cv2.imread("board.png")
board_gray = cv2.imread("board.png", 0)

Ix = Conv2(board_gray, Sobel_X)
Iy = Conv2(board_gray, Sobel_Y)

sq_Ix = Conv2(Ix**2, Gaussain_Filter)
sq_Iy = Conv2(Iy*2, Gaussain_Filter)
IxIy = Conv2(Ix*Iy, Gaussain_Filter)

Harris_score = Harris_score_Cal(sq_Ix, sq_Iy, IxIy, 0.04)
board_result = cornerMarking(board, Harris_score, 12, 5)

cv2.imshow("CornerResult", board_result)
cv2.waitKey(0)

