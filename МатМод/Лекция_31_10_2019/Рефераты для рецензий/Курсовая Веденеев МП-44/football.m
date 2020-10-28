function football = football(L, chosenPoint, v0, phi)
clc
close all

hg = 2.2;
hw = 1.75;
d = 24;
dw = 9; % расстояние от стенки до мяча
delta = 0.4; % разброс
gkPoint = randi(4,1);

figure
title('График полета мяча')
xlabel('x')
ylabel('y')

y1 = 0: 0.01: 1.9;
x1 = ones(1, length(y1)) * 9;
plot(x1, y1, '*g')
hold on

y2 = 0: 0.01: 2.2;
x2 = ones(1, length(y2)) * L;
plot(x2, y2, '*r')

hold on
grid on
g=9.8;
v0x=v0*cos(phi);    
v0y=v0*sin(phi);
t1= v0y/g ;
t2= 2* t1;
t=0:0.01:t2;
x = v0x*t;
y = v0y*t-(g*t.*t)/2;

plot(x,y)
hold on


if y(floor(length(x)*9/x(length(x)))) > 1.9
if chosenPoint ~= gkPoint
    if chosenPoint == 1 || chosenPoint == 2
        if y(floor(length(x)*L/x(length(x)))) < delta && y(floor(length(x)*L/x(length(x)))) > 0
            title('Гол забит')
        else
            title('Не попали в угол или не добили до ворот')
        end
    else
        if y(floor(length(x)*L/x(length(x)))) > hg || y(floor(length(x)*L/x(length(x)))) < hg - delta
            title('Гол забит')
        else
            title('Не попали в угол или не добили до ворот')
        end
    end
else
    title('Вратарь отбил удар')
end
else
    title('Мяч попал в стенку')
end

%axis equal