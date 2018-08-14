
fid = fopen("data/cA.data");
cA = fread(fid, "double");
fclose(fid);

% matA = pr_mod_gauss_seidel(SPM, c, e);
error = (cA-matA) ./ cA;
