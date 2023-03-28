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

 typedef enum {
    E_SUCCESS = 0,
    E_FAILURE = 1,
    E_TIMEOUT = 2
 }ITC_STATUS_MESSAGE;

typedef std::map <std::string, Synopsis::DpMetadataValue> Map;
// TODO:  Full remove Expects - work in if and throws?

 class TestASDS : public Synopsis::ASDS {

    public:

        int invocations;

        Synopsis::Status init(size_t bytes, void* memory, Synopsis::Logger *logger) override {
            this->_logger = logger;
            this->invocations = 0;
            return Synopsis::SUCCESS;
        }

        Synopsis::Status deinit() override {
            this->invocations = 0;
            return Synopsis::SUCCESS;
        }

        Synopsis::Status process_data_product(Synopsis::DpMsg msg) override {
            this->invocations++;
            return Synopsis::SUCCESS;
        }

        size_t memory_requirement(void) override {
            return 123;
        }
};

// Globals currently necessary for wrapping (might be pushed to setup later)
static TestASDS itc_asds;
Synopsis::PassthroughASDS pt_asds;
Synopsis::SqliteASDPDB db(":memory:");
Synopsis::StdLogger logger;
Synopsis::LinuxClock clock2;
Synopsis::MaxMarginalRelevanceDownlinkPlanner planner;
Synopsis::Application app(&db, &planner, &logger, &clock2);
void* memory = malloc(128);

// ITC Struct for holding DpMsg Objects
struct itc_dpmsg {
    Synopsis::DpMsg *obj;
};

// ITC Struct for holding DpDBMsg Objects
struct itc_dbdpmsg{
    Synopsis::DpDbMsg *obj;
};

// ITC Struct for holding DpMetadataValue Objects
struct itc_dpmetavalue{
    Synopsis::DpMetadataValue *obj;
};

// ITC Struct for holding linked list of value/type pairs for creating a MAP object later
struct itc_node {
    char* value_type;
    itc_dpmetavalue_t *meta_data_value;
    itc_node_t *next = NULL;

};

// ITC Struct for holding the conversion from Vector
struct itc_dpids{
    int* values;
    int size;
};

/**
 * ITC Function for setup of the TEST ASDS Class
 * @return: VOID
*/
extern int itc_setup_testasds(){
    Synopsis::Status status;
    status = app.add_asds("test_instrument", &itc_asds);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    if (result != ITC_STATUS_MESSAGE::E_SUCCESS){
        // TODO:  How would we like to handle these situations?
        //        Do we just want to use status instead with no extras?
    }
    printf("itc_setup_testasds complete\n");
    return result;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function for setup of the PassThrough ASDS
 * @return: VOID
*/
void itc_setup_ptasds(){
    Synopsis::Status status;
    status = app.add_asds("test_instrument", "type", &pt_asds);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
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
void itc_app_init(size_t bytes, void* memory){ // Remember to free memory
    Synopsis::Status status;
    status = app.init(bytes, memory);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function for De-Initialization of the App
 * NOTICE:  The memoery object that is used and passed about has been freed here
 * as I beleive this should be the last function used.
 * @return: VOID
*/
void itc_app_deinit(){
    Synopsis::Status status;
    status = app.deinit();
    free(memory);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function to retreive the number of invocations from the ASDS
 * @return: INT Representation from asds.invocations
*/
int itc_app_get_invocations(){
    int invocations = itc_asds.invocations;
    printf("INVOCATIONS: %d\n", invocations);
    return invocations;
}

/**
 * ITC Function for creating a dpMSG
 * This function accepts char* enlieu of std::str and converts as necessary
 * TODO:  These use of char* could cause issues maybe?
 * @return:  itc_dpmsg_t - structure which holds the dpmsg object
*/
itc_dpmsg_t* itc_create_dpmsg(char* instrument_name, char* dp_type, char* dp_uri, char* meta_uri, bool meta_usage){
    itc_dpmsg_t *msg;
    Synopsis::DpMsg *dpobj;

    msg = (typeof(msg))malloc(sizeof(*msg));
    std::string inst_name(instrument_name);
    std::string type(dp_type);
    std::string uri(dp_uri);
    std::string metauri(meta_uri);

    dpobj = new Synopsis::DpMsg(inst_name, type, uri, metauri, meta_usage);

    msg->obj = dpobj;

    return msg;
}

/**
 * ITC Function for creating a dpmetadata value
 * @return itc_dpmetavalue_t* - holds the dpmetadatavalue - INT Variant
*/
itc_dpmetavalue_t* itc_create_dpmetadatavalue_int(int int_value){
    itc_dpmetavalue_t *value;
    Synopsis::DpMetadataValue *obj;

    value = (typeof(value))malloc(sizeof(*value));
    obj = new Synopsis::DpMetadataValue(int_value);

    value->obj = obj;
    return value;
}

/**
 * ITC Function for creating a dpmetadata value
 * @return itc_dpmetavalue_t* - holds the dpmetadatavalue - FLOAT Variant
*/
itc_dpmetavalue_t* itc_create_dpmetadatavalue_float(double float_value){
    itc_dpmetavalue_t *value;
    Synopsis::DpMetadataValue *obj;

    value = (typeof(value))malloc(sizeof(*value));
    obj = new Synopsis::DpMetadataValue(float_value);

    value->obj = obj;
    return value;
}

/**
 * ITC Function for creating a dpmetadata value
 * @return itc_dpmetavalue_t* - holds the dpmetadatavalue - STRING Variant
*/
itc_dpmetavalue_t* itc_create_dpmetadatavalue_string(char* string_value){
    itc_dpmetavalue_t *value;
    Synopsis::DpMetadataValue *obj;

    value = (typeof(value))malloc(sizeof(*value));
    std::string s_value(string_value);
    obj = new Synopsis::DpMetadataValue(s_value);

    value->obj = obj;
    return value;
}

// TODO:  Keeping this for now.  But it is not utilized.
void itc_map_put(void* map, char* dp_string, itc_dpmetavalue_t* dpmetavalue){
    std::string dpstring(dp_string);
    Map* m = reinterpret_cast<Map *>(map);
    Synopsis::MetadataType value_type = dpmetavalue->obj->get_type();
    Synopsis::DpMetadataValue realvalue;
    switch(value_type){
        case 0: //int
            realvalue = Synopsis::DpMetadataValue(dpmetavalue->obj->get_int_value());
            break;
        case 1: //float
            realvalue = Synopsis::DpMetadataValue(dpmetavalue->obj->get_float_value());
            break;
        case 2: //string
            realvalue = Synopsis::DpMetadataValue(dpmetavalue->obj->get_string_value());
            break;
        default:
            printf("Invalid value Type when creating DpMetaDataValue!\n");
    }
    m->insert(std::pair<std::string, Synopsis::DpMetadataValue>(dpstring, realvalue));
}

// TODO:  Keeping this for now.  But it is not utilized.
void itc_node_push(itc_node_t* head, char* word, itc_dpmetavalue_t* metavalue){
    itc_node_t* current = head;
    if(current != NULL){
        while(current->next != NULL){
            current = current->next;
        }
    }
    current = (itc_node_t*) malloc(sizeof(itc_node_t));
    current->value_type = (char *)malloc(strlen(word));
    strcpy(current->value_type, word);
    current->meta_data_value = metavalue;
    //printf("Word to be inserted %s\n", word);
    printf("Word Inserted: %s\n", (char *)current->value_type);
    current->next = NULL;
}

/**
 * ITC Function to create DPDBMsg
 * Utilizes Const Char* - Issue?  Maybe?
 * @return: itc-dbdpmsg_t* Structure holding the DpDbMsg object
 * TODO:  Is there an issue with this function?
*/
itc_dbdpmsg_t* itc_create_dbdpmsg(int dp_id, const char* instrument_name, const char* dp_type, const char* dp_uri, size_t dp_size, double science_utility_estimate, int priority_bin, int downlink_state, itc_node_t* meta_node){
    itc_dbdpmsg_t *msg;
    Synopsis::DpDbMsg *dbdpobj;
    std::map<std::string, Synopsis::DpMetadataValue> mp;
    itc_node_t *current = meta_node;
    while(current != NULL){
        Synopsis::DpMetadataValue *tempdpvalue = static_cast<Synopsis::DpMetadataValue *>(current->meta_data_value->obj);
        Synopsis::MetadataType value_type = tempdpvalue->get_type();
        Synopsis::DpMetadataValue realvalue;
        switch(value_type){
        case 0: //int
            realvalue = Synopsis::DpMetadataValue(tempdpvalue->get_int_value());
            break;
        case 1: //float
            realvalue = Synopsis::DpMetadataValue(tempdpvalue->get_float_value());
            break;
        case 2: //string
            realvalue = Synopsis::DpMetadataValue(tempdpvalue->get_string_value());
            break;
        default:
            printf("Invalid value Type when creating DpMetaDataValue!\n");
        }
        mp.insert({std::string(current->value_type), realvalue});
    }

    msg = (typeof(msg))malloc(sizeof(*msg));
    Synopsis::DownlinkState dl_state;
    switch(downlink_state){
        case 0:
            dl_state = Synopsis::DownlinkState::UNTRANSMITTED;
            break;
        case 1:
            dl_state = Synopsis::DownlinkState::TRANSMITTED;
            break;
        case 2: 
            dl_state = Synopsis::DownlinkState::DOWNLINKED;
            break;
        default:
            printf("This is an invalid downlink state must be 0-2!\n");
    }
    printf("Tis is a test: %s\n", instrument_name);
    std::string inst_name(instrument_name);
    std::string dptype(dp_type);
    std::string dpuri(dp_uri);
    dbdpobj = new Synopsis::DpDbMsg(dp_id, inst_name, dptype, dpuri, dp_size, science_utility_estimate, priority_bin, dl_state, (Synopsis::AsdpEntry)mp);
   
    msg->obj = dbdpobj;
    
    return msg;
}

/**
 * ITC Function to initalize the DataBase
 * @return: VOID
*/
void itc_db_init(size_t bytes, void* memory){
    Synopsis::Status status;
    status = db.init(bytes, memory, &logger);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function for Dumb Data Product Entry Testing
 * @return: void
*/
void itc_db_insert_dumb_data_product(){
    Synopsis::Status status;
    Synopsis::DpDbMsg msg(
        -1, "test_instr", "test_type", "file:///data/file.dat",
        101, 0.12345, 7, Synopsis::DownlinkState::UNTRANSMITTED,
        {
            {"test_int", Synopsis::DpMetadataValue(123)},
            {"test_float", Synopsis::DpMetadataValue(123.456)},
            {"test_string", Synopsis::DpMetadataValue("test")}
        }
    );
    status = db.insert_data_product(msg);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function for accepting dumb dpmsg for testing
 * @return: VOID
*/
void itc_app_accept_dumb_dpmsg(){
    Synopsis::Status status;
    Synopsis::DpMsg msg(
        "test_instrument", "test_type",
        "file::///data/file.dat",
        "file::///data/meta.dat",
        true
    );

    status = app.accept_dp(msg);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function for accepting DPMSG created by ITC
 * @param[in] msg: The message to be accepted.
 * @return: VOID
*/
void itc_app_accept_dpmsg(itc_dpmsg_t* msg){
    Synopsis::Status status;
    status = app.accept_dp(*(msg->obj));
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function to insert dataproducts into global DataBase
 * @param[in] msg: The Data Product Message to be inserted into the DB
*/
void itc_db_insert_data_product(itc_dbdpmsg_t *msg){
    Synopsis::Status status;
    status = db.insert_data_product(*(msg->obj));
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
}

/**
 * ITC Function to get dataproduct ID from message
 * @param[in] msg: The message from which to retreive the DPID
 * @return: INT
*/
int itc_msg_get_dp_id(itc_dbdpmsg_t * msg){
    Synopsis::DpDbMsg *tempdbmsg = static_cast<Synopsis::DpDbMsg *>(msg->obj);
    return tempdbmsg->get_dp_id();
}

/**
 * ITC Function to retreive the instrument name from a message
 * @param[in] msg: itc_dpdbmsg_t* from which to retreive the instrument name
 * @return const char* - The instrument Name
 * TODO:  Cause of issues?
*/
const char* itc_msg_get_instrument_name(itc_dbdpmsg_t* msg){
    Synopsis::DpDbMsg *tempdbmsg = static_cast<Synopsis::DpDbMsg *>(msg->obj);
    std::string str = (tempdbmsg->get_instrument_name());
    printf("RB: %s\n", str.c_str());
    const char* retval = str.c_str();
    return retval;
}


//TODO -- Not sure what problem is being caused by this wrapping.  Requires some investigation

//FREE ME
itc_dpids_t *itc_db_list_data_product_ids(){
    std::vector<int> asdp_ids = db.list_data_product_ids();
    itc_dpids_t *array = (itc_dpids_t *)malloc(sizeof(itc_dpids_t));
    int n = asdp_ids.size();
    array->size = n;
    array->values = (int *)malloc(n * sizeof(int));
    std::copy(asdp_ids.begin(), asdp_ids.end(), array->values);
    return array;
}

//FREE ME (When, Where, should this be reworked?)
itc_dbdpmsg_t* itc_db_get_data_product(int id, itc_dbdpmsg_t *msg){
    Synopsis::Status status;
    //msg = (typeof(msg))malloc(sizeof(*msg));
    Synopsis::DpDbMsg temp_msg2;
    status = db.get_data_product(id, temp_msg2);
    ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;

    printf("MESSAGE2: %s\n", temp_msg2.get_instrument_name().c_str());
    msg->obj = &temp_msg2;   

    //EXPECT_EQ(Synopsis::Status::SUCCESS, status);  

    return msg;  
}