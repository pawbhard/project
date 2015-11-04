for((i=0;i<$1;i++))
do
#    echo "/root/Code/project/client/client_run `expr $i % 2` &"
    /root/Code/project/client/client_run `expr $i % 2` &
    
    client=$!
    echo "Pid of client$i is  $client"
done
