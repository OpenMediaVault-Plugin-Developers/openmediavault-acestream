<?php

/**
 * Copyright (C) 2013-2014 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

require_once("openmediavault/object.inc");
require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
require_once("openmediavault/util.inc");
require_once("openmediavault/rpcservice.inc");
require_once("openmediavault/notify.inc");

class OMVRpcServiceAceStream extends OMVRpcServiceAbstract
{
    public function getName() { 
        return "AceStream";
    }

    private function getPluginName()
    {
        return strtolower($this->getName());
    }

    private function getEventMessagePath()
    {
        return sprintf("org.openmediavault.service.%s", $this->getPluginName());
    }

    private function getXPath()
    {
        return sprintf("//services/%s", $this->getPluginName());
    }

    public function initialize()
    {
        $this->registerMethod("getSettings");
        $this->registerMethod("setSettings");
    }

    public function getSettings($params, $context)
    {
        global $xmlConfig; 

        $this->validateMethodContext($context, array(
            "role" => OMV_ROLE_ADMINISTRATOR
        ));

        $object = $xmlConfig->get($this->getXPath());

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXPath()
            );
        }

        $object['enable']     = boolval($object['enable']);
        $object['proxyhost']  = $object['proxyhost'];
        $object['proxyport']  = intval($object['proxyport']);
		
        $object['vlcuse']     = boolval($object['vlcuse']);
        $object['vlchost']    = $object['vlchost'];
        $object['vlcport']    = intval($object['vlcport']);
        $object['vlcoutport'] = intval($object['vlcoutport']);
        $object['vlcpass']    = $object['vlcpass'];

        return $object;
    }

    public function setSettings($params, $context)
    {
        global $xmlConfig;

        $this->validateMethodContext($context, array(
            "role" => OMV_ROLE_ADMINISTRATOR
        ));

        $this->validateMethodParams(
            $params,
            '{
                "type"       : "object",
                "properties" : {
                    "enable"     :{"type":"boolean"},
                    "proxyhost"  :{"type":"string"},
                    "proxyport"  :{"type":"integer"},
                    "vlcuse"     :{"type":"boolean"},
                    "vlchost"    :{"type":"string"},
                    "vlcport"    :{"type":"integer"},
                    "vlcpass"    :{"type":"string"},
                    "vlcoutport" :{"type":"integer"}
                }
            }'
        );
		
        // Update the configuration object. 
        $object = array(
            "enable"     => array_boolval($params, 'enable'),
            "proxyhost"  => $params['proxyhost'],
            "proxyport"  => intval($params['proxyport']),
            "vlcuse"     => array_boolval($params, "vlcuse"),
            "vlchost"    => $params['vlchost'],
            "vlcport"    => intval($params['vlcport']),
            "vlcpass"    => $params['vlcpass'],
            "vlcoutport" => intval($params['vlcoutport']),
        );

        if (false === $xmlConfig->replace($this->getXPath(), $object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED,
                $this->getXPath()
            );
        }

        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(
            OMV_NOTIFY_MODIFY,
            $this->getEventMessagePath(),
            $object
        );
        return $object;
    }
}
// Register the RPC service.
$rpcServiceMgr = &OMVRpcServiceMgr::getInstance();
$rpcServiceMgr->registerService(new OMVRpcServiceAceStream());
