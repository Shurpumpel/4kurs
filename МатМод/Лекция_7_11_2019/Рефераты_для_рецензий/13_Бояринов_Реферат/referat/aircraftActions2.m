% function aircraftActions2
size_t=1001;
g=9.8;
Vrmax=2000;
Varmax=3000;
aarmax=8*g;
armax=6*g;
X0=-5e5;
Y0=1e4;
Z0=4e5;
rexpl=3e3;
Robn=1e4;
RobnS=5e5;
Vx0=2000;
Vy0=0;
Vz0=0;
p1=0.00001;
p2=0.00001;
Vr=zeros(3,size_t);
ar=zeros(3,size_t);
Pr=zeros(3,size_t);
Pre=zeros(3,size_t);
Vre=zeros(3,size_t);
Par=zeros(3,size_t);
Var=zeros(3,size_t);
Upr=zeros(3,size_t);
aar=zeros(3,size_t);
T=0;
dt=1;
Rradar=7e5;
Vpusk=Varmax/10;
S=0;
for i=1:size_t
%%
    %�������
    
    if i==1
        Pr(:,i)=[X0 Y0 Z0];
        Vr(:,i)=[Vx0 Vy0 Vz0];
    else
        Vr(:,i)=Vr(:,i-1)+ar(:,i)*dt;
        if vmod(Vr(:,i))>Vrmax
            Vr(:,i)=Vr(:,i)*Vrmax/vmod(Vr(:,i));
        end
        Pr(:,i)=Pr(:,i-1)+Vr(:,i)*dt;
    end
%%
%�����
    R=sqrt(Pr(1,i)^2+Pr(3,i)^2+Pr(2,i)^2);
    if R<Rradar&&T==0&&i>1
        T=i+1;
    end
    if R<Rradar
        Pre(1,i)=Pr(1,i)+(lerror(R)-1)*10000;
        Pre(2,i)=Pr(2,i)+(lerror(R)-1)*10000;
        Pre(3,i)=Pr(3,i)+(lerror(R)-1)*10000;
        Vre(1,i)=Vr(1,i)+(lerror(R)-1)*100;
        Vre(2,i)=Vr(2,i)+(lerror(R)-1)*100;
        Vre(3,i)=Vr(3,i)+(lerror(R)-1)*100;
    elseif i>1
        Pre(:,i)=Pre(:,i-1);
    end
    %%
    %������
    
    if T~=0
        R=range(Pr(:,i), Par(:,i-1));
        if R<Robn
            x1=Pr(1,i-1);
            y1=Pr(2,i-1);
            z1=Pr(3,i-1);
            Vx=Vr(1,i-1);
            Vy=Vr(2,i-1);
            Vz=Vr(3,i-1);
        else
            x1=Pre(1,i-1);
            y1=Pre(2,i-1);
            z1=Pre(3,i-1);
            Vx=Vre(1,i-1);
            Vy=Vre(2,i-1);
            Vz=Vre(3,i-1);
        end
        if i==T
        	x0=0;y0=0;z0=0; 
        else 
        x0=Par(1,i-1);
        y0=Par(2,i-1);
        z0=Par(3,i-1);
        end
        if i==T
            Vpusk=Varmax/10;
        end
        R0=range([x0 y0 z0], [x1 y1 z1]);
        A=Vx^2+Vz^2+Vy^2-Vpusk^2;
        B=2*(Vx*(x1-x0)+Vz*(z1-z0)+Vy*(y1-y0));
        C=R0^2;
        T=(-B-sqrt(B^2-4*A*C))/(2*A);
        if imag(T)~=0
            T=real(T);
        elseif T<0
             T=(-B+sqrt(B^2-4*A*C))/(2*A);
        end
        x2=x1+Vx*T;
        y2=y1+Vy*T;
        z2=z1+Vz*T;
        R=range([x0 y0 z0], [x2 y2 z2]);
        temp=Varmax*([x2;y2;z2]-Par(:,i-1))/R;
        aar(:,i)=(temp-Var(:,i-1))/dt;
        if vmod(aar(:,i))>aarmax
            aar(:,i)=aar(:,i)*aarmax/vmod(aar(:,i));
        end
        Var(:,i)=Var(:,i-1)+(aar(:,i))*dt;
        if vmod(Var(:,i))>Varmax
            Var(:,i)=Var(:,i)*Varmax/vmod(Var(:,i));
        end

        Par(:,i)=Par(:,i-1)+Var(:,i)*dt;
        Vpusk=vmod(Var(:,i));
        Upr(:,i)=[x2;y2;z2];
        if range(Pr(:,i), Par(:,i))<rexpl
            Par(:,i:size_t)=Par(:,i)*ones(1,size_t-i+1);
            Var(:,i:size_t)=[0; 0; 0]*ones(1,size_t-i+1);
            Pr(:,i:size_t)=Pr(:,i)*ones(1,size_t-i+1);
            break
        end
        if i-T>400
            Par(:,i)=Par(:,i-1);
            Var(:,i)=zeros(3,1);
        end
        if R<RobnS&&i<size_t
            ar(1,i+1)=Vrmax*(Var(3,i));
            ar(2,i+1)=Vrmax*(-Var(2,i));
            ar(3,i+1)=Vrmax*(-Var(1,i));
%             S=S+1;
%            	al=pi/2;
%             be=pi/2;
%             ga=pi/2;
%             if S>100
%                 al=al*(-1+2*randint);
%                 be=be*(-1+2*randint);
%                 ga=ga*(-1+2*randint);
%                 S=0;
%             end
%             ar(:,i+1)=Vrmax*[cos(al)*cos(be) cos(al)*sin(be)*sin(ga)-sin(al)*cos(ga) cos(al)*sin(be)*cos(ga)+sin(al)*sin(ga);...
%                 sin(al)*cos(be) sin(al)*sin(be)*sin(ga)+cos(al)*cos(ga) sin(al)*sin(be)*cos(ga)-cos(al)*sin(ga);...
%                 -sin(be) cos(be)*sin(ga) cos(be)*cos(ga)]*Vr(:,i);
            if vmod(ar(:,i+1))>armax
                ar(:,i+1)=ar(:,i+1)*armax/vmod(ar(:,i+1));
            end 
        else
            ar(:,i+1)=Vrmax.*[10;0;0]-Vr(:,i);
        end  
    end
    
end

%%
figure;
    title('���������� �� ������ ����������������� ����������');
xlabel('x');
ylabel('y');
zlabel('z');
axis([min(Pr(1,:))-1e5 max(Pr(1,:))+1e5 -1e5 max(Pr(2,:))+1e5 0 max(Pr(3,:))+1e5]);
grid;
hold;

for i=1:size_t

%     plot3(Pr(1,i),Pr(2,i),Pr(3,i),'b.',...
%     Par(1,i), Par(2,i), Par(3,i), 'r.',...
%     [Par(1,i) Par(1,i)+1000*(aar(1,i))],...
%     [Par(2,i) Par(2,i)+1000*(aar(2,i))],...
%     [Par(3,i) Par(3,i)+1000*(aar(3,i))],'g--',...
%     Pr(1,Tstart),Pr(2,Tstart),Pr(3,Tstart),'g*'...
%     );   
    plot3(Pr(1,i),Pr(2,i),Pr(3,i),'b.',...
    Par(1,i), Par(2,i), Par(3,i), 'r.');
    if mod(i,10)==0
            plot3(Upr(1,i), Upr(2,i), Upr(3,i),'m+');
    %     plot3([Pre(1,i) Pre(1,i)+10000*Vre(1,i)], [Pre(2,i) Pre(2,i)+10000*Vre(2,i)], [Pre(3,i) Pre(3,i)+10000*Vre(3,i)],'g-');
    end
    if range(Pr(:,i), Par(:,i))<rexpl
        disp('���� ����������')
        fi=0:0.1:2*pi;
        r=rexpl;
        plot3(Par(1,i)+r*cos(fi),Par(2,i)*ones(numel(fi)),Par(3,i)+r*sin(fi),'r*',...
        Par(1,i)*ones(numel(fi)),Par(2,i)+r*cos(fi),Par(3,i)+r*sin(fi),'r*',...
        Par(1,i)+r*cos(fi),Par(2,i)+r*sin(fi),Par(3,i)*ones(numel(fi)),'r*',...
        Par(1,i),Par(2,i),Par(3,i),'ro',...
        Pr(1,i),Pr(2,i),Pr(3,i),'bo');
        break;
    end
    if i>2&&Pr(3,i)==0
        break;
    end
    pause(0.01);
end
%%
%������ ��������� � �����
figure;
title('������ ��������� � �����(���������������� ����������) �������������� ����');
xlabel('�����');
ylabel('���������� �� ������');
grid;
hold;
for i=1:size_t
    plot(i,abs(sqrt((Pr(1,i)-Par(1,i)).^2+(Pr(2,i)-Par(2,i)).^2+(Pr(3,i)-Par(3,i)).^2)),'r.')
    if abs(sqrt((Pr(1,i)-Par(1,i)).^2+(Pr(2,i)-Par(2,i)).^2+(Pr(3,i)-Par(3,i)).^2))<rexpl
        break
    end
end
