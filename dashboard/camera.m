clear
%coordinates of the camera
Xa = 5;
Ya = -0.309;
Za = 2.172;
%view angle of the camera, where e is horisontal and v is vertical
e = 80;
v = 60;
%distance from camera to the corners of the botton edge of the panel
ha = sqrt((Ya)^2 + (Za-0.3)^2);
%panel has corners and points B C D E
%plotting AB
Xb = Xa + ha*sind(e/2);
Yb = 0;
Zb = 0.3;
plot3([Xa Xb], [Ya Yb], [Za Zb])
hold on
%plotting AC
Xc = Xa - ha*sind(e/2);
Yc = 0;
Zc = 0.3;
plot3([Xa Xc], [Ya Yc], [Za Zc])
Yd = 3.5*cosd(20);
Zd = 0.342020*Yd/0.939692+0.3;
%distance from camera to B point
hb = sqrt((Ya-Yd)^2 + (Za-Zd)^2);
Xd = Xa + hb*sind(e/2);
%plotting AD
plot3([Xa Xd], [Ya Yd], [Za Zd])
Xe = Xa - hb*sind(e/2);
Ye = 3.5*cosd(20);
%that's the equation of the surface of our panel
Ze = 0.342020*Ye/0.939692+0.3;
%plotting AE
plot3([Xa Xe], [Ya Ye], [Za Ze])
%as I have 640x480 pixels we fullfill the arrays X and Y with edges of each
%pixel
for i = 1:481
    for j = 1:(640 + 1)
            X(i,j) = Xc+(Xb-Xc+2*(Xc-Xe)*(i-1)/480)/640*(j-1)-(Xc-Xe)/480*(i-1);
            Y(i,j) = (Yd-Yb)/(480)*(i-1)+0*j;
    end
end
%calculating perpendicular 
Yh = (Ya - 0.10919 + 0.36397 * Za)/1.13247;
Zh = 0.342020 * Yh/0.939692+0.3;
H = sqrt((Yh - Ya)^2 + (Zh - Za)^2);
%all surface of the panel devided by quantity of pixels (average pixel
%size)
S1 = 0.5*(-Xc+Xb-Xe+Xd)*3.5/(480*640);
%calculating center points of every pixel
for i = 1:480
    for j = 1:(640)
            Xcent(i,j) = ((Y(i+1,j)-Y(i,j))*(X(i+1,j+1)-X(i,j))*(X(i,j+1)-X(i+1, ...
            j))+X(i,j)*(Y(i+1,j+1)-Y(i,j))*(X(i,j+1)-X(i+1,j))-X(i+1,j)*( ...
            Y(i,j+1)-Y(i+1,j))*(X(i+1,j+1)-X(i,j)))/((Y(i+1,j+1)-Y(i,j))*( ...
            X(i,j+1)-X(i+1,j))-(Y(i,j+1)-Y(i+1,j))*(X(i+1,j+1)-X(i,j)));
            Ycent(i,j) = (Xcent(i,j) - X(i,j))*(Y(i + 1, j + 1) - Y(i,j))/(X(i + 1, ...
            j + 1) - X(i,j)) + Y(i,j);
            %calculating angle from camera to every center point of pixel 
            R(i,j) = atan(H/sqrt((Xcent(i,j)-Xa)^2 + (Ycent(i,j)-Yh)^2 + ...
            (0.342020*Ycent(i,j)/0.939692+0.3-Zh)^2));
            %coverting it to degrees
            alpha(i,j) = rad2deg(R(i,j));
            %to find square of the pixel we need to find coefficient k in
            %(y=kx+b) of its diagonals 
            k1(i,j) = (Y(i+1,j+1)-Y(i,j))/(X(i+1,j+1)-X(i,j));
            k2(i,j) = (Y(i,j+1)-Y(i+1,j))/(X(i,j+1)-X(i+1,j));
            %then agnle between them
            fi(i,j) = atan((abs((k1(i,j)-k2(i,j))/(1+k1(i,j)*k2(i,j)))));
            %and finally square
            S(i,j) = 0.5 * sqrt((X(i,j)-X(i+1,j+1))^2 + (Y(i,j)-Y(i+1,j+1))^2 + ...
            (0.342020*Y(i,j)/0.939692 - 0.342020*Y(i+1,j+1)/ ...
            0.939692)^2) * sqrt((X(i+1,j)-X(i,j+1))^2 + (Y(i+1,j)-Y(i,j+1))^2 + ...
            (0.342020*Y(i+1,j)/0.939692 - 0.342020*Y(i,j+1)/ ...
            0.939692)^2) * sin(fi(i,j));
            %here we are checking if angles is in the range of 15-70 and
            %the size of the pixel should be at least 80% of S1
              if (S(i,j)>0.8*S1)
                Xm(i,j) = X(i,j);
                Ym(i,j) = Y(i,j);
                if (Ym(i,j) ~= 0)
                Zm(i,j) = 0.342020*Ym(i,j)/0.939692+0.3;
                end
            end
    end
end


    
surf(Zm);
shading interp;
axis equal;
