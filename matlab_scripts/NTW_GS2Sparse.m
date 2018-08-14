N = 30;
c = 0.85;
e = 1e-12;

file = fopen("ha.wg");
A = fread(file, [2,inf], 'uint32')';
fclose(file);
Sp = A(2:end, :) + 1;
SPM = sparse(Sp(:,1), Sp(:,2), ones(size(Sp, 1), 1),N, N);