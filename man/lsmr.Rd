% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/lsmr.R
\name{lsmr}
\alias{lsmr}
\title{Solve the linear system A'Ax = Ab using lsmr}
\usage{
lsmr(A, b, lambda = 0, atol = 1e-05, btol = 1e-05, conlim = 1e+18,
  maxit = 50L, localSize = 0L)
}
\arguments{
\item{A}{CsparseMatrix intput (matrix soon to be included)}

\item{b}{numeric vector.}

\item{atol}{tolerance level}

\item{btol}{tolerance level}

\item{conlim}{condition number limit}

\item{maxit}{integer. Maximum number of iterations}

\item{localsize}{integer number of local iterations for reorthogonalization}
}
\value{
list
}
\description{
Solve the linear system A'Ax = Ab using lsmr
}
\examples{
n <- 10000
p <- 100

set.seed(100)
x <- simSparseMatrix(0.8, boolean = FALSE, dim = c(n, p)) 
b <- as.vector(x \%*\% rnorm(p) + rnorm(n))

A <- crossprod(x)
xtb <- as.vector(crossprod(x, b))

system.time( alpha.true <- solve(A, b) )
system.time( alpha.lsmr <- lsmr(x, b) )

max(abs(alpha.lsmr$x - alpha.true))
}

