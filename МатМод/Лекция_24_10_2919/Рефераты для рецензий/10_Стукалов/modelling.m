clc
clear all
close all
[t,Y]= ode15s('SIRSModel',[0,1000], [500000,10000,0,0])
healph = Y(:,1) + Y(:,3);
plot(t,Y(:,1),'-b');
hold on
plot(t,Y(:,2),'-r');
plot(t,Y(:,3),'-g');
plot(t,Y(:,4),'-k');
plot(t,healph,'c');
legend('Группа риска','Зараженные','Выздоровевшие','Погибшие','Здоровые',1);
grid on
