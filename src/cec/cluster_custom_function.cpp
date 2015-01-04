#include "cluster_custom_function.hpp"

namespace gmum {
ClusterCustomFunction::ClusterCustomFunction(int count,
                                             const arma::rowvec& mean, const arma::mat& cov_mat,
                                             const std::string& function_name) :
    ClusterUseCovMat(count, mean, cov_mat), m_function_name(function_name) {
    calculate_entropy();
}

ClusterCustomFunction::ClusterCustomFunction(unsigned int id,
                                             const std::vector<unsigned int> &assignment, const arma::mat &points,
                                             const std::string &function_name) :
    ClusterUseCovMat(id, assignment, points), m_function_name(function_name) {
    calculate_entropy();
}

void ClusterCustomFunction::calculate_entropy() {
    Rcpp::Environment my_env = Rcpp::Environment::global_env();
    Rcpp::Function my_function = my_env[m_function_name];
    m_entropy = Rcpp::as<double>(
                my_function(Rcpp::Named("m", Rcpp::wrap(m_mean)),
                            Rcpp::Named("sigma", Rcpp::wrap(m_cov_mat))));
}
boost::shared_ptr<ClusterUseCovMat> ClusterCustomFunction::create_instance(int count, const arma::rowvec& mean, const arma::mat& mat) {
    return boost::shared_ptr<ClusterUseCovMat>(
                new ClusterCustomFunction(count, mean, mat, m_function_name));
}
}
