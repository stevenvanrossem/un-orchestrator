#!/bin/bash
PATH=/home/unify/un-orchestrator/use-cases/elastic_router/elastic_router_files/nffg_files/er_nffg_virtualizer5_v4.xml
/usr/bin/curl -i -d "@$PATH" -X POST http://localhost:9090/edit-config

#cd ../use-cases/elastic_router/elastic_router_files/nffg_files
