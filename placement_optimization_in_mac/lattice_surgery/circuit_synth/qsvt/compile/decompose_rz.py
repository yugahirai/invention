from qsvt.synthesis.circuit import Circuit
from qsvt.solovay.Decompose_Rz import Decompose_Rz
import tqdm

def _flash_RZ(comp: Circuit, gate: dict, cnt: int):
    cnt = cnt % 8
    if cnt >= 4:
        comp.add_gate({"name": "Z", "targets": gate["targets"]})
        cnt -= 4
    if cnt >= 2:
        comp.add_gate({"name": "S", "targets": gate["targets"]})
        cnt -= 2
    if cnt == 1:
        comp.add_gate({"name": "T", "targets": gate["targets"]})

def _add_RZ(comp: Circuit, gate: dict, eps: float) -> None:
    assert(gate["name"] == "RZ")
    sequence = Decompose_Rz(gate["angle"], eps)
    cnt = 0
    for s in reversed(sequence):
        if s == "H":
            _flash_RZ(comp, gate, cnt)
            comp.add_gate({"name": "H", "targets": gate["targets"]})
            cnt = 0
        elif s == "S":
            cnt += 2
        elif s == "T":
            cnt += 1
    _flash_RZ(comp, gate, cnt)

def _add_RZ_simple(comp: Circuit, gate: dict, eps: float) -> None:
    assert(gate["name"] == "RZ")
    sequence = Decompose_Rz(gate["angle"], eps)
    for s in reversed(sequence):
        if s == "H":
            comp.add_gate({"name": "H", "targets": gate["targets"]})
        elif s == "S":
            comp.add_gate({"name": "S", "targets": gate["targets"]})
        elif s == "T":
            comp.add_gate({"name": "T", "targets": gate["targets"]})

def decompose_rotation(circuit: Circuit, eps: float) -> Circuit:
    comp = Circuit()
    for gate in tqdm.tqdm(circuit.gates):
        if gate["name"] == "RZ":
            _add_RZ(comp, gate, eps)
        elif gate["name"] == "Sdag":
            comp.add_gate({"name": "Z", "targets": gate["targets"]})
            comp.add_gate({"name": "S", "targets": gate["targets"]})
        else:
            comp.add_gate(gate)
    return comp


def verify_decompose_rotation(circuit):
    gate_list = ["Init0", "MeasZ", "MeasX", "H", "X", "Z", "S", "T", "Tdag"] + ["CX"]
    for gate in circuit.gates:
        assert(gate["name"] in gate_list)
        assert("name" in gate)
        assert("targets" in gate)
        assert(isinstance(gate["targets"], list))
        assert(gate["name"] in gate_list)
        if gate["name"] in ["Init0", "MeasZ", "MeasX", "H", "X", "Z", "S", "T", "Tdag"]:
            assert(len(gate["targets"]) == 1)
        if gate["name"] in ["CX"]:
            assert(len(gate["targets"]) == 1)
            assert(len(gate["controls"]) == 1)


def count_decompose_rotation(circuit: Circuit):
    qubit_set = set()
    num_T = 0
    for gate in circuit.gates:
        # print(gate)
        for name in gate["targets"]:
            qubit_set.add(name)
        if "controls" in gate:
            for name in gate["controls"]:
                qubit_set.add(name)
        if gate["name"] in ["T", "Tdag"]:
            num_T += 1

    qubits = list(sorted(list(qubit_set)))
    dic = {
        "num_qubit": len(qubit_set),
        "gate_count": len(circuit.gates),
        "num_T": num_T,
        "qubits": qubits,
        "circuit": circuit.gates,
    }
    return dic
