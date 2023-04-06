 #include "itc_synopsis_bridge.h"

#include "synopsis.hpp"
#include "SqliteASDPDB.hpp"
#include "StdLogger.hpp"
#include "LinuxClock.hpp"
#include "MaxMarginalRelevanceDownlinkPlanner.hpp"
#include "Timer.hpp"
#include "RuleAST.hpp"
#include "StdLogger.hpp"
#include <cstring>
#include <vector>

 typedef enum {
    E_SUCCESS = 0,
    E_FAILURE = 1,
    E_TIMEOUT = 2
 }ITC_STATUS_MESSAGE;

//Owls Test
Synopsis::SqliteASDPDB db(":memory:");
Synopsis::StdLogger logger;
Synopsis::LinuxClock clock2;
Synopsis::MaxMarginalRelevanceDownlinkPlanner planner;

Synopsis::Application app(&db, &planner, &logger, &clock2);
Synopsis::PassthroughASDS pt_asds;

int counter = 0; // Hacky way of adding OWLS Data
std::vector<std::string> prioritized_uris;

int owls_add_dpmsg(){
    Synopsis::Status status = Synopsis::Status::FAILURE;
    if (counter > 6){
        printf("No Data to receive!\n");
    }
    else{
        printf("SYN_APP: Adding Data Product %d\n", counter);
        std::string data_path("/home/nos3/Desktop/github-nos3/fsw/build/exe/cpu1/data/owls/bundle/asdp00000000" + std::to_string(counter) + ".tgz");
        std::string metadata_path("/home/nos3/Desktop/github-nos3/fsw/build/exe/cpu1/data/owls/bundle/asdp00000000" + std::to_string(counter) + "_meta.json");
        Synopsis::DpMsg msg("owls", "helm", data_path, metadata_path, true);

        status = app.accept_dp(msg);
        if (status != Synopsis::Status::SUCCESS) { return status; }    
        counter++;
        return status;
    }
    return status;
}

int owls_prioritize_data(){
    //std::string asdpdb("/home/nos3/Desktop/github-nos3/fsw/build/exe/cpu1/data/owls/owls_bundle_20221223T144226.db");
    std::string rule_file("/home/nos3/Desktop/github-nos3/fsw/build/exe/cpu1/data/owls/empty_rules.json");
    std::string similarity("/home/nos3/Desktop/github-nos3/fsw/build/exe/cpu1/data/owls/owls_similarity_config.json");
    
    Synopsis::Status status;
    std::vector<int> prioritized_list;
    
    status = app.prioritize(rule_file, similarity, 1e9, prioritized_list);
    if (status != Synopsis::Status::SUCCESS) { return status; }

    if(prioritized_uris.size() != 0) {prioritized_uris.clear();}
    
    for (auto i : prioritized_list) {
        Synopsis::DpDbMsg temp_msg;
        app.get_data_product(i, temp_msg);
        prioritized_uris.push_back(temp_msg.get_uri());
        //printf("NAME: %s\n", temp_msg.get_uri().c_str());
    }
    for (auto uri : prioritized_uris){
        printf("URI: %s\n", uri.c_str());
    }

    return Synopsis::Status::SUCCESS;
}

int owls_display_prioritized_data(){
    for (auto uri: prioritized_uris){
        printf("URI: %s\n", uri.c_str());
    }
    return Synopsis::Status::SUCCESS;
}

char* owls_get_prioritized_data(){
    if(prioritized_uris.size() != 0 ){
        size_t len = strlen(prioritized_uris[0].c_str())+1;
        char* returnval = new char[len];
        strcpy(returnval, (char*)prioritized_uris[0].c_str());
        printf("BRIDGE: %s\n", returnval);
        //char* returnval = (char*)prioritized_uris[0].c_str();

        prioritized_uris.erase(prioritized_uris.begin());
        return returnval;
        }
    else{
        return NULL;
    }
}

void owls_destroy_prioritized_data_string(char* deleteme){
    delete[] deleteme;
}

/**
 * ITC Function for setup of the PassThrough ASDS
 * @return: VOID
*/
void itc_setup_ptasds(){
    Synopsis::Status status;
    status = app.add_asds("owls", "helm", &pt_asds);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    if(result == E_SUCCESS){
        printf("ITC PTASDS SETUP SUCCESSFUL!\n");
    }
    else{
        printf("ITC PTASDS SETUP UNSUCCESSFUL!\n");
    }
}

/**
 * ITC Function for setup of the TEST ASDS Class
 * Note:  This requires some setup or some erroneous behavior will occur.
 * @return: size_t memory requirement size required by app
*/
size_t itc_app_get_memory_requiremennt(){
    size_t mem_req_bytes = 0;
    mem_req_bytes = app.memory_requirement();        
    printf("REQ Bytes: %d\n", mem_req_bytes);
    return mem_req_bytes;
}

/**
 * ITC Function for Initializing the App.  
 * Makes use of a memory object that will need to be freed.
 * @return: VOID
*/
void itc_app_init(size_t bytes, void* memory){ 
    Synopsis::Status status;
    status = app.init(bytes, memory);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    if(result == E_SUCCESS){
        printf("ITC APP INIT SUCCESSFUL!\n");
    }
    else{
        printf("ITC APP INIT UNSUCCESSFUL!\n");
    }
}

/**
 * ITC Function for De-Initialization of the App
 * @return: VOID
*/
void itc_app_deinit(){
    Synopsis::Status status;
    status = app.deinit();
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    if(result == E_SUCCESS){
        printf("ITC DEINIT SUCCESSFUL!\n");
    }
    else{
        printf("ITC DEINIT UNSUCCESSFUL!\n");
    }
}