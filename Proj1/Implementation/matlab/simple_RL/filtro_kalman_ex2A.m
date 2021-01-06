clear all
%Read sensor data
for i=1:100
    real(i)=16-1.23*cos(pi*i/360)-0.86*sin(pi*i/360)+5*randn;
end
z=real;

%Initialization of vars
xa(1) = 0;
sigma1=0.5;
a=1;
c=1;
H=1;

pa(1) = 40;
R = 0.01;
tempsimu1 = 100;

%////FILTRO DE KALMAN////%

for i=2:tempsimu1
    t(i) = i;
    
%Propagacao
xp(i) = a*xa(i-1);
p(i) = a*pa(i-1)*a+c*sigma1*c;

%Atualizacao
 KK(i) = p(i)*H/(H*p(i)*H+R);
 pa(i) = p(i) - KK(i)*H*p(i);
 xa(i) = xp(i) + KK(i) * (z(i) - H*xp(i));
 
 end

 %Gráficos
 plot(t,z(1:tempsimu1), '.b', t, xa, '-r')
 %plot(t,pa,'-b');

