
// *****************************************************************************
//
// test rfcomm query tests
//
// *****************************************************************************

#include "btstack-config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdp_query_rfcomm.h"
#include <btstack/hci_cmds.h>
#include <btstack/run_loop.h>

#include "hci.h"
#include "btstack_memory.h"
#include "hci_dump.h"
#include "l2cap.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

static uint8_t  service_index;
static uint8_t  channel_nr[10];
static char*    service_name[10];

static uint8_t  sdp_test_record_list[] = { 0x36, 0x02, 0xE7, 0x35, 0x48,
0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x0A, 0x09, 0x00, 0x04, 0x35, 0x10, 0x35, 0x06, 0x19,
0x01, 0x00, 0x09, 0x00, 0x19, 0x35, 0x06, 0x19, 0x00, 0x19, 0x09, 0x01, 0x00, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x0D, 0x09,
0x01, 0x00, 0x09, 0x01, 0x00, 0x25, 0x11, 0x41, 0x32, 0x44, 0x50, 0x20, 0x41, 0x75, 0x64, 0x69,
0x6F, 0x20, 0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x35, 0x43, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19,
0x11, 0x0C, 0x09, 0x00, 0x04, 0x35, 0x10, 0x35, 0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x17, 0x35,
0x06, 0x19, 0x00, 0x17, 0x09, 0x01, 0x00, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09,
0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x0E, 0x09, 0x01, 0x03, 0x09, 0x01, 0x00, 0x25,
0x0C, 0x41, 0x56, 0x52, 0x43, 0x50, 0x20, 0x54, 0x61, 0x72, 0x67, 0x65, 0x74, 0x35, 0x56, 0x09,
0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x05, 0x09, 0x00, 0x04, 0x35, 0x11, 0x35, 0x03, 0x19, 0x01,
0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x0A, 0x35, 0x03, 0x19, 0x00, 0x08, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A,
0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x05, 0x09, 0x01, 0x00,
0x09, 0x01, 0x00, 0x25, 0x10, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74,
0x20, 0x50, 0x75, 0x73, 0x68, 0x35, 0x5A, 0x09, 0x00, 0x01, 0x35, 0x06, 0x19, 0x11, 0x1F, 0x19,
0x12, 0x03, 0x09, 0x00, 0x04, 0x35, 0x0C, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00,
0x03, 0x08, 0x02, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09,
0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x06, 0x19, 0x11,
0x1E,
0x09, 0x01, 0x05, 0x09, 0x01,
0x00, 0x25, 0x18, 0x48, 0x61, 0x6E, 0x64, 0x73, 0x20, 0x46, 0x72, 0x65, 0x65, 0x20, 0x41, 0x75,
0x64, 0x69, 0x6F, 0x20, 0x47, 0x61, 0x74, 0x65, 0x77, 0x61, 0x79, 0x35, 0x27, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x01, 0x00, 0x25, 0x1A, 0x41, 0x70, 0x70, 0x6C, 0x65, 0x20,
0x4D, 0x61, 0x63, 0x69, 0x6E, 0x74, 0x6F, 0x73, 0x68, 0x20, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62,
0x75, 0x74, 0x65, 0x73, 0x35, 0x58, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x06, 0x09, 0x00,
0x04, 0x35, 0x11, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x0F, 0x35,
0x03, 0x19, 0x00, 0x08, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35,
0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35,
0x06, 0x19, 0x11, 0x06, 0x09, 0x01, 0x00, 0x09, 0x01, 0x00, 0x25, 0x12, 0x4F, 0x42, 0x45, 0x58,
0x20, 0x46, 0x69, 0x6C, 0x65, 0x20, 0x54, 0x72, 0x61, 0x6E, 0x73, 0x66, 0x65, 0x72, 0x35, 0x53,
0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x01, 0x09, 0x00, 0x04, 0x35, 0x0C, 0x35, 0x03, 0x19,
0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x03, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10,
0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09,
0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x01, 0x09, 0x01, 0x00, 0x09, 0x01, 0x00, 0x25,
0x12, 0x42, 0x6C, 0x75, 0x65, 0x74, 0x6F, 0x6F, 0x74, 0x68, 0x2D, 0x50, 0x44, 0x41, 0x2D, 0x53,
0x79, 0x6E, 0x63, 0x35, 0x59, 0x09, 0x00, 0x01, 0x35, 0x06, 0x19, 0x11, 0x12, 0x19, 0x12, 0x03,
0x09, 0x00, 0x04, 0x35, 0x0C, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08,
0x04, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65,
0x6E,
0x09, 0x00, 0x6A, 0x09, 0x01,
0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x08, 0x09, 0x01, 0x02, 0x09, 0x01,
0x00, 0x25, 0x15, 0x48, 0x65, 0x61, 0x64, 0x73, 0x65, 0x74, 0x20, 0x41, 0x75, 0x64, 0x69, 0x6F,
0x20, 0x47, 0x61, 0x74, 0x65, 0x77, 0x61, 0x79, 0x35, 0x6F, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19,
0x11, 0x17, 0x09, 0x00, 0x04, 0x35, 0x1E, 0x35, 0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x0F, 0x35,
0x14, 0x19, 0x00, 0x0F, 0x09, 0x01, 0x00, 0x35, 0x0C, 0x09, 0x08, 0x00, 0x09, 0x08, 0x06, 0x09,
0x86, 0xDD, 0x09, 0x88, 0x0B, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06,
0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08,
0x35, 0x06, 0x19, 0x11, 0x17, 0x09, 0x01, 0x00, 0x09, 0x01, 0x00, 0x25, 0x1C, 0x47, 0x72, 0x6F,
0x75, 0x70, 0x20, 0x41, 0x64, 0x2D, 0x68, 0x6F, 0x63, 0x20, 0x4E, 0x65, 0x74, 0x77, 0x6F, 0x72,
0x6B, 0x20, 0x53, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65
};

// dummy function to allow compile without the stack
extern "C" void sdp_client_query(bd_addr_t remote, uint8_t * des_serviceSearchPattern, uint8_t * des_attributeIDList){
}

// for test purposes
void sdp_query_rfcomm_init();


void handle_query_rfcomm_event(sdp_query_event_t * event, void * context){
    sdp_query_rfcomm_service_event_t * ve;
    sdp_query_complete_event_t * ce;

    switch (event->type){
        case SDP_QUERY_RFCOMM_SERVICE:
            ve = (sdp_query_rfcomm_service_event_t*) event;
            channel_nr[service_index] = ve->channel_nr;
            service_name[service_index] = (char*) malloc(SDP_SERVICE_NAME_LEN+1);
            strncpy(service_name[service_index], (char*) ve->service_name, SDP_SERVICE_NAME_LEN);
            service_name[service_index][SDP_SERVICE_NAME_LEN] = 0;
            // printf("CALLBACK: Service name: '%s', RFCOMM port %u, service index %d\n", service_name[service_index], channel_nr[service_index], service_index);
            service_index++;
            break;
        case SDP_QUERY_COMPLETE:
            ce = (sdp_query_complete_event_t*) event;
            // printf("CALLBACK: Client query response done. \n");
            break;
    }
}


TEST_GROUP(SDPClient){
    uint8_t spp_buffer[sizeof(sdp_test_record_list)];

    void setup(){
        service_index = 0;
        sdp_query_rfcomm_register_callback(handle_query_rfcomm_event, NULL);
        sdp_parser_init();
        sdp_query_rfcomm_init();
    }
};


TEST(SDPClient, QueryRFCOMMWithSyntheticData){

    const char* expected_name[] = {"SDP Response Test1", "SDP Response Test2", "SDP Response Test3"};
    uint8_t expected_channel[] = {10, 11, 12};
    int record_nr = sizeof(expected_channel)/sizeof(uint8_t); 
    int i;
    
    de_create_sequence(spp_buffer);
    for (i=0; i<record_nr; i++){
        uint8_t * record_start = de_push_sequence(spp_buffer);
        sdp_create_spp_service(record_start, expected_channel[i], expected_name[i]);
        de_pop_sequence(spp_buffer, record_start);
    }
    
    sdp_parser_handle_chunk(spp_buffer, de_get_len(spp_buffer));

    CHECK_EQUAL(service_index, record_nr);
    for (i=0; i<service_index; i++){
        STRCMP_EQUAL(expected_name[i], service_name[i]);
        CHECK_EQUAL(expected_channel[i], channel_nr[i]);
    }
}

TEST(SDPClient, QueryRFCOMMWithMacOSXData){
    const char* expected_name[] = {"OBEX Object Push", 
                                    "Hands Free Audio Gat", "OBEX File Transfer",
                                    "Bluetooth-PDA-Sync", "Headset Audio Gatewa"};
    uint8_t expected_channel[] = {10, 2, 15, 3, 4};
    // de_dump_data_element(sdp_test_record_list);

    sdp_parser_handle_chunk(sdp_test_record_list, de_get_len(sdp_test_record_list));
   
    CHECK_EQUAL(service_index, 5);
    int i;
    for (i=0; i<service_index; i++){
        STRCMP_EQUAL(expected_name[i], service_name[i]);
        CHECK_EQUAL(expected_channel[i], channel_nr[i]);
    }          
}


int main (int argc, const char * argv[]){
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
