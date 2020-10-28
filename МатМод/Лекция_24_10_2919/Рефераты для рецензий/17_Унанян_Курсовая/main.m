clc
clear
global L r mu s 




L=10; r=10; mu=0.1; s=2;
[h,t1]=ode45('yp1',[2*r 0],0);
figure
plot(h,t1,'-r')
%axis([0 35 0 5])
grid
title('�������������� �������');
xlabel('�������');
ylabel('�����');

L=10; r=10; mu=0.1; s=2;
[h,t1]=ode45('ap1',[L 0],0);
figure
plot(h,t1,'-r')
%axis([0 35 0 5])
grid
title('������������ �������');
xlabel('�������');
ylabel('�����');


r=(3*L*r*r/4)^(1/3); L=2*r; mu=0.1; s=2;
[h,t1]=ode45('sp1',[L 0],0);
figure
plot(h,t1,'-r')
%axis([0 35 0 5])
grid
title('���');
xlabel('�������');
ylabel('�����');


r=r*(2)^(1/3);  mu=0.1; s=2;
L=r;
[h,t1]=ode45('sp1',[L 0],0);
figure
plot(h,t1,'-r')
%axis([0 35 0 5])
grid
title('�������');
xlabel('�������');
ylabel('�����');

L=10; r=10; mu=0.1; s=2;
L=10; r=(3.14*r*r*L)^(1/3); mu=0.1; s=2;
[h,t1]=ode45('tp1',[r 0],0);
figure
plot(h,t1,'-r')
%axis([0 35 0 5])
grid
title('������');
xlabel('�������');
ylabel('�����');

