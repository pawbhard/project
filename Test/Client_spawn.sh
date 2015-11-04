#!/bin/bash
for((i=0;i<$1;i++))
do
    gnome-terminal -e  "bash -c  "/home/lakshniru/cohort/project/client/client_run &""
    client=$!
    echo "Pid of Cient$i is  $client"
done
