function X=secondMethod(Pr, Vr,Tstart,size_t, dt, Pre, Vre, Varmax, aarmax,Robn,rexpl)
Var=zeros(3,size_t); 
aar=zeros(3,size_t);
Par=zeros(3,size_t); 
Upr=zeros(3,size_t);
R=100000;
Vpusk=0;
if Tstart==1
    Tstart=2;
end
for i=Tstart/dt:size_t 
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
    if i==Tstart
        x0=0;y0=0;z0=0; 
    else 
        x0=Par(1,i-1);
        y0=Par(2,i-1);
        z0=Par(3,i-1);
    end
    if i==Tstart
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
        break
    end
end
X=[Par Var aar Upr];