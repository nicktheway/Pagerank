N = 50;

file = fopen("ha.mat");
A = fread(file, [2,inf], 'uint32')';
fclose(file);
Sp = A(2:end, :) + 1;
SPM = sparse(Sp(:,1), Sp(:,2), ones(size(Sp, 1), 1),N, N);