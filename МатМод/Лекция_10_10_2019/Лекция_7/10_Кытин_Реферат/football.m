x0 = 70;
y0 = 30;

fi0 = 1.38;
w0 = 65;  % Скороть вращения
U0 = 30;
ugol = pi/6;

g = 9.81;
R = 0.115;
Cd = 0.4;
Cm = 1/2;
ros = 74; % Отношение плотности
ro = 1;
kinem = 1.51*10^(-5); % Кинематическая вязкость воздуха при 20 градусах
M = 0.43;
delta = 2.5;

maxLeng = sin(2*ugol)*U0^2/g;
ner = (1+Cm*1/ros)*ros;
L = 8*ner*R/(3*Cd); % Характерный масштаб спирали, по которой в отсутствие гравитации двигался бы мяч
S = w0*R/U0; % Безразмерная величина, определяющая быстроту вращения мяча
T = L/U0;
Tw = 1/(3.1)*ros*R/sqrt(w0*kinem);

figure

Fwidth = 70;
Flength = 105;
Freserve = 7;
Glength = 3;
Gwidth = 7.32;
Fradius = 9.15;
sizePole = [0-Freserve Flength+Freserve 0-Freserve Fwidth+Freserve];

hold on
axis(sizePole)
fill( [0-Freserve Flength+Freserve Flength+Freserve 0-Freserve], [0-Freserve 0-Freserve Fwidth+Freserve Fwidth+Freserve], 'g')
rectangle('Position', [0 0 Flength Fwidth], 'EdgeColor', 'w')
rectangle('Position', [0 Fwidth/2-20 16.5 40], 'EdgeColor', 'w') % Левая штрафная
rectangle('Position', [Flength-16.5 Fwidth/2-20 16.5 40], 'EdgeColor', 'w') % Правые штрафная
rectangle('Position', [0-Glength Fwidth/2-Gwidth/2 Glength Gwidth], 'EdgeColor', 'w') % Левые ворота
rectangle('Position', [Flength Fwidth/2-Gwidth/2 Glength Gwidth], 'EdgeColor', 'w') % Правые ворота
rectangle('Position', [0 Fwidth/2-Gwidth/2-5.5 5.5 Gwidth+11], 'EdgeColor', 'w') % Левая вратарская
rectangle('Position', [Flength-5.5 Fwidth/2-Gwidth/2-5.5 5.5 Gwidth+11], 'EdgeColor', 'w') % Правая вратарская
rectangle('Position',[Flength/2-Fradius,Fwidth/2-Fradius,Fradius*2,Fradius*2],'Curvature',[1,1], 'EdgeColor', 'w') % Центральный круг
rectangle('Position',[11-0.15, Fwidth/2-0.15, 0.3, 0.3],'Curvature',[1,1], 'EdgeColor', 'w') % Левое пенальти
rectangle('Position',[Flength-11-0.15, Fwidth/2-0.15, 0.3, 0.3],'Curvature',[1,1], 'EdgeColor', 'w') % Правое пенальти
line([Flength/2 Flength/2], [0 Fwidth], 'Color', 'w')

s = 0:.1:maxLeng; % Перемещение
x = ones(1,length(s));
y = ones(1,length(s));

x(1) = x0;
y(1) = y0;
x2(1) = x0;
y2(1) = y0;

for i = 2:length(s)
    fi2(i) = fi0+delta*S*(exp(s(i)/L)-1);
    fi(i) =  fi0+delta*S*(exp(s(i)/L)-1)/(1+T/Tw*(exp(s(i)/L)-1));
    x(i) = sin(fi(i))*(s(i)-s(i-1))+x(i-1);
    y(i) = -cos(fi(i))*(s(i)-s(i-1))+y(i-1);
    x2(i) = sin(fi2(i))*(s(i)-s(i-1))+x2(i-1);
    y2(i) = -cos(fi2(i))*(s(i)-s(i-1))+y2(i-1);
   % plot(x(i), y(i), x2(i), y2(i), 'r')
end
comet(x,y)
plot(x,y,'r')
hold off