

default_network="$1"
docker_containers=$(docker ps  --format '{{.Names}}')

echo 
for i in ${docker_containers}; do
    ip=$(docker inspect ${i} --format "{{.NetworkSettings.Networks.${default_network}.IPAddress}}")
    exported=$(echo "${i}: ${ip}" | sed -E 's/.*_(.*)_[[:digit:]]: *(.*)/\1_HOST=\"\2\"/g' | sed -E 's/-/_/g' | sed  -E 's/(.*)/export \U\1/')
    echo ${exported}
done;
