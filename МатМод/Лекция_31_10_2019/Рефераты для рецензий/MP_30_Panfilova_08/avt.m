function avt=avt(alfa, alfa1) %alfa - обстановка на дороге (от 0.9 до 1.4), alfa1 - состояние дороги для пешехода (от 0.9 до 1.5)
m=90; %максимальная вместимость автобуса
t1=0.20; %среднее время входа в автобус для одного человека
t2=0.08; %среднее время выхода из автобуса для одного человека
v=400; %средняя скорость движения автобуса, м/мин
ost=[9 8 6 7 11 14 3]; %массив остановок до ДБ для 7 автобусов
interval=[9 11 8 7 6 14 11]; %интервалы движения
n=0;
t=[0 0 0 0 0 0 0];
t_11=[0 0 0 0 0 0 0];
for j=1:7 %рассчитываем время, необходимое каждому автобусу, чтобы добраться до ДБ от начальной станции
    for i=1:ost(j)
        y=randint(1,1,[1 15]);
        if(y<(m-n))
        n1=y;
        else
        y=randint(1,1,[1 m-n]);
        end
    n2=randint(1,1, [0 n]);
    t_st=abs(n1*t1-n2*t2);
    n=n+n1-n2;
    s=randint(1,1, [400 800]);
    t_dv=(s/v);
    svf=randint(1,1, [0 1]);
    per=randint(1,1, [0 2]);
    t_per=randint(1,1,[0 15])/60;
    t_svf=randint(1,1,[0 45])/60;
    t_z=svf*t_svf+per*t_per;
    t(j)=(t_st+alfa*t_dv+t_z)+t(j);
    end
    n=0;
end 
for j=1:7 %рассчитываем время, необходимое каждому автобусу, чтобы добраться от ДБ до МИЭТа
    for i=1:3
        y=randint(1,1,[1 15]);
        if(y<(m-n))
        n1=y;
        else
        y=randint(1,1,[1 m-n]);
        end
    n2=randint(1,1, [0 n]);
    t_st=abs(n1*t1-n2*t2);
    n=n+n1-n2;
    s=randint(1,1, [400 800]);
    t_dv=(s/v);
    svf=randint(1,1, [0 1]);
    per=randint(1,1, [0 2]);
    t_per=randint(1,1,[0 15])/60;
    t_svf=randint(1,1,[0 45])/60;
    t_z=svf*t_svf+per*t_per;
    t_11(j)=(t_st+alfa*t_dv+t_z)+t_11(j);
    end
    n=0;
end
vr_otpr3=[5; 6]; %время отправления первого автобуса с начальной станции
vr_otpr8=[5; 26];
vr_otpr9=[5; 20];
vr_otpr11=[4; 54];
vr_otpr19=[5; 19];
vr_otpr29=[6; 00];
vr_otpr32=[5; 40];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
t_p_z=randint(1,1,[0 40])/60;
s_p=900; %расстояние между общежитием и ДБ
v_p=85.8; %средняя скорость движения человека, м/мин
t_p=(s_p/v_p)*alfa1+t_p_z; %время, затраченное на путь от общежития до ДБ
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
p=zeros(1,60);%здесь будет массив значений времени, между 8 и 9 часами, когда автобусы будут на остановке ДБ
q=zeros(1,60); %массив значений времени прибытия «оптимальных» автобусов к МИЭТу
w=zeros(1,60); %массив значений времени прибытия «оптимальных» автобусов к ДБ
r=zeros(1,60); %массив значений времени выхода из общежития, чтобы успеть на «оптимальные» автобусы
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Вариант №1. Общежитие - "Дом Быта" - МИЭТ');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:50
    betta=randint(1, 1, [9 12])/10;
    vr_otpr3(2,j)=vr_otpr3(2,j-1)+interval(1)*betta;
    vr_otpr3(1,j)=vr_otpr3(1,j-1);
    vr_pr3(1,j)=vr_otpr3(1,j);
    vr_pr3(2,j)=vr_otpr3(2,j);
    if(vr_otpr3(2,j)>=60)
        vr_otpr3(1,j)=vr_otpr3(1,j)+1;
        vr_otpr3(2,j)=vr_otpr3(2,j)-60;
    end
    if(vr_pr3(2,j)>=60)
        vr_pr3(1,j)=vr_pr3(1,j)+1;
        vr_pr3(2,j)=vr_pr3(2,j)-60;
    end
    vr_otpr3(4,j)=vr_otpr3(2,j)+t(1);
    vr_pr3(4,j)=vr_otpr3(4,j)+t_11(1);
    if(vr_otpr3(4,j)>=60)
        vr_otpr3(3,j)=vr_otpr3(1,j)+1;
        vr_otpr3(4,j)=vr_otpr3(4,j)-60;
    else
        vr_otpr3(3,j)=vr_otpr3(1,j);
    end
    if(vr_pr3(4,j)>=60)
        vr_pr3(3,j)=vr_pr3(1,j)+1;
        vr_pr3(4,j)=vr_pr3(4,j)-60;
    else
        vr_pr3(3,j)=vr_pr3(1,j);
    end
end
for j=2:50
    if((vr_otpr3(3,j)==8)&&(vr_otpr3(3,j-1)==7))
        a(1)=j;
    end
    if ((vr_otpr3(3,j)==9))
        b(1)=j-1;
        break;
    end
end
for j=2:50
    if((vr_pr3(3,j)==8)&&(vr_pr3(4,j)>=35))
        c(1)=j;
    end
    if ((vr_pr3(3,j)==9))
        d(1)=j-1;
        break;
    end
end
disp('Автобус №3');
for i=a(1):b(1)
    vr_otpr3(:,i);
    p(ceil(vr_otpr3(4,i)))=p(ceil(vr_otpr3(4,i)))+1;
end
for i=c(1)-1:d(1)
    vr_otpr3(:,i);
    w(ceil(vr_otpr3(4,i)))=w(ceil(vr_otpr3(4,i)))+1;
    r(ceil(vr_otpr3(4,i)-t_p))=r(ceil(vr_otpr3(4,i)-t_p))+1;
    t_start=ceil(vr_otpr3(4,i)-t_p);
    disp(t_start);
end
for i=c(1)-1:d(1)
    vr_pr3(:,i);
    q(ceil(vr_pr3(4,i)))=q(ceil(vr_pr3(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:70
    betta=randint(1, 1, [9 12])/10;
    vr_otpr8(2,j)=vr_otpr8(2,j-1)+interval(2)*betta;
    vr_otpr8(1,j)=vr_otpr8(1,j-1);
    vr_pr8(1,j)=vr_otpr8(1,j);
    vr_pr8(2,j)=vr_otpr8(2,j);
    if(vr_otpr8(2,j)>=60)
        vr_otpr8(1,j)=vr_otpr8(1,j)+1;
        vr_otpr8(2,j)=vr_otpr8(2,j)-60;
    end
    if(vr_pr8(2,j)>=60)
        vr_pr8(1,j)=vr_pr8(1,j)+1;
        vr_pr8(2,j)=vr_pr8(2,j)-60;
    end
    vr_otpr8(4,j)=vr_otpr8(2,j)+t(2);
    vr_pr8(4,j)=vr_otpr8(4,j)+t_11(2);
    if(vr_otpr8(4,j)>=60)
        vr_otpr8(3,j)=vr_otpr8(1,j)+1;
        vr_otpr8(4,j)=vr_otpr8(4,j)-60;
    else
        vr_otpr8(3,j)=vr_otpr8(1,j);
    end
    if(vr_pr8(4,j)>=60)
        vr_pr8(3,j)=vr_pr8(1,j)+1;
        vr_pr8(4,j)=vr_pr8(4,j)-60;
    else
        vr_pr8(3,j)=vr_pr8(1,j);
    end
    if((vr_otpr8(3,j)==8)&&(vr_otpr8(3,j-1)==7))
        a(2)=j;
    end
    if ((vr_otpr8(3,j)==9))
        b(2)=j-1;
        break;
    end
end
for j=2:70
    if((vr_pr8(3,j)==8)&&(vr_pr8(4,j)>=35))
       c(2)=j;
    end
    if ((vr_pr8(3,j)==9))
       d(2)=j-1;
       break;
    end    
end
for i=a(2):b(2)
    vr_otpr8(:,i);
    p(ceil(vr_otpr8(4,i)))=p(ceil(vr_otpr8(4,i)))+1;
end
disp('Автобус №8');
for i=c(2)-1:d(2)
    vr_otpr8(:,i);
    w(ceil(vr_otpr8(4,i)))=w(ceil(vr_otpr8(4,i)))+1;
    r(ceil(vr_otpr8(4,i)-t_p))=r(ceil(vr_otpr8(4,i)-t_p))+1;
    t_start=ceil(vr_otpr8(4,i)-t_p);
    disp(t_start);
end
for i=c(2)-1:d(2)
    vr_pr8(:,i);
    q(ceil(vr_pr8(4,i)))=q(ceil(vr_pr8(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:70
    betta=randint(1, 1, [9 12])/10;
    vr_otpr9(2,j)=vr_otpr9(2,j-1)+interval(2)*betta;
    vr_otpr9(1,j)=vr_otpr9(1,j-1);
    vr_pr9(1,j)=vr_otpr9(1,j);
    vr_pr9(2,j)=vr_otpr9(2,j);
    if(vr_otpr9(2,j)>=60)
        vr_otpr9(1,j)=vr_otpr9(1,j)+1;
        vr_otpr9(2,j)=vr_otpr9(2,j)-60;
    end
    if(vr_pr9(2,j)>=60)
        vr_pr9(1,j)=vr_pr9(1,j)+1;
        vr_pr9(2,j)=vr_pr9(2,j)-60;
    end
    vr_otpr9(4,j)=vr_otpr9(2,j)+t(3);
    vr_pr9(4,j)=vr_otpr9(4,j)+t_11(3);
    if(vr_otpr9(4,j)>=60)
        vr_otpr9(3,j)=vr_otpr9(1,j)+1;
        vr_otpr9(4,j)=vr_otpr9(4,j)-60;
    else
        vr_otpr9(3,j)=vr_otpr9(1,j);
    end
    if(vr_pr9(4,j)>=60)
        vr_pr9(3,j)=vr_pr9(1,j)+1;
        vr_pr9(4,j)=vr_pr9(4,j)-60;
    else
        vr_pr9(3,j)=vr_pr9(1,j);
    end
    if((vr_otpr9(3,j)==8)&&(vr_otpr9(3,j-1)==7))
        a(3)=j;
    end
    if ((vr_otpr9(3,j)==9))
        b(3)=j-1;
        break;
    end
end
for j=2:70
    if((vr_pr9(3,j)==8)&&(vr_pr9(4,j)>=35))
       c(3)=j;
    end
    if ((vr_pr9(3,j)==9))
       d(3)=j-1;
       break;
    end    
end
for i=a(3):b(3)
    vr_otpr9(:,i);
    p(ceil(vr_otpr9(4,i)))=p(ceil(vr_otpr9(4,i)))+1;
end
disp('Автобус №9');
for i=c(3)-1:d(3)
    vr_otpr9(:,i);
    w(ceil(vr_otpr9(4,i)))=w(ceil(vr_otpr9(4,i)))+1;
    r(ceil(vr_otpr9(4,i)-t_p))=r(ceil(vr_otpr9(4,i)-t_p))+1;
    t_start=ceil(vr_otpr9(4,i)-t_p);
    disp(t_start);
end
for i=c(3)-1:d(3)
    vr_pr9(:,i);
    q(ceil(vr_pr9(4,i)))=q(ceil(vr_pr9(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:70
    betta=randint(1, 1, [9 12])/10;
    vr_otpr11(2,j)=vr_otpr11(2,j-1)+interval(4)*betta;
    vr_otpr11(1,j)=vr_otpr11(1,j-1);
    vr_pr11(1,j)=vr_otpr11(1,j);
    vr_pr11(2,j)=vr_otpr11(2,j);
    if(vr_otpr11(2,j)>=60)
        vr_otpr11(1,j)=vr_otpr11(1,j)+1;
        vr_otpr11(2,j)=vr_otpr11(2,j)-60;
    end
    if(vr_pr11(2,j)>=60)
        vr_pr11(1,j)=vr_pr11(1,j)+1;
        vr_pr11(2,j)=vr_pr11(2,j)-60;
    end
    vr_otpr11(4,j)=vr_otpr11(2,j)+t(4);
    vr_pr11(4,j)=vr_otpr11(4,j)+t_11(4);
    if(vr_otpr11(4,j)>=60)
        vr_otpr11(3,j)=vr_otpr11(1,j)+1;
        vr_otpr11(4,j)=vr_otpr11(4,j)-60;
    else
        vr_otpr11(3,j)=vr_otpr11(1,j);
    end
    if(vr_pr11(4,j)>=60)
        vr_pr11(3,j)=vr_pr11(1,j)+1;
        vr_pr11(4,j)=vr_pr11(4,j)-60;
    else
        vr_pr11(3,j)=vr_pr11(1,j);
    end
    if((vr_otpr11(3,j)==8)&&(vr_otpr11(3,j-1)==7))
        a(4)=j;
    end
    if ((vr_otpr11(3,j)==9))
        b(4)=j-1;
        break;
    end
end
for j=2:70
    if((vr_pr11(3,j)==8)&&(vr_pr11(4,j)>=35))
       c(4)=j;
    end
    if ((vr_pr11(3,j)==9))
       d(4)=j-1;
       break;
    end    
end
for i=a(4):b(4)
    vr_otpr11(:,i);
    p(ceil(vr_otpr11(4,i)))=p(ceil(vr_otpr11(4,i)))+1;
end
disp('Автобус №11');
for i=c(4)-1:d(4)
    vr_otpr11(:,i);
    w(ceil(vr_otpr11(4,i)))=w(ceil(vr_otpr11(4,i)))+1;
    r(ceil(vr_otpr11(4,i)-t_p))=r(ceil(vr_otpr11(4,i)-t_p))+1;
    t_start=ceil(vr_otpr11(4,i)-t_p);
    disp(t_start);
end
for i=c(4)-1:d(4)
    vr_pr11(:,i);
    q(ceil(vr_pr11(4,i)))=q(ceil(vr_pr11(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:70
    betta=randint(1, 1, [9 12])/10;
    vr_otpr19(2,j)=vr_otpr19(2,j-1)+interval(5)*betta;
    vr_otpr19(1,j)=vr_otpr19(1,j-1);
    vr_pr19(1,j)=vr_otpr19(1,j);
    vr_pr19(2,j)=vr_otpr19(2,j);
    if(vr_otpr19(2,j)>=60)
        vr_otpr19(1,j)=vr_otpr19(1,j)+1;
        vr_otpr19(2,j)=vr_otpr19(2,j)-60;
    end
    if(vr_pr19(2,j)>=60)
        vr_pr19(1,j)=vr_pr19(1,j)+1;
        vr_pr19(2,j)=vr_pr19(2,j)-60;
    end
    vr_otpr19(4,j)=vr_otpr19(2,j)+t(5);
    vr_pr19(4,j)=vr_otpr19(4,j)+t_11(5);
    if(vr_otpr19(4,j)>=60)
        vr_otpr19(3,j)=vr_otpr19(1,j)+1;
        vr_otpr19(4,j)=vr_otpr19(4,j)-60;
    else
        vr_otpr19(3,j)=vr_otpr19(1,j);
    end
    if(vr_pr19(4,j)>=60)
        vr_pr19(3,j)=vr_pr19(1,j)+1;
        vr_pr19(4,j)=vr_pr19(4,j)-60;
    else
        vr_pr19(3,j)=vr_pr19(1,j);
    end
    if((vr_otpr19(3,j)==8)&&(vr_otpr19(3,j-1)==7))
        a(5)=j;
    end
    if ((vr_otpr19(3,j)==9))
        b(5)=j-1;
        break;
    end
end
for j=2:70
    if((vr_pr19(3,j)==8)&&(vr_pr19(4,j)>=35))
       c(5)=j;
    end
    if ((vr_pr19(3,j)==9))
       d(5)=j-1;
       break;
    end    
end
for i=a(5):b(5)
    vr_otpr19(:,i);
    p(ceil(vr_otpr19(4,i)))=p(ceil(vr_otpr19(4,i)))+1;
end
disp('Автобус №19');
for i=c(5)-1:d(5)
    vr_otpr19(:,i);
    w(ceil(vr_otpr19(4,i)))=w(ceil(vr_otpr19(4,i)))+1;
    r(ceil(vr_otpr19(4,i)-t_p))=r(ceil(vr_otpr19(4,i)-t_p))+1;
    t_start=ceil(vr_otpr19(4,i)-t_p);
    disp(t_start);
end
for i=c(5)-1:d(5)
    vr_pr19(:,i);
    q(ceil(vr_pr19(4,i)))=q(ceil(vr_pr19(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:70
    betta=randint(1, 1, [9 12])/10;
    vr_otpr29(2,j)=vr_otpr29(2,j-1)+interval(6)*betta;
    vr_otpr29(1,j)=vr_otpr29(1,j-1);
    vr_pr29(1,j)=vr_otpr29(1,j);
    vr_pr29(2,j)=vr_otpr29(2,j);
    if(vr_otpr29(2,j)>=60)
        vr_otpr29(1,j)=vr_otpr29(1,j)+1;
        vr_otpr29(2,j)=vr_otpr29(2,j)-60;
    end
    if(vr_pr29(2,j)>=60)
        vr_pr29(1,j)=vr_pr29(1,j)+1;
        vr_pr29(2,j)=vr_pr29(2,j)-60;
    end
    vr_otpr29(4,j)=vr_otpr29(2,j)+t(6);
    vr_pr29(4,j)=vr_otpr29(4,j)+t_11(6);
    if(vr_otpr29(4,j)>=60)
        vr_otpr29(3,j)=vr_otpr29(1,j)+1;
        vr_otpr29(4,j)=vr_otpr29(4,j)-60;
    else
        vr_otpr29(3,j)=vr_otpr29(1,j);
    end
    if(vr_pr29(4,j)>=60)
        vr_pr29(3,j)=vr_pr29(1,j)+1;
        vr_pr29(4,j)=vr_pr29(4,j)-60;
    else
        vr_pr29(3,j)=vr_pr29(1,j);
    end
    if((vr_otpr29(3,j)==8)&&(vr_otpr29(3,j-1)==7))
        a(6)=j;
    end
    if ((vr_otpr29(3,j)==9))
        b(6)=j-1;
        break;
    end
end
for j=2:70
    if((vr_pr29(3,j)==8)&&(vr_pr29(4,j)>=35))
       c(6)=j;
    end
    if ((vr_pr29(3,j)==9))
       d(6)=j-1;
       break;
    end    
end
for i=a(6):b(6)
    vr_otpr29(:,i);
    p(ceil(vr_otpr29(4,i)))=p(ceil(vr_otpr29(4,i)))+1;
end
disp('Автобус №29');
for i=c(6)-1:d(6)
    vr_otpr29(:,i);
    w(ceil(vr_otpr29(4,i)))=w(ceil(vr_otpr29(4,i)))+1;
    r(ceil(vr_otpr29(4,i)-t_p))=r(ceil(vr_otpr29(4,i)-t_p))+1;
    t_start=ceil(vr_otpr29(4,i)-t_p);
    disp(t_start);
end
for i=c(6)-1:d(6)
    vr_pr29(:,i);
    q(ceil(vr_pr29(4,i)))=q(ceil(vr_pr29(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for j=2:70
    betta=randint(1, 1, [9 12])/10;
    vr_otpr32(2,j)=vr_otpr32(2,j-1)+interval(7)*betta;
    vr_otpr32(1,j)=vr_otpr32(1,j-1);
    vr_pr32(1,j)=vr_otpr32(1,j);
    vr_pr32(2,j)=vr_otpr32(2,j);
    if(vr_otpr32(2,j)>=60)
        vr_otpr32(1,j)=vr_otpr32(1,j)+1;
        vr_otpr32(2,j)=vr_otpr32(2,j)-60;
    end
    if(vr_pr32(2,j)>=60)
        vr_pr32(1,j)=vr_pr32(1,j)+1;
        vr_pr32(2,j)=vr_pr32(2,j)-60;
    end
    vr_otpr32(4,j)=vr_otpr32(2,j)+t(7);
    vr_pr32(4,j)=vr_otpr32(4,j)+t_11(7);
    if(vr_otpr32(4,j)>=60)
        vr_otpr32(3,j)=vr_otpr32(1,j)+1;
        vr_otpr32(4,j)=vr_otpr32(4,j)-60;
    else
        vr_otpr32(3,j)=vr_otpr32(1,j);
    end
    if(vr_pr32(4,j)>=60)
        vr_pr32(3,j)=vr_pr32(1,j)+1;
        vr_pr32(4,j)=vr_pr32(4,j)-60;
    else
        vr_pr32(3,j)=vr_pr32(1,j);
    end
    if((vr_otpr32(3,j)==8)&&(vr_otpr32(3,j-1)==7))
        a(7)=j;
    end
    if ((vr_otpr32(3,j)==9))
        b(7)=j-1;
        break;
    end
end
for j=2:70
    if((vr_pr32(3,j)==8)&&(vr_pr32(4,j)>=35))
       c(7)=j;
    end
    if ((vr_pr32(3,j)==9))
       d(7)=j-1;
       break;
    end    
end
for i=a(7):b(7)
    vr_otpr32(:,i);
    p(ceil(vr_otpr32(4,i)))=p(ceil(vr_otpr32(4,i)))+1;
end
disp('Автобус №32');
for i=c(7)-1:d(7)
    vr_otpr32(:,i);
    w(ceil(vr_otpr32(4,i)))=w(ceil(vr_otpr32(4,i)))+1;
    r(ceil(vr_otpr32(4,i)-t_p))=r(ceil(vr_otpr32(4,i)-t_p))+1;
    t_start=ceil(vr_otpr32(4,i)-t_p);
    disp(t_start);
end
for i=c(7)-1:d(7)
    vr_pr32(:,i);
    q(ceil(vr_pr32(4,i)))=q(ceil(vr_pr32(4,i)))+1;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hold off;
x=1:60;
subplot(4,1,1);
title('Автобусы на остановке "Дом Быта" между 8 и 9 часами');
xlabel('Время');
ylabel('Количество автобусов');
hold on
bar(x,p)
subplot(4,1,2);
title('Оптимальное время выхода из общежития между 8 и 9 часами');
xlabel('Время');
hold on
bar(x,r)
subplot(4,1,3);
title('Время нахождения "оптимальных" автобусов на "Доме быта"');
xlabel('Время');
ylabel('Количество автобусов');
hold on
bar(x,w)
subplot(4,1,4);
title('Время прибытия "оптимальных" автобусов к МИЭТу');
xlabel('Время');
hold on
bar(x,q)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
s_m=2500; %расстояние от общежития до МИЭТа
v_m=v_p;
t_m_o=randint(1,1,[0 40])/60;
t_m=(s_m/v_m)*alfa1+t_m_o; %время, за которое студент дойдёт до МИЭТа
t_m_v(1)=8;
t_m_v(2)=ceil(55-t_m); %минуты, когда надо выйти, чтобы прийти в МИЭТ 
disp('Вариант №2. Общежитие - МИЭТ. Пешком');
t_m_v=t_m_v;
disp(t_m_v);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
t_19_d=0;
disp('Вариант №3. Общежитие - МИЭТ. Автобус №19');
f=zeros(1,60);
g=zeros(1,60);
vr_otpr19_1=[5; 19];
for i=1:8
    y=randint(1,1,[1 15]);
    if(y<(m-n))
    n1=y;
    else
        y=randint(1,1,[1 m-n]);
    end
    n2=randint(1,1, [0 n]);
    t_st=abs(n1*t1-n2*t2);
    n=n+n1-n2;
    s=randint(1,1, [400 800]);
    t_dv=(s/v);
    svf=randint(1,1, [0 1]);
    per=randint(1,1, [0 2]);
    t_per=randint(1,1,[0 15])/60;
    t_svf=randint(1,1,[0 45])/60;
    t_z=svf*t_svf+per*t_per;
    t_19_d=(t_st+alfa*t_dv+t_z)+t_19_d;
end %время, которое автобус едет от нач. станции до "Студенческой"
t_19_d;
t_19_af=0;
for i=1:8
    y=randint(1,1,[1 15]);
    if(y<(m-n))
    n1=y;
    else
        y=randint(1,1,[1 m-n]);
    end
    n2=randint(1,1, [0 n]);
    t_st=abs(n1*t1-n2*t2);
    n=n+n1-n2;
    s=randint(1,1, [400 800]);
    t_dv=(s/v);
    svf=randint(1,1, [0 1]);
    per=randint(1,1, [0 2]);
    t_per=randint(1,1,[0 15])/60;
    t_svf=randint(1,1,[0 45])/60;
    t_z=svf*t_svf+per*t_per;
    t_19_af=(t_st+alfa*t_dv+t_z)+t_19_af;
end %время, которое автобус едет от "Студенческой" до МИЭТа
t_19_af;
for j=2:50
    betta=randint(1, 1, [9 12])/10;
    vr_otpr19_1(2,j)=vr_otpr19_1(2,j-1)+interval(5)*betta;
    vr_otpr19_1(1,j)=vr_otpr19_1(1,j-1);
    vr_pr19_1(1,j)=vr_otpr19_1(1,j);
    vr_pr19_1(2,j)=vr_otpr19_1(2,j);
    if(vr_otpr19_1(2,j)>=60)
        vr_otpr19_1(1,j)=vr_otpr19_1(1,j)+1;
        vr_otpr19_1(2,j)=vr_otpr19_1(2,j)-60;
    end
    if(vr_pr19_1(2,j)>=60)
        vr_pr19_1(1,j)=vr_pr19_1(1,j)+1;
        vr_pr19_1(2,j)=vr_pr19_1(2,j)-60;
    end
    vr_otpr19_1(4,j)=vr_otpr19_1(2,j)+t_19_d;
    vr_pr19_1(4,j)=vr_otpr19_1(4,j)+t_19_af;
    if(vr_otpr19_1(4,j)>=60)
        vr_otpr19_1(3,j)=vr_otpr19_1(1,j)+1;
        vr_otpr19_1(4,j)=vr_otpr19_1(4,j)-60;
    else
        vr_otpr19_1(3,j)=vr_otpr19_1(1,j);
    end
    if(vr_pr19_1(4,j)>=60)
        vr_pr19_1(3,j)=vr_pr19_1(1,j)+1;
        vr_pr19_1(4,j)=vr_pr19_1(4,j)-60;
    else
        vr_pr19_1(3,j)=vr_pr19_1(1,j);
    end
end
for j=2:50
    if ((vr_pr19_1(3,j)==9))
        l=j-1;
        break;
    end
end
for i=l-2:l
    f(ceil(vr_otpr19_1(4,i)))=f(ceil(vr_otpr19_1(4,i)))+1;
    g(ceil(vr_pr19_1(4,i)))=g(ceil(vr_pr19_1(4,i)))+1;
    vr_start_ob19=ceil(vr_otpr19_1(4,i));
    disp(vr_start_ob19);
end
figure;
subplot(2,1,1);
title('"Оптимальный 19" возле общежития');
xlabel('Время');
ylabel('Количество автобусов');
hold on
bar(x,f)
subplot(2,1,2);
title('"Оптимальный 19" возле МИЭТа');
xlabel('Время');
ylabel('Количество автобусов');
hold on
bar(x,g)