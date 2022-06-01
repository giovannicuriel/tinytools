function gerar_cpf() {
CPF=()                                                                        
verificador1=0                                                                
verificador2=0                                                                
algarismo=0                                                                   
for i in $(seq 1 9); do                                                       
algarismo=$(shuf -i 0-9 -n 1)                                               
CPF+=(${algarismo})                                                         
termo1=$(($algarismo*$i))                                                   
termo2=$(($algarismo*($i-1)))                                               
let verificador1+=$termo1                                                   
let verificador2+=$termo2                                                   
done                                                                          
verificador1=$(($verificador1 % 11))                                          
if [ ${verificador1} == 10 ]; then                                            
verificador1=0                                                              
fi                                                                            
                                                                            
let verificador2+=$((${verificador1}*9))                                      
verificador2=$(($verificador2 % 11))                                          
if [ ${verificador2} == 10 ]; then                                            
verificador2=0                                                              
fi                                                                            
CPF+=($verificador1 $verificador2)                                            
FullCPF=$(echo ${CPF[@]} | sed -E 's/([0-9]) /\1/g')                              
echo "${FullCPF[@]:0:3}${FullCPF[@]:3:3}${FullCPF[@]:6:3}${FullCPF[@]:9:2}"
}

  
