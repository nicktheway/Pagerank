
fid = fopen("data/cA.data");
cA = fread(fid, "double");
fclose(fid);

error = (cA-matA) ./ cA;
