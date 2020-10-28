g = 9.81;
h = 1.6;
H = 20;
M = 80;
r = 0.5;
x0 = [ 0; 0 ];

[p,e,t] = initmesh( 'squareg' );
x = p(1,:)'; y = p(2,:)';

V = g * sqrt( 2 * ( H - h ) / g );
u0 = 0;
ut0 = -V * exp( -( ( x - x0(1) ) .^ 2 / (r/4.5) + ( y - x0(2) ) .^ 2 / (r/4.5) ) );
n = 200;
tlist = linspace(0,1,n);
b = [1     1     1     1;
    1     1     1     1;
    1     1     1     1;
    1     1     1     1;
    1     1     1     1;
    1     1     1     1;
    48    48    48    48;
    48    48    48    48;
    49    49    49    49;
    48    48    48    48 ];
d = 1;
c = 200;
a = 0;

f = [ num2str( -pi * r^2 * M * g ), '*exp(-((x-', num2str( x0( 1 ) ), ').^2/(', num2str( r ), '/4.5)+(y-',...
    num2str( x0( 2 ) ),  ').^2/(', num2str( r ), '/4.5)))' ];

uu=hyperbolic(u0,ut0,tlist,b,p,e,t,c,a, f, d );
delta = -1:0.1:1;
[uxy,tn,a2,a3] = tri2grid(p,t,uu(:,1),delta,delta);
gp = [tn;a2;a3];
umax = max(max(uu));
umin = min(min(uu));

newplot
for i = 1 : n
    pdeplot(p,e,t,'xydata',uu(:,i),'zdata',uu(:,i),...
    'mesh','on','xygrid','on','gridparam',gp,...
    'colorbar','off','zstyle','discontinuous');
    axis([-1 1 -1 1 umin umax]); caxis([umin umax]);
    grid on;
    F(i) = getframe;
end
movie( F, 1, 3 )
