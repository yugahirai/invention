import numpy as np
from .lattice_hamiltonian import chain_graph, square_lattice_graph
from .lattice_hamiltonian import J1J2Heisenberg_on_graph
from .util import padding_hamiltonian_term_thread, _from_openfermion_to_list


def Heisenberg_1D(num_spin: int, boundary_type: str, s: float, J2: float = 0.):
    assert(boundary_type in ["OBC", "PBC", "cylinder"])
    with_J2_edges = (J2 != 0.)
    graph, pos_dict = chain_graph(Nx=num_spin, BCtype=boundary_type, with_J2_edges=with_J2_edges)
    ham = J1J2Heisenberg_on_graph(graph, J2=J2, s=s)
    ham = _from_openfermion_to_list(ham)

    num_qubit = 0
    for term in ham:
        for pauli in term[1]:
            idx = pauli[0]
            num_qubit = max(num_qubit, idx+1)
    if np.allclose(s, 1/2):
        assert(num_qubit == num_spin)
    elif np.allclose(s, 1):
        assert(num_qubit == num_spin*2)

    pos_dict_fix = {}
    for ind in range(num_qubit*2):
        pos_dict_fix[ind] = (ind%2, ind//2)

    result = {
        "num_qubit": num_qubit,
        "pos": pos_dict_fix,
        "pauli": ham
    }
    return result


def Heisenberg_2D_ext(lattice: int, boundary_type: str, s: float, J2: float = 0.):
    assert(boundary_type in ["OBC", "PBC", "cylinder"])
    num_spin = lattice ** 2
    with_J2_edges = (J2 != 0.)
    graph, pos_dict = square_lattice_graph(Nx=lattice, Ny=lattice, BCtype=boundary_type, with_J2_edges=with_J2_edges)
    ham = J1J2Heisenberg_on_graph(graph, J2=J2, s=s, is_spin_operator_pauli=False)
    ham = _from_openfermion_to_list(ham)

    num_qubit = 0
    for term in ham:
        for pauli in term[1]:
            idx = pauli[0]
            num_qubit = max(num_qubit, idx+1)
    if np.allclose(s, 1/2):
        assert(num_qubit == num_spin)
    elif np.allclose(s, 1):
        assert(num_qubit == num_spin*2)
    
    result = {
        "num_qubit": num_qubit,
        "pos": pos_dict,
        "pauli": ham
    }
    return result


def _get_tuple_for_sort(pauli_term, pos_dict, num_qubit_per_site):
    items = []
    pauli_string = pauli_term[1]
    for pos_spin_index, char in pauli_string:
        spin = pos_spin_index%num_qubit_per_site
        pos_index = pos_spin_index//num_qubit_per_site
        pos = pos_dict[pos_index]
        item = (pos[0], pos[1], spin)
        items.append(item)
    items = tuple(sorted(items))
    return items

def order_hamiltonian_to_slice(hamiltonian:dict, num_qubit_per_site: int) -> dict:
    pos_dict = hamiltonian["pos"]
    sorted_pauli_terms = sorted(hamiltonian["pauli"], key= lambda x: _get_tuple_for_sort(x, pos_dict, num_qubit_per_site))
    hamiltonian["pauli"] = sorted_pauli_terms

    if 0:
        for term in sorted_pauli_terms:
            tuples = _get_tuple_for_sort(term, pos_dict, num_qubit_per_site)
            assert(len(tuples)==2)
            dist = abs(tuples[0][0]-tuples[1][0]) + abs(tuples[0][1]-tuples[1][1])
            assert( dist in [1, 2] )
            # print(tuples)
    return hamiltonian
