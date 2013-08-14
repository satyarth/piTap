x_a=0;
y_a=0;
x_b=100;
y_b=0;
x_c=0;
y_c=80;
x_d=100;
y_d=80;
a=71;
b=66;
c=0;
d=5;
d_ba=-0.37*(b-a);
d_ca=-0.37*(c-a);
d_dc=-0.37*(d-c);
d_db=-0.37*(d-b);
fun = @(x)(sqrt((x(1)-x_b)^2+(x(2)-y_b)^2)-sqrt((x(1)-x_a)^2+(x(2)-y_a)^2)-d_ba)^2+(sqrt((x(1)-x_d)^2+(x(2)-y_d)^2)-sqrt((x(1)-x_c)^2+(x(2)-y_c)^2)-d_dc)^2+(sqrt((x(1)-x_c)^2+(x(2)-y_c)^2)-sqrt((x(1)-x_a)^2+(x(2)-y_a)^2)-d_ca)^2+(sqrt((x(1)-x_d)^2+(x(2)-y_d)^2)-sqrt((x(1)-x_b)^2+(x(2)-y_b)^2)-d_db)^2;
fminsearch(fun,[50,50])