X = [-1, 2];
Y = [-3, 4];
R_0 = [0, 0];
radius = 0.5;

drawBorders(X, Y);
axis equal;
axis([-5, 5, -5, 5]);
drawParticle(R_0, radius);