%Противоракетная оборона
close all;
%%
%Константы
g=9.8;
Tstart=150;
Vrmax=2000;
Varmax=3000;
aarmax=8*g;
t=0:1000;
P0=[0 0 0];
size_t=length(t);
dt=t(2)-t(1);
X0=-7e5;
Y0=1e5;
Z0=4e5;
rexpl=3e3;
Robn=1e4;
Vx0=2000;
Vy0=0;
Vz0=0;
p1=0.00001;
p2=0.00001;
% p1=0;
% p2=0;
%%

%%
%Ракета
X=initTargetTrajectory(size_t,dt,X0,Y0,Z0,Vx0,Vy0,Vz0, Vrmax);
Pr=X(:,1:size_t);
Vr=X(:,size_t:2*size_t);
plot3(Pr(1,:),Pr(2,:),Pr(3,:),[Pr(1,100) Pr(1,500)],[Pr(2,100) Pr(2,500)],[Pr(3,100) Pr(3,500)],'r*');
title('Траектория полета цели');
xlabel('x');
ylabel('y');
zlabel('z');
hold;
axis([min(Pr(1,:)) max(Pr(1,:)) -1e5 Y0+1e5 0 max(Pr(3,:))]);
grid
%%
%Радар
X=radar(Pr, Vr,size_t);
Pre=X(:,1:size_t);
Vre=X(:,size_t:2*size_t);
% Tstart=X(numel(X));
plot3(Pre(1,:), Pre(2,:), Pre(3,:),'g-');
%%
%антиракета
%Тупое наведение
X=firstMethod(Pr, Pre, Tstart, size_t, dt, Varmax, aarmax,Robn,rexpl);
Par1=X(:,1:size_t);
Var1=X(:,size_t:2*size_t);
aar1=X(:,size_t*2:size_t*3);
%%
%Поперечная сила
acc1=rocketPower(Var1, size_t);
figure 
plot(t, acc1/g);    
grid
title('Перегрузка антиракеты(По кривой погони)');
xlabel('Время');
ylabel('Единицы G');

%%
%Анимация 1-го способа наведения
drawTrajectory(Pr, Par1, aar1,0, size_t,Tstart, rexpl, p1,1)

%%
%Наведение с последовательным упреждением
X=secondMethod(Pr, Vr,Tstart,size_t, dt, Pre, Vre, Varmax, aarmax,Robn,rexpl);
Par2=X(:,1:size_t);
Var2=X(:,size_t:2*size_t);
aar2=X(:,size_t*2:size_t*3);
Upr2=X(:,size_t*3:size_t*4);
%%

%%
%
drawTrajectory(Pr, Par2, aar2,Upr2, size_t,Tstart, rexpl, p2,2)
%%
acc2=rocketPower(Var2, size_t);
figure;
plot(t, acc2/g); 
grid
title('Перегрузка антиракеты(последовательное упреждение');
xlabel('Время');
ylabel('Единицы G');

%% 
aircraftActions;
aircraftActions2;