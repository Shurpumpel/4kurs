function [ dy ] = SIRSModel( t,y )
dy=zeros(4,1);
beta =0.4;
gamma =0.01;
m=0.01;
alpha=0.01;
N=500000;
dy(1)= - beta*y(1)*y(2)/(N- y(4)) + alpha*y(3);
dy(2)= beta*y(1)*y(2)/(N - y(4)) - gamma*y(2)-m*y(2);
dy(3)= gamma*y(2) - alpha*y(3);
dy(4)= m*y(2);
end

