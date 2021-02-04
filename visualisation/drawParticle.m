function drawParticle(position, radius, color)
    phi = 0 : 0.01 : 2 * pi;
    x = position(1) + radius * cos(phi);
    y = position(2) + radius * sin(phi);
    fill(x, y, color);
    hold on;
end

