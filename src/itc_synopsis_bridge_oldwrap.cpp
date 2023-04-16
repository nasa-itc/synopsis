
// typedef std::map <std::string, Synopsis::DpMetadataValue> Map;

//  class TestASDS : public Synopsis::ASDS {

//     public:

//         int invocations;

//         Synopsis::Status init(size_t bytes, void* memory, Synopsis::Logger *logger) override {
//             this->_logger = logger;
//             this->invocations = 0;
//             return Synopsis::SUCCESS;
//         }

//         Synopsis::Status deinit() override {
//             this->invocations = 0;
//             return Synopsis::SUCCESS;
//         }

//         Synopsis::Status process_data_product(Synopsis::DpMsg msg) override {
//             this->invocations++;
//             return Synopsis::SUCCESS;
//         }

//         size_t memory_requirement(void) override {
//             return 123;
//         }
// };

// Globals currently necessary for wrapping (might be pushed to setup later)

// static TestASDS itc_asds;
// Synopsis::PassthroughASDS pt_asds;
// Synopsis::SqliteASDPDB db(":memory:");
// Synopsis::StdLogger logger;
// Synopsis::LinuxClock clock2;
// Synopsis::MaxMarginalRelevanceDownlinkPlanner planner;
// Synopsis::Application app(&db, &planner, &logger, &clock2);

// ITC Struct for holding DpMsg Objects
// struct itc_dpmsg {
//     Synopsis::DpMsg *obj;
// };

// // ITC Struct for holding DpDBMsg Objects
// struct itc_dbdpmsg{
//     Synopsis::DpDbMsg *obj;
// };

// // ITC Struct for holding DpMetadataValue Objects
// struct itc_dpmetavalue{
//     Synopsis::DpMetadataValue *obj;
// };

// ITC Struct for holding linked list of value/type pairs for creating a MAP object later
// struct itc_node {
//     char* value_type;
//     itc_dpmetavalue_t *meta_data_value;
//     itc_node_t *next = NULL;

// };

// extern void itc_test_get_message(){
//     Synopsis::Status status;
//     std::vector<int> asdp_ids;
//     asdp_ids = db.list_data_product_ids();
//     Synopsis::DpDbMsg msg2;
//     status = db.get_data_product(asdp_ids[0], msg2);

//     //TODO:  Error Checking
//     printf("MESSAGE INFORMATION:\n");
//     printf("\tDPID: %d\n", msg2.get_dp_id());
//     printf("\tINSTRUMENT NAME: %s\n", msg2.get_instrument_name().c_str());
//     printf("\tTYPE: %s\n", msg2.get_type().c_str());
//     printf("\tURI: %s\n", msg2.get_uri().c_str());
//     printf("\tDPSIZE: %d\n", (int)msg2.get_dp_size());
// }

/**
 * ITC Function for setup of the TEST ASDS Class
 * @return: VOID
*/
// extern int itc_setup_testasds(){
//     Synopsis::Status status;
//     status = app.add_asds("test_instrument", &itc_asds);
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     if(result == E_SUCCESS){
//         printf("ITC TESTASDS SETUP SUCCESSFUL!\n");
//     }
//     else{
//         printf("ITC TESTASDS SETUP UNSUCCESSFUL!\n");
//     }
//     return result;
// }
/**
 * ITC Function to retreive the number of invocations from the ASDS
 * @return: INT Representation from asds.invocations
*/
// int itc_app_get_invocations(){
//     int invocations = itc_asds.invocations;
//     //printf("INVOCATIONS: %d\n", invocations);
//     return invocations;
// }

/**
 * ITC Function for creating a dpMSG
 * This function accepts char* enlieu of std::str and converts as necessary
 * NOTICE:  The user must use the itc_destroy_dpmsg function to free memory and objects
 * @return:  itc_dpmsg_t - structure which holds the dpmsg object
*/
// itc_dpmsg_t* itc_create_dpmsg(char* instrument_name, char* dp_type, char* dp_uri, char* meta_uri, bool meta_usage){
//     itc_dpmsg_t *msg;
//     Synopsis::DpMsg *dpobj;

//     msg = (typeof(msg))malloc(sizeof(*msg));
//     std::string inst_name(instrument_name);
//     std::string type(dp_type);
//     std::string uri(dp_uri);
//     std::string metauri(meta_uri);

//     dpobj = new Synopsis::DpMsg(inst_name, type, uri, metauri, meta_usage);

//     msg->obj = dpobj;

//     return msg;
// }

/**
 * ITC Function for destruction of itc_dpmsg structure
 * @param itc_dpmsg_t: DPMSG to be destroyed
*/
// void itc_destroy_dpmsg(itc_dpmsg_t *msg){
//     Synopsis::DpMsg *tmp_msg = msg->obj;
//     delete tmp_msg;
//     free(msg);
// }

/**
 * ITC Function for creating a dpmetadata value
 * @return itc_dpmetavalue_t* - holds the dpmetadatavalue - INT Variant
*/
// itc_dpmetavalue_t* itc_create_dpmetadatavalue_int(int int_value){
//     itc_dpmetavalue_t *value;
//     Synopsis::DpMetadataValue *obj;

//     value = (typeof(value))malloc(sizeof(*value));
//     obj = new Synopsis::DpMetadataValue(int_value);

//     value->obj = obj;
//     return value;
// }

/**
 * ITC Function for creating a dpmetadata value
 * @return itc_dpmetavalue_t* - holds the dpmetadatavalue - FLOAT Variant
*/
// itc_dpmetavalue_t* itc_create_dpmetadatavalue_float(double float_value){
//     itc_dpmetavalue_t *value;
//     Synopsis::DpMetadataValue *obj;

//     value = (typeof(value))malloc(sizeof(*value));
//     obj = new Synopsis::DpMetadataValue(float_value);

//     value->obj = obj;
//     return value;
// }

/**
 * ITC Function for creating a dpmetadata value
 * @return itc_dpmetavalue_t* - holds the dpmetadatavalue - STRING Variant
*/
// itc_dpmetavalue_t* itc_create_dpmetadatavalue_string(char* string_value){
//     itc_dpmetavalue_t *value;
//     Synopsis::DpMetadataValue *obj;

//     value = (typeof(value))malloc(sizeof(*value));
//     std::string s_value(string_value);
//     obj = new Synopsis::DpMetadataValue(s_value);

//     value->obj = obj;
//     return value;
// }
/**
 * ITC Function for the destruction of itc_dpmetavalue
 * @param itc_dpmetavalue_t: The itc_dpmetavalue structure to be destroyed
*/
// void itc_destroy_dpmetadatavalue(itc_dpmetavalue_t *metavalue){
//     Synopsis::DpMetadataValue *tmp_value = metavalue->obj;
//     delete tmp_value;
//     free(metavalue);
// }

// TODO:  Keeping this for now.  But it is not utilized.
// void itc_map_put(void* map, char* dp_string, itc_dpmetavalue_t* dpmetavalue){
//     std::string dpstring(dp_string);
//     Map* m = reinterpret_cast<Map *>(map);
//     Synopsis::MetadataType value_type = dpmetavalue->obj->get_type();
//     Synopsis::DpMetadataValue realvalue;
//     switch(value_type){
//         case 0: //int
//             realvalue = Synopsis::DpMetadataValue(dpmetavalue->obj->get_int_value());
//             break;
//         case 1: //float
//             realvalue = Synopsis::DpMetadataValue(dpmetavalue->obj->get_float_value());
//             break;
//         case 2: //string
//             realvalue = Synopsis::DpMetadataValue(dpmetavalue->obj->get_string_value());
//             break;
//         default:
//             printf("Invalid value Type when creating DpMetaDataValue!\n");
//     }
//     m->insert(std::pair<std::string, Synopsis::DpMetadataValue>(dpstring, realvalue));
// }

/**
 * ITC Function for creating a linked list of meta values and types
 * This is used in replacement for vector/map objects C cannot create
 * If there is a better way, let's do it
 * @param itc_node_t: The head node of the linked list
 * @param char*: value type to be inserted
 * @param itc_dpmetavalue_t: DPmetavalue to be inserted
*/
// void itc_node_push(itc_node_t* head, char* word, itc_dpmetavalue_t* metavalue){
//     itc_node_t* current = head;
    
//     itc_node_t* newnode = (itc_node_t*) malloc(sizeof(itc_node_t));
//     newnode->value_type = (char *)malloc(strlen(word));
//     strcpy(newnode->value_type, word);
//     newnode->meta_data_value = metavalue;
//     newnode->next = NULL;

//     if(current != NULL){
//         while(current->next != NULL){
//             current = current->next;
//         }
//         current->next = newnode;
//     }
//     else{
//         current = newnode;
//     }
// }

/**
 * ITC Function for the destruction of vector/map replacement linked list
 * @param itc_node_t: The start of the list to be destroyed
*/
// void itc_destroy_nodes(itc_node_t *head){
//     itc_node_t* current;
//     while(head != NULL){
//         current = head;
//         head = head->next;

//         free(current->value_type);
//         itc_destroy_dpmetadatavalue(current->meta_data_value);
//         free(current);
//     }
// }

/**
 * ITC Function to create DPDBMsg
 * Utilizes Const Char* - Issue?  Maybe?
 * @return: itc-dbdpmsg_t* Structure holding the DpDbMsg object
*/
// itc_dbdpmsg_t* itc_create_dbdpmsg(int dp_id, const char* instrument_name, const char* dp_type, const char* dp_uri, size_t dp_size, double science_utility_estimate, int priority_bin, int downlink_state, itc_node_t* meta_node){
//     itc_dbdpmsg_t *msg;
//     Synopsis::DpDbMsg *dbdpobj;
//     std::map<std::string, Synopsis::DpMetadataValue> mp;
//     itc_node_t *current = meta_node;
//     while(current != NULL){
//         Synopsis::DpMetadataValue *tempdpvalue = static_cast<Synopsis::DpMetadataValue *>(current->meta_data_value->obj);
//         Synopsis::MetadataType value_type = tempdpvalue->get_type();
//         Synopsis::DpMetadataValue realvalue;
//         switch(value_type){
//         case 0: //int
//             realvalue = Synopsis::DpMetadataValue(tempdpvalue->get_int_value());
//             break;
//         case 1: //float
//             realvalue = Synopsis::DpMetadataValue(tempdpvalue->get_float_value());
//             break;
//         case 2: //string
//             realvalue = Synopsis::DpMetadataValue(tempdpvalue->get_string_value());
//             break;
//         default:
//             printf("Invalid value Type when creating DpMetaDataValue!\n");
//         }
//         mp.insert({std::string(current->value_type), realvalue});
//     }
//
//     msg = (typeof(msg))malloc(sizeof(*msg));
//     Synopsis::DownlinkState dl_state;
//     switch(downlink_state){
//         case 0:
//             dl_state = Synopsis::DownlinkState::UNTRANSMITTED;
//             break;
//         case 1:
//             dl_state = Synopsis::DownlinkState::TRANSMITTED;
//             break;
//         case 2: 
//             dl_state = Synopsis::DownlinkState::DOWNLINKED;
//             break;
//         default:
//             printf("This is an invalid downlink state must be 0-2!\n");
//     }
//     //printf("Tis is a test: %s\n", instrument_name);
//     std::string inst_name(instrument_name);
//     std::string dptype(dp_type);
//     std::string dpuri(dp_uri);
//     dbdpobj = new Synopsis::DpDbMsg(dp_id, inst_name, dptype, dpuri, dp_size, science_utility_estimate, priority_bin, dl_state, (Synopsis::AsdpEntry)mp);
   
//     msg->obj = dbdpobj;
    
//     return msg;
// }

/**
 * ITC Function for the destruction of DPDBMsg structures
 * @param itc_dbdpmsg_t*: The message to be destroyed
*/
// void itc_destroy_dbdpbmsg(itc_dbdpmsg_t *msg){
//     Synopsis::DpDbMsg *temp_msg = msg->obj;
//     delete temp_msg;
//     free(msg);
// }

/**
 * ITC Function to initalize the DataBase
 * @return: VOID
*/
// void itc_db_init(size_t bytes, void* memory){
//     Synopsis::Status status;
//     status = db.init(bytes, memory, &logger);
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     if(result == E_SUCCESS){
//         printf("ITC DB INIT SUCCESSFUL!\n");
//     }
//     else{
//         printf("ITC DB INIT UNSUCCESSFUL!\n");
//     }
// }

/**
 * ITC Function for Dumb Data Product Entry Testing
 * @return: void
*/
// void itc_db_insert_dumb_data_product(){
//     Synopsis::Status status;
//     Synopsis::DpDbMsg msg(
//         -1, "test_instr", "test_type", "file:///data/file.dat",
//         101, 0.12345, 7, Synopsis::DownlinkState::UNTRANSMITTED,
//         {
//             {"test_int", Synopsis::DpMetadataValue(123)},
//             {"test_float", Synopsis::DpMetadataValue(123.456)},
//             {"test_string", Synopsis::DpMetadataValue("test")}
//         }
//     );
//     status = db.insert_data_product(msg);
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     //EXPECT_EQ(Synopsis::Status::SUCCESS, status);
// }

/**
 * ITC Function for accepting dumb dpmsg for testing
 * @return: VOID
*/
// void itc_app_accept_dumb_dpmsg(){
//     Synopsis::Status status;
//     Synopsis::DpMsg msg(
//         "test_instrument", "test_type",
//         "file::///data/file.dat",
//         "file::///data/meta.dat",
//         true
//     );

//     status = app.accept_dp(msg);
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     if(result == E_SUCCESS){
//         printf("ITC ACCEPT DUMB DPMSG SUCCESSFUL!\n");
//     }
//     else{
//         printf("ITC ACCEPT DUMB DPMSG UNSUCCESSFUL!\n");
//     }
// }

/**
 * ITC Function for accepting DPMSG created by ITC
 * @param[in] msg: The message to be accepted.
 * @return: VOID
*/
// void itc_app_accept_dpmsg(itc_dpmsg_t* msg){
//     Synopsis::Status status;
//     status = app.accept_dp(*(msg->obj));
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     if(result == E_SUCCESS){
//         printf("ITC ACCEPT DPMSG SUCCESSFUL!\n");
//     }
//     else{
//         printf("ITC ACCEPT DPMSG UNSUCCESSFUL!\n");
//     }
// }

/**
 * ITC Function to insert dataproducts into global DataBase
 * @param[in] msg: The Data Product Message to be inserted into the DB
*/
// void itc_db_insert_data_product(itc_dbdpmsg_t *msg){
//     Synopsis::Status status;
//     status = db.insert_data_product(*(msg->obj));
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     if(result == E_SUCCESS){
//         printf("ITC DB INSERT DP SUCCESSFUL!\n");
//     }
//     else{
//         printf("ITC DB INSERT DP UNSUCCESSFUL!\n");
//     }
// }

/**
 * ITC Function to get dataproduct ID from message
 * @param[in] msg: The message from which to retreive the DPID
 * @return: INT
*/
// int itc_msg_get_dp_id(itc_dbdpmsg_t * msg){
//     Synopsis::DpDbMsg *tempdbmsg = msg->obj;
//     return tempdbmsg->get_dp_id();
// }

/**
 * ITC Function to retreive the instrument name from a message
 * @param[in] msg: itc_dpdbmsg_t* from which to retreive the instrument name
 * @return const char* - The instrument Name
 * NOTICE: User must free the resulting memory.
*/
// char* itc_msg_get_instrument_name(itc_dbdpmsg_t* msg){
//     Synopsis::DpDbMsg *tempdbmsg = msg->obj;
//     std::string str = (tempdbmsg->get_instrument_name());
//     char *returnval =  (char *)malloc(sizeof(char) * (str.length() + 1));
//     str.copy(returnval, str.length() + 1, 0);
//     returnval[str.length()] = '\0';
//     return returnval;
// }

/**
 * ITC Function to retrieve dbdpmsg type
 * @param itc_dbdpmsg_t*: The message from which to retreive the type
 * NOTICE: User must free the resulting memory.
*/
// char* itc_msg_get_type(itc_dbdpmsg_t* msg){
//     Synopsis::DpDbMsg *tempdbmsg = msg->obj;
//     std::string str = tempdbmsg->get_type();
//     char *returnval = (char *)malloc(sizeof(char) * (str.length() + 1));
//     str.copy(returnval, str.length() + 1, 0);
//     returnval[str.length()] = '\0';
//     return returnval; 
// }

/**
 * ITC Function to retrieve dbdpmsg type
 * @param itc_dbdpmsg_t*: The message from which to retreive the type
 * NOTICE: Destruction function must be utilized to free this structure
*/
// itc_dpids_t *itc_db_list_data_product_ids(){
//     std::vector<int> asdp_ids = db.list_data_product_ids();
//     itc_dpids_t *array = (itc_dpids_t *)malloc(sizeof(itc_dpids_t));
//     int n = asdp_ids.size();
//     array->size = n;
//     array->values = (int *)malloc(n * sizeof(int));
//     std::copy(asdp_ids.begin(), asdp_ids.end(), array->values);
//     return array;
// }

/**
 * ITC Function for the destruction of the dataproduct id structure
 * @param itc_dpids_t*: The list to be destroyed
*/
// void itc_db_destroy_data_product_ids(itc_dpids_t* dpids){
//     free(dpids->values);
//     free(dpids);
// }

/**
 * ITC Function to get data product from db based on dataproduct id
 * @param int id: The ID from the database to create a copy against.
*/
// itc_dbdpmsg_t* itc_db_get_data_product(int id){
//     Synopsis::Status status;
//     itc_dbdpmsg_t* msg = (typeof(msg))malloc(sizeof(*msg));
//     Synopsis::DpDbMsg *temp_msg2 = new Synopsis::DpDbMsg();
//     status = db.get_data_product(id, *temp_msg2);
//     ITC_STATUS_MESSAGE result = (ITC_STATUS_MESSAGE)status;
//     if(result == E_SUCCESS){
//         printf("ITC GET DB DP SUCCESSFUL!\n");
//     }
//     else{
//         printf("ITC GET DB DP UNSUCCESSFUL!\n");
//     }
//     msg->obj = temp_msg2; 
//     return msg;  
// }
/**
 * ITC Function to destroy DBDP Msg structure
 * @param itc_dbdpmsg_t*: The message structure to be destroyed
*/
// void itc_db_destroy_data_product(itc_dbdpmsg_t *msg){
//     Synopsis::DpDbMsg *temp_msg = msg->obj;
//     delete temp_msg;
//     free(msg);
// }















// printf("******************\n");
    // int syn_app_status;
    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return0: %d\n", syn_app_status);

    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return1: %d\n", syn_app_status);

    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return2: %d\n", syn_app_status);

    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return3: %d\n", syn_app_status);

    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return4: %d\n", syn_app_status);

    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return5: %d\n", syn_app_status);

    // syn_app_status = owls_add_dpmsg();
    // printf("Owls Add Return6: %d\n", syn_app_status);

    // syn_app_status = owls_prioritize_data();
    // printf("Owls Prioritize Return: %d\n", syn_app_status);

    // syn_app_status = itc_display_prioritized_data();
    // printf("Data Return: %d\n", syn_app_status);

    // char* test_string = itc_get_prioritized_data();
    // printf("SYN_APP: TEST: %s\n", test_string);
    // itc_destroy_prioritized_data_string(test_string);
    // printf("\n");
    
    // syn_app_status = itc_display_prioritized_data();
    // printf("Data Return: %d\n", syn_app_status);

    // status2 = owls_test();//"/data/owls/owls_bundle_20221223T144226.db", "/data/owls/empty_rules.json", "/data/owls/owls_similarity_config.json");
    // printf("RETURN CODE: %d\n", status2);
    // itc_display_prioritized_data();
    // char* test_string = itc_get_prioritized_data();
    // printf("SYN_APP: TEST: %s\n", test_string);
    // itc_destroy_prioritized_data_string(test_string);
    // printf("\n");
    // itc_display_prioritized_data();
    // char* test_string2 = itc_get_prioritized_data();
    // printf("SYN_APP: TEST2: %s\n", test_string2);
    // itc_destroy_prioritized_data_string(test_string2);
    
    // Verify that invocations are at 0
    // bool invocations = FALSE;
    // invocations = verify_invocations(0, (char *)"APP INIT");

    // //TODO:  Handle BOOL?

    // // Test dumb, pre-configured dpmsg
    // itc_app_accept_dumb_dpmsg();
    // // Verify that invocations has increased
    // verify_invocations(1, (char *)"DUMB DPMSG");
    
    // // Repeat the above test to validate invocation increase
    // itc_app_accept_dumb_dpmsg();
    // // Verify that invocations has increased
    // verify_invocations(2, (char *)"DUMB DPMSG2");

    // // Create DPMSG
    // itc_dpmsg_t *temp_dpmsg = itc_create_dpmsg((char *)"test_instrument",(char *)"test_type", (char *)"file::///data/file.dat", (char *)"file::///data/meta.dat", true);

    // // Accept DPMSG
    // itc_app_accept_dpmsg(temp_dpmsg);
    // // Verify that invocations has increased
    // verify_invocations(3, (char *)"ITC DPMSG");
    // itc_destroy_dpmsg(temp_dpmsg);
    
    // // Initalize Database with NULL memory, Logger currently built in (can move to our own control if necessary)
    // itc_db_init(0, NULL);

    // // Create DPDBMSG
    // itc_node_t *start = NULL; // Build metadata linked list to be converted into vector
    // itc_node_push(start, (char *)"test_int", itc_create_dpmetadatavalue_int(123));
    // itc_node_push(start, (char *)"test_float", itc_create_dpmetadatavalue_float(123.456));
    // itc_node_push(start, (char *)"test_string", itc_create_dpmetadatavalue_string((char *)"test"));

    // itc_dbdpmsg_t *dpdbmsg = itc_create_dbdpmsg(-1, "test_instr", "test_type", "file:///data/file.dat", 101, 0.12345, 7, 0, start);

    // // Insert the Data Product
    // itc_db_insert_data_product(dpdbmsg);
    // // Verify that DP ID is greater than 0
    // if(itc_msg_get_dp_id(dpdbmsg) > 0){
    //     printf("ITC DP ID > 0: SUCCESS!\n");
    // }
    // else{
    //     printf("ITC DP ID > 0: UNSUCCESSFUL!\n");
    // }
    // // Verify that ASDP IDs size is 1
    // itc_dpids_t* asdp_ids = itc_db_list_data_product_ids();
    // if(asdp_ids->size == 1){
    //     printf("ITC ASDP_IDs SIZE = 1: SUCCESS\n");
    // }
    // else{
    //     printf("ITC ASDP_IDs SIZE = 1: UNSUCCESSFUL\n");
    // }

    // itc_dbdpmsg_t *copy_msg = NULL;
    // copy_msg = itc_db_get_data_product(asdp_ids->values[0]);
    // char *msg_type = NULL;
    // msg_type = itc_msg_get_type(copy_msg);  // Verify that the copied message type remains the same
    // printf("ITC GET MSG TYPE: %s\n", msg_type);
    // free(msg_type);

    // itc_destroy_nodes(start);
    // itc_destroy_dbdpbmsg(dpdbmsg);
    // itc_destroy_dbdpbmsg(copy_msg);
    // itc_db_destroy_data_product_ids(asdp_ids);