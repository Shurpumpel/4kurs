function acc=rocketPower(Var, size_t)
%
acc=zeros(1,size_t);
for i=2:size_t
    if vmod(Var(:,i))~=0&&vmod(Var(:,i-1))~=0
        acc(i)=vmod(Var(:,i))*sin(acos(scal(Var(:,i),Var(:,i-1))/(vmod(Var(:,i))*vmod(Var(:,i-1)))));
        else acc(i)=0;
    end
    if i>1
        if abs(acc(i)-acc(i-1))>100
            acc(i)=0;
        end
    end
end
