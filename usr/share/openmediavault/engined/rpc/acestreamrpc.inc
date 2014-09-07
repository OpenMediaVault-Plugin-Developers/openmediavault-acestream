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
		$object['proxyhost'] = $object['proxyhost'];
		$object['proxyport'] = intval($object['proxyport']);
		
		$object['vlcuse'] = boolval($object['vlcuse']);
		$object['vlchost'] = $object['vlchost'];
		$object['vlcport'] = intval($object['vlcport']);
		$object['vlcoutport'] = intval($object['vlcoutport']);
		$object['vlcpass'] = $object['vlcpass'];
		return $object;
	}

	public function setSettings($params, $context) {
		global $xmlConfig; 
		$this->validateMethodContext($context, array("role" => OMV_ROLE_ADMINISTRATOR)); 
		
		$this->validateMethodParams($params, '{
			"type":"object",
			"properties":{
				"enable":{"type":"boolean"},
				"proxyhost":{"type":"string"},
				"proxyport":{"type":"integer"},
				"vlcuse":{"type":"boolean"},
				"vlchost":{"type":"string"},
				"vlcport":{"type":"integer"},
				"vlcpass":{"type":"string"},
				"vlcoutport":{"type":"integer"}
			}
			}
		');
		
		$xpath = "//services/acestream"; // Structure of the xml schema
		$oldObject = $xmlConfig->get($xpath); // Read data from the xmlConfig file
		
		if(is_null($oldObject)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,$xpath);
		}
	  
		// Update the configuration object. 
		$object = array(
			"enable" => array_boolval($params, 'enable'),
			"proxyhost" => $params['proxyhost'],
			"proxyport" => intval($params['proxyport']),
			"vlcuse" => array_boolval($params['vlcuse']),
			"vlchost" => $params['vlchost'],
			"vlcport" => intval($params['vlcport']),
			"vlcpass" => $params['vlcpass'],
			"vlcoutport" => intval($params['vlcoutport']),
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
