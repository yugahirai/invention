from __future__ import annotations
from qsvt.hamiltonian import Heisenberg_2D_ext, Heisenberg_1D, FermiHubbard2D, order_hamiltonian_to_slice

def generate_hamiltonian(size: int, ham_type: str, boundary_type: str, s: float, J2: float):
    if ham_type == "Heisenberg1D":
        ham = Heisenberg_1D(size, boundary_type, s, J2)
    elif ham_type == "Heisenberg2D":
        ham = Heisenberg_2D_ext(size, boundary_type, s, J2)
        num_qubit_per_site = int(s*2+1e-10)
        ham = order_hamiltonian_to_slice(ham, num_qubit_per_site=num_qubit_per_site)
    elif ham_type == "FermiHubbard2D":
        ham = FermiHubbard2D(size, boundary_type)
    else:
        assert(f"Unknown Hamiltonian type: {ham_type}")
    return ham


def ham_debug():
    n = 2
    s = 1
    J2 = 1/2
    import pprint
    ham = Heisenberg_2D_ext(n, "PBC", s, 0)
    pprint.pprint(ham)
    ham = Heisenberg_2D_ext(n, "PBC", s, J2)
    pprint.pprint(ham)
    ham = Heisenberg_2D_ext(n, "OBC", s, 0)
    pprint.pprint(ham)
    ham = Heisenberg_2D_ext(n, "OBC", s, J2)
    pprint.pprint(ham)
    ham = Heisenberg_1D(n, "PBC", s, 0)
    pprint.pprint(ham)
    ham = Heisenberg_1D(n, "PBC", s, J2)
    pprint.pprint(ham)
    ham = Heisenberg_1D(n, "OBC", s, 0)
    pprint.pprint(ham)
    ham = Heisenberg_1D(n, "OBC", s, J2)
    pprint.pprint(ham)
    ham = FermiHubbard2D(n, "PBC", decomposition="jordan")
    pprint.pprint(ham)
    ham = FermiHubbard2D(n, "OBC", decomposition="jordan")
    pprint.pprint(ham)
    ham = FermiHubbard2D(n, "PBC", decomposition="bravyi")
    pprint.pprint(ham)
    ham = FermiHubbard2D(n, "OBC", decomposition="bravyi")
    pprint.pprint(ham)

ham_debug()

