function [angle, newEdgeX, newEdgeY] = aor(edgeX, edgeY, maxRadius)
    line = polyfit(edgeX, edgeY, 1); % initial guess
    d = 3 * maxRadius;
    amountToCut = floor(0.05 * length(edgeX));
    % d here is a maximum of all possible distances from an edge point
    % to the approximating line on a current step
    while (d >= 2 * maxRadius && amountToCut >= 1)
        amountToCut = floor(0.05 * length(edgeX));
        edgeX = edgeX(1 : end - amountToCut);
        edgeY = edgeY(1 : end - amountToCut);
        [edgeX, edgeY, d] = sortByDist(edgeX, edgeY, line);
        line = polyfit(edgeX, edgeY, 1);
    end
    angle = atand(line(1));
    newEdgeX = edgeX;
    newEdgeY = edgeY;
    x_plot = [min(newEdgeX) - 10, max(newEdgeX) + 10];
    y_plot = polyval(line, x_plot);
    plot(x_plot, y_plot, 'Color', [0.6350, 0.0780, 0.1840], 'LineWidth', 1.5);
end

function d = distance(line, point)
    % line is an array in format [k, m] corresponding to the y = kx + m
    a = line(1);
    b = -1;
    c = line(2);
    
    d = abs(a * point(1) + b * point(2) + c) / (a^2 + b^2);
end

function [newGridX, newGridY, maxDist] = sortByDist(gridX, gridY, line)
    dist = zeros(1, length(gridX));
    newGridX = zeros(1, length(gridX));
    newGridY = zeros(1, length(gridY));
    
    for i = 1 : length(gridX)
        dist(i) = distance(line, [gridX(i), gridY(i)]);
    end
    
    [dist, I] = sort(dist);
    maxDist = dist(end);
    for i = 1 : length(I)
        newGridX(i) = gridX(I(i));
        newGridY(i) = gridY(I(i));
    end
end