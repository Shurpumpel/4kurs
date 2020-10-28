function y=vmod(v)
P0=[0;0;0];
if length(v)~=3
    error('vmod');
end
y=range(P0,v);