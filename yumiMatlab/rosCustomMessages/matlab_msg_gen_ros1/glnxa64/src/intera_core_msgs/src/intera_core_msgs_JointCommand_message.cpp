// Copyright 2019-2021 The MathWorks, Inc.
// Common copy functions for intera_core_msgs/JointCommand
#include "boost/date_time.hpp"
#include "boost/shared_array.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4265)
#pragma warning(disable : 4458)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4267)
#pragma warning(disable : 4068)
#pragma warning(disable : 4245)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#endif //_MSC_VER
#include "ros/ros.h"
#include "intera_core_msgs/JointCommand.h"
#include "visibility_control.h"
#include "MATLABROSMsgInterface.hpp"
#include "ROSPubSubTemplates.hpp"
class INTERA_CORE_MSGS_EXPORT intera_core_msgs_msg_JointCommand_common : public MATLABROSMsgInterface<intera_core_msgs::JointCommand> {
  public:
    virtual ~intera_core_msgs_msg_JointCommand_common(){}
    virtual void copy_from_struct(intera_core_msgs::JointCommand* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const intera_core_msgs::JointCommand* msg, MultiLibLoader loader, size_t size = 1);
};
  void intera_core_msgs_msg_JointCommand_common::copy_from_struct(intera_core_msgs::JointCommand* msg, const matlab::data::Struct& arr,
               MultiLibLoader loader) {
    try {
        //header
        const matlab::data::StructArray header_arr = arr["Header"];
        auto msgClassPtr_header = getCommonObject<std_msgs::Header>("std_msgs_msg_Header_common",loader);
        msgClassPtr_header->copy_from_struct(&msg->header,header_arr[0],loader);
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Header' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Header' is wrong type; expected a struct.");
    }
    try {
        //mode
        const matlab::data::TypedArray<int32_t> mode_arr = arr["Mode"];
        msg->mode = mode_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Mode' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Mode' is wrong type; expected a int32.");
    }
    try {
        //names
        const matlab::data::CellArray names_cellarr = arr["Names"];
        size_t nelem = names_cellarr.getNumberOfElements();
        for (size_t idx=0; idx < nelem; ++idx){
        	const matlab::data::CharArray names_arr = names_cellarr[idx];
        	msg->names.push_back(names_arr.toAscii());
        }
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Names' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Names' is wrong type; expected a string.");
    }
    try {
        //position
        const matlab::data::TypedArray<double> position_arr = arr["Position"];
        size_t nelem = position_arr.getNumberOfElements();
        	msg->position.resize(nelem);
        	std::copy(position_arr.begin(), position_arr.begin()+nelem, msg->position.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Position' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Position' is wrong type; expected a double.");
    }
    try {
        //velocity
        const matlab::data::TypedArray<double> velocity_arr = arr["Velocity"];
        size_t nelem = velocity_arr.getNumberOfElements();
        	msg->velocity.resize(nelem);
        	std::copy(velocity_arr.begin(), velocity_arr.begin()+nelem, msg->velocity.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Velocity' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Velocity' is wrong type; expected a double.");
    }
    try {
        //acceleration
        const matlab::data::TypedArray<double> acceleration_arr = arr["Acceleration"];
        size_t nelem = acceleration_arr.getNumberOfElements();
        	msg->acceleration.resize(nelem);
        	std::copy(acceleration_arr.begin(), acceleration_arr.begin()+nelem, msg->acceleration.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Acceleration' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Acceleration' is wrong type; expected a double.");
    }
    try {
        //effort
        const matlab::data::TypedArray<double> effort_arr = arr["Effort"];
        size_t nelem = effort_arr.getNumberOfElements();
        	msg->effort.resize(nelem);
        	std::copy(effort_arr.begin(), effort_arr.begin()+nelem, msg->effort.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Effort' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Effort' is wrong type; expected a double.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T intera_core_msgs_msg_JointCommand_common::get_arr(MDFactory_T& factory, const intera_core_msgs::JointCommand* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","Header","Mode","Names","Position","Velocity","Acceleration","Effort","POSITIONMODE","VELOCITYMODE","TORQUEMODE","TRAJECTORYMODE"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("intera_core_msgs/JointCommand");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::Header>("std_msgs_msg_Header_common",loader);
    outArray[ctr]["Header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // mode
    auto currentElement_mode = (msg + ctr)->mode;
    outArray[ctr]["Mode"] = factory.createScalar(currentElement_mode);
    // names
    auto currentElement_names = (msg + ctr)->names;
    auto namesoutCell = factory.createCellArray({currentElement_names.size(),1});
    for(size_t idxin = 0; idxin < currentElement_names.size(); ++ idxin){
    	namesoutCell[idxin] = factory.createCharArray(currentElement_names[idxin]);
    }
    outArray[ctr]["Names"] = namesoutCell;
    // position
    auto currentElement_position = (msg + ctr)->position;
    outArray[ctr]["Position"] = factory.createArray<intera_core_msgs::JointCommand::_position_type::const_iterator, double>({currentElement_position.size(),1}, currentElement_position.begin(), currentElement_position.end());
    // velocity
    auto currentElement_velocity = (msg + ctr)->velocity;
    outArray[ctr]["Velocity"] = factory.createArray<intera_core_msgs::JointCommand::_velocity_type::const_iterator, double>({currentElement_velocity.size(),1}, currentElement_velocity.begin(), currentElement_velocity.end());
    // acceleration
    auto currentElement_acceleration = (msg + ctr)->acceleration;
    outArray[ctr]["Acceleration"] = factory.createArray<intera_core_msgs::JointCommand::_acceleration_type::const_iterator, double>({currentElement_acceleration.size(),1}, currentElement_acceleration.begin(), currentElement_acceleration.end());
    // effort
    auto currentElement_effort = (msg + ctr)->effort;
    outArray[ctr]["Effort"] = factory.createArray<intera_core_msgs::JointCommand::_effort_type::const_iterator, double>({currentElement_effort.size(),1}, currentElement_effort.begin(), currentElement_effort.end());
    // POSITION_MODE
    auto currentElement_POSITION_MODE = (msg + ctr)->POSITION_MODE;
    outArray[ctr]["POSITIONMODE"] = factory.createScalar(static_cast<int32_t>(currentElement_POSITION_MODE));
    // VELOCITY_MODE
    auto currentElement_VELOCITY_MODE = (msg + ctr)->VELOCITY_MODE;
    outArray[ctr]["VELOCITYMODE"] = factory.createScalar(static_cast<int32_t>(currentElement_VELOCITY_MODE));
    // TORQUE_MODE
    auto currentElement_TORQUE_MODE = (msg + ctr)->TORQUE_MODE;
    outArray[ctr]["TORQUEMODE"] = factory.createScalar(static_cast<int32_t>(currentElement_TORQUE_MODE));
    // TRAJECTORY_MODE
    auto currentElement_TRAJECTORY_MODE = (msg + ctr)->TRAJECTORY_MODE;
    outArray[ctr]["TRAJECTORYMODE"] = factory.createScalar(static_cast<int32_t>(currentElement_TRAJECTORY_MODE));
    }
    return std::move(outArray);
  } 
class INTERA_CORE_MSGS_EXPORT intera_core_msgs_JointCommand_message : public ROSMsgElementInterfaceFactory {
  public:
    virtual ~intera_core_msgs_JointCommand_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType type);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType type);
    virtual std::shared_ptr<MATLABRosbagWriterInterface> generateRosbagWriterInterface(ElementType type);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          intera_core_msgs_JointCommand_message::generatePublisherInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSPublisherImpl<intera_core_msgs::JointCommand,intera_core_msgs_msg_JointCommand_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         intera_core_msgs_JointCommand_message::generateSubscriberInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSSubscriberImpl<intera_core_msgs::JointCommand,intera_core_msgs::JointCommand::ConstPtr,intera_core_msgs_msg_JointCommand_common>>();
  }
#include "ROSbagTemplates.hpp" 
  std::shared_ptr<MATLABRosbagWriterInterface>
         intera_core_msgs_JointCommand_message::generateRosbagWriterInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSBagWriterImpl<intera_core_msgs::JointCommand,intera_core_msgs_msg_JointCommand_common>>();
  }
#include "register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(intera_core_msgs_msg_JointCommand_common, MATLABROSMsgInterface<intera_core_msgs::JointCommand>)
CLASS_LOADER_REGISTER_CLASS(intera_core_msgs_JointCommand_message, ROSMsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER
//gen-1