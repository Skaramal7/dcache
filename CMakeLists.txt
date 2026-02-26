cmake_minimum_required(VERSION 4.1)
project(dcache C)

set(CMAKE_C_STANDARD 11)

include_directories(communication)
include_directories(storage)

add_executable(dCacheClient
        dCacheClient.c
        communication/message.c
        storage/datastorage.c)

add_executable(dCacheServer
        dCacheServer.c
        communication/message.c
        storage/datastorage.c
)
