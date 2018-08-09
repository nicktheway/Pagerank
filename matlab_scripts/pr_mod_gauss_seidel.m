function [pi, resvec, iter, tmvec] = pr_mod_gauss_seidel(B, alpha, tol)
% PR MOD GAUSS SEIDEL is an implementation of the modified Gauss-Seidel
% iteration method for the PageRank problem.
% PARAMETERS:
%	B: 		a sparse square adjacency matrix denoting the web graph
%			with B(i,j) = 1 if page j is linked to page i.
%	alpha: 	the damping factor.
%	tol: 	the tolerance of the method.
% RETURNS:
%	pi: 	approximation of the PageRank vector for the given matrix B.
%	resvec:	vector of the residual norms at each iteration.
%	iter:	the number of iterations.
%	tmvec:	vector of the total elapsed time (s) after each iteration.
% Rien den Besten
% Copyright (c) August 2010

tic;

[S, d, n] = create_d_s(B);
alphaS = alpha * S;
p = ones(n,1)/n;
b = (1 - alpha) * p;

% Create M and Rt
M_Gm = speye(n) - tril(alphaS,0);
Rt_Gm = triu(alphaS,1);

% Starting values
pi = p;
iter = 0;
r = b - pi + alphaS*pi + alpha*p*dot(d,pi);
normr = norm(r);
resvec = [normr];
tmvec = [toc];

% Modified Gauss-Seidel iteration
while normr > tol

	pi = M_Gm \ (Rt_Gm * pi + alpha*p*dot(d,pi) + b);

	r = b - pi + alphaS*pi + alpha*p*dot(d,pi);
	normr = norm(r);

	resvec = [resvec; normr];
	tmvec = [tmvec; toc];
	iter = iter + 1;
end
