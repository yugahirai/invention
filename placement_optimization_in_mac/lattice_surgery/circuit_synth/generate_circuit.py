from __future__ import annotations
import numpy as np
import itertools
import os
from qsvt.synthesis.gate_qrom import create_control_qrom_simple, create_control_qrom_sawtooth, create_control_qrom_dist_improve, create_control_qrom_dist_improve_earlydrop
import json
import pickle
from qsvt.synthesis.circuit import Circuit
from qsvt.synthesis.qsvt import verify, count, SELECT
from qsvt.hamiltonian import Heisenberg_2D, Heisenberg_2D_ext, Heisenberg_1D, FermiHubbard2D, order_hamiltonian_to_slice
from qsvt.hamiltonian.util import padding_hamiltonian_term_thread
from qsvt.compile.decompose_control import decompose_controls, verify_decompose_control, count_decompose_control
# from qsvt.compile.decompose_rz import decompose_rotation, verify_decompose_rotation, count_decompose_rotation


def gate_select_simple(c: Circuit, reg_term: list[str], reg_hilbert: list[str], reg_term_ancilla: list[str], pauli_list: list) -> None:
    create_control_qrom_simple(c, reg_term, reg_hilbert, reg_term_ancilla, None, pauli_list)


def gate_select_sawtooth(c: Circuit, reg_term: list[str], reg_hilbert: list[str], reg_term_ancilla: list[str], pauli_list: list) -> None:
    create_control_qrom_sawtooth(c, reg_term, reg_hilbert, reg_term_ancilla, None, pauli_list)


def gate_select_dist(c: Circuit, reg_term: list[str], reg_hilbert: list[str], reg_term_ancilla: list[str], pauli_list: list, dup: int) -> None:
    create_control_qrom_dist_improve(c, reg_term, reg_hilbert, reg_term_ancilla, None, pauli_list, dup)

def gate_select_dist_earlydrop(c: Circuit, reg_term: list[str], reg_hilbert: list[str], reg_term_ancilla: list[str], pauli_list: list, dup: int) -> None:
    create_control_qrom_dist_improve_earlydrop(c, reg_term, reg_hilbert, reg_term_ancilla, None, pauli_list, dup)



def SELECT(ham: dict, dup: int):
    num_system = ham["num_qubit"]
    num_term = len(ham["pauli"])
    num_termlog = int(np.log2(num_term-1e-10))+1
    if 2*num_term < 2**dup:
        raise ValueError("too many parallelization")
    
    reg_control = [f"control_{idx}" for idx in range(num_termlog)]
    reg_control_ancillary = [f"control_ancilla_{idx}" for idx in range(num_termlog)]

    def to_str(tup):
        return "_".join(map(str, tup))

    reg_system = [f"system_{to_str(ham['pos'][idx])}" for idx in range(num_system)]

    select_circuit = Circuit()

    padded_pauli = padding_hamiltonian_term_thread(ham["pauli"], 2**dup)
    # print(ham["pauli"])
    # print(padded_pauli)
    # print(len(ham["pauli"]), len(padded_pauli), 2**dup)
    gate_select_dist_earlydrop(select_circuit, reg_control, reg_system, reg_control_ancillary, padded_pauli, dup)
    return select_circuit


def process_SELECT(hamiltonian: dict, dup: int = 1) -> tuple[dict, Circuit]:
    circuit = SELECT(hamiltonian, dup)
    verify(circuit)

    gates = []
    creg_cnt = 0
    for gate in circuit.gates:
        if gate["name"] == "CX":
            control = gate["controls"][0]
            target = gate["targets"][0]
            gates.append( {"name": "CX", "targets": [target], "controls": [control] })
            # print(gate)
        elif gate["name"] == "CY":
            control = gate["controls"][0]
            target = gate["targets"][0]
            gates.append( {"name": "H", "targets": [target] })
            gates.append( {"name": "S", "targets": [target] })
            gates.append( {"name": "CX", "targets": [target], "controls": [control] })
            gates.append( {"name": "Sdag", "targets": [target] })
            gates.append( {"name": "H", "targets": [target] })
            # print(gate)
        elif gate["name"] == "CZ":
            control = gate["controls"][0]
            target = gate["targets"][0]
            gates.append( {"name": "H", "targets": [target] })
            gates.append( {"name": "CX", "targets": [target], "controls": [control] })
            gates.append( {"name": "H", "targets": [target] })
            # int(gate)
        elif gate["name"] == "CCX":
            assert(gate["note"] in ["start_t3", "end_t3"])
            if gate["note"] == "start_t3":
                control1 = gate["controls"][0]
                control2 = gate["controls"][1]
                target = gate["targets"][0]
                creg = f"creg{creg_cnt}"
                gates.append( {"name": "MAGIC_MOVE", "targets": [target] })
                gates.append( {"name": "CX", "targets": [target], "controls": [control2] })
                gates.append( {"name": "Tdag", "targets": [target] })
                gates.append( {"name": "CX", "targets": [target], "controls": [control1] })
                gates.append( {"name": "T", "targets": [target] })
                gates.append( {"name": "CX", "targets": [target], "controls": [control2] })
                gates.append( {"name": "Tdag", "targets": [target] })
                gates.append( {"name": "H", "targets": [target] })
                gates.append( {"name": "Sdag", "targets": [target] })
            else:
                control1 = gate["controls"][0]
                control2 = gate["controls"][1]
                target = gate["targets"][0]
                creg = f"creg{creg_cnt}"
                gates.append( {"name": "MX", "targets": [target], "output": creg})
                gates.append( {"name": "H", "targets": [control2], "condition": creg })
                gates.append( {"name": "CX", "targets": [control2], "controls": [control1], "condition": creg })
                gates.append( {"name": "H", "targets": [control2], "condition": creg })
                creg_cnt += 1
        elif gate["name"]  == "barrior":
            continue
        elif gate["name"] == "X":
            continue
        else:
            assert(False)

    gate_T_replace = []
    for gate in gates:
        if gate["name"] == "T" or gate["name"] == "Tdag":
            creg = f"creg{creg_cnt}"
            gate_T_replace.append( {"name": "MAGIC_MZZ", "targets": gate["targets"], "output": creg})
            gate_T_replace.append( {"name": "S", "targets": gate["targets"], "condition": creg})
            creg_cnt += 1
        elif gate["name"] == "Sdag":
            gate["name"] = "S"
            gate_T_replace.append(gate)
        else:
            gate_T_replace.append(gate)

    return gate_T_replace


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


def generate_SELECT(size: int, ham_type: str, boundary_type: str, s: float, J2: float, ham: dict, dup: int, folder: str = "circuit"):
    gates = process_SELECT(ham, dup)

    file_head = f"result_SELECT_{size}_{ham_type}_{boundary_type}_{s}_{J2}_{dup}"
    fout = open(f"./{folder}/{file_head}.circ", "wb")
    pickle.dump(gates, fout)
    fout.close()

    fout = open(f"./{folder}/{file_head}.json", "w")
    json.dump(gates, fout)
    fout.close()
    return gates


def convert_SELECT(size: int, ham_type: str, boundary_type: str, s: float, J2: float, ham: tuple, dup: int, folder: str = "circuit"):
    file_head = f"result_SELECT_{size}_{ham_type}_{boundary_type}_{s}_{J2}_{dup}"
    fout = open(f"./{folder}/{file_head}.circ", "rb")
    gates = pickle.load(fout)
    fout.close()

    s = ""
    for gate in gates:
        s += f'{gate["name"]} {gate["targets"][0]}'
        if "controls" in gate:
            s += f' {gate["controls"][0]}'
        if "output" in gate:
            s += f' _out_ {gate["output"]}'
        if "condition" in gate:
            s += f' _cond_ {gate["condition"]}'
        s += "\n"
    fout = open(f"./{folder}/{file_head}.raw", "w")
    fout.write(s)
    fout.close()


def ham_debug():
    n = 2
    s = 1
    J2 = 1/2
    ham = Heisenberg_2D(n)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_2D_ext(n, "PBC", s, 0)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_2D_ext(n, "PBC", s, J2)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_2D_ext(n, "OBC", s, 0)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_2D_ext(n, "OBC", s, J2)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_1D(n, "PBC", s, 0)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_1D(n, "PBC", s, J2)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_1D(n, "OBC", s, 0)
    print(ham)
    print(len(ham[1]))
    ham = Heisenberg_1D(n, "OBC", s, J2)
    print(ham)
    print(len(ham[1]))
    ham = FermiHubbard2D(n, "PBC", decompodition="jordan")
    print(ham)
    print(len(ham[1]))
    ham = FermiHubbard2D(n, "OBC", decompodition="jordan")
    print(ham)
    print(len(ham[1]))
    ham = FermiHubbard2D(n, "PBC", decompodition="bravyi")
    print(ham)
    print(len(ham[1]))
    ham = FermiHubbard2D(n, "OBC", decompodition="bravyi")
    print(ham)
    print(len(ham[1]))


def generate_J1J2():
    # size = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
    size = [4,6,8,10,12]
    # ham_type = ["Heisenberg1D", "Heisenberg2D"]
    ham_type = ["Heisenberg2D"]
    max_dup_cnt = 6 + 1
    # boundary_type = ["PBC", "OBC"]
    boundary_type = ["cylinder"]
    s = [1/2, ]
    J2 = [1/2, ]
    args = list(itertools.product(size, ham_type, boundary_type, s, J2))

    for arg in args:
        ham = generate_hamiltonian(*arg)
        ham["num_pauli"] = len(ham["pauli"])

        file_head = f"result_SELECT_{arg[0]}_{arg[1]}_{arg[2]}_{arg[3]}_{arg[4]}"
        fout = open(f"./hamiltonian/{file_head}.ham", "w")
        json.dump(ham, fout)
        fout.close()

        num_pauli = len(ham["pauli"])
        max_dup = min(int(np.log2(num_pauli+1e-10)), max_dup_cnt)
        for dup in range(max_dup):
            print("generate : ", arg, dup)
            generate_SELECT(*arg, ham, dup)
            convert_SELECT(*arg, ham, dup)
            # print(info)


def generate_S1Chain():
    size = [10,20,40,80,160,320,640,1280]
    ham_type = ["Heisenberg1D"]
    boundary_type = ["OBC"]
    max_dup_cnt = 6 + 1
    s = [1, ]
    J2 = [0, ]
    args = list(itertools.product(size, ham_type, boundary_type, s, J2))

    for arg in args:
        ham = generate_hamiltonian(*arg)
        ham["num_pauli"] = len(ham["pauli"])

        file_head = f"result_SELECT_{arg[0]}_{arg[1]}_{arg[2]}_{arg[3]}_{arg[4]}"
        fout = open(f"./hamiltonian/{file_head}.ham", "w")
        json.dump(ham, fout)
        fout.close()

        num_pauli = len(ham["pauli"])
        max_dup = min(int(np.log2(num_pauli+1e-10)), max_dup_cnt)
        for dup in range(max_dup):
            print("generate : ", arg, dup)
            generate_SELECT(*arg, ham, dup)
            convert_SELECT(*arg, ham, dup)
            # print(info)


def generate_FermiHubbard():
    size = [4,6,8,10]
    max_dup_cnt = 4+1
    ham_type = ["FermiHubbard2D"]
    boundary_type = ["cylinder"]
    args = list(itertools.product(size, ham_type, boundary_type))

    for arg in args:
        ham = generate_hamiltonian(*arg, None, None)
        ham["num_pauli"] = len(ham["pauli"])
        for idx in range(len(ham["pauli"])):
            term = ham["pauli"][idx]
            term_fix = (np.real(term[0]), term[1])
            ham["pauli"][idx] = term_fix

        file_head = f"result_SELECT_{arg[0]}_{arg[1]}_{arg[2]}_0_0"
        fout = open(f"./hamiltonian/{file_head}.ham", "w")
        json.dump(ham, fout)
        fout.close()

        num_pauli = len(ham["pauli"])
        max_dup = min(int(np.log2(num_pauli+1e-10)), max_dup_cnt)
        for dup in range(max_dup):
            print("generate : ", arg, dup)
            generate_SELECT(*arg,0,0, ham, dup)
            convert_SELECT(*arg,0,0, ham, dup)
            # print(info)


if not os.path.exists("circuit"):
    os.mkdir("circuit")
if not os.path.exists("hamiltonian"):
    os.mkdir("hamiltonian")

generate_J1J2()
generate_S1Chain()
generate_FermiHubbard()

