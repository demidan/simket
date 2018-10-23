#include "qubits.h"
#include <iterator>
#include <map>
#include "unsupported/Eigen/KroneckerProduct"
//#include "unsupported/Eigen/MatrixFunctions"

using namespace Eigen;

// class QuBit
std::set<QuBit*> QuBit::all = {};

static void QuBit::clean_all() {
    for(auto q : all) {
        delete q;
    }
    all.clear();
}

QuBit::QuBit() {
    reset();
    all.insert(this);
}
void QuBit::reset() {
    //joint_ket = std::make_shared<JointKet>({this});
    JointKet({this}, Eigen::VectorXcd::Unit(2, 0));
}

int QuBit::get_index() {
    int index = find_index(this->joint_ket->qubits, this);
    assert(index != -1);
    return index;
}

// class JointKet
JointKet::JointKet(std::set<QuBit*> qubits_, VectorXcd ket_) :
    ket(ket_),
    qubits(qubits_)
{
    assert(qubits.size() >= 1);
    for(auto q : qubits) {
        q->joint_ket = std::make_shared<JointKet>(*this);
    }
}

int bits_perm(int i, std::vector<int> bit_indices) {
    int j = 0;
    int mask = 1;
    int touched = 0;
    for (int ind : bit_indices) {
        if (i & 1<<ind)
            j |= mask;
        touched |= 1<<ind;
        mask <<= 1;
    }
    while (i & ~touched) {
        if (~touched & 1) {
            if (i & 1)
                j |= mask;
            mask <<= 1;
        }
        i       >>= 1;
        touched >>= 1;
    }
    return j;
}
PermutationMatrix<Dynamic> ket_perm(std::vector<int> qubit_indices, int size) {
    ArrayXi indices = ArrayXi(1<<qubit_indices.size());
    assert(size >= qubit_indices.size());
    for (int i = 0; i < size; ++i) {
        indices[i] = bits_perm(i, qubit_indices);
    }
    return PermutationMatrix<Dynamic>(indices);
}

// useful identity: $(A \otimes B) (x \otimes y) = (A x) \otimes (B y)$,
//  where $\times$ -- is Kronecker product (~ tensor product), $A x$ -- matrix multiplication
void apply_quop(const MatrixXcd &quop, std::vector<QuBit*> args) {
    assert(quop.rows() == quop.cols());
    assert(quop.rows() == 1<<args.size());
    
    auto tangle = std::set<QuBit*>();
    auto kets   = std::set<JointKet*>();
    for (auto a : args) {
        auto k = a->joint_ket;
        //tangle.merge(k->qubits);
        tangle.insert(begin(k->qubits), end(k->qubits));
        kets.insert(k.get());
    }
    
    VectorXcd new_ket = VectorXcd::Unit(1,1);
    int new_ket_size = 1<<tangle.size();
    for (auto k : kets) {
        new_ket = kroneckerProduct(k->ket, new_ket).eval();
    }
    assert(new_ket.size() == new_ket_size);
    
    int base_i = 0;
    auto ket_bases = std::map<JointKet*, int>();
    for (auto k : kets) {
        ket_bases[k] = base_i;
        base_i += k->ket.size();
    }
    std::vector<int> arg_indices = {};
    for (auto a : args) {
        arg_indices.push_back(ket_bases[a->joint_ket.get()] + find_index(a->joint_ket->qubits, a));
    }
    PermutationMatrix<Dynamic> perm = ket_perm(arg_indices, new_ket_size);
    
    int id_size = tangle.size() - args.size();
    new_ket = (kroneckerProduct(MatrixXcd::Identity(id_size,id_size), quop) * (new_ket * perm)) * perm.transpose();
    
    JointKet(tangle, new_ket);
}

const Matrix2cd X_op = Matrix2cd() <<
    0,  1,
    1,  0;
const Matrix2cd Y_op = Matrix2cd() <<
     0,     {0, -1},
    {0, 1},  0;
const Matrix2cd Z_op = Matrix2cd() <<
    1,  0,
    0, -1;
const Matrix2cd S_op = Matrix2cd() <<
    1,  0,
    0, {0, 1};
const Matrix2cd T_op = Matrix2cd() <<
    1,  0,
    0, {1, 1}/std::sqrt(2);

const MatrixXcd c_op(const MatrixXcd &op, int n_c) {
    assert(n_c >= 1);
    return MatrixXcd() << 
        MatrixXcd::Identity(1<<n_c, 1<<n_c), MatrixXcd::Zero(1<<n_c, op.cols()),
        MatrixXcd::Zero(op.rows(),  1<<n_c), op
}

const Matrix2cd H_op = std::sqrt(2)/2 * Matrix2cd() <<
    1,  1,
    1, -1;
const Matrix2cd Swap_op = Matrix4cd() <<
    1,  0,  0,  0,
    0,  0,  1,  0,
    0,  1,  0,  0,
    0,  0,  0,  1;
