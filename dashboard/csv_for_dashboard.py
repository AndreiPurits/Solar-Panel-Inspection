import cv2
import numpy as np
import imutils
import csv

average_area_data = []
defects_data = []

iterationz_range = range(1, 33, 4)
min_contour_area_range = range(80, 141, 20)
max_contour_area_range = range(300, 461, 20)
threshold_range = range(90, 241, 15)

def process_image(input_image, min_contour_area, max_contour_area, threshold_value, iterations_value):
    defects_data = 0
    average_area_data = 0
    img = cv2.imread(input_image)
    img = cv2.normalize(img, img, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, thresh = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    kernel = np.ones((3, 28), np.uint8)
    opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations_value)
    sure_bg = cv2.dilate(opening, kernel, iterations=3)
    dist_transform = cv2.distanceTransform(opening, cv2.DIST_L2, 5) 
    ret, sure_fg = cv2.threshold(dist_transform, 0.001 * dist_transform.max(), 255, 0)
    sure_fg = np.uint8(sure_fg)
    unknown = cv2.subtract(sure_bg, sure_fg)
    ret, markers = cv2.connectedComponents(sure_fg)
    markers = markers + 1
    markers[unknown == 255] = 0
    markers = cv2.watershed(img, markers)
    mask = np.where(markers != 1, 1, 0).astype(np.uint8)
    result2 = cv2.bitwise_and(img, img, mask=mask)

    edges = cv2.Canny(image=result2, threshold1=29, threshold2=30)
    cnts = cv2.findContours(edges, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
    cnts = imutils.grab_contours(cnts)
    cnts = sorted(cnts, key=cv2.contourArea, reverse=True)

    for c in range(len(cnts)):
        [x, y, w, h] = cv2.boundingRect(cnts[c])
        if w * h < 50000:
            cv2.rectangle(opening, (x, y), (x + w, y + h), (255, 255, 255), -1)
    opening = cv2.morphologyEx(opening, cv2.MORPH_OPEN, kernel=np.ones((5, 5), np.uint8))
    cnts = cv2.findContours(opening, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
    cnts = imutils.grab_contours(cnts)
    cnts = sorted(cnts, key=cv2.contourArea, reverse=True)

    if len(cnts) >= 1:
        rot_rect = cv2.minAreaRect(cnts[0])
        (x, y), (w, h), angle = rot_rect
        box = cv2.boxPoints(rot_rect)
        box = np.intp(box)

        roi = img[min(box[:, 1]):max(box[:, 1]), min(box[:, 0]):max(box[:, 0])]

        img_gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
        imgу = cv2.normalize(img_gray, img_gray, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
        _, binary_mask = cv2.threshold(imgу, threshold_value, 255, cv2.THRESH_BINARY)
        contours, _ = cv2.findContours(binary_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        selected_contours = []
        if len(contours) != 0:
            for contour in contours:
                area = cv2.contourArea(contour)
                if min_contour_area < area < max_contour_area:
                    selected_contours.append(contour)
            if selected_contours:
                defects_data = len(selected_contours)
                average_area_data = np.mean([cv2.contourArea(contour) for contour in selected_contours])

    return defects_data, average_area_data


with open('output_data.csv', 'w', newline='') as csvfile:
    # Define CSV writer
    csv_writer = csv.writer(csvfile)
    
    # Write header
    csv_writer.writerow(['average_area_data', 'defects_data', 'min_contour_area', 'max_contour_area', 'iterationz', 'threshold_value'])
    
    for iterationz in iterationz_range:
        for min_contour_area in min_contour_area_range:
            for max_contour_area in max_contour_area_range:
                for threshold_value in threshold_range:
                    defects_data, average_area_data = process_image("defect_frame_12.png", min_contour_area, max_contour_area, threshold_value, iterationz)
                    
                    csv_writer.writerow([average_area_data, defects_data, min_contour_area, max_contour_area, iterationz, threshold_value])