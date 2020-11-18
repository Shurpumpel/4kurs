%%
%Задание 1
u=3;
ms=30;
mp=0;
mt0=220;
m0=250;
mt=mt0:-1:0;
v=u.*log(m0./(mp+ms+mt));
plot(mt,v);
hold on;
grid on;
xlabel('масса топлива, т');
ylabel('скорость ракеты, км/ч');
set(gca,'XDir','reverse');
vmax=u*log(m0/(mp+ms))
%%
%Задание 2
m0=308;
m1=173;
m2=100;
m3=25;
mp=12;
u=3;
lambda=0.1; 
mt1=m0:-0.5:m0-m1*(1-lambda);
v1=0+u*log(m0./(mt1));
plot(mt1, v1, 'LineWidth',2);
grid on; 
hold on; 
mt2=m0-m1:-0.5:m0-m1-m2*(1-lambda);
v2=v1(length(v1))+u*log((m0-m1)./(mt2));
plot(mt2, v2,'r','LineWidth',2); 
mt3=m0-m1-m2:-0.5:m0-m1-m2-m3*(1-lambda);
v3=v2(length(v2))+u*log((m0-m1-m2)./(mt3));
plot(mt3, v3,'g','LineWidth',2); 
legend('изменение скорости со всеми ступенями','изменение скорости с отброшенной 1 ступенью','изменение скорости с отброшенными 1 и 2 ступенями');
set(gca, 'XDir','reverse');
xlabel('масса ракеты, т');
ylabel('скорость ракеты, км/с'); 
vmax=u*log((m0/(mp+lambda*m1+m2+m3))*((mp+m2+m3)/(mp+lambda*m2+m3))*((mp+m3)/(mp+lambda*m3)))

