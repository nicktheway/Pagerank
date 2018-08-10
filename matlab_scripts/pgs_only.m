% ===================================
% pagerank_gs
% ===================================
function [x flag hist dt]  = pagerank_gs(P, options)
% use gauss-seidel computation

if (options.verbose > 0)
   fprintf('gauss-seidel computation...\n');
end;

tol = 1e-7;
v = ones(n,1)./n;
maxiter = 500;
c = 0.85;

x = v;
if (isfield(options, 'x0'))
    x = options.x0;
else
    % this is dumb, but we need to make sure
    % we actually get x it's own memory...
    
    % right now, Matlab just has a ``shadow copy''
    x(1) = x(1)-1.0;
    x(1) = x(1)+1.0;    
end;

delta = 1;
iter = 0;
P = -c*P;

hist = zeros(maxiter,1);

dt = 0;
while (delta > tol && iter < maxiter)
    tic;
    xold = pagerank_gs_mult(P,x,(1-c)*v);
    dt = dt + toc;
    
    delta = norm(x - xold,1);
    hist(iter+1) = delta;
    
    if (options.verbose > 0)
        fprintf('iter=%d; delta=%f\n', iter, delta);
    end;
    
    iter = iter + 1;
end;

% resize hist
hist = hist(1:iter);

% renormalize the vector to have norm 1
x = x./norm(x,1);

% default is convergence
flag = 0;

if (delta > tol && iter == maxiter)
    warning('pagerank:didNotConverge', ...
        'The PageRank algorithm did not converge after %i iterations', ...
        maxiter);
    flag = 1;
end;
