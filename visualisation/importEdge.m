function [edgeX, edgeY] = importEdge()
    edge = importdata('edge.txt', ' ');
    
    X = edge(:, 1);
    Y = edge(:, 2);
    [~, maxIndex] = max(Y);
    j = 1;
     
     % updating the edge by taking into account ohly the particles 
     % lying to the right of the highest one
     for i = 1 : length(X)
         if (X(i) >= X(maxIndex))
             edgeX(j) = X(i);
             edgeY(j) = Y(i);
             j = j + 1;
         end
     end
end