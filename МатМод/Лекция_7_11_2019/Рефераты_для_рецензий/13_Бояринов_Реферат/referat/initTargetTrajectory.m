function X=initTargetTrajectory(size_t,dt,X0,Y0,Z0,Vx0,Vy0,Vz0,Vrmax)
%Ракета
Vr=zeros(3,size_t);
ar=zeros(3,size_t);
Pr=zeros(3,size_t);
for i=1:size_t
    if i/dt<100
        ar(:,i)=[5 0 4];
    elseif i/dt<800
        ar(:,i)=[10 30*cos(i*2*pi/200) 30*sin(i*2*pi/300)];
%         ar(:,i)=[10 0 -2];
%         ar(:,i)=[10 10*cos(i*2*pi/200) 6-g];
%         ar(:,i)=[10 0 -3+10*cos(i*2*pi/200)];
    else
        ar(:,i)=[5 0 -4];
    end

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
end 
Pr(1,Pr(3,:)<0)=max(Pr(1,Pr(3,:)>0));
Vr(1,Pr(3,:)<0)=0;
Vr(3,Pr(3,:)<0)=0;
Pr(3,Pr(3,:)<0)=0;
X=[Pr Vr];