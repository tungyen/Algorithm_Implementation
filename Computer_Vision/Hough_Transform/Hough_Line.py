import numpy as np
import matplotlib.pyplot as plt
import cv2
import random

def drawLine(l, img, r = None):
    
    """
    Input:
        l: N * 2 array containing the x, y coordination of the line to be drown
        img: 3 channel original image
        r: Boolean, user parameter for randomly selecting a color for the line
    """
    
    if r != None:
        color = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
    else:
        color = (255, 0, 0)
    for x, y in l:
        img[x, y] = color
        
def addThick(lines, t, h, w):
    
    """
    Input:
        lines: N * 2 array containing the x, y coordination of the line to be thickened
        t: Scalar, the thickness added to the line
        h: Scalar, the height of the original image
        w: Scalar, the width of the original image
    Output:
        lines: N * 2 array containing the x, y coordination of the line after thickening
    """

    new_points = []
    N = lines.shape[0]
    for i in range(N - 1):
        start_point = lines[i]
        end_point = lines[i + 1]
        direction = end_point - start_point
        direction /= np.linalg.norm(direction)
        perpendicular = np.array([-direction[1], direction[0]])
        for j in range(-t // 2, t // 2 + 1):
            new_point = start_point + j * perpendicular
            new_points.append(new_point)

    new_points = np.array(new_points)
    if new_points.shape[0] != 0 and lines.shape[0] != 0:
        lines = np.concatenate((new_points, lines), axis=0)
        lines = np.array([[x, y] for (x, y) in lines if (y >=0 and y<w) and (x >= 0 and x < h)])
    return lines

def getLinePoints(ro, theta, h, w, t=1):
    
    """
    Input:
        ro: Scalar, rho of the line
        theta: Scalar, theta of the line
        h: Scalar, the height of the original image
        w: Scalar, the width of the original image
        t: Scalar, the thickness added to the line
    Output:
        lines: N * 2 array containing the x, y coordination of the line after thickening
    """
    
    S = np.sin(theta)
    C = np.cos(theta)
    X = np.arange(h)
    if S != 0:
        m = -C / S
        b = ro / S
        Y = m * X + b
    else:
        X = np.round(np.ones((w, ))*ro)
        Y = np.arange(w)
    lines = np.array([[x, y] for (x, y) in zip(X, Y) if (y >=0 and y<w) and (x >= 0 and x < h)])
    
    if t > 1:
        lines = addThick(lines, t, h, w)
    lines = lines.astype(np.int64)
    return lines

def inRange(desire_theta, theta):
    
    """
    Input:
        desired_theta: [[a1, b1], [a2, b2],......, [an, bn]], the designated angle range for Hough Line
        theta: Scalar, theta of the line
    Output:
        check: Boolean, whether the theta is in one of any range
    """
    
    check = False
    for r in desire_theta:
        if theta >= r[0] and theta <= r[1]:
            check = True
            break
    return check

def Hough_Transform(img, vote_thres, ratio, method, img_name, thick=1, range=None):
    
    """
    Input:
        img: Input 3 channel image
        vote_thres: Scalar, the minimum voting number for a line
        ratio: Scalar, the minimum ratio compared to the maximum voting for a line
        method: String, "Number" for vote_thres, and "Ration" for ratio
        img_name: String, name for the saving picture
        thick: Scalar, thickness for the result lines
        range: [[a1, b1], [a2, b2],......, [an, bn]], the designated angle range for Hough Line
    Output:
        line_loc: N * M * 2 Array. N is the number of lines, M is the point number of each line, with [x, y] coordination
    """
    
    # Initializing parameter
    img_new = np.copy(img)
    ro_resolution = 1
    theta_resolution = 1
    h, w = img_new.shape
    
    # Initializing the acummulator
    thetas = np.arange(-90, 91, theta_resolution)
    thetasR = thetas*np.pi/180.0
    ro_max = round(np.sqrt(h**2 + w**2))
    ro = np.arange(-ro_max, ro_max+1, ro_resolution)
    acummulator = np.zeros((len(ro), len(thetas)), dtype=np.uint64)
    pixels = np.argwhere(img_new != 0)
    
    # constructing the acummulator
    for x, y in pixels:
        for i, theta in enumerate(thetasR):
            r = x * np.cos(theta) + y * np.sin(theta)
            ro_index = round(r + ro_max)
            acummulator[ro_index, i] += 1
     
    # Plotting the acummulator
    plt.imshow(acummulator, cmap='gray', interpolation='nearest', aspect='auto', extent=[-90, 90, -ro_max, ro_max])
    t = "Hough-Transform for " + img_name
    plt.title(t)
    plt.xlabel('Theta')
    plt.ylabel('Rho')
    plt.colorbar()
    plt.show()
    
    # Drawing the line on the original image
    if method == "Number":
        lines = np.argwhere(acummulator >= vote_thres)
    elif method == "Ratio":
        thres = ratio * np.amax(acummulator)
        lines = np.argwhere(acummulator >= thres)
        
    if range == None:
        lines = [[ro[r], thetasR[t]] for (r, t) in lines]
    else:
        lines = [[ro[r], thetasR[t]] for (r, t) in lines if inRange(range, thetas[t])]
    
    for r, t in lines:
        print("Theta of line is: ", t*180/np.pi)
    
    line_loc = [getLinePoints(r, t, h, w, thick) for (r, t) in lines]
    return line_loc

def testHoughLine(path, vote_thres, ratio, method, img_name, thick=1, range=None):
    
    """
    Input:
        path: Path of the input 3 channel image
        vote_thres: Scalar, the minimum voting number for a line
        ratio: Scalar, the minimum ratio compared to the maximum voting for a line
        method: String, "Number" for vote_thres, and "Ration" for ratio
        img_name: String, name for the saving picture
        thick: Scalar, thickness for the result lines
        range: [[a1, b1], [a2, b2],......, [an, bn]], the designated angle range for Hough Line
    """
    
    img = cv2.imread(path)
    RGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    edge = cv2.Canny(gray, 150, 255)
    result = np.copy(RGB)
    result_filter = np.copy(RGB)
    plt.imshow(img)
    plt.show()
    plt.imshow(edge, cmap='gray')
    plt.show()

    # Result of the lane for Hough-Transform
    line_loc_lane = Hough_Transform(edge, vote_thres, ratio, method, img_name, thick)
    for l in line_loc_lane:
        drawLine(l, result)
        
    # Plotting the lane result
    plt.imshow(result)
    plt.title("Line finding from Lane")
    plt.colorbar()
    plt.show()
    filename = img_name + "_HoughLine.png"
    cv2.imwrite(filename, result)

    # Result of the lane after filtering for Hough-Transform
    line_loc_filter = Hough_Transform(edge, vote_thres, ratio, method, img_name, thick, range)
    for l in line_loc_filter:
        drawLine(l, result_filter)
        
    # Plotting the test result
    plt.imshow(result_filter)
    plt.title("Line finding from Lane after filter")
    plt.colorbar()
    plt.show()
    filename = img_name + "_HoughLineFilter.png"
    cv2.imwrite(filename, result_filter)

desire = [[25, 35], [-40, -30]]
testHoughLine("lane.png", 2, 0.75, "Ratio","Lane", 10, desire)