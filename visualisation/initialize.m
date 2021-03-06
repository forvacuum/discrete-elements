function initialize(actionType)
%% Main informaion
    border = [0 15 0 20];
    timestep = 1e-4;
    
    f = fopen('..\auxiliary\info.txt', 'w');
    fprintf(f, '%f ', border);
    fprintf(f, '\n%f\n%d\n%d\n%d\n', timestep, actionType);
    fclose(f);
    
%% Particle constants

    stiffnessRepulsive = 1e4;
    stiffnessAttractive = 0;
    stiffnessShear = 7e3;
    particleFriction = 0.1;
    wallFriction = 0;
    floorFriction = 1;
    criticalDistance = 0.1;
    
    f = fopen('..\auxiliary\const.txt', 'w');
    fprintf(f, '%f\n%f\n%f\n%f\n%f\n%f\n%f\n', stiffnessRepulsive, stiffnessAttractive,...
        stiffnessShear, particleFriction, wallFriction, floorFriction, criticalDistance);
    fclose(f);
    
%% Generator information
    particlesAmount = 20;
    minRadius = 1;
    maxRadius = 1.5;
%     density = 0.07;
    mass = 0.6;
    f = fopen('..\auxiliary\generatorinfo.txt', 'w');
    fprintf(f, '%d\n%f\n%f\n%f\n', particlesAmount, minRadius, maxRadius, mass);
    fclose(f);
end