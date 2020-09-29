//--------------------------------------------------------------------------
// Code generated by the SmartSoft MDSD Toolchain
// The SmartSoft Toolchain has been developed by:
//  
// Service Robotics Research Center
// University of Applied Sciences Ulm
// Prittwitzstr. 10
// 89075 Ulm (Germany)
//
// Information about the SmartSoft MDSD Toolchain is available at:
// www.servicerobotik-ulm.de
//
// Please do not modify this file. It will be re-generated
// running the code generator.
//--------------------------------------------------------------------------

#include "Component3dReconstructionOpcUaBackendPortFactory.hh"

// include all potentially required pattern implementations
#include <SeRoNetSDK/SeRoNet/OPCUA/Client/PushClient.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Client/EventClient.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Client/QClientOPCUA.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Client/SendClient.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Client/QueryClient.hpp>

#include <SeRoNetSDK/SeRoNet/OPCUA/Server/PushServer.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Server/EventServer.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Server/SendServer.hpp>
#include <SeRoNetSDK/SeRoNet/OPCUA/Server/QueryServer.hpp>

// include referenced CommunicationObject SeRoNetSDK self description implementations
#include "CommManipulatorObjectsOpcUa/CommManipulatorIdOpcUa.hh"
#include "DomainVisionOpcUa/Comm3dPointCloudOpcUa.hh"
#include "DomainVisionOpcUa/CommDepthImageOpcUa.hh"
#include "DomainVisionOpcUa/CommVideoImageOpcUa.hh"

// create a static instance of the OpcUaBackendPortFactory
static Component3dReconstructionOpcUaBackendPortFactory OpcUaBackendPortFactory;

Component3dReconstructionOpcUaBackendPortFactory::Component3dReconstructionOpcUaBackendPortFactory()
{  
	componentImpl = 0;
	Component3dReconstruction::instance()->addPortFactory("OpcUa_SeRoNet", this);
}

Component3dReconstructionOpcUaBackendPortFactory::~Component3dReconstructionOpcUaBackendPortFactory()
{  }

void Component3dReconstructionOpcUaBackendPortFactory::initialize(Component3dReconstruction *component, int argc, char* argv[])
{
	componentImpl = new SeRoNet::Utils::Component(component->connections.component.name);
}

int Component3dReconstructionOpcUaBackendPortFactory::onStartup()
{
	if (!component_thread.joinable()) {
    	component_thread = std::thread(&Component3dReconstructionOpcUaBackendPortFactory::task_execution, this);
    	return 0;
    }
	return -1;
}

Smart::IPushClientPattern<DomainVision::CommDepthImage> * Component3dReconstructionOpcUaBackendPortFactory::createDepthImagePushServiceIn()
{
	return new SeRoNet::OPCUA::Client::PushClient<DomainVision::CommDepthImage>(componentImpl);
}


Smart::IPushServerPattern<DomainVision::Comm3dPointCloud> * Component3dReconstructionOpcUaBackendPortFactory::createPointCloudPushServiceOut(const std::string &serviceName)
{
	return new SeRoNet::OPCUA::Server::PushServer<DomainVision::Comm3dPointCloud>(componentImpl, serviceName);
}

Smart::IQueryServerPattern<CommManipulatorObjects::CommManipulatorId, DomainVision::Comm3dPointCloud> * Component3dReconstructionOpcUaBackendPortFactory::createPointCloudQueryServiceAnsw(const std::string &serviceName)
{
	return new SeRoNet::OPCUA::Server::QueryServer<CommManipulatorObjects::CommManipulatorId, DomainVision::Comm3dPointCloud>(componentImpl, serviceName);
}

Smart::IPushServerPattern<DomainVision::CommVideoImage> * Component3dReconstructionOpcUaBackendPortFactory::createRGBImagePushServiceOut(const std::string &serviceName)
{
	return new SeRoNet::OPCUA::Server::PushServer<DomainVision::CommVideoImage>(componentImpl, serviceName);
}


int Component3dReconstructionOpcUaBackendPortFactory::task_execution()
{
	componentImpl->run();
	return 0;
}

int Component3dReconstructionOpcUaBackendPortFactory::onShutdown(const std::chrono::steady_clock::duration &timeoutTime)
{
	// stop component-internal infrastructure
	componentImpl->stopRunning();
	// wait on component thread to exit
	if (component_thread.joinable()) {
		// FIXME: don't wait infinetly (use timeoutTime here)
    	component_thread.join();
    }
	return 0;
}

void Component3dReconstructionOpcUaBackendPortFactory::destroy()
{
	// clean-up component's internally used resources
	delete componentImpl;
}