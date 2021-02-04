function initialize(actionType)
%% Main informaion
    border = [0 100 0 70];
    timestep = 1e-4;
    
    f = fopen('C:\Users\Veronika\discrete-elements\auxiliary\info.txt', 'w');
    fprintf(f, '%f ', border);
    fprintf(f, '\n%f\n%d\n%d\n%d\n', timestep, actionType);
    fclose(f);
    
%% Particle constants
%     stiffnessRepulsive = 1000;
%     stiffnessAttractive = 10;
%     stiffnessShear = 10;
%     particleFriction = 0.3;

    stiffnessRepulsive = 1e4;
    stiffnessAttractive = 0;
    stiffnessShear = 7e3;
    particleFriction = 0.1;
    wallFriction = 0;
    floorFriction = 1;
    criticalDistance = 0.1;
    
    f = fopen('C:\Users\Veronika\discrete-elements\auxiliary\const.txt', 'w');
    fprintf(f, '%f\n%f\n%f\n%f\n%f\n%f\n%f\n', stiffnessRepulsive, stiffnessAttractive,...
        stiffnessShear, particleFriction, wallFriction, floorFriction, criticalDistance);
    fclose(f);
    
%% Generator information
    particlesAmount = 1000;
    minRadius = 1;
    maxRadius = 1.5;
%     density = 0.07;
    mass = 0.6;
    f = fopen('C:\Users\Veronika\discrete-elements\auxiliary\generatorinfo.txt', 'w');
    fprintf(f, '%d\n%f\n%f\n%f\n', particlesAmount, minRadius, maxRadius, mass);
    fclose(f);
end