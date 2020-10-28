function X=radar(Pr, Vr,size_t)
%Радар
Pre=Pr;
Vre=Vr;
T=zeros(3,1);
Rradar=7e5;
for i=1:size_t;
    R=sqrt(Pr(1,i)^2+Pr(3,i)^2+Pr(2,i)^2);
    if R<Rradar&&T(3,1)==0
        T(3,1)=i;
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
end
X=[Pre Vre T];