function plotEnergy(energy, time, N)
    totalEnergy = zeros(length(time), 1);
    
    figure;
    for i = 1 : N
       totalEnergy = totalEnergy + energy(:, i);
    end
    
    figure;
    maximum = max(totalEnergy);
    minimum = min(totalEnergy);
    plot(time, totalEnergy);
    ylim([minimum - 1, maximum + 1]);
    
    grid on;

end