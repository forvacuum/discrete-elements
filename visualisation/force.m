function F = force(radius, particlePosition, borderX, borderY, stiffness,  mass)
    delta = 1 : 4;
    F = 0;
    e_1 = [1; 0];
    e_2 = [0; 1];
    
    delta(1) = radius - (particlePosition(1) - borderX(1));
    if(delta(1) < 0)
        delta(1) = 0;
    end
    F = F + stiffness * delta(1) * e_1;
    
    delta(2) = radius + particlePosition(1) - borderX(2);
    if(delta(2) < 0)
        delta(2) = 0;
    end
     F = F - stiffness * delta(2) * e_1;
     
    delta(3) = radius - (particlePosition(2) - borderY(1));
    if(delta(3) < 0)
        delta(3) = 0;
    end
    F = F + stiffness * delta(3) * e_2;
    
    delta(4) = radius + particlePosition(2) - borderY(2);
    if(delta(4) < 0)
        delta(4) = 0;
    end
     F = F - stiffness * delta(4) * e_2;
    
    F = F - mass * 9.8 * e_2;
end