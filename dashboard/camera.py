import math


Xa = 5; # X coordinate of the camera
angle_of_plate = 20; 


def Calculation(Ya, Za, horizontal, vertical):
    h = math.radians(horizontal)
    v = math.radians(vertical)
    ang_plate = math.radians(angle_of_plate)
    ha = math.sqrt((Ya)**2 + (Za-0.3)**2)
    Xb = Xa + ha*math.sin(h/2)
    Yb = 0
    Zb = 0.3
    Xc = Xa - ha*math.sin(h/2)
    Yc = 0
    Zc = 0.3
    hb = math.sqrt((Ya-1.7)**2 + (Za-0.91875)**2)
    Xd = Xa + hb*math.sin(h/2)
    Yd = 3.5*math.cos(ang_plate)
    Zd = 0.342020*Yd/0.939692+0.3
    Xe = Xa - hb*math.sin(h/2)
    Ye = 3.5*math.cos(ang_plate)
    Ze = 0.342020*Ye/0.939692+0.3
    L = int((vertical-10)/vertical*600 + 1)

    X = [[0] * (481) for i in range(L)]
    Y = [[0] * (481) for i in range(L)]
    for i in range(L):
        for j in range(481):
            X[i][j] = Xb+(Xc-Xb+(-Xd+Xe+Xb-Xc)/(L-1)*(i-1))/480*(j-1)-(-Xd+Xe+Xb-Xc)/(2*(L-1))*(i-1)
            Y[i][j] = (Yd-Yb)/(L-1)*(i-1)+0*j
         
    Yh = (Ya - 0.10919 + 0.36397 * Za)/1.13247
    Zh = 0.342020 * Yh/0.939692+0.3
    H = math.sqrt((Yh - Ya)**2 + (Zh - Za)**2)

    Xcent = [[0] * (480) for i in range(L-1)]
    Ycent = [[0] * (480) for i in range(L-1)]
    R = [[0] * (480) for i in range(L-1)]
    alpha = [[0] * (480) for i in range(L-1)]
    Zm = [[0] * (480) for i in range(L-1)]


    for i in range(L-1):
        for j in range(480):
            Xcent[i][j] = ((Y[i+1][j]-Y[i][j])*(X[i+1][j+1]-X[i][j])*(X[i][j+1]-X[i+1][j])+X[i][j]*(Y[i+1][j+1]-Y[i][j])*(X[i][j+1]-X[i+1][j])-X[i+1][j]*(Y[i][j+1]-Y[i+1][j])*(X[i+1][j+1]-X[i][j]))/((Y[i+1][j+1]-Y[i][j])*(X[i][j+1]-X[i+1][j])-(Y[i][j+1]-Y[i+1][j])*(X[i+1][j+1]-X[i][j]))
            Ycent[i][j]= (Xcent[i][j] - X[i][j])*(Y[i+1][j+1] - Y[i][j])/(X[i+1][j+1] - X[i][j]) + Y[i][j]
            R[i][j]= math.atan(H/math.sqrt((Xcent[i][j]-Xa)**2 + (Ycent[i][j]-Yh)**2 + (0.342020*Ycent[i][j]/0.939692+0.3-Zh)**2))
            alpha[i][j] = math.degrees(R[i][j])
            if alpha[i][j]>15 and alpha[i][j]<70 and Y[i][j] != 0:
                Zm[i][j] = 0.342020*Y[i][j]/0.939692+0.3
            else:
                Zm[i][j] = 0
    with open('Zm.txt','w') as f:
        for row in Zm:
            f.write(str(row) + "\n")     

Calculation(-0.309, 2.172, 60, 80)