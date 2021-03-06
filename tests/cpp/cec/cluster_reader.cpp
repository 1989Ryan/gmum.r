#include "cluster_reader.hpp"
#include <stdexcept>

ClusterReader::ClusterReader(const char * name, unsigned int dim) {
    m_energy = -1;
    m_folder_name = std::string(name);
    m_dim = dim;
}

std::string ClusterReader::prefix() {
	//TODO: fix path to ../../inst
#ifdef TEST_DATA_PATH
    return std::string(TEST_DATA_PATH) + "/cec/" + m_folder_name + "/";
#else
    return std::string("../../inst/dev/datasets/cec/") + m_folder_name + "/";
#endif
}

std::string ClusterReader::cluster_path() {
    return prefix() + "cluster.txt";
}

std::string ClusterReader::input_path() {
    return prefix() + "input.txt";
}

std::string ClusterReader::energy_path() {
    return prefix() + "energy.txt";
}

std::string ClusterReader::dimension_path() {
    return prefix() + "dimension.txt";
}

void ClusterReader::normalize_clustering(std::vector<int> &clusters)
{
    int min = *(std::min_element(clusters.begin(), clusters.end()));
    for (std::vector<int>::iterator it = clusters.begin();
         it != clusters.end(); ++it) {
        *it -= min;
    }
}

void ClusterReader::read_points() {
    // std::cout << " read points " << std::endl;
    if(m_dim == 0)
    {
        read_dimension();
    }

    std::ifstream file(input_path().c_str());
    if (file.is_open()) {
        std::string line;
        while (file.good()) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::vector<double> current_vector;
            if (line.size() < m_dim)
                continue;
            for (unsigned int i = 0; i < m_dim; ++i) {
                double x;
                ss >> x;
                current_vector.push_back(x);
            }
            m_points.push_back(current_vector);
        }
        file.close();
    } else {
        std::cerr << "Failed to open " << input_path() << std::endl;
        throw std::runtime_error(input_path() + "Failed to open ");
    }
    // std::cout << "Finish reading opening. Read " << m_points.size() << std::endl;
}

void ClusterReader::read_clustering() {
    // std::cout << "reading clusters " << std::endl;
    std::ifstream file(cluster_path().c_str());
    std::vector<int> clusters;
    if (file.is_open()) {
        std::string line;
        while (file.good()) {
            std::getline(file, line);
            std::stringstream ss(line);
            if (line.size() == 0)
                continue;
            int x;
            ss >> x;
            clusters.push_back(x);
        }
        file.close();
        normalize_clustering(clusters);
        for(int i = 0; i < clusters.size(); ++i)
        {
            m_clustering.push_back(clusters[i]);
        }
    } else {
        std::cerr << "Failed to open " << cluster_path() << std::endl;
        throw std::runtime_error(cluster_path() + "Failed to open ");
    }
    // std::cout << "Finished reading clusters. Read " << m_clustering.size() << std::endl;
}

void ClusterReader::read_energy() {
    std::ifstream file(energy_path().c_str());
    if (file.is_open()) {
        file >> m_energy;
        file.close();
    } else {
        std::cerr << "Failed to open " << energy_path() << std::endl;
        throw std::runtime_error(energy_path() + "Failed to open ");
    }
}

void ClusterReader::read_dimension() {
    std::ifstream file(dimension_path().c_str());
    if (file.is_open()) {
        file >> m_dim;
        file.close();
    } else {
        std::cerr << "Failed to open " << dimension_path() << std::endl;
        throw std::runtime_error(dimension_path() + "Failed to open ");
    }
}
void ClusterReader::get_points(std::vector<std::vector<double> > & out) {
    if (m_points.size() == 0)
        read_points();
    for (std::vector<std::vector<double> >::iterator it = m_points.begin();
         it != m_points.end(); ++it)
        out.push_back(*it);

}

void ClusterReader::get_clustering(std::vector<unsigned int> & out) {
    if (m_clustering.size() == 0)
        read_clustering();
    for (std::vector<unsigned int>::iterator it = m_clustering.begin();
         it != m_clustering.end(); ++it)
        out.push_back(*it);
}

double ClusterReader::get_energy() {
    if (m_energy == -1)
        read_energy();
    return m_energy;
}

double ClusterReader::get_dimension() {
    return m_dim;
}

arma::mat ClusterReader::get_points_in_matrix() {
    if (m_points.size() == 0)
        read_points();
    unsigned int n = m_points.size();
    arma::mat result(n, m_dim);
    for (unsigned int i = 0; i < n; ++i) {
        for (unsigned int j = 0; j < m_dim; ++j) {
            result(i, j) = m_points[i][j];
        }
    }
    return result;
}

