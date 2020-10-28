function X=firstMethod(Pr,Pre,Tstart, size_t, dt, Varmax, aarmax,Robn,rexpl)
Var=zeros(3,size_t); 
Par=zeros(3,size_t); 
aar=zeros(3,size_t);
Var(:,Tstart)=[0 0 Varmax/10];
for i=Tstart/dt+1:size_t
        R=range(Pr(:,i), Par(:,i-1));
        if R<Robn
            temp=Varmax*(Pr(:,i)-Par(:,i-1))/R;
        else
            temp=Varmax*(Pre(:,i)-Par(:,i-1))/R;
        end;
        aar(:,i)=(temp-Var(:,i-1))/dt;  
        if vmod(aar(:,i))>aarmax
            aar(:,i)=aar(:,i)*aarmax/vmod(aar(:,i));
        end
        Var(:,i)=Var(:,i-1)+aar(:,i)*dt;
        if vmod(Var(:,i))>Varmax
            Var(:,i)=Var(:,i)*Varmax/vmod(Var(:,i));
        end
        Par(:,i)=Par(:,i-1)+Var(:,i)*dt;
        if range(Pr(:,i), Par(:,i))<rexpl
            Par(:,i:size_t)=Par(:,i)*ones(1,size_t-i+1);
            Var(:,i:size_t)=[0; 0; 0]*ones(1,size_t-i+1);
            break
        end
end
X=[Par Var aar];