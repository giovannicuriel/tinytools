#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#define PORT 9092
  
void marshall(uint8_t *& output, uint32_t & offset, std::string data) {
    int16_t temp = ntohs(data.length());
    memcpy(output, &temp, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    output += sizeof(uint16_t);;

    const char * str = data.c_str();

    memcpy(output, str, data.length());
    offset += data.length();    
    output += data.length();
}

void marshall(uint8_t *& output, uint32_t & offset, int32_t data) {
    int32_t temp = ntohl(data);
    memcpy(output, &temp, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    output += sizeof(uint32_t);
}

void marshall(uint8_t *& output, uint32_t & offset, int16_t data) {
    int16_t temp = ntohs(data);
    memcpy(output, &temp, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    output += sizeof(uint16_t);
}


struct partition_t {
    int32_t partition;
    int32_t replicas;
    partition_t() {
        partition = 0;
        replicas = 0;
    }
    void marshall(uint8_t *& output, uint32_t & offset) {
        ::marshall(output, offset, partition);
        ::marshall(output, offset, replicas);
    }
};

struct config_entries_t {
    std::string config_name;
    std::string config_value;
    void marshall(uint8_t *& output, uint32_t & offset) {
        ::marshall(output, offset, config_name);
        ::marshall(output, offset, config_value);
    }
};

struct KafkaMessage {
    int32_t size;
    KafkaMessage() { 
        size = 0;
    }
    void marshall(uint8_t *& output, uint32_t & offset) {
        std::cout << "Marshalling KafkaMessage" << std::endl;
        std::cout << "Size is: " << size << std::endl;
        ::marshall(output, offset, size);
    }
};

struct RequestMessage {
    KafkaMessage header;
    int16_t api_key;
    int16_t api_version;
    int32_t correlation_id;
    std::string client_id;
    void marshall(uint8_t *& output, uint32_t & offset) {
        std::cout << "Marshalling RequestMessage" << std::endl;
        header.marshall(output, offset);
        ::marshall(output, offset, api_key);
        ::marshall(output, offset, api_version);
        ::marshall(output, offset, correlation_id);
        ::marshall(output, offset, client_id);
    }
};

struct CreateTopicMessage {
    RequestMessage header;
    std::string topic;
    int32_t num_partitions;
    int16_t replication_factor;
    partition_t replica_assignment;
    config_entries_t config_entries;
    int32_t timeout;
    int32_t size;

    void marshall(uint8_t *& output, uint32_t & offset) {
        std::cout << "Marshalling CreateTopicMessage" << std::endl;
        size = 1;
        
        header.marshall(output, offset);
        ::marshall(output, offset, size);
        ::marshall(output, offset, topic);
        ::marshall(output, offset, num_partitions);
        ::marshall(output, offset, replication_factor);
        replica_assignment.marshall(output, offset);
        config_entries.marshall(output, offset);
        ::marshall(output, offset, timeout);
    }
};


int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    inet_pton(AF_INET, "172.25.0.3", &serv_addr.sin_addr);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Connection failed." << std::endl;
        return -1;
    }

    CreateTopicMessage message;
    message.topic = "teste-12345";
    message.num_partitions = 2;
    message.replication_factor = 1;
    message.timeout = 5000;
    message.header.api_key = 19;
    message.header.api_version = 0;
    message.header.correlation_id = 98765;
    message.header.client_id = "rdkakfa";

    uint32_t offset = 0;
    uint32_t totalSize = 0;
    uint8_t data[256] = { 0 };
    uint8_t * cursor = & data [0];
    message.marshall(cursor, offset);
    message.header.header.size = offset;
    totalSize = offset + 4;
    cursor = & data [0];
    offset = 0;
    message.header.marshall(cursor, offset);
    std::cout << "Sending message: size" << totalSize << std::endl;
    int ret = send(sock, data, totalSize, 0);
    std::cout << "Hello message sent" << std::endl;
    valread = read(sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}