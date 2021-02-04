function [r, v] = Verlet(time, dt, init_pos, init_vel, stiffness, radius, mass, borderX, borderY)
%первая координата - номер оси, вторая - номер шага
    r(1, 1) = init_pos(1);
    r(2, 1) = init_pos(2);
    v(1, 1) = init_vel(1);
    v(2, 1) = init_vel(2);
    
    for i = 2 : length(time)
        for j = 1 : 2
            f_i1 = force(radius, r(:, i - 1), borderX, borderY, stiffness, mass);
            r(j, i) = r(j, i - 1) + v(j, i - 1) * dt + f_i1(j) / (2 * mass) * dt ^ 2;
            f_i = force(radius, r(:, i), borderX, borderY, stiffness, mass);
            v(j, i) = v(j, i - 1) + dt / (2 * mass) * (f_i1(j) + f_i(j));
        end
    end
end