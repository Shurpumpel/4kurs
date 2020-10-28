function drawTrajectory(Pr, Par, aar,Upr, size_t,Tstart, rexpl, p,f)
%%
figure;
if f==1
    title('Траектория по кривой погони');
else
    title('Траектория по методу последовательного упреждения');
end
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
if mod(i,10)==0&&f==2
        plot3(Upr(1,i), Upr(2,i), Upr(3,i),'m+');
    %plot3([Pre(1,i) Pre(1,i)+10000*Vre(1,i)], [Pre(2,i) Pre(2,i)+10000*Vre(2,i)], [Pre(3,i) Pre(3,i)+10000*Vre(3,i)],'g-');
end
    if range(Pr(:,i), Par(:,i))<rexpl
        disp('Цель уничтожена')
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
    pause(p);
end
%%
%График сближения с целью
figure;
if f==1
    title('График сближения с целью(по кривой погони)');
else
    title('График сближения с целью(последовательно упреждение)');
end
xlabel('Время');
ylabel('Расстояние до ракеты');
grid;
hold;
for i=1:size_t
    plot(i,abs(sqrt((Pr(1,i)-Par(1,i)).^2+(Pr(2,i)-Par(2,i)).^2+(Pr(3,i)-Par(3,i)).^2)),'r.')
    if abs(sqrt((Pr(1,i)-Par(1,i)).^2+(Pr(2,i)-Par(2,i)).^2+(Pr(3,i)-Par(3,i)).^2))<rexpl
        break
    end
end