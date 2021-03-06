
#include "geninv.h"
#include "utils.h"

using namespace Rcpp;
using namespace RcppEigen;


//port faster generalized inverse
RcppExport SEXP ginv(SEXP GG)
{
  using namespace Rcpp;
  using namespace RcppEigen;
  try {
    using Eigen::Map;
    using Eigen::MatrixXd;
    using Eigen::Lower;
    const Eigen::Map<MatrixXd> G(as<Map<MatrixXd> >(GG));
    const int n(G.rows());
    const int m(G.cols());
    const int mn(std::min(n, m));
    
    bool transp(false);
    double tol(1.0e-10);
    MatrixXd A(MatrixXd(mn, mn));
    MatrixXd L(MatrixXd(mn, mn).setZero());
    
    
    
    if (n < m) {
      transp = true;
      A = xxt(G);
    } else {
      A = xtx(G);
    }

    int r = 0;
    for (int k = 0; k < mn; k++) {
      r++;
      
      if (r == 1) {
        L.block(k, r - 1, mn - k, 1) = A.block(k, k, mn - k, 1);
      } else {
        L.block(k, r - 1, mn - k, 1) = A.block(k, k, mn - k, 1) - 
                L.block(k, 0, mn - k, r - 1) * L.block(k, 0, 1, r - 1).adjoint();
      }
      
      if (L(k, r - 1) > tol) {
        L.block(k, r - 1, 1, 1) = L.block(k, r - 1, 1, 1).array().sqrt();
        if (k + 1 < mn) {
          L.block(k + 1, r - 1, mn - k - 1, 1) = L.block(k + 1, r - 1, mn - k - 1, 1) / L(k, r - 1);
        }
      } else {
        r--;
      }
    }

    MatrixXd M(MatrixXd(r, r));
    M = xtx(L.block(0, 0, mn, r)).inverse();

    MatrixXd Y(MatrixXd(m, n));
    
    if (transp) {
      Y = G.adjoint() * L.block(0, 0, mn, r) * M * M * L.block(0, 0, mn, r).adjoint();
    } else {
      Y = L.block(0, 0, mn, r) * M * M * L.block(0, 0, mn, r).adjoint() * G.adjoint();
    }

    return wrap(Y);
  } catch (std::exception &ex) {
    forward_exception_to_r(ex);
  } catch (...) {
    ::Rf_error("C++ exception (unknown reason)");
  }
  return R_NilValue; //-Wall
}


//port faster generalized inverse
RcppExport SEXP geninv(SEXP GG)
{
  using namespace Rcpp;
  using namespace RcppEigen;
  try {
    using Eigen::Map;
    using Eigen::MatrixXd;
    using Eigen::Lower;
    const Eigen::Map<MatrixXd> G(as<Map<MatrixXd> >(GG));
    const int n(G.rows());
    const int m(G.cols());
    const int mn(std::min(n, m));
    
    bool transp(false);
    double tol(1.0e-10);
    MatrixXd A(MatrixXd(mn, mn));
    MatrixXd L(MatrixXd(mn, mn).setZero());
    
    
    
    if (n < m) {
      transp = true;
      A = xxt(G);
    } else {
      A = xtx(G);
    }

    int r = 0;
    for (int k = 0; k < mn; k++) {
      r++;
      
      if (r == 1) {
        L.block(k, r - 1, mn - k, 1) = A.block(k, k, mn - k, 1);
      } else {
        L.block(k, r - 1, mn - k, 1) = A.block(k, k, mn - k, 1) - 
                L.block(k, 0, mn - k, r - 1) * L.block(k, 0, 1, r - 1).adjoint();
      }
      
      if (L(k, r - 1) > tol) {
        L.block(k, r - 1, 1, 1) = L.block(k, r - 1, 1, 1).array().sqrt();
        if (k + 1 < mn) {
          L.block(k + 1, r - 1, mn - k - 1, 1) = L.block(k + 1, r - 1, mn - k - 1, 1) / L(k, r - 1);
        }
      } else {
        r--;
      }
    }

    MatrixXd M(MatrixXd(r, r));
    M = xtx(L.block(0, 0, mn, r)).inverse();

    MatrixXd Y(MatrixXd(m, n));
    
    if (transp) {
      Y = G.adjoint() * L.block(0, 0, mn, r) * M * M * L.block(0, 0, mn, r).adjoint();
    } else {
      Y = L.block(0, 0, mn, r) * M * M * L.block(0, 0, mn, r).adjoint() * G.adjoint();
    }

    return wrap(Y);
  } catch (std::exception &ex) {
    forward_exception_to_r(ex);
  } catch (...) {
    ::Rf_error("C++ exception (unknown reason)");
  }
  return R_NilValue; //-Wall
}


/*
//port faster generalized inverse
RcppExport SEXP geninv_sparse(SEXP GG)
{
  using namespace Rcpp;
  using namespace RcppEigen;
  try {
    using Eigen::MappedSparseMatrix;
    using Eigen::SparseMatrix;
    using Eigen::Map;
    using Eigen::MatrixXd;
    using Eigen::Lower;
    using Eigen::SparseLU;
    //using Eigen::SuperLU;
    using Eigen::VectorXcd;
    
    typedef MappedSparseMatrix<double> MSpMat;
    typedef SparseMatrix<double> SpMat;

    const SpMat G(as<MSpMat>(GG));
    const int n(G.rows());
    const int m(G.cols());
    const int mn(std::min(n, m));
    
    bool transp(false);
    double tol(1.0e-10);
    double epsilon(1.0e-10);
    SpMat A(SpMat(mn, mn));
    SpMat A_sparse(SpMat(mn, mn));
    SpMat L(SpMat(mn, mn));
    
    double val;
    
    if (n < m) {
      transp = true;
      A = G * G.adjoint();
    } else {
      A = G.adjoint() * G;
    }
    //A = MatrixXd(A_sparse);

    int r = 0;
    for (int k = 0; k < mn; k++) {
      r++;
      
      if (r == 1) {
        L.block(k, r - 1, mn - k, 1) = A.block(k, k, mn - k, 1);
      } else {
        L.block(k, r - 1, mn - k, 1) = A.block(k, k, mn - k, 1) - 
                L.block(k, 0, mn - k, r - 1) * L.block(k, 0, 1, r - 1).adjoint();
      }
      
      val = L.coeffRef(k, r - 1);
      
      if (val > tol) {
        L.block(k, r - 1, 1, 1) = L.block(k, r - 1, 1, 1).cwiseSqrt();
        if (k + 1 < mn) {
          L.block(k + 1, r - 1, mn - k - 1, 1) = L.block(k + 1, r - 1, mn - k - 1, 1) / L.coeffRef(k, r - 1);
        }
      } else {
        r--;
      }
    }
    
    SpMat M(SpMat(r, r));
    
    M = L.block(0, 0, mn, r).adjoint() * L.block(0, 0, mn, r);

    SpMat matInv(r,r);
    Eigen::SparseLU<SparseMatrix<double> > solver(M);
    VectorXd base(r), invCol(r);
    //matInv.startFill();
    for (int i=0; i<r; ++i)
    {
      base = VectorXd::Unit(r,i);
      invCol = solver.solve(base);
      for (int j=0; j<r; ++j)
        if (abs(invCol[i]) < epsilon)
           matInv.insert(i,j) = invCol[i];
    }
    //matInv.endFill();

    SpMat Y(SpMat(m, n));
    
    if (transp) {
      Y = G.adjoint() * L.block(0, 0, mn, r) * matInv * matInv * L.block(0, 0, mn, r).adjoint();
    } else {
      Y = L.block(0, 0, mn, r) * matInv * matInv * L.block(0, 0, mn, r).adjoint() * G.adjoint();
    }

    return wrap(Y);
  } catch (std::exception &ex) {
    forward_exception_to_r(ex);
  } catch (...) {
    ::Rf_error("C++ exception (unknown reason)");
  }
  return R_NilValue; //-Wall
}

*/