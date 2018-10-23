#include "Eigen/Core"
#include <memory>
#include <vector>
#include <set>

class JointKet;
class QuBit {
    // all qubits would live through whole simulation, and would be deleted when simulation finished
    static std::set<QuBit*> all;
  public:
    friend class    JointKet;
    std::shared_ptr<JointKet> joint_ket;
    int get_index();
    
    QuBit();
    void reset();
    static void clean_all();
};

class JointKet {
  public:
    Eigen::VectorXcd ket; // dynamic length column-vector of std::complex<double>
    std::set<QuBit*> qubits;
    JointKet(std::set<QuBit*> qubits_, Eigen::VectorXcd ket_);
};

template<typename Container, typename Key>
typename Container::difference_type find_index(const Container &cont, const Key &key) {
    auto pos = cont.find(key);
    if (pos == cont.end()) {
        return -1;
    }
    return std::distance(cont.begin(), pos);
}

// quop is dynamic size (square) matrix of std::complex<double>
void apply_quop(Eigen::MatrixXcd quop, std::vector<QuBit*> args);

extern const Eigen::Matrix2cd X_op;
extern const Eigen::Matrix2cd Y_op;
extern const Eigen::Matrix2cd Z_op;
extern const Eigen::Matrix2cd S_op;
extern const Eigen::Matrix2cd T_op;
       const Eigen::MatrixXcd c_op(const Eigen::MatrixXcd &op, int n_c = 1);
extern const Eigen::Matrix2cd H_op;
extern const Eigen::Matrix2cd Swap_op;