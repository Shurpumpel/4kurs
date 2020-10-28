function a=alfa(t)
a=zeros(1,numel(t));
for i=1:numel(t)
if t(i)<400
    a(i)=1./(t(i)+(2)/(pi)).^(1);
elseif t(i)<500
    a(i)=-(t(i)+400)*(pi/600)+8*pi/6;
else 
    a(i)=-pi/6;
end
end