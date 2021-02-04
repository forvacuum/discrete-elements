function drawBorders(border, width)
    plot([border(1), border(1)], border(3 : 4), 'k', 'LineWidth', width);
    hold on;
    plot([border(2), border(2)], border(3 : 4), 'k', 'LineWidth', width);
    plot(border(1 : 2), [border(3), border(3)], 'k', 'LineWidth', width);
    plot(border(1 : 2), [border(4), border(4)], 'k', 'LineWidth', width);
    
    color = [204, 204, 204] ./ 255;
    plot([border(2) / 2, border(2) / 2], border(3 : 4), 'Color', color, 'LineStyle', ':', 'LineWidth', width / 4);
end