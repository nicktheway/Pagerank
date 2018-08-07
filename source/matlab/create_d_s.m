function [S, d, n] = create_d_s(B)
% CREATE_D_S A function that creates a probability matrix
% for a given web graph matrix and lists the dangling nodes in a vector.
% 
%`PARAMETERS:
%   B: a sparse square adjacency matrix denoting the web graph
%		with B(i,j) = 1 if page j is linked to page i.
% RETURNS:
% 	S: probability matrix
%	d: vector denoting the position of dangling nodes
%	n: size of matrices B and S
%
% Rien den Besten
% Copyright (c) August 2010

[n,n] = size(B);

d = zeros(1,n);
D = sparse(n,n);

for j = 1:n
	%Find indices of non-zero elements in column j of B:
	B_column_indices{j} = find(B(:,j));

	%Count number of non-zero elements in column j of B:
	LO_column_indices(j) = length(B_column_indices{j});

	% Fill d and D:
	if LO_column_indices(j) > 0 % column j is no dangling column
		D(j,j) = 1 / LO_column_indices(j);
	else % column j is a dangling column
		d(j) = 1;
	end
end

S = B * D;
