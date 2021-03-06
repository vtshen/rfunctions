% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/products.R
\name{sliced.crossprod}
\alias{sliced.crossprod}
\title{Compute X[,indices]'y}
\usage{
sliced.crossprod(x, y, indices)
}
\arguments{
\item{x}{matrix input}

\item{y}{numeric vector.}

\item{indices}{integer vector of X columns to use}
}
\value{
x[,indices]'y
}
\description{
Compute X[,indices]'y
}
\examples{
n.obs <- 1e4
n.vars <- 2000
n.idx <- 500

x <- matrix(rnorm(n.obs * n.vars), n.obs, n.vars)
y <- rnorm(n.obs)
idx <- sample.int(n.vars, n.idx)

## compute x[,idx]'y
xpy <- sliced.crossprod(x, y, idx)

xpy2 <- crossprod(x[,idx], y)

all.equal(xpy, xpy2)

}

