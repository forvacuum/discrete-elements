function fig = renderModel(trajectory, border, radius, borderWidth)
    N = size(trajectory, 2) / 2; %total amount of particles in the system
    color = [180, 180, 180] ./ 255;
    border(2) = border(2) * 2;
    fig = figure;
    
    for j = 1 : N
        r = trajectory(size(trajectory, 1), (2 * j - 1) : 2 * j);
        drawParticle(r, radius(j), color);
        axis equal;
        axis(border);
        hold on;
    end
    drawBorders(border, borderWidth);
end