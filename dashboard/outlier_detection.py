import cv2
import numpy as np
import imutils

img = cv2.imread("defect_frame_270.png")
img = cv2.normalize(img, img, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

ret, thresh = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
kernel = np.ones((3, 28), np.uint8)
opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=23)
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

edges = cv2.Canny(image=result2, threshold1=21, threshold2=21) 
cnts = cv2.findContours(edges, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key=cv2.contourArea, reverse=True)

for c in range(len(cnts)):
        [x, y, w, h] = cv2.boundingRect(cnts[c])
        if w*h<50000:
            cv2.rectangle(opening, (x, y), (x + w, y + h), (255, 255, 255), -1)     
opening=cv2.morphologyEx(opening, cv2.MORPH_OPEN, kernel = np.ones((5,5),np.uint8))
cnts = cv2.findContours(opening, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key=cv2.contourArea, reverse=True)

if len(cnts) >= 1:
    rot_rect = cv2.minAreaRect(cnts[0])
    (x, y), (w, h), angle = rot_rect
    box = cv2.boxPoints(rot_rect)
    box = np.int0(box)

    roi = img[min(box[:,1]):max(box[:,1]), min(box[:,0]):max(box[:,0])]

    img_gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
    imgу = cv2.normalize(img_gray, img_gray, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    _, binary_mask = cv2.threshold(imgу, 200, 255, cv2.THRESH_BINARY)
    contours, _ = cv2.findContours(binary_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    min_contour_area = 100
    max_contour_area = 500
    selected_contours = [contour for contour in contours if cv2.contourArea(contour) > min_contour_area and cv2.contourArea(contour) < max_contour_area]

    result = np.copy(roi)

    for contour in selected_contours:
        x, y, w, h = cv2.boundingRect(contour)
        cv2.drawContours(result, [contour], -1, (0, 255, 0), thickness=cv2.FILLED)
        cv2.rectangle(result, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cv2.putText(result, "Defect", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

else:
    print("No suitable contours found.")

cv2.imshow("Highlighted Areas with Rounded Rectangles (ROI)", result)
cv2.imshow("Initial image", img)

cv2.waitKey(0)
cv2.destroyAllWindows()
