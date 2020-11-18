
%Задание 1
h=5;
g=9.8;
v0=800;
t=sqrt(2*h/g);
l=v0*t;
L=normrnd(l,20,500,1);
v=L.*sqrt(g/2/h);
plot(L,v);
hold on;
grid on;
xlabel('Дальность выстрела');
ylabel('Скорость пули');
vmin=min(v);
vmax=max(v);
x=vmin:(vmax-vmin)/10:vmax;
figure;
hist(v,x);
grid on
title('Распределение скоростей пули');
%ошибка
s=0;
for i=1:500
s=s+(l-L(i))*(l-L(i));
end
dL= sqrt(s/500);
error = dL/l * 100
%%
%Задание 2
g=9.8;
v0=800;
L=5;
M=50;
m=0.005;
N=500;
c=130;
l=25000;
dt=370;
a0=acos(1-(m*v0*v0)/(2*(m+M)*g*L) + (c*m*dt+l*m)/((m+M)*g*L));
a=normrnd(a0,a0*0.01,N,1);
v=sqrt( 2*( (m+M)*g*L*(1-cos(a)) + (c*m*dt+l*m)) / m );
plot(a,v);
hold on;
grid on;
xlabel('Угол отклонения');
ylabel('Скорость пули');
vmin=min(v);
vmax=max(v);
x=vmin:(vmax-vmin)/10:vmax;
figure;
hist(v,x);
grid on
%ощибка
sum=0;
for i=1:N 
    sum = sum + abs(a0-a(i))*abs(a0-a(i)); 
end 
error = sqrt(sum/N) * 100
%%
%Задание 3
g=9.8;
v0=800;
L=5; 
M=50;
m=0.005; 
N=500;
a0=acos(1-(m*m*v0*v0)/(2*(m+M)*(m+M)*g*L)); 
a=normrnd(a0,a0*0.01,N,1); 
v=sqrt(2*((m+M)*g*L*(1-cos(a)))/m)*sqrt((m+M)/m); 
plot(a,v);
hold on;
grid on;
xlabel('Угол отклонения');
ylabel('Скорость пули');
vmin=min(v);
vmax=max(v);
x=vmin:(vmax-vmin)/10:vmax;
figure;
hist(v,x);
grid on
%ошибка
sum=0;
for i=1:N 
    sum = sum + abs(a0-a(i))*abs(a0-a(i)); 
end 
error = sqrt(sum/N) * 100