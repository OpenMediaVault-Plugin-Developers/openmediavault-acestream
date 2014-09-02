<?php
require_once("openmediavault/object.inc");
require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
require_once("openmediavault/util.inc");
require_once("openmediavault/rpcservice.inc");
require_once("openmediavault/notify.inc");

class OMVRpcServiceAceStream extends OMVRpcServiceAbstract {
		public function getName() { 
			return "AceStream";
		}
        /* Initialize the RPC service. Different methods of the RPC service are declared here*/
        public function initialize() {
                $this->registerMethod("getSettings");
                $this->registerMethod("setSettings");
        }
        public function getSettings($params, $context) {
		global $xmlConfig; 
		$this->validateMethodContext($context, array("role" => OMV_ROLE_ADMINISTRATOR)); 
		$xpath = "//services/acestream"; // Structure of the xml schema
		$object = $xmlConfig->get($xpath); // Read data from the xmlConfig file 
		if(is_null($object)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,$xpath);
		}
		$object['enable'] = boolval($object['enable']);
		$object['streamhost'] = $object['streamhost'];
		$object['streamport'] = intval($object['streamport']);
		$object['proxyhost'] = $object['proxyhost'];
		$object['proxyport'] = intval($object['proxyport']);
		return $object;
	}

	public function setSettings($params, $context) {
		global $xmlConfig; 
		$this->validateMethodContext($context, array("role" => OMV_ROLE_ADMINISTRATOR)); 
		
		$this->validateMethodParams($params, '{
		"type":"object",
		"properties":{
			"enable":{"type":"boolean"},
			"streamhost":{"type":"string"},
			"streamport":{"type":"integer"},
			"proxyhost":{"type":"string"},
			"proxyport":{"type":"integer"}}
			}'
		);
		
		$xpath = "//services/acestream"; // Structure of the xml schema
		$oldObject = $xmlConfig->get($xpath); // Read data from the xmlConfig file
		
		if(is_null($oldObject)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,$xpath);
		}
	  
		// Update the configuration object. 
		$object = array(
			"enable" => array_boolval($params, 'enable'),
			"streamhost" => $params['streamhost'],
			"streamport" => intval($params['streamport']),
			"proxyhost" => $params['proxyhost'],
			"proxyport" => intval($params['proxyport']),
		);
		// Update the xmlConfig file. If it fails then generate an error
		if(FALSE === $xmlConfig->replace("//services/acestream", $object)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
		}
		// Notify configuration changes.
		$dispatcher = &OMVNotifyDispatcher::getInstance();
		$dispatcher->notify(OMV_NOTIFY_MODIFY, "org.openmediavault.services.acestream", $object);
		// Return the configuration object.
		return $object;
	}
}
// Register the RPC service.
$rpcServiceMgr = &OMVRpcServiceMgr::getInstance(); // Get the "root" instance for the Services 
$rpcServiceMgr->registerService(new OMVRpcServiceAceStream()); // Register a new instance of the RPC service described above
?>