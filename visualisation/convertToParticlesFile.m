function convertToParticlesFile(oldOutputFile, infoFile)
    tmp = importdata(oldOutputFile, ' ');
    coordinateList = tmp(size(tmp, 1), 2 : size(tmp, 2));
    
    tmp = importdata(infoFile, ' ');
    radiusList = tmp(5 : length(tmp));
    
    f = fopen('C:\Users\Veronika\discrete-elements\auxiliary\converted.txt', 'w');
    for i = 1 : length(radiusList)
        fprintf(f, '%f 0 %f %f 0 0 \n', radiusList(i), coordinateList(2 * i - 1), coordinateList(2 * i));
    end
    fclose(f);
end