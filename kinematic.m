clear all;
clc;
r = 1;
l = 1;
kalpha = 8;
kbheta = -1.5;
krho = 3;
initial = [-5 -5];
goal = [0 0];
%theta = 0.523;
global v;
global omega;
theta = pi;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
X = [];
Y = [];
for k = 1:41    
    rho = sqrt((initial(1,1) - goal(1,1))^2 + (initial(1,2) - goal(1,2))^2);
    dx = (initial(1,1) - goal(1,1));
    dy = (initial(1,2) - goal(1,2));
    alpha = -theta + atan(dy/dx);
    bheta = -theta - alpha;

    drho = -krho * rho * cos(alpha);
    dalpha = (krho * sin(alpha)) - (kalpha*alpha) - (kbheta*bheta);
    dbeta = (-krho * sin(alpha));

    v = krho * rho;
    omega = (kalpha*alpha)+(kbheta*bheta);
    
    x = initial(1,1);
    y = initial(1,2);
    [t fun]=ode45('fun2',[0 1],[x y theta]);
    
    X = [X,fun(k,1)];
    Y = [Y,fun(k,2)];
    
    initial(1,1) = fun(k,1);
    initial(1,2) = fun(k,2);
    theta        = fun(k,3);
        
end   

   plot(X,Y)
   
