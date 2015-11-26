# UN Switch Manager

The switch manager allows the un-orchestrator to interact with the virtual
switch (vSwitch) for management purposes, such as the creation of a new LSI, the
creation of new virtual ports, and more.

Thanks to the proper vSwitch plugin, the un-orchestrator can interact with a
specific virtual switch. New plugins (and then the support for a new vSwitch)
can be added in the folder `switch_manager/plugins`.
Each plugin is required to define a class implementing the interface
[switch_manager/switch_manager.h](switch_manager/switch_manager.h).

Moreover, following files:
* `orchestrator/CMakeLists.txt`
* `orchestrator/node_resource_manager/graph_manager/graph_manager.h`

must be properly updated so that the new plugin is recognized by the system.
