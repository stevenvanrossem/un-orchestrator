import json
from math import cos, sin, pi
from er_utils import DPPort
import logging

def build_graph(DP_dict, base_file='base_ER.json'):
    json_file = open(base_file).read()
    graph_dict = json.loads(json_file)

    sap_list = []
    x = 0
    y = 0
    R = 1
    num_DP = len(DP_dict)
    theta = (2 * pi) / num_DP
    i = 1
    j = 0
    for ovs_name in DP_dict:
        logging.info('graph ovs:{0}'.format(ovs_name))
        # extra rotation to avoid ovs node is just below ctrl node
        add = 0
        if num_DP % 4 == 0:
            add = theta / 2

        if num_DP > 1:
            x = R * cos((theta * i) + add)
            y = R * sin((theta * i) + add)
            i += 1

        ovs_dict={'id':ovs_name, 'label': ovs_name, 'x': x, 'y': y, 'size': 3}
        graph_dict['nodes'].append(ovs_dict)


        for port in DP_dict[ovs_name].ports:
            id = 'e{0}'.format(j)

            source = port.DP.name
            if port.port_type == DPPort.Internal:
                target = port.linked_port.DP.name
            elif port.port_type == DPPort.External:
                sap_name = port.linked_port.ifname
                if sap_name not in sap_list:
                    sap_list.append(sap_name)
                target = sap_name

            edge_dict = {'id': id, 'source': source, 'target': target}
            graph_dict['edges'].append(edge_dict)
            j += 1

    x = 0
    y = 0
    R = 2
    num_saps = len(sap_list)
    theta = (2 * pi) / num_saps
    i = 1
    for sap_name in sap_list:

        # extra rotation to avoid ovs node is just below ctrl node
        add = 0
        if num_saps % 4 == 0:
            add = theta / 2

        if num_saps > 1:
            x = R * cos((theta * i) + add)
            y = R * sin((theta * i) + add)
            i += 1

        sap_dict = {'id': sap_name, 'label': sap_name, 'x': x, 'y': y, 'size': 2}
        graph_dict['nodes'].append(sap_dict)

    logging.info('graph send:{0}'.format(json.dumps(graph_dict, indent=4)))
    return graph_dict



