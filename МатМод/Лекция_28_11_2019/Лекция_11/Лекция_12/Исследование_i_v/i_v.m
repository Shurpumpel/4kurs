function p=i_v(x,a,b)
%
a=1/sqrt(a);
k1=b/a;
k1=k1/(exp(a)-exp(-a));
k2=1-exp(-a);
k3=1-exp(a);
p=k1*(k2*exp(x*a)+k3*exp(-x*a))+1-b*x;